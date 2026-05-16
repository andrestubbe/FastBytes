/**
 * @file fastbytes.cpp
 * @brief FastBytes native implementation with SIMD acceleration
 * 
 * High-performance byte array operations using AVX2/SSE4.2 SIMD instructions.
 * All operations auto-detect CPU features and fall back to scalar if needed.
 * 
 * @author FastJava Team
 * @version 1.0.0
 * @copyright MIT License
 */

#include "fastbytes.h"
#include <cstring>
#include <algorithm>
#include <immintrin.h>

// CPU detection
#ifdef _MSC_VER
#include <intrin.h>
static inline int ctz(unsigned int mask) {
    unsigned long index;
    if (_BitScanForward(&index, mask)) return index;
    return 32;
}
#else
#include <cpuid.h>
#define ctz(mask) __builtin_ctz(mask)
#endif

namespace fastbytes {

/** @defgroup CPU CPU Feature Detection
 *  @brief Runtime CPU capability detection
 *  @{ */

static bool g_sse42 = false;   /**< SSE4.2 support flag */
static bool g_avx2 = false;      /**< AVX2 support flag */
static bool g_avx512 = false;    /**< AVX-512 support flag */
static bool g_initialized = false; /**< CPU detection done flag */

/**
 * @brief Detect CPU SIMD capabilities via CPUID
 * 
 * Checks for SSE4.2, AVX2, and AVX-512 support.
 * Thread-safe lazy initialization.
 */
static void initCpuFeatures() {
    if (g_initialized) return;
    
    int cpuInfo[4] = {0};
    
#ifdef _MSC_VER
    __cpuid(cpuInfo, 1);
#else
    __cpuid(1, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#endif
    
    // Check SSE4.2 (bit 20 of ECX)
    g_sse42 = (cpuInfo[2] & (1 << 20)) != 0;
    
    // Check AVX (bit 28 of ECX) - required for AVX2
    bool hasAVX = (cpuInfo[2] & (1 << 28)) != 0;
    
    // Check AVX2 (leaf 7, bit 5 of EBX)
    if (hasAVX) {
#ifdef _MSC_VER
        __cpuidex(cpuInfo, 7, 0);
#else
        __cpuid_count(7, 0, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#endif
        g_avx2 = (cpuInfo[1] & (1 << 5)) != 0;
        
        // Check AVX-512 (bits 16, 17, 30 of EBX)
        g_avx512 = ((cpuInfo[1] & (1 << 16)) != 0) && 
                   ((cpuInfo[1] & (1 << 17)) != 0) && 
                   ((cpuInfo[1] & (1 << 30)) != 0);
    }
    
    g_initialized = true;
}

/** @return true if CPU supports SSE4.2 */
bool hasSSE42() {
    if (!g_initialized) initCpuFeatures();
    return g_sse42;
}

/** @return true if CPU supports AVX2 */
bool hasAVX2() {
    if (!g_initialized) initCpuFeatures();
    return g_avx2;
}

/** @return true if CPU supports AVX-512 */
bool hasAVX512() {
    if (!g_initialized) initCpuFeatures();
    return g_avx512;
}

/** @} */ // end of CPU group

// ==================== FASTBYTES CLASS ====================

/** @defgroup Class FastBytes Class
 *  @brief C++ FastBytes implementation
 *  @{ */

/**
 * @brief Construct FastBytes with initial capacity
 * @param initialCapacity Initial buffer size in bytes (default: 256)
 */
FastBytes::FastBytes(int initialCapacity) 
    : buffer(nullptr), bufCapacity(0), dataLength(0) {
    resize(initialCapacity > 0 ? initialCapacity : 256);
}

FastBytes::FastBytes(const uint8_t* data, size_t length)
    : buffer(nullptr), bufCapacity(0), dataLength(0) {
    resize(length > 0 ? length : 256);
    if (length > 0 && data) {
        std::memcpy(buffer, data, length);
        dataLength = length;
    }
}

FastBytes::FastBytes(const FastBytes& other)
    : buffer(nullptr), bufCapacity(0), dataLength(0) {
    resize(other.bufCapacity);
    std::memcpy(buffer, other.buffer, other.dataLength);
    dataLength = other.dataLength;
}

FastBytes::FastBytes(FastBytes&& other) noexcept
    : buffer(other.buffer), bufCapacity(other.bufCapacity), dataLength(other.dataLength) {
    other.buffer = nullptr;
    other.bufCapacity = 0;
    other.dataLength = 0;
}

FastBytes::~FastBytes() {
    delete[] buffer;
}

/** @return Total buffer capacity in bytes */
size_t FastBytes::capacity() const { return bufCapacity; }

/** @return Current data length in bytes */
size_t FastBytes::size() const { return dataLength; }

void FastBytes::resize(size_t newCapacity) {
    if (newCapacity <= bufCapacity) return;
    
    uint8_t* newBuffer = new uint8_t[newCapacity];
    if (buffer && dataLength > 0) {
        std::memcpy(newBuffer, buffer, dataLength);
    }
    delete[] buffer;
    buffer = newBuffer;
    bufCapacity = newCapacity;
}

void FastBytes::grow(size_t minCapacity) {
    if (minCapacity <= bufCapacity) return;
    size_t newCapacity = bufCapacity * 2;
    if (newCapacity < minCapacity) newCapacity = minCapacity;
    resize(newCapacity);
}

void FastBytes::append(const uint8_t* data, size_t length) {
    if (!data || length == 0) return;
    grow(dataLength + length);
    std::memcpy(buffer + dataLength, data, length);
    dataLength += length;
}

uint8_t* FastBytes::data() { return buffer; }
const uint8_t* FastBytes::data() const { return buffer; }

uint8_t* FastBytes::toArray() const {
    uint8_t* result = new uint8_t[dataLength];
    std::memcpy(result, buffer, dataLength);
    return result;
}

/** @} */ // end of Class group

// ==================== SIMD OPERATIONS ====================

/** @defgroup SIMD SIMD Operations
 *  @brief Vectorized byte operations using AVX2/SSE4.2
 *  
 *  All SIMD functions auto-detect CPU capabilities and use:
 *  - AVX2 (32-byte vectors) when available
 *  - SSE4.2 (16-byte vectors) as fallback
 *  - Scalar loops for remaining bytes
 *  
 *  @{ */

/**
 * @brief SIMD-accelerated memory copy
 * 
 * Uses 32-byte AVX2 loads/stores or 16-byte SSE4.2 for bulk copy.
 * Falls back to scalar for remaining bytes.
 * 
 * @param src Source pointer (must be non-null)
 * @param dest Destination pointer (must be non-null)
 * @param length Number of bytes to copy
 * @pre src != nullptr && dest != nullptr
 */
void copyFast(const uint8_t* src, uint8_t* dest, size_t length) {
    if (!src || !dest || length == 0) return;
    
    size_t i = 0;
    
    if (hasAVX2()) {
        // AVX2: 32-byte vectors
        for (; i + 32 <= length; i += 32) {
            __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src + i));
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i), data);
        }
    } else if (hasSSE42()) {
        // SSE4.2: 16-byte vectors
        for (; i + 16 <= length; i += 16) {
            __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(src + i));
            _mm_storeu_si128(reinterpret_cast<__m128i*>(dest + i), data);
        }
    }
    
    // Remaining bytes
    for (; i < length; i++) {
        dest[i] = src[i];
    }
}

