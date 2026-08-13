package fastbytes.benchmark;

import fastbytes.FastBytes;
import org.openjdk.jmh.annotations.*;
import java.util.concurrent.TimeUnit;
import java.util.zip.Adler32;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 2)
@Measurement(iterations = 3)
public class JMH_Hash {

    @Param({"1048576", "104857600"}) // 1MB, 100MB
    private int size;

    private byte[] data;

    @Setup
    public void setup() {
        data = new byte[size];
    }

    @Benchmark
    public long javaAdler32() {
        Adler32 adler = new Adler32();
        adler.update(data);
        return adler.getValue();
    }

    @Benchmark
    public int fastXXH32() {
        return FastBytes.hashXXH32(data, 42);
    }
}
