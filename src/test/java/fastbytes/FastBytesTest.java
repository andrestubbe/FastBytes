package fastbytes;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Unit tests for FastBytes SIMD operations.
 */
public class FastBytesTest {
    
    @Test
    public void testCopy() {
        byte[] src = {1, 2, 3, 4, 5};
        byte[] dest = new byte[5];
        
        FastBytes.copy(src, 0, dest, 0, 5);
        
        assertArrayEquals(src, dest);
    }
    
    @Test
    public void testFill() {
        byte[] arr = new byte[100];
        
        FastBytes.fill(arr, (byte) 42);
        
        for (byte b : arr) {
            assertEquals(42, b);
        }
    }
    
    @Test
    public void testIndexOf() {
        byte[] data = {0, 1, 2, 3, 4, 5, 42, 7, 8, 9};
        
        int pos = FastBytes.indexOf(data, (byte) 42);
        
        assertEquals(6, pos);
    }
    
    @Test
    public void testIndexOfNotFound() {
        byte[] data = {0, 1, 2, 3, 4, 5};
        
        int pos = FastBytes.indexOf(data, (byte) 99);
        
        assertEquals(-1, pos);
    }
    
    @Test
    public void testEquals() {
        byte[] a = {1, 2, 3, 4, 5};
        byte[] b = {1, 2, 3, 4, 5};
        byte[] c = {1, 2, 3, 4, 6};
        
        assertTrue(FastBytes.equals(a, b));
        assertFalse(FastBytes.equals(a, c));
    }
    
    @Test
    public void testHashFNV1a() {
        byte[] data = "Hello World".getBytes();
        
        int hash1 = FastBytes.hashFNV1a(data);
        int hash2 = FastBytes.hashFNV1a(data);
        
        assertEquals(hash1, hash2); // Deterministic
    }
    
    @Test
    public void testXor() {
        byte[] a = {0x01, 0x02, 0x03, 0x04};
        byte[] b = {0xFF, 0xFF, 0xFF, 0xFF};
        byte[] out = new byte[4];
        
        FastBytes.xor(a, b, out);
        
        assertEquals((byte) 0xFE, out[0]);
        assertEquals((byte) 0xFD, out[1]);
        assertEquals((byte) 0xFC, out[2]);
        assertEquals((byte) 0xFB, out[3]);
    }
    
    @Test
    public void testReverse() {
        byte[] data = {1, 2, 3, 4, 5};
        
        FastBytes.reverse(data);
        
        assertEquals((byte) 5, data[0]);
        assertEquals((byte) 4, data[1]);
        assertEquals((byte) 3, data[2]);
        assertEquals((byte) 2, data[3]);
        assertEquals((byte) 1, data[4]);
    }
}
