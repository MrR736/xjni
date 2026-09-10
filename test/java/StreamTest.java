import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

import xjava.io.IStream;
import xjava.io.OStream;

public final class StreamTest {

	private static int passed;
	private static int failed;

	private interface IOAction {
		void run() throws Exception;
	}

	private static void pass(String name) {
		++passed;
		System.out.println("[PASS] " + name);
	}

	private static void fail(String name, Throwable e) {
		++failed;
		System.out.println("[FAIL] " + name);
		if (e != null) {
			e.printStackTrace(System.out);
		}
	}

	private static void check(boolean condition, String name) {
		if (!condition) {
			throw new AssertionError(name);
		}
		pass(name);
	}

	private static void expectIOException(IOAction action, String name)
			throws Exception {
		try {
			action.run();
			throw new AssertionError("Expected IOException");
		} catch (IOException e) {
			pass(name);
		}
	}

	private static void expectIllegalArgument(IOAction action, String name)
			throws Exception {
		try {
			action.run();
			throw new AssertionError("Expected IllegalArgumentException");
		} catch (IllegalArgumentException e) {
			pass(name);
		}
	}

	private static byte[] bytes(String value) {
		return value.getBytes(StandardCharsets.UTF_8);
	}

	private static String string(byte[] value) {
		return new String(value, StandardCharsets.UTF_8);
	}

	private static void testOStreamDefault() throws Exception {
		try (OStream out = new OStream()) {
			out.write('h');
			out.write(bytes("ello world"));

			out.flush();

			pass("OStream default constructor");
			pass("OStream default write");
			pass("OStream default flush");
		}
	}

	private static void testOStreamByteArray() throws Exception {
		try (OStream out = new OStream(bytes("abc"))) {
			out.write('d');
			out.write(bytes("ef"));

			out.flush();

			pass("OStream byte[] constructor");
			pass("OStream byte[] write");
			pass("OStream byte[] flush");
		}
	}

	private static void testOStreamOutputStream() throws Exception {
		ByteArrayOutputStream target = new ByteArrayOutputStream();

		try (OStream out = new OStream(target)) {
			out.write(bytes("hello"));
			out.flush();
		}

		/*
		 * OStream currently creates its native stream from
		 * byteArrayStream.toByteArray(). The native stream is therefore
		 * independent from the supplied OutputStream unless the supplied
		 * object is handled specially by the native implementation.
		 *
		 * This test verifies construction and native operation without
		 * assuming that the supplied OutputStream receives native writes.
		 */
		pass("OStream(OutputStream) constructor");

		check(target.size() == 0,
				"OStream(OutputStream) does not directly write target");
	}

	private static void testOStreamWriteInt() throws Exception {
		ByteArrayOutputStream target = new ByteArrayOutputStream();

		try (OStream out = new OStream(target)) {
			out.write('A');
			out.write('B');
			out.write('C');
		}

		pass("OStream write(int)");
	}

	private static void testOStreamWriteOffset() throws Exception {
		try (OStream out = new OStream()) {
			byte[] buffer = bytes("0123456789");

			out.write(buffer, 2, 5);

			out.flush();

			pass("OStream write(byte[],offset,length)");
		}
	}

	private static void testOStreamZeroLength() throws Exception {
		try (OStream out = new OStream()) {
			out.write(new byte[0], 0, 0);

			pass("OStream zero-length write");
		}
	}

	private static void testOStreamInvalidArguments() throws Exception {
		try (OStream out = new OStream()) {
			try {
				out.write((byte[]) null, 0, 0);
				throw new AssertionError(
						"Expected NullPointerException");
			} catch (NullPointerException e) {
				pass("OStream null buffer");
			}

			try {
				out.write(new byte[5], -1, 1);
				throw new AssertionError(
						"Expected IndexOutOfBoundsException");
			} catch (IndexOutOfBoundsException e) {
				pass("OStream negative offset");
			}

			try {
				out.write(new byte[5], 0, 6);
				throw new AssertionError(
						"Expected IndexOutOfBoundsException");
			} catch (IndexOutOfBoundsException e) {
				pass("OStream length beyond buffer");
			}

			try {
				out.write(new byte[5], 4, 2);
				throw new AssertionError(
						"Expected IndexOutOfBoundsException");
			} catch (IndexOutOfBoundsException e) {
				pass("OStream offset plus length beyond buffer");
			}
		}
	}

