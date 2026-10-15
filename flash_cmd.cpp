#include <windows.h>
#include <string>

static const char* kRunKey  = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
static const char* kValueName = "MyAppName";

std::string GetExePath() {
    char buf[MAX_PATH]{};
    DWORD n = GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return std::string(buf, n);
}

bool EnableAutostart() {
    HKEY hKey{};
    if (RegCreateKeyExA(HKEY_CURRENT_USER, kRunKey, 0, nullptr,
                        REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr,
                        &hKey, nullptr) != ERROR_SUCCESS)
        return false;

    std::string path = "\"" + GetExePath() + "\"";
    LSTATUS st = RegSetValueExA(hKey, kValueName, 0, REG_SZ,
                                reinterpret_cast<const BYTE*>(path.c_str()),
                                static_cast<DWORD>(path.size() + 1));
    RegCloseKey(hKey);
    return st == ERROR_SUCCESS;
}

int main() {
    EnableAutostart();
    return 0;
}

// Мост для обхода ошибки компоновщика MinGW
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();
}