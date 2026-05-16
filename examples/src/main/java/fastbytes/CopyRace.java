package fastbytes;

public class CopyRace {
    public static void main(String[] args) throws Exception {
        int size = 1024 * 1024 * 1024; // 1GB
        System.out.println("=== 3-WAY Copy Race (20 Iterations) ===");
        System.out.println("Baseline: Java | Classic: SIMD | Pro: Streaming SIMD");
        
        byte[] src = new byte[size];
        byte[] dest = new byte[size];
        
        for (int i = 1; i <= 20; i++) {
            // Java
            long start = System.currentTimeMillis();
            System.arraycopy(src, 0, dest, 0, size);
            long javaTime = System.currentTimeMillis() - start;
            
            // Legacy SIMD
            start = System.currentTimeMillis();
            FastBytes.copyLegacy(src, 0, dest, 0, size);
            long legacyTime = System.currentTimeMillis() - start;
            
            // Pro SIMD (Streaming)
            start = System.currentTimeMillis();
            FastBytes.copy(src, 0, dest, 0, size);
            long proTime = System.currentTimeMillis() - start;

            System.out.printf("Iter %2d | Java: %3d ms | Classic: %3d ms | Pro: %3d ms | Best: %s\n", 
                i, javaTime, legacyTime, proTime, 
                (proTime <= legacyTime && proTime <= javaTime ? "PRO 🚀" : 
                (legacyTime <= javaTime ? "CLASSIC ⚡" : "JAVA 🐢")));
            
            Thread.sleep(50);
        }
    }
}
