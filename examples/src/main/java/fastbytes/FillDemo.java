package fastbytes;

/**
 * Technical Example: SIMD-accelerated 1GB Fill
 */
public class FillDemo {
    public static void main(String[] args) {
        int size = 1024 * 1024 * 1024; // 1GB
        System.out.println("Allocating 1GB...");
        byte[] data = new byte[size];

        System.out.println("Starting FastBytes fill...");
        long start = System.currentTimeMillis();
        FastBytes.fill(data, (byte) 0xAA);
        long time = System.currentTimeMillis() - start;

        System.out.printf("FastBytes Fill 1GB: %d ms (%.2f GB/s)\n", 
            time, (size / 1024.0 / 1024.0 / 1024.0) / (time / 1000.0));
    }
}
