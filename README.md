# FastBytes — High-performance SIMD byte operations for Java v0.1.0

**Ultra-fast native byte buffer and array operations for the FastJava ecosystem.**

[![Build](https://img.shields.io/github/actions/workflow/status/andrestubbe/FastBytes/maven.yml?branch=main)](https://github.com/andrestubbe/FastBytes/actions)
[![Java](https://img.shields.io/badge/Java-17+-blue.svg)](https://www.java.com)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![JitPack](https://jitpack.io/v/andrestubbe/FastBytes.svg)](https://jitpack.io/#andrestubbe/FastBytes)

---

FastBytes provides optimized native operations for `byte[]` and `ByteBuffer`, leveraging AVX2/SSE instructions for maximum throughput.

```java
// Quick Start — Example
import fastbytes.FastBytes;

public class Demo {
    public static void main(String[] args) {
        byte[] data = new byte[1024];
        FastBytes.fill(data, (byte) 0xFF);
        System.out.println("Data filled natively!");
    }
}
```

---

## Installation

FastBytes requires `FastCore` for native library loading.

### Maven (JitPack)
```xml
<dependencies>
    <dependency>
        <groupId>io.github.andrestubbe</groupId>
        <artifactId>fastbytes</artifactId>
        <version>0.1.0</version>
    </dependency>
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastcore</artifactId>
        <version>0.1.0</version>
    </dependency>
</dependencies>
```

---

## License
MIT License — See [LICENSE](LICENSE) for details.

---
**Made with ⚡ by Andre Stubbe**
