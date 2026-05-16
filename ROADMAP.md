# FastBytes Roadmap 🗺️

**Vision:** To provide the fastest possible memory and byte manipulation primitives for the JVM by aggressively bypassing memory-bound bottlenecks.

## 🟢 v0.2.0: The "Memory Wall" Update
- [x] **Non-Temporal Stores**: Bypass CPU cache for 1GB+ copy/fill operations to avoid pollution.
- [x] **Software Prefetching**: Force-loading cachelines to hide RAM latency during searches.
- [ ] **32-Byte Alignment Enforcement**: Native `malloc` alignment for zero-penalty SIMD access.
- [ ] **Branchless Logic**: Remove remaining mispredict-prone branches in utility loops.

## 🟡 v0.5.0: The "Hashing & Patterns" Update
- [ ] **SIMD Hashing (XXH3)**: Replace scalar FNV1a with a vector-parallelized hash engine.
- [ ] **Multi-Pattern Search**: SIMD-accelerated Aho-Corasick or Two-Way search.
- [ ] **ARM NEON Port**: Native parity for Apple Silicon and Graviton processors.

## 🔴 v1.0.0: The "Enterprise" Update
- [ ] **AVX-512 Fallbacks**: Leverage 512-bit registers for ultra-high throughput scanning.
- [ ] **HugePages Support**: VirtualAlloc with `MEM_LARGE_PAGES` for TLB miss elimination.
- [ ] **NUMA-Aware Parallelism**: Multi-threaded primitives for large-scale server workloads.

---
**Focus:** Search is our USP. We optimize for throughput where Java stops.