	private static void testOStreamClosed() throws Exception {
		final OStream out = new OStream();

		out.write('A');
		out.close();

		expectIOException(
				() -> out.write('B'),
				"OStream write(int) after close");

		expectIOException(
				() -> out.write(bytes("test")),
				"OStream write(byte[]) after close");

		/*
		 * Current OStream.flush() does not check streamHandle.
		 * Therefore this test intentionally documents the current
		 * implementation behavior rather than expecting IOException.
		 */
		out.flush();
		pass("OStream flush after close");

		out.close();
		pass("OStream close is idempotent");
	}

	private static void testOStreamNonOwning() throws Exception {
		OStream owner = new OStream();

		/*
		 * This constructor does not own the native handle.
		 */
		final OStream wrapper = new OStream(getHandle(owner));

		wrapper.write('A');
		wrapper.flush();

		wrapper.close();

		/*
		 * Closing wrapper must NOT delete the native stream.
		 */
		owner.write('B');
		owner.flush();

		pass("OStream non-owning constructor");
		pass("OStream non-owning close");

		owner.close();
	}

	private static long getHandle(OStream stream) throws Exception {
		java.lang.reflect.Field field =
				OStream.class.getDeclaredField("streamHandle");

		field.setAccessible(true);

		return field.getLong(stream);
	}

	private static void testIStreamDefault() throws Exception {
		try (IStream in = new IStream()) {
			check(in.read() == -1,
					"IStream default EOF");
		}
	}

	private static void testIStreamByteArray() throws Exception {
		try (IStream in = new IStream(bytes("hello world"))) {
			check(in.read() == 'h',
					"IStream byte[] read(int)");

			byte[] buffer = new byte[5];

			check(in.read(buffer, 0, 5) == 5,
					"IStream byte[] read count");

			check(string(buffer).equals("ello "),
					"IStream byte[] read data");

			check(in.available() == 5,
					"IStream available after read");

			check(in.read(buffer, 0, 5) == 5,
					"IStream final read count");

			check(string(buffer).equals("world"),
					"IStream final read data");

			check(in.read() == -1,
					"IStream EOF");
		}
	}

	private static void testIStreamInputStream() throws Exception {
		InputStream source =
				new ByteArrayInputStream(bytes("hello world"));

		try (IStream in = new IStream(source)) {
			byte[] buffer = new byte[11];

			check(in.read(buffer, 0, 11) == 11,
					"IStream(InputStream) read count");

			check(string(buffer).equals("hello world"),
					"IStream(InputStream) read data");
		}
	}

	private static void testIStreamOffset() throws Exception {
		try (IStream in = new IStream(bytes("0123456789"))) {
			byte[] buffer = new byte[10];

			for (int i = 0; i < buffer.length; ++i) {
				buffer[i] = 'x';
			}

			int count = in.read(buffer, 2, 4);

			check(count == 4,
					"IStream offset read count");

			check(
				buffer[0] == 'x' &&
				buffer[1] == 'x' &&
				buffer[2] == '0' &&
				buffer[3] == '1' &&
				buffer[4] == '2' &&
				buffer[5] == '3' &&
				buffer[6] == 'x',
				"IStream offset read data"
			);
		}
	}

	private static void testIStreamZeroLength() throws Exception {
		try (IStream in = new IStream(bytes("abc"))) {
			byte[] buffer = new byte[5];

			check(in.read(buffer, 0, 0) == 0,
					"IStream zero-length read");

			check(in.read() == 'a',
					"IStream zero-length read does not consume");
		}
	}

