public final class XXJNITest {
    static {
        System.loadLibrary("xxjni_cpp_test");
    }

    private XXJNITest() {}

    public static native String runAll();

    public static void main(String[] args) {
        String result = runAll();
        System.out.print(result);

        if (!result.contains("ALL TESTS PASSED")) {
            System.exit(1);
        }
    }
}