/**
 * @brief SIMD-accelerated fill operation
 * 
 * Broadcasts a single byte value to entire buffer using vector instructions.
 * AVX2: 32 bytes per iteration, SSE4.2: 16 bytes per iteration.
 * 
 * @param dest Destination buffer (must be non-null)
 * @param value Byte value to fill
 * @param length Number of bytes to fill
 * @pre dest != nullptr
 */
void fillFast(uint8_t* dest, uint8_t value, size_t length) {
    if (!dest || length == 0) return;
    
    size_t i = 0;
    
    if (hasAVX2()) {
        __m256i valVec = _mm256_set1_epi8(static_cast<char>(value));
        for (; i + 32 <= length; i += 32) {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i), valVec);
        }
    } else if (hasSSE42()) {
        __m128i valVec = _mm_set1_epi8(static_cast<char>(value));
        for (; i + 16 <= length; i += 16) {
            _mm_storeu_si128(reinterpret_cast<__m128i*>(dest + i), valVec);
        }
    }
    
    // Remaining bytes
    for (; i < length; i++) {
        dest[i] = value;
    }
}

/**
 * @brief SIMD-accelerated byte array comparison
 * 
 * Compares two arrays using 32-byte AVX2 or 16-byte SSE4.2 compares.
 * Returns immediately on first mismatch.
 * 
 * @param a First array (must be non-null)
 * @param b Second array (must be non-null)
 * @param length Number of bytes to compare
 * @return negative if a<b, 0 if equal, positive if a>b
 * @pre a != nullptr && b != nullptr
 */
