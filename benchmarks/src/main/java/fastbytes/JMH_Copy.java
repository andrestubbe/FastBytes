package fastbytes;

import org.openjdk.jmh.annotations.*;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.SECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 2, time = 1)
@Measurement(iterations = 3, time = 1)
public class JMH_Copy {

    @Param({"1024", "1048576", "104857600"}) // 1KB, 1MB, 100MB
    private int size;

    private byte[] src;
    private byte[] dest;

    @Setup
    public void setup() {
        src = new byte[size];
        dest = new byte[size];
    }

    @Benchmark
    public void testJavaCopy() {
        System.arraycopy(src, 0, dest, 0, size);
    }

    @Benchmark
    public void testFastBytesCopy() {
        FastBytes.copy(src, 0, dest, 0, size);
    }
}
