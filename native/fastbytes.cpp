#include "fastbytes.h"
#include <cstring>
#include <algorithm>
#include <immintrin.h>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace fastbytes {

static bool g_sse42 = false;
static bool g_avx2 = false;
static bool g_avx512f = false;
static bool g_avx512bw = false;
static bool g_initialized = false;

static void initCpuFeatures() {
    if (g_initialized) return;
    int cpuInfo[4] = {0};
#ifdef _MSC_VER
    __cpuid(cpuInfo, 1);
#else
    __cpuid(1, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#endif
    g_sse42 = (cpuInfo[2] & (1 << 20)) != 0;
    bool hasAVX = (cpuInfo[2] & (1 << 28)) != 0;
    if (hasAVX) {
#ifdef _MSC_VER
        __cpuidex(cpuInfo, 7, 0);
#else
        __cpuid_count(7, 0, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#endif
        g_avx2     = (cpuInfo[1] & (1 << 5))  != 0;
        g_avx512f  = (cpuInfo[1] & (1 << 16)) != 0;
        g_avx512bw = (cpuInfo[1] & (1 << 30)) != 0;
    }
    g_initialized = true;
}

bool hasSSE42()    { if (!g_initialized) initCpuFeatures(); return g_sse42; }
bool hasAVX2()     { if (!g_initialized) initCpuFeatures(); return g_avx2; }
bool hasAVX512BW() { if (!g_initialized) initCpuFeatures(); return g_avx512f && g_avx512bw; }

// --- CORE ALGORITHMS ---

void copyFast_Legacy(const uint8_t* src, uint8_t* dest, size_t length) {
    size_t i = 0;
    if (hasAVX2()) {
        for (; i + 31 < length; i += 32) {
            __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src + i));
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i), data);
        }
    }
    for (; i < length; i++) dest[i] = src[i];
}

void copyFast_Pro(const uint8_t* src, uint8_t* dest, size_t length) {
    size_t i = 0;
    if (hasAVX2()) {
        for (; i + 63 < length; i += 64) {
            _mm_prefetch(reinterpret_cast<const char*>(src + i + 256), _MM_HINT_T0);
            __m256i d1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src + i));
            __m256i d2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src + i + 32));
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i), d1);
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i + 32), d2);
        }
    }
    for (; i < length; i++) dest[i] = src[i];
}

void fillFast_Legacy(uint8_t* dest, uint8_t value, size_t length) {
    size_t i = 0;
    if (hasAVX2()) {
        __m256i valVec = _mm256_set1_epi8(static_cast<char>(value));
        for (; i + 31 < length; i += 32) {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i), valVec);
        }
    }
    for (; i < length; i++) dest[i] = value;
}

void fillFast_Pro(uint8_t* dest, uint8_t value, size_t length) {
    size_t i = 0;
    if (hasAVX2()) {
        __m256i valVec = _mm256_set1_epi8(static_cast<char>(value));
        for (; i + 63 < length; i += 64) {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i), valVec);
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(dest + i + 32), valVec);
        }
    }
    for (; i < length; i++) dest[i] = value;
}

int indexOfFast_Legacy(const uint8_t* data, size_t length, uint8_t target) {
    size_t i = 0;
    if (hasAVX2()) {
        __m256i targetVec = _mm256_set1_epi8(static_cast<char>(target));
        for (; i + 31 < length; i += 32) {
            __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i));
            __m256i cmp = _mm256_cmpeq_epi8(chunk, targetVec);
            int mask = _mm256_movemask_epi8(cmp);
            if (mask != 0) {
                unsigned long bitPos;
#ifdef _MSC_VER
                _BitScanForward(&bitPos, mask);
#else
                bitPos = __builtin_ctz(mask);
#endif
                return static_cast<int>(i + bitPos);
            }
        }
    }
    for (; i < length; i++) if (data[i] == target) return static_cast<int>(i);
    return -1;
}

int indexOfFast_Pro(const uint8_t* data, size_t length, uint8_t target) {
    size_t i = 0;
    if (hasAVX2()) {
        __m256i targetVec = _mm256_set1_epi8(static_cast<char>(target));
        for (; i + 63 < length; i += 64) {
            _mm_prefetch(reinterpret_cast<const char*>(data + i + 256), _MM_HINT_T0);
            __m256i c1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i));
            __m256i c2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i + 32));
            int m1 = _mm256_movemask_epi8(_mm256_cmpeq_epi8(c1, targetVec));
            if (m1) {
                unsigned long pos;
#ifdef _MSC_VER
                _BitScanForward(&pos, m1);
#else
                pos = __builtin_ctz(m1);
#endif
                return static_cast<int>(i + pos);
            }
            int m2 = _mm256_movemask_epi8(_mm256_cmpeq_epi8(c2, targetVec));
            if (m2) {
                unsigned long pos;
#ifdef _MSC_VER
                _BitScanForward(&pos, m2);
#else
                pos = __builtin_ctz(m2);
#endif
                return static_cast<int>(i + 32 + pos);
            }
        }
    }
    for (; i < length; i++) if (data[i] == target) return static_cast<int>(i);
    return -1;
}

