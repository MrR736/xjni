import java.math.BigInteger;
import java.math.BigDecimal;

public class TestXJNIPrintf {

	static {
		System.loadLibrary("xjni_test");
	}

	private native String formatWithJNI(String format, Object... args);

	public static void main(String[] args) {
		TestXJNIPrintf t = new TestXJNIPrintf();

		String result = t.formatWithJNI(
			"String: %-12s\n" +
			"Integer: %+06d, Hex: %04X\n" +
			"Long: %20d, Hex: %016X\n" +
			"Float: %08.2f, Double: %10.4f, Scientific: %.3e\n" +
			"Boolean: %b, Char: %c, Percent: %%\n" +
			"BigInteger: %s\nBigDecimal: %s\n",
			"Hello",
			Integer.valueOf(42), 255,
			Long.valueOf(1234567890123L), Long.valueOf(1234567890123L),
			Float.valueOf(3.14159f), Double.valueOf(2.71828), Double.valueOf(0.00123),
			Boolean.TRUE, Character.valueOf('界'),
			new BigInteger("123456789012345678901234567890"),
			new BigDecimal("3.141592653589793238462643383279")
		);

		System.out.println(result);
	}
}
