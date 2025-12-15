public class TestStringArray {
    static { System.loadLibrary("xjni_test"); }

    // Native methods
    private native String[] createStringArray(String[] input);
    private native void printStringArray(String[] arr);

    public static void main(String[] args) {
        TestStringArray test = new TestStringArray();

        String[] input = {"Hello", "JNI", "World"};
        String[] output = test.createStringArray(input);

        System.out.println("Returned from native code:");
        for (String s : output) {
            System.out.println(s);
        }

        System.out.println("\nPrinting array via native:");
        test.printStringArray(output);
    }
}
