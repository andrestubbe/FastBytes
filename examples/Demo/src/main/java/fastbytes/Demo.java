package fastbytes;

import java.util.Random;

/**
 * YouTube Demo 1: Java vs FastBytes: 1GB Copy Race
 * 
 * Demonstrates the massive throughput difference between System.arraycopy
 * and FastBytes SIMD-accelerated copy for large memory blocks.
 */
public class Demo {
    public static void main(String[] args) {
        int size = 1024 * 1024 * 1024; // 1GB
        System.out.println("=== Java vs FastBytes: 1GB Copy Race ===");
        System.out.println("Allocating 2GB of memory...");
        byte[] src = new byte[size];
        byte[] dest = new byte[size];
        new Random().nextBytes(src);

        System.out.println("\n[Race starting in 3 seconds...]");
        try { Thread.sleep(3000); } catch (Exception e) {}

        // --- Java Race ---
        System.out.print("Java System.arraycopy running... ");
        long start = System.currentTimeMillis();
        System.arraycopy(src, 0, dest, 0, size);
        long javaTime = System.currentTimeMillis() - start;
        double javaSpeed = (size / 1024.0 / 1024.0 / 1024.0) / (javaTime / 1000.0);
        System.out.printf("DONE in %d ms (%.2f GB/s)\n", javaTime, javaSpeed);

        // --- FastBytes Race ---
        System.out.print("FastBytes.copy (AVX2) running... ");
        start = System.currentTimeMillis();
        FastBytes.copy(src, 0, dest, 0, size);
        long fastTime = System.currentTimeMillis() - start;
        double fastSpeed = (size / 1024.0 / 1024.0 / 1024.0) / (fastTime / 1000.0);
        System.out.printf("DONE in %d ms (%.2f GB/s)\n", fastTime, fastSpeed);

        System.out.println("\n----------------------------------------");
        System.out.printf("WINNER: FastBytes (%.1fx faster)\n", (double)javaTime / fastTime);
        System.out.println("----------------------------------------");
    }
}
