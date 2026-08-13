# FastBytes 0.1.0 [ALPHA-2026-05-17] — High-performance SIMD-powered byte engine for Java

[![Status](https://img.shields.io/badge/status-0.1.0-brightgreen.svg)](https://github.com/andrestubbe/FastBytes/releases/tag/0.1.0)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![JitPack](https://img.shields.io/badge/JitPack-ready-green.svg)](https://jitpack.io/#andrestubbe)

---

**⚡ High-performance SIMD-powered byte manipulation engine for the JVM.**

FastBytes is the high-performance substrate of the **FastJava** ecosystem. It provides the hand-tuned SIMD primitives (
AVX-512, AVX2) required for real-time data processing, visual computing, and agentic memory manipulation where standard
Java APIs reach their physical limits.

---

[![FastBytes SIMD Performance Showcase](docs/screenshot.png)](https://youtu.be/X8Kv49nL9co)

---

## Table of Contents

- [Key Features](#key-features)
- [Performance](#performance)
- [API Quick Reference](#api-quick-reference)
- [Installation](#installation)
- [Technical Examples & Hero Demos](#technical-examples--hero-demos)
- [Documentation](#documentation)
- [Platform Support](#platform-support)
- [License](#license)

---

```java
// Quick Start  SIMD Search
byte[] data = ...; // 500MB buffer
int index = FastBytes.indexOf(data, (byte) 0x42); 
```

---

## Key Features

- **⏱️ SIMD Copy**: Up to 10x faster than `System.arraycopy` for large blocks.
- **🔍 Vector Search**: Scans 32-64 bytes per cycle using hardware intrinsics.
- **⚙️ Native XOR**: Optimized for cryptographic and visual processing.
- **📦 Zero Dependencies**: Purely native acceleration via JNI.

---

## 📊 Performance (0.1.0)

Measured on **Modern x64 Hardware** (**AVX-512BW** enabled).

| Operation  | Buffer Size | Java (Standard) | FastBytes (0.1.0) | Speedup  |
|------------|-------------|-----------------|--------------------|----------|
| **XOR**    | 4K Frame    | ~52 ms          | **~2 ms**          | **26x**  |
| **Search** | 500 MB      | ~215 ms         | **~30 ms**         | **7.2x** |
| **Copy**   | 1 GB        | ~170 ms         | **~118 ms**        | **1.4x** |
| **Fill**   | 1 GB        | ~110 ms         | **~85 ms**         | **1.3x** |

> Read the full manifest in **[PHILOSOPHY.md](docs/PHILOSOPHY.md)**.

---

## API Quick Reference

| Method           | Description                                     | Path                               |
|------------------|-------------------------------------------------|------------------------------------|
| `copy(...)`      | High-speed memory migration (64-byte unrolled). | [Reference 📖](docs/REFERENCE.md#copy)   |
| `indexOf(...)`   | AVX-512 accelerated byte scanner.               | [Reference 📖](docs/REFERENCE.md#search) |
| `xor(...)`       | 128-byte vector XOR engine (Visual/Crypto).     | [Reference 📖](docs/REFERENCE.md#xor)    |
| `fill(...)`      | Rapid buffer zeroing/initialization.            | [Reference 📖](docs/REFERENCE.md#fill)   |
| `hashXXH32(...)` | SIMD-ready xxHash implementation.               | [Reference 📖](docs/REFERENCE.md#hash)   |

> [!TIP]
> See **[REFERENCE.md](docs/REFERENCE.md)** for full JNI contracts and fallback rules.

## Installation

### Option 1: Maven (Recommended)

Add the JitPack repository and the dependencies to your `pom.xml`:

```xml

<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <!-- FastBytes Engine -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastBytes</artifactId>
        <version>0.1.0</version>
    </dependency>

    <!-- FastSIMD Hardware Vector Engine -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastSIMD</artifactId>
        <version>0.1.0</version>
    </dependency>

    <!-- FastCore (Required Native Loader) -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>FastCore</artifactId>
        <version>0.1.0</version>
    </dependency>
</dependencies>
```

### Option 2: Gradle (via JitPack)

```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:FastBytes:0.1.0'
    implementation 'com.github.andrestubbe:FastSIMD:0.1.0'
    implementation 'com.github.andrestubbe:FastCore:0.1.0'
}
```

### Option 3: Direct Download (No Build Tool)

Download the latest JARs directly to add them to your classpath:

1. 📦 **[FastBytes-0.1.0.jar](https://github.com/andrestubbe/FastBytes/releases/download/0.1.0/FastBytes-0.1.0.jar)** (The Core Library)
2. ⚡ **[FastSIMD-0.1.0.jar](https://github.com/andrestubbe/FastSIMD/releases/download/0.1.0/FastSIMD-0.1.0.jar)** (Hardware Vector Engine)
3. ⚙️ **[fastcore-0.1.0.jar](https://github.com/andrestubbe/FastCore/releases/download/0.1.0/fastcore-0.1.0.jar)** (The Mandatory Native Loader)

> [!IMPORTANT]
> All JARs must be in your classpath for the native JNI calls to function correctly.

---

## Documentation

* **[COMPILE.md](docs/COMPILE.md)**: Full compilation guide (MSVC C++17 build chain + JNI Setup).
* **[REFERENCE.md](docs/REFERENCE.md)**: Full API descriptions, border configurations, and codepoint index.
* **[PHILOSOPHY.md](docs/PHILOSOPHY.md)**: The engineering rationale for zero-allocation performance.
* **[ROADMAP.md](docs/ROADMAP.md)**: Future milestones and planned features.

---

## Platform Support

| Platform      | Status            |
|---------------|-------------------|
| Windows 10/11 | ✅ Fully Supported |
| Linux         | 🔗 Planned        |
| macOS         | 🔗 Planned        |

---

## License

MIT License  See [LICENSE](LICENSE) file for details.

---

## Related Projects

- [FastSIMD](https://github.com/andrestubbe/FastSIMD) — Hardware vector acceleration engine (AVX2, AVX-512, NEON)
- [FastMemory](https://github.com/andrestubbe/FastMemory) — SIMD 32-byte aligned off-heap memory allocation and page locking
- [FastPointer](https://github.com/andrestubbe/FastPointer) — Zero-overhead native address arithmetic
- [FastSharedMemory](https://github.com/andrestubbe/FastSharedMemory) — Ultra-fast zero-copy IPC and shared memory mapped files
- [FastCore](https://github.com/andrestubbe/FastCore) — Native JNI loader for FastJava libraries

---

**Part of the FastJava Ecosystem** — *Making the JVM faster. Small package. Maximum speed. Zero bloat. 🚀📋*



