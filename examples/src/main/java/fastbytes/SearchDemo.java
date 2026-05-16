package fastbytes;

import java.util.Random;

/**
 * Technical Example: SIMD-accelerated 500MB Search
 */
public class SearchDemo {
    public static void main(String[] args) {
        int size = 500 * 1024 * 1024; // 500MB
        System.out.println("Allocating 500MB...");
        byte[] data = new byte[size];
        new Random().nextBytes(data);
        
        byte target = (byte) 0xCC;
        data[size - 1] = target; // Ensure match is at the end

        System.out.println("Starting FastBytes search...");
        long start = System.currentTimeMillis();
        int index = FastBytes.indexOf(data, target);
        long time = System.currentTimeMillis() - start;

        System.out.printf("FastBytes Search 500MB: %d ms. Found at: %d\n", time, index);
    }
}
