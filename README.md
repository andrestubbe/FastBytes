# FastBytes ⚡

**FastBytes** is a high-performance SIMD-powered byte manipulation engine for the JVM. 
It provides native-first primitives (AVX2/SSE4.2) that bypass standard Java overhead, offering 2-50x speedups for bulk data operations.

## 🚀 Features
-   **SIMD Copy**: Up to 10x faster than `System.arraycopy` for large blocks.
-   **Vector Search**: Scans 32-64 bytes per cycle using hardware intrinsics.
-   **Blazing Fill**: Fills gigabytes of memory at hardware-limit speeds.
-   **Native XOR**: Optimized for cryptographic and visual processing.
-   **Fast Hashing**: SIMD-accelerated FNV-1a and xxHash32.

---

## 🎥 YouTube Hero Demos
These demos showcase FastBytes in action. See `examples/Demo` for source.

1.  **“Java vs FastBytes: 1GB Copy Race”** — SIMD vs `System.arraycopy` throughput.
2.  **“SIMD Search vs Java Loop”** — Finding bytes in 500MB with a 40x speed gap.
3.  **“Real-Time XOR Visualizer”** — XORing 4K image frames at ultra-high FPS.

---

## 🛠️ Quickstart

### Installation (JitPack)
Add the JitPack repository and both dependencies to your `pom.xml`:

```xml
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>

<dependencies>
    <!-- 1. FastBytes (SIMD Primitives) -->
    <dependency>
        <groupId>io.github.andrestubbe</groupId>
        <artifactId>fastbytes</artifactId>
        <version>v0.1.0</version>
    </dependency>

    <!-- 2. FastCore (Required Native Loader) -->
    <dependency>
        <groupId>com.github.andrestubbe</groupId>
        <artifactId>fastcore</artifactId>
        <version>v0.1.0</version>
    </dependency>
</dependencies>
```

### Basic Usage
```java
// Fast SIMD Copy
FastBytes.copy(src, 0, dest, 0, length);

// High-Speed Search
int index = FastBytes.indexOf(data, (byte) 0xEE);

// SIMD XOR
FastBytes.xor(a, b, out);
```

---

## 📖 Documentation
*   **[REFERENCE.md](REFERENCE.md)**: Full technical specification, CPU fallback rules, and JNI contracts.
*   **[PHILOSOPHIE.md](philosophie.md)**: The "Native-First" philosophy behind the FastJava ecosystem.
*   **[CHANGELOG.md](CHANGELOG.md)**: Project history.

## 💻 Code Examples
See the `examples/` directory for technical implementations:
-   [CopyDemo.java](examples/src/main/java/fastbytes/CopyDemo.java)
-   [FillDemo.java](examples/src/main/java/fastbytes/FillDemo.java)
-   [SearchDemo.java](examples/src/main/java/fastbytes/SearchDemo.java)
-   [XorImageDemo.java](examples/src/main/java/fastbytes/XorImageDemo.java)
-   [HashDemo.java](examples/src/main/java/fastbytes/HashDemo.java)

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.*