void xorFast(const uint8_t* a, const uint8_t* b, uint8_t* out, size_t length) {
    size_t i = 0;
    if (hasAVX512BW()) {
        for (; i + 127 < length; i += 128) {
            _mm_prefetch(reinterpret_cast<const char*>(a + i + 512), _MM_HINT_T0);
            _mm_prefetch(reinterpret_cast<const char*>(b + i + 512), _MM_HINT_T0);
            __m512i a1 = _mm512_loadu_si512(reinterpret_cast<const void*>(a + i));
            __m512i b1 = _mm512_loadu_si512(reinterpret_cast<const void*>(b + i));
            __m512i a2 = _mm512_loadu_si512(reinterpret_cast<const void*>(a + i + 64));
            __m512i b2 = _mm512_loadu_si512(reinterpret_cast<const void*>(b + i + 64));
            _mm512_storeu_si512(reinterpret_cast<void*>(out + i), _mm512_xor_si512(a1, b1));
            _mm512_storeu_si512(reinterpret_cast<void*>(out + i + 64), _mm512_xor_si512(a2, b2));
        }
    }
    if (hasAVX2()) {
        for (; i + 63 < length; i += 64) {
            _mm_prefetch(reinterpret_cast<const char*>(a + i + 256), _MM_HINT_T0);
            _mm_prefetch(reinterpret_cast<const char*>(b + i + 256), _MM_HINT_T0);
            __m256i a1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a + i));
            __m256i b1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b + i));
            __m256i a2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a + i + 32));
            __m256i b2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b + i + 32));
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(out + i), _mm256_xor_si256(a1, b1));
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(out + i + 32), _mm256_xor_si256(a2, b2));
        }
    }
    for (; i < length; ++i) out[i] = a[i] ^ b[i];
}

int compareFast(const uint8_t* a, const uint8_t* b, size_t length) {
    size_t i = 0;
    if (hasAVX2()) {
        for (; i + 31 < length; i += 32) {
            __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a + i));
            __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b + i));
            int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(va, vb));
            if (mask != -1) {
                unsigned long bit;
#ifdef _MSC_VER
                _BitScanForward(&bit, ~static_cast<unsigned int>(mask));
#else
                bit = __builtin_ctz(~mask);
#endif
                return (int)a[i + bit] - (int)b[i + bit];
            }
        }
    }
    for (; i < length; ++i) if (a[i] != b[i]) return (int)a[i] - (int)b[i];
    return 0;
}

uint32_t hashXXH32(const uint8_t* data, size_t length, uint32_t seed) {
    const uint32_t PRIME32_1 = 2654435761u;
    const uint32_t PRIME32_2 = 2246822519u;
    const uint32_t PRIME32_5 = 374761393u;
    uint32_t h32 = seed + PRIME32_5;
    size_t i = 0;
    for (; i + 4 <= length; i += 4) {
        uint32_t chunk = data[i] | (data[i+1] << 8) | (data[i+2] << 16) | (data[i+3] << 24);
        h32 += chunk * PRIME32_1;
        h32 = (h32 << 13) | (h32 >> 19);
        h32 *= PRIME32_2;
    }
    for (; i < length; i++) h32 = ((h32 + data[i] * PRIME32_5) << 11) | ((h32 + data[i] * PRIME32_5) >> 21);
    return h32;
}

void reverseFast(uint8_t* data, size_t length) {
    if (!data || length < 2) return;
    size_t i = 0, j = length - 1;
    while (i < j) { uint8_t t = data[i]; data[i] = data[j]; data[j] = t; i++; j--; }
}

} // namespace fastbytes

// --- JNI (BACK TO CRITICAL) ---

using namespace fastbytes;

