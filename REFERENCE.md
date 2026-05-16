# FastBytes Reference

## 1. CPU Feature Model
*   **AVX-512BW** — detected via CPUID leaf 7. Primary path for **XOR** and **Search** (64-byte vectors).
*   **AVX2** — detected via CPUID leaf 7 (EBX bit 5). Enables 32-byte vector ops with 64-byte unrolling.
*   **SSE4.2** — detected via CPUID (ECX bit 20). 16-byte fallback.
*   **Fallback rule**: AVX-512 → AVX2 → SSE4.2 → scalar.

## 2. Guarantees
*   **Zero-Copy**: All operations use `GetPrimitiveArrayCritical` for direct memory access.
*   **Unaligned Access**: Safe on all byte boundaries (uses `loadu`/`storeu`).
*   **Thread-Safety**: All static native methods are thread-safe.
*   **Deterministic Latency**: Hand-tuned intrinsics eliminate JIT-related variance.

## 3. Supported Operations

### Copy
`copy(byte[] src, int srcPos, byte[] dest, int destPos, int length)`
*   **Pro**: 64-byte unrolled loop with 256B software prefetching.
*   **Legacy**: Standard 32-byte AVX2 loop.

### Fill
`fill(byte[] array, byte value)`
*   **Pro**: 64-byte unrolled stores.
*   **Legacy**: Standard 32-byte stores.

### Compare
`compare(byte[] a, byte[] b)`
*   **Optimized**: 32-byte vector comparison with hardware-accelerated mismatch detection via `_BitScanForward`.

### Search
`indexOf(byte[] array, byte value)`
*   **Pro**: 64-byte unrolled scanner with AVX-512 support. Uses `movemask` and bit-scanning for index recovery.
*   **Legacy**: Standard 32-byte scanner.

### XOR
`xor(byte[] a, byte[] b, byte[] out)`
*   **Extreme**: AVX-512BW path processing **128 bytes per iteration** (unrolled).
*   **Fallback**: AVX2 64-byte unrolled.

### Hash
`hashXXH32(byte[] data, int seed)`
*   High-performance scalar implementation of xxHash32.

### Reverse
`reverse(byte[] array)`
*   In-place scalar reversal.

## 4. JNI & Memory Contracts
*   **Direct Memory Pinning**: No implicit copies are made by the JNI bridge.
*   **Short Duration**: Native calls are optimized for sub-millisecond execution to minimize GC impact.
*   **No Allocation**: All operations work on pre-allocated Java arrays or buffers.

## 5. Platform Support
| Feature | AVX-512 | AVX2 | SSE4.2 |
| :--- | :---: | :---: | :---: |
| **XOR** | ✓ | ✓ | ✓ |
| **Search** | ✓ | ✓ | ✓ |
| **Copy** | – | ✓ | ✓ |
| **Compare** | – | ✓ | ✓ |

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.*
