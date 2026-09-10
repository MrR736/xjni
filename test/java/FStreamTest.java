import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

import xjava.io.IFStream;
import xjava.io.OFStream;

public final class FStreamTest {

	private static int passed;
	private static int failed;

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

	private interface IOAction {
		void run() throws Exception;
	}

	private static File tempFile(String prefix) throws IOException {
		File file = File.createTempFile(prefix, ".tmp");
		file.deleteOnExit();
		return file;
	}

	private static void delete(File file) {
		if (file != null) {
			try {
				file.delete();
			} catch (Exception ignored) {
			}
		}
	}

	private static void testIFStreamConstructors() throws Exception {
		File file = tempFile("xjni-ifstream-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write("hello world".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				check(in.read() == 'h',
						"IFStream(File): read()");
			}

			try (IFStream in = new IFStream(file.getPath())) {
				check(in.read() == 'h',
						"IFStream(String): read()");
			}

		} finally {
			delete(file);
		}
	}

	private static void testIFStreamRead() throws Exception {
		File file = tempFile("xjni-ifstream-read-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write("hello world".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				check(in.read() == 'h',
						"IFStream read single byte");

				byte[] buffer = new byte[5];

				int n = in.read(buffer, 0, 5);

				check(n == 5,
						"IFStream read(byte[],offset,length) count");

				check(new String(buffer, StandardCharsets.UTF_8)
						.equals("ello "),
						"IFStream read(byte[],offset,length) data");

				n = in.read(buffer, 0, 5);

				check(n == 5,
						"IFStream second read count");

				check(new String(buffer, StandardCharsets.UTF_8)
						.equals("world"),
						"IFStream second read data");

				check(in.read() == -1,
						"IFStream EOF");
			}

		} finally {
			delete(file);
		}
	}

	private static void testIFStreamOffset() throws Exception {
		File file = tempFile("xjni-ifstream-offset-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write("0123456789".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				byte[] buffer = new byte[10];

				for (int i = 0; i < buffer.length; ++i) {
					buffer[i] = 'x';
				}

				int n = in.read(buffer, 2, 4);

				check(n == 4,
						"IFStream offset read count");

				check(buffer[0] == 'x' &&
				      buffer[1] == 'x' &&
				      buffer[2] == '0' &&
				      buffer[3] == '1' &&
				      buffer[4] == '2' &&
				      buffer[5] == '3' &&
				      buffer[6] == 'x',
						"IFStream offset read data");
			}

		} finally {
			delete(file);
		}
	}

	private static void testIFStreamAvailable() throws Exception {
		File file = tempFile("xjni-ifstream-available-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write("1234567890".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				check(in.available() == 10,
						"IFStream available initially");

				check(in.read() == '1',
						"IFStream available read");

				check(in.available() == 9,
						"IFStream available after read");

				byte[] buffer = new byte[4];

				check(in.read(buffer, 0, 4) == 4,
						"IFStream available second read");

				check(in.available() == 5,
						"IFStream available after second read");
			}

		} finally {
			delete(file);
		}
	}

	private static void testIFStreamSkip() throws Exception {
		File file = tempFile("xjni-ifstream-skip-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write("0123456789".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				check(in.skip(3) == 3,
						"IFStream skip(3)");

				check(in.read() == '3',
						"IFStream read after skip");

				check(in.skip(4) == 4,
						"IFStream skip(4)");

				check(in.read() == '8',
						"IFStream read after second skip");

				check(in.skip(100) == 1,
						"IFStream skip beyond EOF");

				check(in.read() == -1,
						"IFStream EOF after skip beyond EOF");

				check(in.skip(0) == 0,
						"IFStream skip(0)");

				check(in.skip(-1) == 0,
						"IFStream skip(-1)");
			}

		} finally {
			delete(file);
		}
	}

	private static void testIFStreamMark() throws Exception {
		File file = tempFile("xjni-ifstream-mark-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write("abcdef".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				check(!in.markSupported(),
						"IFStream markSupported()");

				in.mark(100);

				check(in.read() == 'a',
						"IFStream read after mark");

				try {
					in.reset();
					throw new AssertionError(
							"reset() should throw IOException");
				} catch (IOException e) {
					pass("IFStream reset() throws IOException");
				}
			}

		} finally {
			delete(file);
		}
	}

	private static void testIFStreamClosed() throws Exception {
		File file = tempFile("xjni-ifstream-closed-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write("hello".getBytes(StandardCharsets.UTF_8));
			}

			final IFStream in = new IFStream(file);

			try {
				in.close();

				expectIOException(
						() -> in.read(),
						"IFStream read() after close");

				expectIOException(
						() -> in.read(new byte[10], 0, 10),
						"IFStream read(buffer) after close");

				expectIOException(
						() -> in.available(),
						"IFStream available() after close");

				expectIOException(
						() -> in.skip(1),
						"IFStream skip() after close");

				in.close();

				pass("IFStream close() is idempotent");
			} finally {
				try {
					in.close();
				} catch (IOException ignored) {
				}
			}

		} finally {
			delete(file);
		}
	}

	private static void testOFStreamConstructors() throws Exception {
		File file = tempFile("xjni-ofstream-constructors-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write('A');
			}

			try (IFStream in = new IFStream(file)) {
				check(in.read() == 'A',
						"OFStream(File): write");
			}

			try (OFStream out = new OFStream(file.getPath(), false)) {
				out.write('B');
			}

			try (IFStream in = new IFStream(file)) {
				check(in.read() == 'B',
						"OFStream(String, false): truncate");
				check(in.read() == -1,
						"OFStream(String, false): only new data");
			}

		} finally {
			delete(file);
		}
	}

	private static void testOFStreamWrite() throws Exception {
		File file = tempFile("xjni-ofstream-write-");

		try {
			try (OFStream out = new OFStream(file)) {
				out.write('H');

				byte[] data = "ello world".getBytes(
						StandardCharsets.UTF_8);

				out.write(data, 0, data.length);

				out.flush();
			}

			try (IFStream in = new IFStream(file)) {
				byte[] buffer = new byte[11];

				check(in.read(buffer, 0, 11) == 11,
						"OFStream write count");

				check(new String(buffer, StandardCharsets.UTF_8)
						.equals("Hello world"),
						"OFStream write data");
			}

		} finally {
			delete(file);
		}
	}

	private static void testOFStreamOffset() throws Exception {
		File file = tempFile("xjni-ofstream-offset-");

		try {
			try (OFStream out = new OFStream(file)) {
				byte[] buffer = "xxxxxxxxxx".getBytes(
						StandardCharsets.UTF_8);

				out.write(buffer, 0, buffer.length);

				byte[] data = "012345".getBytes(
						StandardCharsets.UTF_8);

				out.write(data, 1, 4);
			}

			try (IFStream in = new IFStream(file)) {
				byte[] buffer = new byte[14];

				int n = in.read(buffer, 0, buffer.length);

				check(n == 14,
						"OFStream offset write count");

				check(new String(buffer, StandardCharsets.UTF_8)
						.equals("xxxxxxxxxx1234"),
						"OFStream offset write data");
			}

		} finally {
			delete(file);
		}
	}

	private static void testOFStreamAppend() throws Exception {
		File file = tempFile("xjni-ofstream-append-");

		try {
			try (OFStream out = new OFStream(file, false)) {
				out.write("hello".getBytes(StandardCharsets.UTF_8));
			}

			try (OFStream out = new OFStream(file, true)) {
				out.write(" world".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				byte[] buffer = new byte[11];

				check(in.read(buffer, 0, 11) == 11,
						"OFStream append count");

				check(new String(buffer, StandardCharsets.UTF_8)
						.equals("hello world"),
						"OFStream append data");
			}

		} finally {
			delete(file);
		}
	}

	private static void testOFStreamTruncate() throws Exception {
		File file = tempFile("xjni-ofstream-truncate-");

		try {
			try (OFStream out = new OFStream(file, false)) {
				out.write("0123456789".getBytes(
						StandardCharsets.UTF_8));
			}

			try (OFStream out = new OFStream(file, false)) {
				out.write("abc".getBytes(StandardCharsets.UTF_8));
			}

			try (IFStream in = new IFStream(file)) {
				byte[] buffer = new byte[10];

				check(in.read(buffer, 0, 10) == 3,
						"OFStream truncate count");

				check(new String(buffer, 0, 3,
						StandardCharsets.UTF_8).equals("abc"),
						"OFStream truncate data");

				check(in.read() == -1,
						"OFStream truncate EOF");
			}

		} finally {
			delete(file);
		}
	}

	private static void testOFStreamFlush() throws Exception {
		File file = tempFile("xjni-ofstream-flush-");

		OFStream out = null;
		IFStream in = null;

		try {
			out = new OFStream(file);

			out.write("hello".getBytes(StandardCharsets.UTF_8));
			out.flush();

			in = new IFStream(file);

			byte[] buffer = new byte[5];

			check(in.read(buffer, 0, 5) == 5,
					"OFStream flush count");

			check(new String(buffer, StandardCharsets.UTF_8)
					.equals("hello"),
					"OFStream flush data");

		} finally {
			if (in != null) {
				try {
					in.close();
				} catch (IOException ignored) {
				}
			}

			if (out != null) {
				try {
					out.close();
				} catch (IOException ignored) {
				}
			}

			delete(file);
		}
	}

	private static void testOFStreamClosed() throws Exception {
		File file = tempFile("xjni-ofstream-closed-");

		try {
    		final OFStream out = new OFStream(file);

			try {
    			out.close();

    			expectIOException(
						() -> out.write(1),
						"OFStream write() after close");

    			expectIOException(
						() -> out.write(new byte[10], 0, 10),
						"OFStream write(buffer) after close");

    			expectIOException(
						() -> out.flush(),
						"OFStream flush() after close");

    			out.close();

    			pass("OFStream close() is idempotent");
			} finally {
    			try {
					out.close();
				} catch (IOException ignored) {
				}
			}

		} finally {
			delete(file);
    	}
	}



	private static void testInvalidArguments() throws Exception {
		File file = tempFile("xjni-fstream-invalid-");

		try {
			try (OFStream out = new OFStream(file)) {

				try {
					out.write((byte[]) null, 0, 0);
					throw new AssertionError(
							"Expected NullPointerException");
				} catch (NullPointerException e) {
					pass("OFStream null buffer");
				}

				try {
					out.write(new byte[5], -1, 1);
					throw new AssertionError(
							"Expected IndexOutOfBoundsException");
				} catch (IndexOutOfBoundsException e) {
					pass("OFStream negative offset");
				}

				try {
					out.write(new byte[5], 0, 6);
					throw new AssertionError(
							"Expected IndexOutOfBoundsException");
				} catch (IndexOutOfBoundsException e) {
					pass("OFStream length beyond buffer");
				}
			}

			try (IFStream in = new IFStream(file)) {

				try {
					in.read((byte[]) null, 0, 0);
					throw new AssertionError(
							"Expected NullPointerException");
				} catch (NullPointerException e) {
					pass("IFStream null buffer");
				}

				try {
					in.read(new byte[5], -1, 1);
					throw new AssertionError(
							"Expected IndexOutOfBoundsException");
				} catch (IndexOutOfBoundsException e) {
					pass("IFStream negative offset");
				}

				try {
					in.read(new byte[5], 0, 6);
					throw new AssertionError(
							"Expected IndexOutOfBoundsException");
				} catch (IndexOutOfBoundsException e) {
					pass("IFStream length beyond buffer");
				}
			}

		} finally {
			delete(file);
		}
	}

	public static void runAll() {
		passed = 0;
		failed = 0;

		System.out.println("=== FStream Tests ===");

		try {
			testIFStreamConstructors();
		} catch (Throwable e) {
			fail("IFStream constructors", e);
		}

		try {
			testIFStreamRead();
		} catch (Throwable e) {
			fail("IFStream read", e);
		}

		try {
			testIFStreamOffset();
		} catch (Throwable e) {
			fail("IFStream offset", e);
		}

		try {
			testIFStreamAvailable();
		} catch (Throwable e) {
			fail("IFStream available", e);
		}

		try {
			testIFStreamSkip();
		} catch (Throwable e) {
			fail("IFStream skip", e);
		}

		try {
			testIFStreamMark();
		} catch (Throwable e) {
			fail("IFStream mark/reset", e);
		}

		try {
			testIFStreamClosed();
		} catch (Throwable e) {
			fail("IFStream closed", e);
		}

		try {
			testOFStreamConstructors();
		} catch (Throwable e) {
			fail("OFStream constructors", e);
		}

		try {
			testOFStreamWrite();
		} catch (Throwable e) {
			fail("OFStream write", e);
		}

		try {
			testOFStreamOffset();
		} catch (Throwable e) {
			fail("OFStream offset", e);
		}

		try {
			testOFStreamAppend();
		} catch (Throwable e) {
			fail("OFStream append", e);
		}

		try {
			testOFStreamTruncate();
		} catch (Throwable e) {
			fail("OFStream truncate", e);
		}

		try {
			testOFStreamFlush();
		} catch (Throwable e) {
			fail("OFStream flush", e);
		}

		try {
			testOFStreamClosed();
		} catch (Throwable e) {
			fail("OFStream closed", e);
		}

		try {
			testInvalidArguments();
		} catch (Throwable e) {
			fail("Invalid arguments", e);
		}

		System.out.println();
		System.out.println("=== FStream Test Result ===");
		System.out.println("passed: " + passed);
		System.out.println("failed: " + failed);

		if (failed != 0) {
			throw new AssertionError("FStream tests failed");
		}
	}

	public static void main(String[] args) {
		runAll();
	}
}
