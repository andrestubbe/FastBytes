package fastbytes;

public class SearchRace {
    public static void main(String[] args) throws Exception {
        int size = 500 * 1024 * 1024; // 500MB
        System.out.println("=== 3-WAY Search Race (20 Iterations) ===");
        System.out.println("Baseline: Java | Classic: SIMD | Pro: Prefetching SIMD");
        
        byte[] data = new byte[size];
        byte target = (byte) 0xEE;
        data[size - 1] = target; 
        
        for (int i = 1; i <= 20; i++) {
            // Java
            long start = System.nanoTime();
            int idx = -1;
            for (int j = 0; j < size; j++) { if (data[j] == target) { idx = j; break; } }
            long javaTime = System.nanoTime() - start;
            
            // Legacy SIMD
            start = System.nanoTime();
            FastBytes.indexOfLegacy(data, target, 0);
            long legacyTime = System.nanoTime() - start;
            
            // Pro SIMD (Prefetch)
            start = System.nanoTime();
            FastBytes.indexOf(data, target, 0);
            long proTime = System.nanoTime() - start;

            System.out.printf("Iter %2d | Java: %6.2f ms | Classic: %6.2f ms | Pro: %6.2f ms | Best: %s\n", 
                i, javaTime / 1_000_000.0, legacyTime / 1_000_000.0, proTime / 1_000_000.0,
                (proTime <= legacyTime && proTime <= javaTime ? "PRO 🚀" : 
                (legacyTime <= javaTime ? "CLASSIC ⚡" : "JAVA 🐢")));
            
            Thread.sleep(50);
        }
    }
}
