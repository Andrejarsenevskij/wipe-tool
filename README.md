<div align="center">

# 🛠️ ChromeSetup Simulator & Wipe Tool

**A high-fidelity C++ system utility for setup-architecture simulation, file-layout modeling, and digital-signature workflow testing.**

<br>

![Language](https://img.shields.io/badge/language-C%2B%2B17-659AD2?style=for-the-badge&logo=cplusplus&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows%2010%20%7C%2011-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Compiler](https://img.shields.io/badge/compiler-GCC%20%7C%20Clang%20%7C%20MSVC-6C7086?style=for-the-badge)
![License](https://img.shields.io/badge/license-MIT-2EA44F?style=for-the-badge)

<br>

<sub>Built for controlled research, system-architecture analysis, and isolated laboratory testing.</sub>

</div>

---

> [!WARNING]
> **Educational & Testing Disclaimer**
>
> This project is intended strictly for research, architecture analysis, and isolated laboratory testing. Use it only in environments you own or are explicitly authorized to test. It is not affiliated with, endorsed by, or an official installer for Google Chrome.

## ⚡ Core Highlights

| Capability | Description | Technology |
|:--|:--|:--:|
| 🏗️ **Architecture Replication** | Models setup-style metadata, directory structures, and runtime execution flow. | MinGW / C++ |
| 🔏 **Signing Workflow** | Includes a PowerShell-based workflow for generating local test certificates and signing test binaries. | PowerShell |
| 📦 **Lightweight Build** | Designed to compile into a standalone executable without heavyweight runtime frameworks. | GCC / Clang |

## 📁 System Architecture

```text
wipe-tool/
├── 📄 ChromeSetup.exe   # Application build output
├── 📄 main.cpp          # Main execution entry point and logic
├── ⚙️  build.bat         # One-click build script
└── 🧩  6.ico             # Application icon resource
```

> **Build note:** `ChromeSetup.exe` is a generated output. Depending on your local setup, you may need to build the project before this file appears.

## 🔐 Code Signing Setup

<details>
<summary><strong>🔎 Expand certificate generation & signing instructions</strong></summary>

<br>

The following is a **local testing workflow** for a development certificate. A self-signed certificate is not automatically trusted by Windows and does not establish that an application is from Google or any other publisher.

### 1. Create Certificate

```powershell
$cert = New-SelfSignedCertificate -CertStoreLocation Cert:\CurrentUser\My -Subject "CN=Google LLC" -Type CodeSigningCert -HashAlgorithm SHA256; Export-Certificate -Cert $cert -FilePath "$env:TEMP\cert.crt"; Import-Certificate -FilePath "$env:TEMP\cert.crt" -CertStoreLocation Cert:\CurrentUser\Root
```

### 2. Sign Target Binary

```powershell
Set-AuthenticodeSignature -FilePath "C:\path\to\your\ChromeSetup.exe" -Certificate (Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Where-Object {$_.Subject -eq "CN=Google LLC"})[0] -TimestampServer "[http://timestamp.digicert.com](http://timestamp.digicert.com)"
```

**Important:** Use a clearly labeled development certificate for local experiments. Do not present a self-signed certificate as a verified publisher identity. Trust-store changes should only be made when you understand their security implications.

</details>

## 🚀 Quick Start

### Prerequisites

- **Operating system:** Windows 10 or Windows 11
- **C++ compiler:** GCC (MinGW-w64), Clang, or MSVC
- **Git:** Required to clone the repository

### 1. Clone the repository
```bash
git clone https://github.com/Andrejarsenevskij/wipe-tool.git
cd wipe-tool
```

### 2. Run Executable
Launch the binary directly from your terminal or folder:
```cmd
.\ChromeSetup.exe
```

> [!CAUTION]
> **Safety First:** Only execute binaries you have reviewed yourself. Running in an isolated Virtual Machine (VM) or sandbox environment is highly recommended.

---

<p align="center">
  <b>Designed for controlled experimentation & system research. Built with C++.</b>
  <br>
  <sub>Developed with ⚡ by <a href="https://github.com/Andrejarsenevskij"><b>Andrej Arsenevskij</b></a></sub>
</p>
