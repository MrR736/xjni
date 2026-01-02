import java.io.StringReader;

public class TestStringReader {
	static { System.loadLibrary("xjni_test"); }

	private native void StringReaderEnsureOpen(StringReader sr);
	private native int StringReaderRead(StringReader sr);
	private native int StringReaderReadCharArrayIntInt( StringReader sr, char[] array, int offset, int len);
	private native long StringReaderReadSkip(StringReader sr, long ns);
	private native boolean StringReaderReadReady(StringReader sr);
	private native boolean StringReaderReadMarkSupported(StringReader sr);
	private native void StringReaderMark(StringReader sr, int readAheadLimit);
	private native void StringReaderReset(StringReader sr);
	private native void StringReaderClose(StringReader sr);

	public static void main(String[] args) {
		TestStringReader t = new TestStringReader();

		StringReader sr = new StringReader("Hello JNI World");

		// ensureOpen()
		t.StringReaderEnsureOpen(sr);

		// read()
		int c1 = t.StringReaderRead(sr);
		System.out.println("read(): " + (char) c1);

		// read(char[], int, int)
		char[] buf = new char[5];
		int n = t.StringReaderReadCharArrayIntInt(sr, buf, 0, buf.length);
		System.out.println("read(buf): " + new String(buf, 0, n));

		// ready()
		System.out.println("ready(): " + t.StringReaderReadReady(sr));

		// markSupported()
		System.out.println("markSupported(): " + t.StringReaderReadMarkSupported(sr));

		// mark + read + reset
		if (t.StringReaderReadMarkSupported(sr)) {
			t.StringReaderMark(sr, 100);
			int c2 = t.StringReaderRead(sr);
			System.out.println("after mark read(): " + (char) c2);
			t.StringReaderReset(sr);
			int c3 = t.StringReaderRead(sr);
			System.out.println("after reset read(): " + (char) c3);
		}

		// skip()
		long skipped = t.StringReaderReadSkip(sr, 3);
		System.out.println("skipped: " + skipped);

		// close()
		t.StringReaderClose(sr);

		System.out.println("DONE");
	}
}
