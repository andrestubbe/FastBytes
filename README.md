# FastBytes — High-performance SIMD Byte Operations for Java v0.1.0

**Ultra-fast native byte array and buffer operations for the FastJava ecosystem. Optimized for raw throughput via AVX2/SSE.**

[![Build](https://img.shields.io/github/actions/workflow/status/andrestubbe/FastBytes/maven.yml?branch=main)](https://github.com/andrestubbe/FastBytes/actions)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![JitPack](https://jitpack.io/v/andrestubbe/FastBytes.svg)](https://jitpack.io/#andrestubbe/FastBytes)

---

**FastBytes** provides a set of native primitives to manipulate memory at the hardware limit. It is a fundamental dependency for the FastJava ecosystem, enabling zero-copy data pipelines.

```java
// Quick Start — Example
import fastbytes.FastBytes;
import java.nio.ByteBuffer;

public class Demo {
    public static void main(String[] args) {
        ByteBuffer buffer = ByteBuffer.allocateDirect(1024 * 1024);
        // Native SIMD-accelerated memory fill
        FastBytes.fill(buffer, (byte) 0);
        System.out.println("1MB buffer cleared natively!");
    }
}
```

---

## Table of Contents
- [Features](#features)
- [Performance](#performance)
- [Installation](#installation)
- [License](#license)
- [Related Projects](#related-projects)

---

## Features
- **⚡ SIMD Primitives**: Native implementation of `fill`, `copy`, `search`, and `compare`.
- **📦 Zero-Allocation**: Operate directly on native memory (DirectBuffers).
- **🚀 Cache-Friendly**: Optimized for modern CPU architectures (AVX2/SSE4.2).
- **🖇️ Ecosystem Foundation**: Powering FastString and FastJSON.

---

## Installation

FastBytes requires **FastCore** (the unified native loader) to function.

### Maven (JitPack)
```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <!-- FastBytes Module -->
    <dependency>
        <groupId>io.github.andrestubbe</groupId>
        <artifactId>fastbytes</artifactId>
        <version>0.1.0</version>
    </dependency>

    <!-- FastCore (Mandatory Native Loader) -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastcore</artifactId>
        <version>0.1.0</version>
    </dependency>
</dependencies>
```

### Gradle (JitPack)
```groovy
repositories {
    maven { url 'https://jitpack.io' }
}

dependencies {
    implementation 'io.github.andrestubbe:fastbytes:0.1.0'
    implementation 'com.github.andrestubbe:fastcore:0.1.0'
}
```

---

## License
MIT License — See [LICENSE](LICENSE) for details.

---

## Related Projects
- [FastCore](https://github.com/andrestubbe/FastCore) — Native Library Loader
- [FastString](https://github.com/andrestubbe/FastString) — SIMD-accelerated Strings
- [FastJSON](https://github.com/andrestubbe/FastJSON) — High-speed JSON parsing

---
**Made with ⚡ by Andre Stubbe**
