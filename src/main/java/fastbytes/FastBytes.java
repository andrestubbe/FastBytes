package fastbytes;

import fastcore.FastCore;
import java.lang.ref.Cleaner;

/**
 * High-performance byte array operations with SIMD acceleration.
 */
public class FastBytes implements AutoCloseable {
    
    private static final Cleaner cleaner = Cleaner.create();
    private final Cleaner.Cleanable cleanable;
    private long nativeHandle;
    
    static {
        FastCore.loadLibrary("fastbytes");
    }
    
    public FastBytes(int capacity) {
        this.nativeHandle = nativeCreate(capacity);
        this.cleanable = cleaner.register(this, new NativeCleanup(nativeHandle));
    }
    
    public FastBytes(byte[] data) {
        this.nativeHandle = nativeFromBytes(data);
        this.cleanable = cleaner.register(this, new NativeCleanup(nativeHandle));
    }
    
    // ==================== STATIC SIMD OPERATIONS (PRO) ====================
    
    public static native void copy(byte[] src, int srcPos, byte[] dest, int destPos, int length);
    public static native void fill(byte[] array, int fromIndex, int toIndex, byte value);
    public static native int indexOf(byte[] array, byte value, int fromIndex);
    public static native void xor(byte[] a, byte[] b, byte[] out);
    public static native int hashXXH32(byte[] data, int seed);

    // ==================== STATIC SIMD OPERATIONS (LEGACY) ====================
    
    public static native void copyLegacy(byte[] src, int srcPos, byte[] dest, int destPos, int length);
    public static native void fillLegacy(byte[] array, int fromIndex, int toIndex, byte value);
    public static native int indexOfLegacy(byte[] array, byte value, int fromIndex);

    // ==================== ADDITIONAL OPERATIONS ====================

    public static native int hashFNV1a(byte[] data);
    public static native void reverse(byte[] array);
    public static native void swapBytes(byte[] array, int groupSize);
    public static native void secureZero(byte[] array);
    public static native int compare(byte[] a, byte[] b);
    public static native int lastIndexOf(byte[] array, byte value);
    public static native int count(byte[] array, byte value);

    // ==================== UTILITIES ====================

    public static void fill(byte[] array, byte value) {
        fill(array, 0, array.length, value);
    }
    
    public static int indexOf(byte[] array, byte value) {
        return indexOf(array, value, 0);
    }
    
    public static boolean equals(byte[] a, byte[] b) {
        if (a == b) return true;
        if (a == null || b == null) return false;
        if (a.length != b.length) return false;
        return compare(a, b) == 0;
    }

    // ==================== INSTANCE METHODS ====================
    
    public native int capacity();
    public native int size();
    public native void resize(int newCapacity);
    public native byte[] toArray();
    public native FastBytes append(byte[] data);
    
    @Override
    public void close() {
        if (nativeHandle != 0) {
            cleanable.clean();
            nativeHandle = 0;
        }
    }
    
    private static class NativeCleanup implements Runnable {
        private final long handle;
        NativeCleanup(long handle) { this.handle = handle; }
        @Override
        public void run() { if (handle != 0) nativeDestroy(handle); }
    }
    
    // ==================== NATIVE PRIVATE ====================
    
    private static native long nativeCreate(int capacity);
    private static native long nativeFromBytes(byte[] data);
    private static native void nativeDestroy(long handle);
}