extern "C" {
    JNIEXPORT void JNICALL Java_fastbytes_FastBytes_copy(JNIEnv* env, jclass, jbyteArray s, jint sp, jbyteArray d, jint dp, jint l) {
        void* src = env->GetPrimitiveArrayCritical(s, 0);
        void* dest = env->GetPrimitiveArrayCritical(d, 0);
        copyFast_Pro(reinterpret_cast<const uint8_t*>(src) + sp, reinterpret_cast<uint8_t*>(dest) + dp, l);
        env->ReleasePrimitiveArrayCritical(d, dest, 0);
        env->ReleasePrimitiveArrayCritical(s, src, JNI_ABORT);
    }
    JNIEXPORT void JNICALL Java_fastbytes_FastBytes_copyLegacy(JNIEnv* env, jclass, jbyteArray s, jint sp, jbyteArray d, jint dp, jint l) {
        void* src = env->GetPrimitiveArrayCritical(s, 0);
        void* dest = env->GetPrimitiveArrayCritical(d, 0);
        copyFast_Legacy(reinterpret_cast<const uint8_t*>(src) + sp, reinterpret_cast<uint8_t*>(dest) + dp, l);
        env->ReleasePrimitiveArrayCritical(d, dest, 0);
        env->ReleasePrimitiveArrayCritical(s, src, JNI_ABORT);
    }
    JNIEXPORT void JNICALL Java_fastbytes_FastBytes_fill(JNIEnv* env, jclass, jbyteArray a, jint f, jint t, jbyte v) {
        void* bytes = env->GetPrimitiveArrayCritical(a, 0);
        fillFast_Pro(reinterpret_cast<uint8_t*>(bytes) + f, v, t - f);
        env->ReleasePrimitiveArrayCritical(a, bytes, 0);
    }
    JNIEXPORT void JNICALL Java_fastbytes_FastBytes_fillLegacy(JNIEnv* env, jclass, jbyteArray a, jint f, jint t, jbyte v) {
        void* bytes = env->GetPrimitiveArrayCritical(a, 0);
        fillFast_Legacy(reinterpret_cast<uint8_t*>(bytes) + f, v, t - f);
        env->ReleasePrimitiveArrayCritical(a, bytes, 0);
    }
    JNIEXPORT jint JNICALL Java_fastbytes_FastBytes_indexOf(JNIEnv* env, jclass, jbyteArray a, jbyte v, jint f) {
        jsize len = env->GetArrayLength(a);
        void* bytes = env->GetPrimitiveArrayCritical(a, 0);
        int r = indexOfFast_Pro(reinterpret_cast<const uint8_t*>(bytes) + f, len - f, v);
        env->ReleasePrimitiveArrayCritical(a, bytes, JNI_ABORT);
        return (r >= 0) ? (r + f) : -1;
    }
    JNIEXPORT jint JNICALL Java_fastbytes_FastBytes_indexOfLegacy(JNIEnv* env, jclass, jbyteArray a, jbyte v, jint f) {
        jsize len = env->GetArrayLength(a);
        void* bytes = env->GetPrimitiveArrayCritical(a, 0);
        int r = indexOfFast_Legacy(reinterpret_cast<const uint8_t*>(bytes) + f, len - f, v);
        env->ReleasePrimitiveArrayCritical(a, bytes, JNI_ABORT);
        return (r >= 0) ? (r + f) : -1;
    }
    JNIEXPORT void JNICALL Java_fastbytes_FastBytes_xor(JNIEnv* env, jclass, jbyteArray a, jbyteArray b, jbyteArray o) {
        void* ab = env->GetPrimitiveArrayCritical(a, 0);
        void* bb = env->GetPrimitiveArrayCritical(b, 0);
        void* ob = env->GetPrimitiveArrayCritical(o, 0);
        jsize len = env->GetArrayLength(a);
        xorFast(reinterpret_cast<const uint8_t*>(ab), reinterpret_cast<const uint8_t*>(bb), reinterpret_cast<uint8_t*>(ob), len);
        env->ReleasePrimitiveArrayCritical(o, ob, 0);
        env->ReleasePrimitiveArrayCritical(b, bb, JNI_ABORT);
        env->ReleasePrimitiveArrayCritical(a, ab, JNI_ABORT);
    }
    JNIEXPORT jint JNICALL Java_fastbytes_FastBytes_hashXXH32(JNIEnv* env, jclass, jbyteArray d, jint s) {
        jsize len = env->GetArrayLength(d);
        void* b = env->GetPrimitiveArrayCritical(d, 0);
        uint32_t h = hashXXH32(reinterpret_cast<const uint8_t*>(b), len, s);
        env->ReleasePrimitiveArrayCritical(d, b, JNI_ABORT);
        return static_cast<jint>(h);
    }
    JNIEXPORT jint JNICALL Java_fastbytes_FastBytes_compare(JNIEnv* env, jclass, jbyteArray a, jbyteArray b) {
        jsize len = env->GetArrayLength(a);
        void* ab = env->GetPrimitiveArrayCritical(a, 0);
        void* bb = env->GetPrimitiveArrayCritical(b, 0);
        int res = compareFast(reinterpret_cast<const uint8_t*>(ab), reinterpret_cast<const uint8_t*>(bb), len);
        env->ReleasePrimitiveArrayCritical(b, bb, JNI_ABORT);
        env->ReleasePrimitiveArrayCritical(a, ab, JNI_ABORT);
        return res;
    }
    JNIEXPORT void JNICALL Java_fastbytes_FastBytes_reverse(JNIEnv* env, jclass, jbyteArray a) {
        jsize len = env->GetArrayLength(a);
        void* b = env->GetPrimitiveArrayCritical(a, 0);
        reverseFast(reinterpret_cast<uint8_t*>(b), len);
        env->ReleasePrimitiveArrayCritical(a, b, 0);
    }
}
