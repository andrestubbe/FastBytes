package fastbytes.benchmark;

import fastbytes.FastBytes;
import org.openjdk.jmh.annotations.*;
import java.util.concurrent.TimeUnit;
import java.util.Arrays;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 2)
@Measurement(iterations = 3)
public class JMH_Fill {

    @Param({"1048576", "1073741824"}) // 1MB, 1GB
    private int size;

    private byte[] data;

    @Setup
    public void setup() {
        data = new byte[size];
    }

    @Benchmark
    public void javaFill() {
        Arrays.fill(data, (byte) 42);
    }

    @Benchmark
    public void fastFill() {
        FastBytes.fill(data, (byte) 42);
    }
}
