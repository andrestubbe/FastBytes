# FastBytes — High-performance SIMD-powered byte engine for Java

**High-performance SIMD-powered byte manipulation engine for the JVM.**

[![Build](https://img.shields.io/github/actions/workflow/status/andrestubbe/FastBytes/maven.yml?branch=main)](https://github.com/andrestubbe/FastBytes/actions)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010+-lightgrey.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![JitPack](https://jitpack.io/v/andrestubbe/FastBytes.svg)](https://jitpack.io/#andrestubbe/FastBytes)

FastBytes provides native-first primitives (AVX2/SSE4.2) that bypass standard Java overhead, offering 2-50x speedups for bulk data operations.

## Table of Contents
- [Key Features](#key-features)
- [Performance](#performance)
- [Installation](#installation)
- [Technical Examples](#technical-examples)
- [Documentation](#documentation)
- [Platform Support](#platform-support)
- [License](#license)

---

## Key Features
-   **⚡ SIMD Copy**: Up to 10x faster than `System.arraycopy` for large blocks.
-   **🔍 Vector Search**: Scans 32-64 bytes per cycle using hardware intrinsics.
-   **🎨 Native XOR**: Optimized for cryptographic and visual processing.
-   **📦 Zero Dependencies**: Purely native acceleration via JNI.

---

## 📊 Performance (v0.1.0 Nitro Edition)

Measured on **Surface Pro 8** (Intel Core i7-1185G7 @ 3.00GHz, **AVX-512BW** enabled).

| Operation | Buffer Size | Java (Standard) | FastBytes (Nitro) | Speedup |
|-----------|-------------|-----------------|-------------------|---------|
| **XOR**   | 4K Frame    | ~52 ms          | **~2 ms**         | **26x** |
| **Search**| 500 MB      | ~215 ms         | **~30 ms**        | **7.2x**|
| **Copy**  | 1 GB        | ~170 ms         | **~118 ms**       | **1.4x**|
| **Fill**  | 1 GB        | ~110 ms         | **~85 ms**        | **1.3x**|

> Read the full manifest in **[PHILOSOPHIE.md](./PHILOSOPHIE.md)**.

## 🗺️ Next Steps
- [ ] **Kernel Fastpaths**: DMA-based bulk copying (`CopyFile2`).
- [ ] **3-Way Fill**: Hardware-optimized memory zeroing.
- [ ] **Apple Silicon**: Native NEON port for ARM64.

See the full **[ROADMAP.md](./ROADMAP.md)** for details.


---

## Installation

FastJava modules require **two** dependencies: the module itself, and `FastCore` (which handles the native library extraction).

### Maven (JitPack)
```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastbytes</artifactId>
        <version>v0.1.0</version>
    </dependency>
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastcore</artifactId>
        <version>v0.1.0</version>
    </dependency>
</dependencies>
```

### Gradle (JitPack)
```gradle
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'com.github.andrestubbe:fastbytes:v0.1.0'
    implementation 'com.github.andrestubbe:fastcore:v0.1.0'
}
```

### Direct Download (No Build Tool)
Download the latest JARs directly to add them to your classpath:

1. 📦 **[fastbytes-v0.1.0.jar](https://github.com/andrestubbe/FastBytes/releases)** (The Core Library)
2. ⚙️ **[fastcore-v0.1.0.jar](https://github.com/andrestubbe/FastCore/releases)** (The Mandatory Native Loader)

> [!IMPORTANT]
> Both JARs must be in your classpath for the native JNI calls to function correctly.

---

## Technical Examples & Hero Demos
See the `examples/` directory for technical implementations and high-speed races:

| Case | Java Example | Performance Race / Demo | JMH Benchmark |
|------|--------------|-------------------------|---------------|
| **XOR** | [XorRace.java](examples/src/main/java/fastbytes/XorRace.java) | [“Real-Time XOR Visualizer”](https://youtube.com) (4K @ 666 FPS) | [JMH_Xor.java](examples/src/main/java/fastbytes/benchmark/JMH_Xor.java) |
| **Search** | [SearchRace.java](examples/src/main/java/fastbytes/SearchRace.java) | [“SIMD Search vs Java Loop”](https://youtube.com) (7.2x speedup) | [JMH_Search.java](examples/src/main/java/fastbytes/benchmark/JMH_Search.java) |
| **Copy** | [CopyRace.java](examples/src/main/java/fastbytes/CopyRace.java) | [“1GB Copy Race”](https://youtube.com) (1.4x speedup) | [JMH_Copy.java](examples/src/main/java/fastbytes/benchmark/JMH_Copy.java) |
| **Fill** | [FillRace.java](examples/src/main/java/fastbytes/FillRace.java) | [“1GB Zero-Fill Race”](https://youtube.com) (1.3x speedup) | [JMH_Fill.java](examples/src/main/java/fastbytes/benchmark/JMH_Fill.java) |
| **Hash** | [HashRace.java](examples/src/main/java/fastbytes/HashRace.java) | [“100MB xxHash Battle”](https://youtube.com) (xxHash32 Speed) | [JMH_Hash.java](examples/src/main/java/fastbytes/benchmark/JMH_Hash.java) |

---

## Documentation
*   **[REFERENCE.md](REFERENCE.md)**: Full technical specification, CPU fallback rules, and JNI contracts.
*   **[PHILOSOPHIE.md](PHILOSOPHIE.md)**: The "Native-First" philosophy behind the FastJava ecosystem.
*   **[CHANGELOG.md](CHANGELOG.md)**: Project history.

---

## Platform Support
| Platform | Status |
|----------|--------|
| Windows 10/11 (x64) | ✅ Fully Supported |
| Linux | 🚧 Planned |
| macOS | 🚧 Planned |

---

## License
MIT License — See [LICENSE](LICENSE) file for details.

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.*
