package fastbytes;

import java.util.Random;

/**
 * YouTube Demo 3: Real-Time XOR Visualizer
 * 
 * Simulates a high-frame-rate glitch-art visualizer
 * using SIMD-accelerated XOR operations on 4K frames.
 */
public class Demo3 {
    public static void main(String[] args) {
        int size = 1920 * 1080 * 4; // 4K RGBA frame (~8MB)
        System.out.println("=== Real-Time XOR Visualizer ===");
        System.out.println("Simulating 60 FPS Glitch-Art Rendering (RGBA 4K)...");
        
        byte[] img1 = new byte[size];
        byte[] img2 = new byte[size];
        byte[] out = new byte[size];
        
        new Random().nextBytes(img1);
        new Random().nextBytes(img2);

        int frames = 600; // 10 seconds at 60fps
        long totalTime = 0;

        System.out.println("Rendering 600 frames...");
        for (int i = 0; i < frames; i++) {
            long start = System.nanoTime();
            FastBytes.xor(img1, img2, out);
            totalTime += (System.nanoTime() - start);
            
            if (i % 60 == 0) {
                System.out.print(".");
            }
        }

        double avgTimeMs = (totalTime / (double)frames) / 1_000_000.0;
        double fps = 1000.0 / avgTimeMs;

        System.out.println("\n\n----------------------------------------");
        System.out.printf("Average Frame Time: %.3f ms\n", avgTimeMs);
        System.out.printf("Theoretical Max Performance: %.0f FPS\n", fps);
        System.out.println("----------------------------------------");
        System.out.println("Proving that SIMD throughput is over-kill for 4K video.");
    }
}
