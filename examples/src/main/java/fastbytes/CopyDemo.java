package fastbytes;

import java.util.Random;

/**
 * Technical Example: High-performance 1GB Copy
 */
public class CopyDemo {
    public static void main(String[] args) {
        int size = 1024 * 1024 * 1024; // 1GB
        System.out.println("Allocating 2GB...");
        byte[] src = new byte[size];
        byte[] dest = new byte[size];
        new Random().nextBytes(src);

        System.out.println("Starting FastBytes copy...");
        long start = System.currentTimeMillis();
        FastBytes.copy(src, 0, dest, 0, size);
        long time = System.currentTimeMillis() - start;

        System.out.printf("FastBytes Copy 1GB: %d ms (%.2f GB/s)\n", 
            time, (size / 1024.0 / 1024.0 / 1024.0) / (time / 1000.0));
    }
}
