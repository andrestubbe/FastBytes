package fastbytes;

import java.util.Random;

/**
 * YouTube Demo 2: SIMD Search vs Java Loop
 * 
 * Demonstrates the 40x speed gap when searching for a byte
 * in a 500MB buffer using SIMD instructions.
 */
public class Demo2 {
    public static void main(String[] args) {
        int size = 500 * 1024 * 1024; // 500MB
        System.out.println("=== SIMD Search vs Java Loop (500MB) ===");
        byte[] data = new byte[size];
        new Random().nextBytes(data);
        
        byte target = (byte) 0xEE;
        data[size - 100] = target; // Put target near the very end to maximize work

        System.out.println("Searching for 0xEE in 500MB buffer...");

        // --- Java Loop ---
        long start = System.nanoTime();
        int index = -1;
        for (int i = 0; i < size; i++) {
            if (data[i] == target) {
                index = i;
                break;
            }
        }
        long javaTime = System.nanoTime() - start;
        System.out.printf("Java Manual Loop: %.2f ms\n", javaTime / 1_000_000.0);

        // --- FastBytes SIMD ---
        start = System.nanoTime();
        int fastIndex = FastBytes.indexOf(data, target);
        long fastTime = System.nanoTime() - start;
        System.out.printf("FastBytes.indexOf (SIMD): %.2f ms\n", fastTime / 1_000_000.0);

        System.out.println("\n----------------------------------------");
        System.out.printf("SPEED GAP: %.1fx\n", (double)javaTime / fastTime);
        System.out.printf("Match found at index: %d\n", fastIndex);
        System.out.println("----------------------------------------");
    }
}
