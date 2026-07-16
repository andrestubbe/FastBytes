package fastbytes.benchmark;
import fastbytes.FastBytes;

import org.openjdk.jmh.annotations.*;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.SECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 2, time = 1)
@Measurement(iterations = 3, time = 1)
public class JMH_Xor {

    @Param({"8294400"}) // 4K frame-ish size
    private int size;

    private byte[] img1;
    private byte[] img2;
    private byte[] out;

    @Setup
    public void setup() {
        img1 = new byte[size];
        img2 = new byte[size];
        out = new byte[size];
    }

    @Benchmark
    public void testFastBytesXor() {
        FastBytes.xor(img1, img2, out);
    }
}
