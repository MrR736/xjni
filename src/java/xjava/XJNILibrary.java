package xjava;

public final class XJNILibrary {
	static { System.loadLibrary("xjni"); }
	protected XJNILibrary() { }

	public static void ensureLoaded() { }
}
