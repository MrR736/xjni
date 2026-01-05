public class TestStringBuilder {
	static { System.loadLibrary("xjni_test"); }

	// Declare the native methods
	public native StringBuilder NewStringBuilder();
	public native void StringBuilderAppendString(StringBuilder sb, String str);
	public native void StringBuilderAppendInt(StringBuilder sb, int num);
	public native void StringBuilderInsertString(StringBuilder sb, int offset, String str);
	public native int StringBuilderIndexOf(StringBuilder sb, String str);
	public native void StringBuilderReverse(StringBuilder sb);


	public static void main(String[] args) {
		// Create a new StringBuilder instance
		TestStringBuilder test = new TestStringBuilder();
		StringBuilder sb = test.NewStringBuilder();

		// Test append method
		test.StringBuilderAppendString(sb, "Hello, ");
		test.StringBuilderAppendString(sb, "World!");
		System.out.println("Appended String: " + sb.toString()); // Expected: "Hello, World!"

		// Test append int
		test.StringBuilderAppendInt(sb, 123);
		System.out.println("Appended Int: " + sb.toString()); // Expected: "Hello, World!123"

		// Test insert method
		test.StringBuilderInsertString(sb, 13, "Java ");
		System.out.println("After Insert: " + sb.toString()); // Expected: "Hello, Java World!123"

		// Test indexOf method
		int index = test.StringBuilderIndexOf(sb, "Java");
		System.out.println("Index of 'Java': " + index); // Expected: 13

		// Test reverse method
		test.StringBuilderReverse(sb);
		System.out.println("Reversed StringBuilder: " + sb.toString()); // Expected: "321!dlroW avaJ ,olleH"
	}
}
