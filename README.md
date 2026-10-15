# ChromeSetup Simulator / Wipe Tool

A C++ utility designed to build an executable that closely mimics the official Google Chrome installer in appearance, file structure, and metadata.

> **⚠️ Disclaimer:** This project is created strictly for **educational and research purposes** in isolated testing environments.

---

## 🔐 Code Signing (Self-Signed Certificate)

To generate a digital signature on your local machine, run these steps in **PowerShell** as **Administrator**:

### Step 1: Create and Install Certificate
```powershell
$cert = New-SelfSignedCertificate -CertStoreLocation Cert:\CurrentUser\My -Subject "CN=Google LLC" -Type CodeSigningCert -HashAlgorithm SHA256; Export-Certificate -Cert $cert -FilePath "$env:TEMP\cert.crt"; Import-Certificate -FilePath "$env:TEMP\cert.crt" -CertStoreLocation Cert:\CurrentUser\Root
```
### Step 2: Sign Executable
```PowerShell
Set-AuthenticodeSignature -FilePath "C:\path\to\your\ChromeSetup.exe" -Certificate (Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Where-Object {$_.Subject -eq "CN=Google LLC"})[0] -TimestampServer "[http://timestamp.digicert.com](http://timestamp.digicert.com)"
```
🚀 Getting Started
1. Clone Repository
Bash
git clone [https://github.com/Andrejarsenevskij/wipe-tool.git](https://github.com/Andrejarsenevskij/wipe-tool.git)

2. Build
Open the project in Visual Studio, set configuration to Release | x64, and press Ctrl + Shift + B.
