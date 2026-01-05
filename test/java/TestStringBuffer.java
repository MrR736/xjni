public class TestStringBuffer {
    static { System.loadLibrary("xjni_test"); }

    public native StringBuffer NewStringBuffer();
    public native StringBuffer NewStringBufferString(String str);
    public native StringBuffer NewStringBufferCapacity(int c);
    public native void StringBufferAppendString(StringBuffer sb, String str);
    public native String StringBufferToString(StringBuffer sb);
    public native int StringBufferLength(StringBuffer sb);
    public native void StringBufferReverse(StringBuffer sb);

    public static void main(String[] args) {
        TestStringBuffer test = new TestStringBuffer();

        // Test 1: Create a new StringBuffer and append a string
        StringBuffer sb = test.NewStringBuffer();
        test.StringBufferAppendString(sb, "Hello, ");
        test.StringBufferAppendString(sb, "JNI!");
        System.out.println("StringBuffer content after append: " + test.StringBufferToString(sb)); // Expected: "Hello, JNI!"

        // Test 2: Check the length of the StringBuffer
        int length = test.StringBufferLength(sb);
        System.out.println("StringBuffer length: " + length); // Expected: 13

        // Test 3: Reverse the StringBuffer and check content again
        test.StringBufferReverse(sb);
        System.out.println("StringBuffer after reverse: " + test.StringBufferToString(sb)); // Expected: "!INJ ,olleH"

        // Test 4: Create a new StringBuffer and append a string
        StringBuffer sb2 = test.NewStringBufferCapacity(16);
        test.StringBufferAppendString(sb2, "Hello, ");
        test.StringBufferAppendString(sb2, "JNI!");
        System.out.println("New StringBuffer Capacity content: " + test.StringBufferToString(sb2)); // Expected: "Hello, JNI!"

        // Test 5: Create a StringBuffer with an initial string
        StringBuffer sb3 = test.NewStringBufferString("Initial String");
        System.out.println("New StringBuffer content: " + test.StringBufferToString(sb3)); // Expected: "Initial String"
    }
}
