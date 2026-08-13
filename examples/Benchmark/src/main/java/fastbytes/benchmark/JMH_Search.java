package fastbytes.benchmark;
import fastbytes.FastBytes;

import org.openjdk.jmh.annotations.*;
import java.util.concurrent.TimeUnit;
import java.util.Arrays;

@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.SECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 2, time = 1)
@Measurement(iterations = 3, time = 1)
public class JMH_Search {

    @Param({"104857600"}) // 100MB
    private int size;

    private byte[] data;
    private byte target = (byte) 0xEE;

    @Setup
    public void setup() {
        data = new byte[size];
        Arrays.fill(data, (byte) 0);
        data[size - 1] = target; // Force full scan
    }

    @Benchmark
    public int testJavaSearch() {
        for (int i = 0; i < data.length; i++) {
            if (data[i] == target) return i;
        }
        return -1;
    }

    @Benchmark
    public int testFastBytesSearch() {
        return FastBytes.indexOf(data, target);
    }
}
