import java.io.StringWriter;

public class TestStringWriter {

	static { System.loadLibrary("xjni_test"); }

	public native void StringWriterWriteCharArrayIntInt(StringWriter sw,char[] array,int offset,int length);
	public native void StringWriterWriteInt(StringWriter sw,int value);
	public native void StringWriterWriteString(StringWriter sw,String str);
	public native void StringWriterWriteStringIntInt(StringWriter sw,String str,int offset,int length);

	public native void StringWriterFlush(StringWriter sw);

	public native void StringWriterAppendChar(StringWriter sw,char ch);

	public static void main(String[] args) {
		TestStringWriter test = new TestStringWriter();
		StringWriter sw = new StringWriter();

		test.StringWriterWriteString(sw, "Hello ");
		test.StringWriterAppendChar(sw, 'W');
		test.StringWriterWriteString(sw, "orld");
		test.StringWriterAppendChar(sw, '!');
		test.StringWriterWriteInt(sw, 32); // Output: ' '
		test.StringWriterWriteInt(sw, 49); // Output: '1'
		test.StringWriterWriteInt(sw, 50); // Output: '2'
		test.StringWriterWriteInt(sw, 51); // Output: '3'

		test.StringWriterFlush(sw);

		System.out.println(sw.toString());
	}
}
