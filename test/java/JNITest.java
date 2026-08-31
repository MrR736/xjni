public final class JNITest {
    static { System.loadLibrary("xxjni_cpp_test"); }
    public static native String runAll();
    public static void main(String[] args) {
        String result = runAll();
        System.out.print(result);
        if (!result.contains("ALL TESTS PASSED")) throw new AssertionError(result);
    }
}