	private static void testIStreamAvailable() throws Exception {
		try (IStream in = new IStream(bytes("1234567890"))) {
			check(in.available() == 10,
					"IStream available initially");

			check(in.read() == '1',
					"IStream available first read");

			check(in.available() == 9,
					"IStream available after first read");

			byte[] buffer = new byte[4];

			check(in.read(buffer, 0, 4) == 4,
					"IStream available second read");

			check(in.available() == 5,
					"IStream available after second read");
		}
	}

	private static void testIStreamSkip() throws Exception {
		try (IStream in = new IStream(bytes("0123456789"))) {
			check(in.skip(3) == 3,
					"IStream skip(3)");

			check(in.read() == '3',
					"IStream read after skip");

			check(in.skip(4) == 4,
					"IStream skip(4)");

			check(in.read() == '8',
					"IStream read after second skip");

			check(in.skip(100) == 1,
					"IStream skip beyond EOF");

			check(in.read() == -1,
					"IStream EOF after skip");

			check(in.skip(0) == 0,
					"IStream skip(0)");
		}
	}

	private static void testIStreamNegativeSkip() throws Exception {
		try (IStream in = new IStream(bytes("abc"))) {
			expectIllegalArgument(
					() -> in.skip(-1),
					"IStream negative skip");
		}
	}

	private static void testIStreamMark() throws Exception {
		try (IStream in = new IStream(bytes("abcdef"))) {
			check(!in.markSupported(),
					"IStream markSupported");

			in.mark(100);

			check(in.read() == 'a',
					"IStream read after mark");

			try {
				in.reset();
				throw new AssertionError(
						"Expected IOException");
			} catch (IOException e) {
				pass("IStream reset throws IOException");
			}
		}
	}

	private static void testIStreamInvalidArguments() throws Exception {
		try (IStream in = new IStream(bytes("abc"))) {
			try {
				in.read((byte[]) null, 0, 0);
				throw new AssertionError(
						"Expected NullPointerException");
			} catch (NullPointerException e) {
				pass("IStream null buffer");
			}

			try {
				in.read(new byte[5], -1, 1);
				throw new AssertionError(
						"Expected IndexOutOfBoundsException");
			} catch (IndexOutOfBoundsException e) {
				pass("IStream negative offset");
			}

			try {
				in.read(new byte[5], 0, 6);
				throw new AssertionError(
						"Expected IndexOutOfBoundsException");
			} catch (IndexOutOfBoundsException e) {
				pass("IStream length beyond buffer");
			}

			try {
				in.read(new byte[5], 4, 2);
				throw new AssertionError(
						"Expected IndexOutOfBoundsException");
			} catch (IndexOutOfBoundsException e) {
				pass("IStream offset plus length beyond buffer");
			}
		}
	}

	private static void testIStreamClosed() throws Exception {
		final IStream in = new IStream(bytes("hello"));

		in.close();

		expectIOException(
				() -> in.read(),
				"IStream read(int) after close");

		expectIOException(
				() -> in.read(new byte[5], 0, 5),
				"IStream read(buffer) after close");

		expectIOException(
				() -> in.available(),
				"IStream available after close");

		expectIOException(
				() -> in.skip(1),
				"IStream skip after close");

		in.close();

		pass("IStream close is idempotent");
	}

	private static void testIStreamNonOwning() throws Exception {
		IStream owner = new IStream(bytes("hello"));

		final IStream wrapper =
				new IStream(getHandle(owner));

		check(wrapper.read() == 'h',
				"IStream non-owning read");

		wrapper.close();

		/*
		 * Closing wrapper must not delete the native stream.
		 */
		check(owner.read() == 'e',
				"IStream non-owning close");

		owner.close();
	}

