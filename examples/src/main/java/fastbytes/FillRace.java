package fastbytes;

public class FillRace {
    public static void main(String[] args) throws Exception {
        int size = 1024 * 1024 * 1024; // 1GB
        System.out.println("=== Java vs FastBytes: 1GB Fill Race (20 Iterations) ===");
        byte[] data = new byte[size];
        byte value = (byte) 0xFF;
        
        for (int i = 1; i <= 20; i++) {
            // Java
            long start = System.currentTimeMillis();
            java.util.Arrays.fill(data, value);
            long javaTime = System.currentTimeMillis() - start;
            
            // FastBytes
            start = System.currentTimeMillis();
            FastBytes.fill(data, value);
            long fastTime = System.currentTimeMillis() - start;

            double ratio = (double)javaTime / Math.max(1, fastTime);
            System.out.printf("Iteration %2d | Java: %3d ms | FastBytes: %3d ms | Speedup: %.1fx %s\n", 
                i, javaTime, fastTime, ratio, (ratio >= 1.0 ? "🚀" : "🐢"));
            
            Thread.sleep(50);
        }
    }
}
