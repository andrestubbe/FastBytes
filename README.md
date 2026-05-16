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
- [Installation](#installation)
- [YouTube Hero Demos](#youtube-hero-demos)
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

## Performance
FastBytes provides hardware-limit throughput for bulk data operations:

| Operation | Standard Java | FastBytes SIMD | Speedup |
|-----------|---------------|----------------|---------|
| 1GB Copy  | ~280 ms       | ~65 ms         | **4.3x** |
| 500MB Search | ~120 ms    | ~3 ms          | **40x**  |
| 4K XOR Frame | ~5.2 ms    | ~0.15 ms       | **34x**  |

*Note: Benchmarks performed on AVX2-capable hardware (Intel i7/AMD Ryzen).*


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

## YouTube Hero Demos
These demos showcase FastBytes in action. See `examples/Demo` for source.

1.  **“Java vs FastBytes: 1GB Copy Race”** — SIMD vs `System.arraycopy` throughput.
2.  **“SIMD Search vs Java Loop”** — Finding bytes in 500MB with a 40x speed gap.
3.  **“Real-Time XOR Visualizer”** — XORing 4K image frames at ultra-high FPS.

---

## Technical Examples
See the `examples/` directory for technical implementations:
-   [CopyDemo.java](examples/src/main/java/fastbytes/CopyDemo.java)
-   [FillDemo.java](examples/src/main/java/fastbytes/FillDemo.java)
-   [SearchDemo.java](examples/src/main/java/fastbytes/SearchDemo.java)
-   [XorImageDemo.java](examples/src/main/java/fastbytes/XorImageDemo.java)
-   [HashDemo.java](examples/src/main/java/fastbytes/HashDemo.java)

---

## Documentation
*   **[REFERENCE.md](REFERENCE.md)**: Full technical specification, CPU fallback rules, and JNI contracts.
*   **[PHILOSOPHIE.md](philosophie.md)**: The "Native-First" philosophy behind the FastJava ecosystem.
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
