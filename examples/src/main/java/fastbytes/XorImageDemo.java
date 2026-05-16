package fastbytes;

/**
 * Technical Example: SIMD XOR on 4K Data
 */
public class XorImageDemo {
    public static void main(String[] args) {
        int size = 1920 * 1080 * 4; // 4K RGBA
        byte[] img1 = new byte[size];
        byte[] img2 = new byte[size];
        byte[] out = new byte[size];

        System.out.println("Starting FastBytes XOR (4K Frame)...");
        long start = System.nanoTime();
        FastBytes.xor(img1, img2, out);
        long timeNs = System.nanoTime() - start;

        System.out.printf("FastBytes XOR: %.3f ms\n", timeNs / 1_000_000.0);
    }
}
