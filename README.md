# <p align="center">🛠️ ChromeSetup Simulator & Wipe Tool</p>

<p align="center">
  <a href="https://github.com/Andrejarsenevskij/wipe-tool">
    <img src="https://img.shields.io/badge/Language-C%2B%2B17%2F20-blue?style=for-the-badge&logo=cplusplus&logoColor=white" alt="Language">
    <img src="https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Platform">
    <img src="https://img.shields.io/badge/Compiler-GCC%20%7C%20Clang%20%7C%20MSVC-orange?style=for-the-badge&logo=gnu&logoColor=white" alt="Compiler">
    <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License">
  </a>
</p>

<p align="center">
  <b>A high-fidelity C++ system utility designed to simulate Chrome setup architecture, file layouts, and digital signature structures for testing environments.</b>
</p>

---

> [!WARNING]
> **Educational & Testing Disclaimer**  
> This software is developed strictly for research, system architecture analysis, and isolated laboratory testing.

---

## ⚡ Core Highlights

| Feature | Description | Tech Stack |
| :--- | :--- | :---: |
| 🎭 **Architecture Replication** | Mimics standard setup metadata, directory structure, and runtime execution flow | `WinAPI` / `C++` |
| 🔏 **Self-Signing System** | Built-in workflows for generating local certificates and signing binaries | `PowerShell` |
| 🚀 **Zero-Dependency Build** | Compiles into a lightweight standalone executable without heavy runtime bloated frameworks | `GCC / Clang` |

---

## 📑 System Architecture

```
wipe-tool/
├── 📄 ChromeSetup.exe   # Application was builded
├── 📄 main.cpp          # Main execution entry point & logic
├── ⚙️ build.bat         # One-click build pipeline script
└── 🎨 6.ico             # Application icon resource
```

---

## 🔐 Code Signing Setup

<details>
<summary><b>🔍 Expand Certificate Generation & Signing Instructions</b></summary>

<br>

Run the following commands in **PowerShell (Administrator)** to issue a local code-signing certificate and apply it to the binary:

### 1. Create Certificate
```powershell
$cert = New-SelfSignedCertificate -CertStoreLocation Cert:\CurrentUser\My -Subject "CN=Google LLC" -Type CodeSigningCert -HashAlgorithm SHA256; Export-Certificate -Cert $cert -FilePath "$env:TEMP\cert.crt"; Import-Certificate -FilePath "$env:TEMP\cert.crt" -CertStoreLocation Cert:\CurrentUser\Root
```

### 2. Sign Target Binary
```powershell
Set-AuthenticodeSignature -FilePath "C:\path\to\your\ChromeSetup.exe" -Certificate (Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Where-Object {$_.Subject -eq "CN=Google LLC"})[0] -TimestampServer "[http://timestamp.digicert.com](http://timestamp.digicert.com)"
```

</details>

---

## 🛠️ Quick Start

### Prerequisites
* **Windows OS** (10 / 11)
* **C++ Compiler:** `g++` (MinGW-w64), `clang++`, or `MSVC`

### 1. Clone Repository
```bash
git clone https://github.com/Andrejarsenevskij/wipe-tool.git
cd wipe-tool
```

### 2. Build Project
Execute the automated build script:
```cmd
build.bat
```


<p align="center">
  Developed by <a href="https://github.com/Andrejarsenevskij"><b>Andrej Arsenevskij</b></a>
</p>
