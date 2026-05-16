# FastBytes Roadmap 🗺️

**Vision:** To provide the fastest possible memory and byte manipulation primitives for the JVM by aggressively bypassing memory-bound bottlenecks.

## 🟢 v0.1.0: The "Nitro" Release (Current)
- [x] **AVX-512BW Support**: Native 64-byte vector processing for XOR and Search.
- [x] **64-Byte Unrolling**: Double-width loops across all core primitives.
- [x] **Software Prefetching**: Aggressive 256B/512B cache-ahead loading.
- [x] **Zero-Copy JNI**: Critical Path memory pinning via `GetPrimitiveArrayCritical`.
- [x] **3-Way Search**: Intelligent switching between Scalar, Classic SIMD, and Pro SIMD.

## 🟡 v0.2.0: The "Kernel & Alignment" Update
- [ ] **Kernel Fastpaths**: Use `CopyFile2` or `sendfile` style DMA for bulk file copying.
- [ ] **3-Way Fill**: Strategy switching between `rep stosq`, AVX2, and Streaming Stores.
- [ ] **Alignment Enforcement**: Internal memory alignment checks for 64-byte boundaries.
- [ ] **Non-Temporal Stores**: Opt-in streaming stores to prevent cache pollution on huge buffers.

## 🟠 v0.5.0: The "Hashing & Patterns" Update
- [ ] **SIMD Hashing (XXH3)**: Replace current XXH32 with a vector-parallelized XXH3 engine.
- [ ] **Multi-Pattern Search**: SIMD-accelerated Aho-Corasick or Two-Way search.
- [ ] **ARM NEON Port**: Native parity for Apple Silicon and Graviton processors.

## 🔴 v1.0.0: The "Enterprise" Update
- [ ] **HugePages Support**: VirtualAlloc with `MEM_LARGE_PAGES` for TLB miss elimination.
- [ ] **NUMA-Aware Parallelism**: Multi-threaded primitives for large-scale server workloads.
- [ ] **Auto-Tuning Engine**: Dynamic prefetch distance adjustment based on CPU cache size.

---
**Focus:** Search is our USP. We optimize for throughput where Java stops.
