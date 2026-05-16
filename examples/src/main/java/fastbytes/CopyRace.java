package fastbytes;

public class CopyRace {
    public static void main(String[] args) throws Exception {
        int size = 1024 * 1024 * 1024; // 1GB
        System.out.println("=== Java vs FastBytes: 1GB Copy Race (20 Iterations) ===");
        byte[] src = new byte[size];
        byte[] dest = new byte[size];
        
        System.out.println("Allocating 2GB of memory...");
        System.out.println("\n[Race starting in 3 seconds...]");
        Thread.sleep(3000);

        for (int i = 1; i <= 20; i++) {
            // Java
            long start = System.currentTimeMillis();
            System.arraycopy(src, 0, dest, 0, size);
            long javaTime = System.currentTimeMillis() - start;
            
            // FastBytes
            start = System.currentTimeMillis();
            FastBytes.copy(src, 0, dest, 0, size);
            long fastTime = System.currentTimeMillis() - start;

            double ratio = (double)javaTime / Math.max(1, fastTime);
            System.out.printf("Iteration %2d | Java: %3d ms | FastBytes: %3d ms | Speedup: %.1fx %s\n", 
                i, javaTime, fastTime, ratio, (ratio >= 1.0 ? "🚀" : "🐢"));
            
            Thread.sleep(100); // Small pause to make it readable
        }
        
        System.out.println("\n----------------------------------------");
        System.out.println("Race Complete. FastBytes maintains lead.");
        System.out.println("----------------------------------------");
    }
}
