# FastBytes Reference

## 1. CPU Feature Model
*   **SSE4.2** — detected via CPUID (ECX bit 20). Enables 16-byte vector ops.
*   **AVX2** — detected via CPUID leaf 7 (EBX bit 5). Enables 32-byte vector ops.
*   **AVX-512** — detected via CPUID leaf 7 (EBX bits 16, 17, 30). *Not yet used.*
*   **Fallback rule**: AVX2 → SSE4.2 → scalar.

## 2. Guarantees
*   All operations are safe on **unaligned memory**.
*   All static methods are **thread-safe**.
*   Instance methods require external synchronization.
*   JNI always copies back modified arrays unless explicitly marked `JNI_ABORT`.

## 3. Supported Operations

### Copy
`copy(byte[] src, int srcPos, byte[] dest, int destPos, int length)`
*   **AVX2**: 32-byte loads/stores.
*   **SSE4.2**: 16-byte loads/stores.
*   **Scalar tail** for remaining bytes.

### Fill
`fill(byte[] array, byte value)`
*   Broadcast via `_mm256_set1_epi8` or `_mm_set1_epi8`.
*   Scalar tail.

### Compare / Equals
`compare(byte[] a, byte[] b)` / `equals(byte[] a, byte[] b)`
*   Vector compare, early exit on mismatch.

### Search
`indexOf(byte[] array, byte value)`
*   Vector compare → `movemask` → `ctz`.
*   Scalar fallback.

### Count
`count(byte[] array, byte value)`
*   AVX2 horizontal sum via `_mm256_sad_epu8`.

### Hash
`hashFNV1a(byte[] data)` — scalar FNV-1a.
`hashXXH32(byte[] data, int seed)` — simplified scalar xxHash32.

### XOR
`xor(byte[] a, byte[] b, byte[] out)`
*   Vector XOR, scalar tail.

### Reverse / Swap
`reverse(byte[] array)` — scalar swap.
`swapBytes(byte[] array, int groupSize)` — scalar group reversal.

## 4. JNI Contracts
*   `GetByteArrayElements` is used for all array access.
*   Inputs are never modified unless explicitly documented.
*   Outputs are written back with mode 0.
*   Critical sections (`toArrayFast`) must not block.

## 5. Native Buffer Semantics
*   `nativeCreate(capacity)` allocates a growable buffer.
*   `nativeFromBytes(data)` copies Java bytes into native memory.
*   `resize(newCapacity)` only grows.
*   `append(data)` auto-grows.
*   `toArray()` returns a fresh Java copy.

## 6. Error Handling
*   Null arrays → no-op.
*   Negative indices/lengths → Java throws before JNI.
*   After `close()`, all native calls throw `IllegalStateException`.

## 7. Fallback Matrix

| Feature | AVX2 | SSE4.2 | Scalar |
| :--- | :---: | :---: | :---: |
| **copy** | ✓ | ✓ | ✓ |
| **fill** | ✓ | ✓ | ✓ |
| **compare** | ✓ | partial | ✓ |
| **indexOf** | ✓ | ✓ | ✓ |
| **count** | ✓ | – | ✓ |
| **xor** | ✓ | ✓ | ✓ |

## 8. Stability
*   All listed operations are stable.
*   Hash implementations may be replaced with SIMD versions in the future.
*   AVX-512 and NEON paths are planned but not guaranteed yet.

---
**Part of the FastJava Ecosystem** — *Making the JVM faster.*
