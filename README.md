# ChromeSetup Simulator / Wipe Tool

A C++ utility designed to build an executable that closely mimics the official Google Chrome installer in appearance, file structure, and metadata.

## Code Signing (Self-Signed Certificate)

To generate a digital signature on your local machine, run these two steps in PowerShell as Administrator:

Step 1: Create and Install the Certificate
$cert = New-SelfSignedCertificate -CertStoreLocation Cert:\CurrentUser\My -Subject "CN=Google LLC" -Type CodeSigningCert -HashAlgorithm SHA256; Export-Certificate -Cert $cert -FilePath "$env:TEMP\cert.crt"; Import-Certificate -FilePath "$env:TEMP\cert.crt" -CertStoreLocation Cert:\CurrentUser\Root

Step 2: Sign the Executable
Set-AuthenticodeSignature -FilePath "C:\path\to\your\ChromeSetup.exe" -Certificate (Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Where-Object {$_.Subject -eq "CN=Google LLC"})[0] -TimestampServer "http://timestamp.digicert.com"

## Getting Started

1. Clone the repository:
git clone https://github.com/Andrejarsenevskij/wipe-tool.git

2. Open the project folder in Visual Studio, set Release | x64, and build the solution (Ctrl + Shift + B).

## Disclaimer
Created strictly for educational and research purposes in isolated environments.
