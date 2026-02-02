public class TestXJNILOG {
	static { System.loadLibrary("xjni_test"); }

	// Native test entry
	public static native void nativeLogTest();

	public static void main(String[] args) {
		TestXJNILOG t = new TestXJNILOG();
		t.nativeLogTest();
	}
}
