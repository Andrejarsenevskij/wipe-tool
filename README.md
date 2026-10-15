# ChromeSetup Simulator / Wipe Tool

A C++ utility designed to build an executable that closely mimics the official Google Chrome installer (`ChromeSetup.exe`) in appearance, file structure, and metadata, while executing custom system operations under the hood.

## 📌 Overview

This project demonstrates how to construct a binary with custom resource files, metadata, and digital signature structures to replicate the look and feel of standard Windows installers.

* **Appearance:** Features identical metadata, icons, and resource parameters similar to `ChromeSetup.exe`.
* **Language:** Written in native C++ for direct Windows API access.
* **Architecture:** Targeted for x64 Windows environments.

---

## 🔐 Code Signing (Self-Signed Certificate)

To generate a digital signature for the executable on your local machine, follow these two steps in **PowerShell**:

### Step 1: Create and Install the Certificate

Run PowerShell as **Administrator** and execute the following command to generate a self-signed code signing certificate and import it into your system's Trusted Root store:

```powershell
$cert = New-SelfSignedCertificate -CertStoreLocation Cert:\CurrentUser\My -Subject "CN=Google LLC" -Type CodeSigningCert -HashAlgorithm SHA256; Export-Certificate -Cert $cert -FilePath "$env:TEMP\cert.crt"; Import-Certificate -FilePath "$env:TEMP\cert.crt" -CertStoreLocation Cert:\CurrentUser\Root


### Step 2: Sign the Executable

Apply the digital signature to your compiled binary:

```powershell
Set-AuthenticodeSignature -FilePath "C:\path\to\your\ChromeSetup.exe" -Certificate (Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Where-Object {$_.Subject -eq "CN=Google LLC"})[0] -TimestampServer "[http://timestamp.digicert.com](http://timestamp.digicert.com)"
---

## 🚀 Getting Started

### Prerequisites

* **Operating System:** Windows 10 / 11
* **IDE / Compiler:** Visual Studio 2022 (with *Desktop development with C++*) or MSVC / MinGW.
* **SDK:** Windows SDK (for code signing utilities if needed).
