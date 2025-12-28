import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class TestXJNIStress {

    static { System.loadLibrary("xjni_test"); }

    // Native methods must declare checked exceptions
    public native void testIOException() throws java.io.IOException;
    public native void testUTFException() throws java.io.UTFDataFormatException;
    public native void testFileNotFoundException() throws java.io.FileNotFoundException;
    public native void testUnsupportedEncodingException() throws java.io.UnsupportedEncodingException;

    private static final int THREADS = 8;
    private static final int ITERATIONS = 1000;

    public static void main(String[] args) throws InterruptedException {
        TestXJNIStress t = new TestXJNIStress();

        ExecutorService pool = Executors.newFixedThreadPool(THREADS);

        Runnable[] tasks = new Runnable[]{
            () -> {
                for (int i = 0; i < ITERATIONS; i++) {
                    try { t.testIOException(); }
                    catch (java.io.IOException e) { System.out.println(e.getMessage()); }
                }
            },
            () -> {
                for (int i = 0; i < ITERATIONS; i++) {
                    try { t.testUTFException(); }
                    catch (java.io.UTFDataFormatException e) { System.out.println(e.getMessage()); }
                }
            },
            () -> {
                for (int i = 0; i < ITERATIONS; i++) {
                    try { t.testFileNotFoundException(); }
                    catch (java.io.FileNotFoundException e) { System.out.println(e.getMessage()); }
                }
            },
            () -> {
                for (int i = 0; i < ITERATIONS; i++) {
                    try { t.testUnsupportedEncodingException(); }
                    catch (java.io.UnsupportedEncodingException e) { System.out.println(e.getMessage()); }
                }
            }
        };

        // Submit each task multiple times to stress threads
        for (int i = 0; i < THREADS; i++) {
            pool.submit(tasks[i % tasks.length]);
        }

        pool.shutdown();
        pool.awaitTermination(1, TimeUnit.MINUTES);

        System.out.println("Multithreaded stress test finished.");
    }
}
