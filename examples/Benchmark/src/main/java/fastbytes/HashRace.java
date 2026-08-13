package fastbytes;

public class HashRace {
    public static void main(String[] args) throws Exception {
        int size = 100 * 1024 * 1024; // 100MB
        System.out.println("=== Java vs FastBytes: 100MB Hash Race (20 Iterations) ===");
        byte[] data = new byte[size];
        
        for (int i = 1; i <= 20; i++) {
            // Java (Adler32)
            java.util.zip.Adler32 adler = new java.util.zip.Adler32();
            long start = System.currentTimeMillis();
            adler.update(data);
            long javaTime = System.currentTimeMillis() - start;
            
            // FastBytes (xxHash32)
            start = System.currentTimeMillis();
            FastBytes.hashXXH32(data, 42);
            long fastTime = System.currentTimeMillis() - start;

            double ratio = (double)javaTime / Math.max(1, fastTime);
            System.out.printf("Iteration %2d | Java: %3d ms | FastBytes: %3d ms | Speedup: %.1fx %s\n", 
                i, javaTime, fastTime, ratio, (ratio >= 1.0 ? "🚀" : "🐢"));
            
            Thread.sleep(50);
        }
    }
}
