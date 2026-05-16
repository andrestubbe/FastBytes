package fastbytes;

public class SearchRace {
    public static void main(String[] args) throws Exception {
        int size = 500 * 1024 * 1024; // 500MB
        System.out.println("=== Java vs FastBytes: 500MB Search Race (20 Iterations) ===");
        byte[] data = new byte[size];
        byte target = (byte) 0xEE;
        java.util.Arrays.fill(data, (byte) 0);
        data[size - 100] = target; // Target at the end
        
        System.out.println("Allocating 500MB of memory...");
        System.out.println("\n[Race starting in 3 seconds...]");
        Thread.sleep(3000);

        for (int i = 1; i <= 20; i++) {
            // Java
            long start = System.nanoTime();
            int index = -1;
            for (int j = 0; j < size; j++) {
                if (data[j] == target) {
                    index = j;
                    break;
                }
            }
            long javaTime = System.nanoTime() - start;
            
            // FastBytes
            start = System.nanoTime();
            int fastIndex = FastBytes.indexOf(data, target);
            long fastTime = System.nanoTime() - start;

            double ratio = (double)javaTime / Math.max(1, fastTime);
            System.out.printf("Iteration %2d | Java: %6.2f ms | FastBytes: %6.2f ms | Speedup: %.1fx %s\n", 
                i, javaTime / 1_000_000.0, fastTime / 1_000_000.0, ratio, (ratio >= 1.0 ? "🚀" : "🐢"));
            
            Thread.sleep(100);
        }
        
        System.out.println("\n----------------------------------------");
        System.out.println("Search Race Complete.");
        System.out.println("----------------------------------------");
    }
}
