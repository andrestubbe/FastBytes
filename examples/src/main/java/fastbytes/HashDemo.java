package fastbytes;

import java.util.Random;

/**
 * Technical Example: Fast Hashing (xxHash32)
 */
public class HashDemo {
    public static void main(String[] args) {
        int size = 100 * 1024 * 1024; // 100MB
        byte[] data = new byte[size];
        new Random().nextBytes(data);

        System.out.println("Starting FastBytes xxHash32...");
        long start = System.currentTimeMillis();
        int hash = FastBytes.hashXXH32(data, 0);
        long time = System.currentTimeMillis() - start;

        System.out.printf("FastBytes xxHash32 100MB: %d ms. Hash: 0x%08X\n", time, hash);
    }
}