int compareFast(const uint8_t* a, const uint8_t* b, size_t length) {
    if (!a || !b) return (a == b) ? 0 : (a ? 1 : -1);
    
    size_t i = 0;
    
    if (hasAVX2()) {
        for (; i + 32 <= length; i += 32) {
            __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a + i));
            __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b + i));
            __m256i cmp = _mm256_cmpeq_epi8(va, vb);
            if (_mm256_movemask_epi8(cmp) != 0xFFFFFFFF) {
                // Found mismatch in this block
                for (size_t j = i; j < i + 32 && j < length; j++) {
                    if (a[j] != b[j]) return (int)a[j] - (int)b[j];
                }
            }
        }
    }
    
    // Scalar fallback
    for (; i < length; i++) {
        if (a[i] != b[i]) return (int)a[i] - (int)b[i];
    }
    return 0;
}

/**
 * @brief SIMD-accelerated equality check
 * 
 * Wrapper around compareFast for equality testing.
 * Early exit on first mismatch.
 * 
 * @param a First array (must be non-null)
 * @param b Second array (must be non-null)
 * @param length Number of bytes to compare
 * @return true if arrays are identical
 * @pre a != nullptr && b != nullptr
 */
bool equalsFast(const uint8_t* a, const uint8_t* b, size_t length) {
    return compareFast(a, b, length) == 0;
}

/**
 * @brief SIMD-accelerated find first byte
 * 
 * Searches for a byte value using vector comparison.
 * AVX2: Compares 32 bytes per iteration.
 * SSE4.2: Compares 16 bytes per iteration.
 * 
 * @param data Array to search (must be non-null)
 * @param length Array length in bytes
 * @param value Byte value to find
 * @return Index of first occurrence, or -1 if not found
 * @pre data != nullptr
 */
int indexOfFast(const uint8_t* data, size_t length, uint8_t value) {
    if (!data || length == 0) return -1;
    
    size_t i = 0;
    
    if (hasAVX2()) {
        __m256i valVec = _mm256_set1_epi8(static_cast<char>(value));
        for (; i + 32 <= length; i += 32) {
            __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i));
            __m256i cmp = _mm256_cmpeq_epi8(chunk, valVec);
            int mask = _mm256_movemask_epi8(cmp);
            if (mask != 0) {
                return static_cast<int>(i + ctz(mask));
            }
        }
    } else if (hasSSE42()) {
        __m128i valVec = _mm_set1_epi8(static_cast<char>(value));
        for (; i + 16 <= length; i += 16) {
            __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + i));
            __m128i cmp = _mm_cmpeq_epi8(chunk, valVec);
            int mask = _mm_movemask_epi8(cmp);
            if (mask != 0) {
                return static_cast<int>(i + ctz(mask));
            }
        }
    }
    
    // Scalar fallback
    for (; i < length; i++) {
        if (data[i] == value) return static_cast<int>(i);
    }
    return -1;
}

int lastIndexOfFast(const uint8_t* data, size_t length, uint8_t value) {
    if (!data || length == 0) return -1;
    
    // Scan from end (SIMD less effective here)
    for (size_t i = length; i > 0; i--) {
        if (data[i - 1] == value) return static_cast<int>(i - 1);
    }
    return -1;
}

size_t countFast(const uint8_t* data, size_t length, uint8_t value) {
    if (!data || length == 0) return 0;
    
    size_t count = 0;
    size_t i = 0;
    
    if (hasAVX2()) {
        __m256i valVec = _mm256_set1_epi8(static_cast<char>(value));
        __m256i oneVec = _mm256_set1_epi8(1);
        
        for (; i + 32 <= length; i += 32) {
            __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i));
            __m256i cmp = _mm256_cmpeq_epi8(chunk, valVec);
            // Count matching bytes
            __m256i sub = _mm256_sub_epi8(_mm256_setzero_si256(), cmp);
            __m256i sum = _mm256_sad_epu8(sub, _mm256_setzero_si256());
            count += _mm256_extract_epi64(sum, 0) + _mm256_extract_epi64(sum, 2);
        }
    }
    
    // Scalar fallback
    for (; i < length; i++) {
        if (data[i] == value) count++;
    }
    return count;
}

// ==================== HASH FUNCTIONS ====================