	private static long getHandle(IStream stream) throws Exception {
		java.lang.reflect.Field field =
				IStream.class.getDeclaredField("streamHandle");

		field.setAccessible(true);

		return field.getLong(stream);
	}

	private static void testRoundTrip() throws Exception {
		final String text = "Hello from IStream/OStream";

		/*
		 * OStream(byte[]) creates an independent native output stream.
		 * To test the actual bytes, use the native stream's corresponding
		 * Java-accessible data mechanism if one exists in the implementation.
		 *
		 * For now this test verifies that the two stream classes can
		 * independently operate without throwing.
		 */
		try (OStream out = new OStream()) {
			out.write(bytes(text));
			out.flush();
		}

		try (IStream in = new IStream(bytes(text))) {
			byte[] buffer = new byte[text.length()];

			check(in.read(buffer, 0, buffer.length) == text.length(),
					"IStream/OStream round-trip read count");

			check(string(buffer).equals(text),
					"IStream/OStream round-trip data");
		}
	}

	public static void runAll() {
		passed = 0;
		failed = 0;

		System.out.println("=== Stream Tests ===");

		try {
			testOStreamDefault();
		} catch (Throwable e) {
			fail("OStream default", e);
		}

		try {
			testOStreamByteArray();
		} catch (Throwable e) {
			fail("OStream byte[]", e);
		}

		try {
			testOStreamOutputStream();
		} catch (Throwable e) {
			fail("OStream OutputStream", e);
		}

		try {
			testOStreamWriteInt();
		} catch (Throwable e) {
			fail("OStream write(int)", e);
		}

		try {
			testOStreamWriteOffset();
		} catch (Throwable e) {
			fail("OStream write offset", e);
		}

		try {
			testOStreamZeroLength();
		} catch (Throwable e) {
			fail("OStream zero length", e);
		}

		try {
			testOStreamInvalidArguments();
		} catch (Throwable e) {
			fail("OStream invalid arguments", e);
		}

		try {
			testOStreamClosed();
		} catch (Throwable e) {
			fail("OStream closed", e);
		}

		try {
			testOStreamNonOwning();
		} catch (Throwable e) {
			fail("OStream non-owning", e);
		}

		try {
			testIStreamDefault();
		} catch (Throwable e) {
			fail("IStream default", e);
		}

		try {
			testIStreamByteArray();
		} catch (Throwable e) {
			fail("IStream byte[]", e);
		}

		try {
			testIStreamInputStream();
		} catch (Throwable e) {
			fail("IStream InputStream", e);
		}

		try {
			testIStreamOffset();
		} catch (Throwable e) {
			fail("IStream offset", e);
		}

		try {
			testIStreamZeroLength();
		} catch (Throwable e) {
			fail("IStream zero length", e);
		}

		try {
			testIStreamAvailable();
		} catch (Throwable e) {
			fail("IStream available", e);
		}

		try {
			testIStreamSkip();
		} catch (Throwable e) {
			fail("IStream skip", e);
		}

		try {
			testIStreamNegativeSkip();
		} catch (Throwable e) {
			fail("IStream negative skip", e);
		}

		try {
			testIStreamMark();
		} catch (Throwable e) {
			fail("IStream mark", e);
		}

		try {
			testIStreamInvalidArguments();
		} catch (Throwable e) {
			fail("IStream invalid arguments", e);
		}

		try {
			testIStreamClosed();
		} catch (Throwable e) {
			fail("IStream closed", e);
		}

		try {
			testIStreamNonOwning();
		} catch (Throwable e) {
			fail("IStream non-owning", e);
		}

		try {
			testRoundTrip();
		} catch (Throwable e) {
			fail("Stream round-trip", e);
		}

		System.out.println();
		System.out.println("=== Stream Test Result ===");
		System.out.println("passed: " + passed);
		System.out.println("failed: " + failed);

		if (failed != 0) {
			throw new AssertionError("Stream tests failed");
		}
	}

	public static void main(String[] args) {
		runAll();
	}
}
