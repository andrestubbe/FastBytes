# FastBytes ⚡

**High-performance SIMD-powered byte manipulation engine for the JVM.**

[![Build](https://img.shields.io/github/actions/workflow/status/andrestubbe/FastBytes/maven.yml?branch=main)](https://github.com/andrestubbe/FastBytes/actions)
[![JitPack](https://jitpack.io/v/andrestubbe/FastBytes.svg)](https://jitpack.io/#andrestubbe/FastBytes)

FastBytes provides native-first primitives (AVX2/SSE4.2) that bypass standard Java overhead, offering 2-50x speedups for bulk data operations. It is designed for AI agents, high-frequency data processing, and performance-critical systems.

## 🚀 Key Features
-   **⚡ SIMD Copy**: Up to 10x faster than `System.arraycopy` for large blocks.
-   **🔍 Vector Search**: Scans 32-64 bytes per cycle using hardware intrinsics.
-   **🎨 Native XOR**: Optimized for cryptographic and visual processing.
-   **📦 Zero Dependencies**: Purely native acceleration via JNI.

---

## 🎥 YouTube Hero Demos
These demos showcase FastBytes in action. See `examples/Demo` for source.

1.  **“Java vs FastBytes: 1GB Copy Race”** — SIMD vs `System.arraycopy` throughput.
2.  **“SIMD Search vs Java Loop”** — Finding bytes in 500MB with a 40x speed gap.
3.  **“Real-Time XOR Visualizer”** — XORing 4K image frames at ultra-high FPS.

---

## 🛠️ Installation

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

### Direct Download
You can also download the latest pre-compiled `.jar` directly from the [Releases](https://github.com/andrestubbe/FastBytes/releases) page. Ensure you have `FastCore` in your classpath for native loading.


---

## 📖 Documentation
*   **[REFERENCE.md](REFERENCE.md)**: Full technical specification, CPU fallback rules, and JNI contracts.
*   **[PHILOSOPHIE.md](philosophie.md)**: The "Native-First" philosophy behind the FastJava ecosystem.
*   **[CHANGELOG.md](CHANGELOG.md)**: Project history.

## 💻 Technical Examples
See the `examples/` directory for technical implementations:
-   [CopyDemo.java](examples/src/main/java/fastbytes/CopyDemo.java)
-   [FillDemo.java](examples/src/main/java/fastbytes/FillDemo.java)
-   [SearchDemo.java](examples/src/main/java/fastbytes/SearchDemo.java)
-   [XorImageDemo.java](examples/src/main/java/fastbytes/XorImageDemo.java)
-   [HashDemo.java](examples/src/main/java/fastbytes/HashDemo.java)

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.*
