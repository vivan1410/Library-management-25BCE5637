<div align="center">
  <img src="https://img.icons8.com/m_outlined/200/FFFFFF/library.png" alt="Library Logo" width="80" />
  <h1>Library Management System</h1>
  <p>
    <b>A High-Performance, Serverless Library Management Solution Engineered in C++ via WebAssembly.</b>
  </p>

  [![C++](https://img.shields.io/badge/C++17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
  [![WebAssembly](https://img.shields.io/badge/WebAssembly-654FF0?style=flat-square&logo=webassembly&logoColor=white)](https://webassembly.org/)
  [![Supabase](https://img.shields.io/badge/Supabase-3ECF8E?style=flat-square&logo=supabase&logoColor=white)](https://supabase.com/)
  [![Status](https://img.shields.io/badge/Status-Active-success.svg?style=flat-square)](#)
</div>

---

## 📌 Executive Summary

This project reimagines standard console-based applications by bridging the gap between low-level performance and modern web accessibility. The **Library Management System** is a robust, strictly Object-Oriented C++ application compiled directly into WebAssembly (WASM). 

Operating entirely on the client-side without relying on traditional backend server instances, it provides native C++ execution speeds within a browser environment while maintaining real-time data persistence through a cloud-based **Supabase** synchronization bridge.

## 🚀 Technical Highlights

- **Object-Oriented Architecture**: Strict adherence to OOP paradigms, utilizing robust class hierarchies for state, memory, and entity management.
- **WASM Translation**: Leverages Emscripten to translate high-performance C++ code into binary instructional formats deployable universally across browsers.
- **Distributed State Synchronization**: Real-time asynchronous data injection seamlessly syncing the volatile WebAssembly virtual file system with a persistent Supabase PostgreSQL database.
- **Custom DOM I/O Bridge**: Features an intercepted standard input/output stream (`iostream`) layer, transforming local CLI commands into an aesthetically tailored graphical terminal.

## 🏗️ System Architecture

```mermaid
graph LR
    A[C++ Application Core] -->|Emscripten| B(WebAssembly Binary)
    B -->|JS Glue Code| C{Browser Terminal Engine}
    C -->|Asynchronous REST| D[(Supabase Cloud Database)]
    D -->|Initial Hydration| C
```

## ⚙️ Core Functionality

- **Inventory Management**: Instantiate and serialize dynamic `Book` objects via structural memory allocation.
- **Circulation Control**: Atomic issue and return subroutines integrated with precise cloud-status flagging.
- **Data Indexing**: High-efficiency retrieval queries for physical inventory audits spanning precise titles and numerical identification.

## 💻 Local Deployment Process

To run WebAssembly binaries locally, standard HTTP cross-origin security protocols mandate a localized development server. 

```bash
# 1. Clone the repository
git clone https://github.com/vivan1410/Library-management-25BCE5637.git

# 2. Navigate to the deployment directory
cd Library-management-25BCE5637

# 3. Mount the WebAssembly network server
python -m http.server 8000
```
*Navigate to `http://localhost:8000/` in a modern browser to initialize the WASM runtime environment.*
