# FastBytes Roadmap 🗺️

**Vision:** To provide the fastest possible memory and byte manipulation primitives for the JVM on all major architectures.

## 🟢 Short-term (v0.2.x)
- [ ] **AVX-512 Support**: Leverage ultra-wide registers for 2x throughput on modern Intel/AMD CPUs.
- [ ] **ARM NEON Optimization**: Native parity for Apple Silicon and Graviton processors.
- [ ] **Expanded Search API**: Native `indexOf` for multi-byte patterns.

## 🟡 Mid-term (v0.5.x)
- [ ] **Native Memory Pooling**: Custom allocator to bypass `malloc`/`free` overhead.
- [ ] **Zero-Copy Serialization Helpers**: Direct support for FlatBuffers/Cap'n Proto structures.
- [ ] **Bit Manipulation Layer**: SIMD-accelerated bitsets and bit-counting.

## 🔴 Long-term (v1.0.x)
- [ ] **Multi-Platform Native Loader**: Beyond Windows — Linux and macOS stability.
- [ ] **Automated Micro-benchmarking CI**: Per-commit performance regression testing.

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.*