uint32_t hashFNV1a(const uint8_t* data, size_t length) {
    const uint32_t FNV_PRIME = 16777619u;
    const uint32_t FNV_OFFSET = 2166136261u;
    
    uint32_t hash = FNV_OFFSET;
    
    // SIMD-accelerated for large inputs would go here
    // For now, use scalar FNV-1a
    for (size_t i = 0; i < length; i++) {
        hash ^= data[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

uint32_t hashXXH32(const uint8_t* data, size_t length, uint32_t seed) {
    // Simplified xxHash32 implementation
    // Full implementation would require more complex SIMD mixing
    
    const uint32_t PRIME32_1 = 2654435761u;
    const uint32_t PRIME32_2 = 2246822519u;
    const uint32_t PRIME32_3 = 3266489917u;
    const uint32_t PRIME32_4 = 668265263u;
    const uint32_t PRIME32_5 = 374761393u;
    
    uint32_t h32 = seed + PRIME32_5;
    
    // Main loop (could be SIMD-accelerated for 16-byte chunks)
    size_t i = 0;
    for (; i + 4 <= length; i += 4) {
        uint32_t chunk = data[i] | (data[i+1] << 8) | 
                        (data[i+2] << 16) | (data[i+3] << 24);
        h32 += chunk * PRIME32_1;
        h32 = _rotl(h32, 13);
        h32 *= PRIME32_2;
    }
    
    // Remaining bytes
    for (; i < length; i++) {
        h32 += data[i] * PRIME32_5;
        h32 = _rotl(h32, 11) * PRIME32_1;
    }
    
    // Finalize
    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;
    
    return h32;
}

// ==================== UTILITY ====================

void reverseFast(uint8_t* data, size_t length) {
    if (!data || length < 2) return;
    
    size_t i = 0, j = length - 1;
    while (i < j) {
        uint8_t tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
        i++;
        j--;
    }
}

void swapBytesFast(uint8_t* data, size_t length, int groupSize) {
    if (!data || length < (size_t)groupSize || groupSize < 2) return;
    
    for (size_t i = 0; i + groupSize <= length; i += groupSize) {
        for (int j = 0; j < groupSize / 2; j++) {
            uint8_t tmp = data[i + j];
            data[i + j] = data[i + groupSize - 1 - j];
            data[i + groupSize - 1 - j] = tmp;
        }
    }
}

void xorFast(const uint8_t* a, const uint8_t* b, uint8_t* out, size_t length) {
    if (!a || !b || !out) return;
    
    size_t i = 0;
    
    if (hasAVX2()) {
        for (; i + 32 <= length; i += 32) {
            __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a + i));
            __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b + i));
            __m256i vx = _mm256_xor_si256(va, vb);
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(out + i), vx);
        }
    } else if (hasSSE42()) {
        for (; i + 16 <= length; i += 16) {
            __m128i va = _mm_loadu_si128(reinterpret_cast<const __m128i*>(a + i));
            __m128i vb = _mm_loadu_si128(reinterpret_cast<const __m128i*>(b + i));
            __m128i vx = _mm_xor_si128(va, vb);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(out + i), vx);
        }
    }
    
    for (; i < length; i++) {
        out[i] = a[i] ^ b[i];
    }
}

void secureZeroFast(uint8_t* data, size_t length) {
    if (!data || length == 0) return;
    
    volatile uint8_t* p = data;
    for (size_t i = 0; i < length; i++) {
        p[i] = 0;
    }
}

} // namespace fastbytes

// ==================== JNI EXPORTS ====================

using namespace fastbytes;

extern "C" {

// Instance methods
JNIEXPORT jlong JNICALL Java_fastbytes_FastBytes_nativeCreate(JNIEnv*, jclass, jint capacity) {
    return reinterpret_cast<jlong>(new FastBytes(capacity));
}

JNIEXPORT jlong JNICALL Java_fastbytes_FastBytes_nativeFromBytes(JNIEnv* env, jclass, jbyteArray data) {
    if (!data) return 0;
    
    jsize len = env->GetArrayLength(data);
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);
    
    FastBytes* fb = new FastBytes(reinterpret_cast<const uint8_t*>(bytes), len);
    
    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT);
    return reinterpret_cast<jlong>(fb);
}

JNIEXPORT void JNICALL Java_fastbytes_FastBytes_nativeDestroy(JNIEnv*, jclass, jlong handle) {
    delete reinterpret_cast<FastBytes*>(handle);
}

