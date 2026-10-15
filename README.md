# ChromeSetup Simulator / Wipe Tool

A C++ utility designed to build an executable that closely mimics the official Google Chrome installer (`ChromeSetup.exe`) in appearance, file structure, and metadata, while executing custom system operations under the hood.

## 📌 Overview

This project demonstrates how to construct a binary with custom resource files, metadata, and digital signature structures to replicate the look and feel of standard Windows installers.

* **Appearance:** Features identical metadata, icons, and resource parameters similar to `ChromeSetup.exe`.
* **Language:** Written in native C++ for direct Windows API access.
* **Architecture:** Targeted for x64 Windows environments.

---

## 🚀 Getting Started

### Prerequisites

* **Operating System:** Windows 10 / 11
* **IDE / Compiler:** Visual Studio 2022 (with *Desktop development with C++*) or MSVC / MinGW.
* **SDK:** Windows SDK (for code signing utilities if needed).
