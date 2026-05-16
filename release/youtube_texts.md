# YouTube Release Texts — FastBytes Hero Demos

---

## 🏎️ Demo 1: The 1GB Copy Race (Java vs FastBytes)

**Title:** Java vs FastBytes: Can we beat System.arraycopy? 🚀
**Description:**
In this video, we put the standard JVM `System.arraycopy` against the FastBytes SIMD-accelerated copy engine. 
Watch as we move 1GB of data across memory blocks using AVX2 and SSE4.2 instructions.

**Results:**
- Standard Java: [Insert Time] ms
- FastBytes SIMD: [Insert Time] ms
- **Speedup:** [Insert X]x faster

**Source Code:** `examples/src/main/java/fastbytes/Demo.java`
**GitHub:** https://github.com/andrestubbe/FastBytes

#Java #Performance #SIMD #Programming #FastJava

---

## 🔍 Demo 2: SIMD Search Race (Linear vs Vector)

**Title:** 40x Faster Searching in Java? SIMD indexOf vs For-Loop ⚡
**Description:**
Is a standard for-loop enough? For large data buffers, hardware-accelerated vector searching can process 32 bytes in a single CPU cycle. 
We search for a specific byte in a 500MB buffer and compare the results.

**Results:**
- Standard For-Loop: [Insert Time] ms
- FastBytes Vector Search: [Insert Time] ms
- **The Gap:** SIMD wins by [Insert Time] ms!

**Source Code:** `examples/src/main/java/fastbytes/Demo2.java`
**GitHub:** https://github.com/andrestubbe/FastBytes

#Coding #Algorithm #SIMD #JavaPerformance #LowLatency

---

## 🎨 Demo 3: Real-Time XOR Visualizer (Glitch Art)

**Title:** Real-Time 4K Glitch Art with SIMD XOR ⚡🎨
**Description:**
XORing large images is computationally expensive, but with AVX2, we can process millions of pixels in microseconds. 
This demo shows two image buffers being XORed in real-time to create a visual glitch-art effect, proving the throughput of FastBytes for media processing.

**Key Highlight:**
- Processing 1920x1080 4K frames at ultra-high FPS.
- Minimal CPU overhead thanks to native vectorization.

**Source Code:** `examples/src/main/java/fastbytes/Demo3.java`
**GitHub:** https://github.com/andrestubbe/FastBytes

#CreativeCoding #SIMD #Java #VisualArt #GraphicsProgramming
