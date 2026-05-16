package fastbytes;

public class XorRace {
    public static void main(String[] args) throws Exception {
        int width = 3840; // 4K
        int height = 2160;
        int size = width * height * 4; // RGBA
        System.out.println("=== 3-WAY XOR Race (20 Iterations) ===");
        System.out.println("Scenario: 4K RGBA Video Frame Glitch-XOR");
        System.out.println("Baseline: Java | FastBytes: SIMD");
        
        byte[] a = new byte[size];
        byte[] b = new byte[size];
        byte[] out = new byte[size];
        
        for (int i = 1; i <= 20; i++) {
            // Java
            long start = System.currentTimeMillis();
            for (int j = 0; j < size; j++) {
                out[j] = (byte)(a[j] ^ b[j]);
            }
            long javaTime = System.currentTimeMillis() - start;
            
            // FastBytes SIMD
            start = System.currentTimeMillis();
            FastBytes.xor(a, b, out);
            long fastTime = System.currentTimeMillis() - start;

            double fps = 1000.0 / Math.max(1, fastTime);
            double ratio = (double)javaTime / Math.max(1, fastTime);
            
            System.out.printf("Iter %2d | Java: %3d ms | FastBytes: %3d ms | Speedup: %.1fx | Est: %.0f FPS %s\n", 
                i, javaTime, fastTime, ratio, fps, (ratio >= 1.0 ? "🚀" : "🐢"));
            
            Thread.sleep(50);
        }
        
        System.out.println("\n----------------------------------------");
        System.out.println("XOR Race Complete. SIMD kills the bandwidth wall.");
        System.out.println("----------------------------------------");
    }
}