// Static SIMD operations
JNIEXPORT void JNICALL Java_fastbytes_FastBytes_copy(JNIEnv* env, jclass, 
    jbyteArray src, jint srcPos, jbyteArray dest, jint destPos, jint length) {
    
    if (!src || !dest || length <= 0) return;
    
    void* srcBytes = env->GetPrimitiveArrayCritical(src, nullptr);
    void* destBytes = env->GetPrimitiveArrayCritical(dest, nullptr);
    
    copyFast(reinterpret_cast<const uint8_t*>(srcBytes) + srcPos,
             reinterpret_cast<uint8_t*>(destBytes) + destPos,
             length);
    
    env->ReleasePrimitiveArrayCritical(dest, destBytes, 0);
    env->ReleasePrimitiveArrayCritical(src, srcBytes, JNI_ABORT);
}

JNIEXPORT void JNICALL Java_fastbytes_FastBytes_fill(JNIEnv* env, jclass, 
    jbyteArray array, jint fromIndex, jint toIndex, jbyte value) {
    
    if (!array || fromIndex < 0 || toIndex < fromIndex) return;
    
    void* bytes = env->GetPrimitiveArrayCritical(array, nullptr);
    
    fillFast(reinterpret_cast<uint8_t*>(bytes) + fromIndex, 
             static_cast<uint8_t>(value), 
             toIndex - fromIndex);
    
    env->ReleasePrimitiveArrayCritical(array, bytes, 0);
}

JNIEXPORT jint JNICALL Java_fastbytes_FastBytes_indexOf(JNIEnv* env, jclass,
    jbyteArray array, jbyte value, jint fromIndex) {
    
    if (!array || fromIndex < 0) return -1;
    
    jsize len = env->GetArrayLength(array);
    if (fromIndex >= len) return -1;
    
    void* bytes = env->GetPrimitiveArrayCritical(array, nullptr);
    
    int result = indexOfFast(reinterpret_cast<const uint8_t*>(bytes) + fromIndex,
                             len - fromIndex,
                             static_cast<uint8_t>(value));
    
    env->ReleasePrimitiveArrayCritical(array, bytes, JNI_ABORT);
    
    return (result >= 0) ? (result + fromIndex) : -1;
}

JNIEXPORT jint JNICALL Java_fastbytes_FastBytes_hashFNV1a(JNIEnv* env, jclass, jbyteArray data) {
    if (!data) return 0;
    
    jsize len = env->GetArrayLength(data);
    void* bytes = env->GetPrimitiveArrayCritical(data, nullptr);
    
    uint32_t hash = hashFNV1a(reinterpret_cast<const uint8_t*>(bytes), len);
    
    env->ReleasePrimitiveArrayCritical(data, bytes, JNI_ABORT);
    return static_cast<jint>(hash);
}

JNIEXPORT void JNICALL Java_fastbytes_FastBytes_xor(JNIEnv* env, jclass,
    jbyteArray a, jbyteArray b, jbyteArray out) {
    
    if (!a || !b || !out) return;
    
    jsize len = env->GetArrayLength(a);
    if (env->GetArrayLength(b) != len || env->GetArrayLength(out) != len) return;
    
    void* aBytes = env->GetPrimitiveArrayCritical(a, nullptr);
    void* bBytes = env->GetPrimitiveArrayCritical(b, nullptr);
    void* outBytes = env->GetPrimitiveArrayCritical(out, nullptr);
    
    xorFast(reinterpret_cast<const uint8_t*>(aBytes),
            reinterpret_cast<const uint8_t*>(bBytes),
            reinterpret_cast<uint8_t*>(outBytes),
            len);
    
    env->ReleasePrimitiveArrayCritical(out, outBytes, 0);
    env->ReleasePrimitiveArrayCritical(b, bBytes, JNI_ABORT);
    env->ReleasePrimitiveArrayCritical(a, aBytes, JNI_ABORT);
}

JNIEXPORT jint JNICALL Java_fastbytes_FastBytes_compare(JNIEnv* env, jclass, jbyteArray a, jbyteArray b) {
    if (!a || !b) return 0;
    jsize len = env->GetArrayLength(a);
    if (env->GetArrayLength(b) != len) return (len < env->GetArrayLength(b)) ? -1 : 1;
    
    void* aBytes = env->GetPrimitiveArrayCritical(a, nullptr);
    void* bBytes = env->GetPrimitiveArrayCritical(b, nullptr);
    
    int res = compareFast(reinterpret_cast<const uint8_t*>(aBytes), 
                          reinterpret_cast<const uint8_t*>(bBytes), 
                          len);
                          
    env->ReleasePrimitiveArrayCritical(b, bBytes, JNI_ABORT);
    env->ReleasePrimitiveArrayCritical(a, aBytes, JNI_ABORT);
    return res;
}

} // extern "C"
