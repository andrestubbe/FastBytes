package fastbytes;

public class Demo {
    public static void main(String[] args) throws Exception {
        System.out.println("=================================================");
        System.out.println("⚡ FastBytes Unified Demo & SIMD Speed Race ⚡");
        System.out.println("=================================================\n");

        System.out.println("1. Running Search Race...");
        SearchRace.main(args);

        System.out.println("\n2. Running Copy Race...");
        CopyRace.main(args);

        System.out.println("\n3. Running Fill Race...");
        FillRace.main(args);

        System.out.println("\n4. Running Hash Race...");
        HashRace.main(args);

        System.out.println("\n5. Running XOR Race...");
        XorRace.main(args);

        System.out.println("\n=================================================");
        System.out.println("✅ All FastBytes SIMD Races Completed!");
        System.out.println("=================================================");
    }
}
