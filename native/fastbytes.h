#ifndef FASTBYTES_H
#define FASTBYTES_H

#include <jni.h>
#include <cstdint>
#include <cstddef>

// Version info
#define FASTBYTES_VERSION_MAJOR 1
#define FASTBYTES_VERSION_MINOR 0
#define FASTBYTES_VERSION_PATCH 0

namespace fastbytes {

/**
 * FastBytes C++ implementation class.
 * SIMD-accelerated byte array operations.
 */
class FastBytes {
public:
    // Constructors
    FastBytes(int initialCapacity = 256);
    FastBytes(const uint8_t* data, size_t length);
    FastBytes(const FastBytes& other);
    FastBytes(FastBytes&& other) noexcept;
    ~FastBytes();
    
    // Buffer management
    size_t capacity() const;
    size_t size() const;
    void resize(size_t newCapacity);
    void append(const uint8_t* data, size_t length);
    
    // Data access
    uint8_t* data();
    const uint8_t* data() const;
    uint8_t* toArray() const;
    
private:
    uint8_t* buffer;
    size_t bufCapacity;
    size_t dataLength;
    
    void grow(size_t minCapacity);
};

// ==================== SIMD OPERATIONS ====================

/**
 * SIMD-accelerated memory copy.
 * Uses AVX2 (32 bytes) or SSE4.2 (16 bytes) when available.
 */
void copyFast(const uint8_t* src, uint8_t* dest, size_t length);

/**
 * SIMD-accelerated fill.
 * Broadcasts value to all bytes using vector operations.
 */
void fillFast(uint8_t* dest, uint8_t value, size_t length);

/**
 * SIMD-accelerated byte comparison.
 * Returns: negative if a<b, 0 if equal, positive if a>b
 */
int compareFast(const uint8_t* a, const uint8_t* b, size_t length);

/**
 * SIMD-accelerated equality check.
 * Early exit on first mismatch.
 */
bool equalsFast(const uint8_t* a, const uint8_t* b, size_t length);

/**
 * SIMD-accelerated find first byte.
 * Returns index or -1 if not found.
 */
int indexOfFast(const uint8_t* data, size_t length, uint8_t value);

/**
 * SIMD-accelerated find last byte.
 */
int lastIndexOfFast(const uint8_t* data, size_t length, uint8_t value);

/**
 * SIMD-accelerated byte count.
 */
size_t countFast(const uint8_t* data, size_t length, uint8_t value);

// ==================== HASH FUNCTIONS ====================

/**
 * FNV-1a 32-bit hash - SIMD accelerated for large inputs.
 */
uint32_t hashFNV1a(const uint8_t* data, size_t length);

/**
 * xxHash32 - SIMD accelerated.
 */
uint32_t hashXXH32(const uint8_t* data, size_t length, uint32_t seed);

// ==================== UTILITY ====================

/**
 * Reverse bytes in-place.
 */
void reverseFast(uint8_t* data, size_t length);

/**
 * Swap byte order (endianness conversion).
 * groupSize: 2 (16-bit), 4 (32-bit), or 8 (64-bit)
 */
void swapBytesFast(uint8_t* data, size_t length, int groupSize);

/**
 * XOR two byte arrays.
 */
void xorFast(const uint8_t* a, const uint8_t* b, uint8_t* out, size_t length);

/**
 * Secure zero (prevents compiler optimization).
 */
void secureZeroFast(uint8_t* data, size_t length);

// ==================== CPU FEATURE DETECTION ====================

bool hasSSE42();
bool hasAVX2();
bool hasAVX512();

} // namespace fastbytes

#endif // FASTBYTES_H
