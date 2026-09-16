package xjava.io;

import static org.junit.Assert.*;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class FILETest {

	private File tempFile;

	@Before
	public void setUp() throws Exception {
		tempFile = File.createTempFile("xjava-file-test-", ".tmp");
		assertTrue(tempFile.exists());
	}

	@After
	public void tearDown() {
		if (tempFile != null) {
			tempFile.delete();
		}
	}

	@Test
	public void testInitialState() {
		FILE file = new FILE(tempFile.getPath());

		assertFalse(file.isOpen());
		assertEquals(tempFile.getPath(), file.getPath());
	}

	@Test
	public void testFromFile() {
		File normalFile = new File(tempFile.getPath());

		FILE file = FILE.fromFile(normalFile);

		assertNotNull(file);
		assertEquals(normalFile.getPath(), file.getPath());
		assertFalse(file.isOpen());
	}

	@Test
	public void testFromFileNull() {
		assertThrows(
			NullPointerException.class,
			() -> FILE.fromFile(null)
		);
	}

	@Test
	public void testToFile() {
		FILE file = new FILE(tempFile.getPath());

		File result = FILE.toFile(file);

		assertSame(file, result);
	}

	@Test
	public void testToFileNull() {
		assertNull(FILE.toFile(null));
	}

	@Test
	public void testOpenRead() throws Exception {
		byte[] data = "hello xjava".getBytes(StandardCharsets.UTF_8);
		Files.write(tempFile.toPath(), data);

		FILE file = new FILE(tempFile.getPath());

		assertFalse(file.isOpen());

		file.open("rb");

		try {
			assertTrue(file.isOpen());
		} finally {
			file.close();
		}

		assertFalse(file.isOpen());
	}

	@Test
	public void testOpenWrite() throws Exception {
		FILE file = new FILE(tempFile.getPath());

		file.open("wb");

		try {
			assertTrue(file.isOpen());
		} finally {
			file.close();
		}

		assertFalse(file.isOpen());
		assertTrue(tempFile.exists());
	}

	@Test
	public void testOpenAppend() throws Exception {
		Files.write(
			tempFile.toPath(),
			"first".getBytes(StandardCharsets.UTF_8)
		);

		FILE file = new FILE(tempFile.getPath());

		file.open("ab");

		try {
			assertTrue(file.isOpen());
		} finally {
			file.close();
		}

		assertFalse(file.isOpen());
	}

	@Test
	public void testModeConstructor() throws Exception {
		FILE file = new FILE(
			tempFile.getParent(),
			tempFile.getName(),
			"wb"
		);

		try {
			assertTrue(file.isOpen());
		} finally {
			file.close();
		}

		assertFalse(file.isOpen());
	}

	@Test
	public void testParentChildConstructor() throws Exception {
		File parent = tempFile.getParentFile();

		FILE file = new FILE(
			parent.getPath(),
			tempFile.getName()
		);

		assertEquals(tempFile.getPath(), file.getPath());
		assertFalse(file.isOpen());
	}

	@Test
	public void testNullMode() {
		FILE file = new FILE(tempFile.getPath());

		assertThrows(
			NullPointerException.class,
			() -> file.open(null)
		);
	}

	@Test
	public void testDoubleOpen() throws Exception {
		FILE file = new FILE(tempFile.getPath());

		file.open("rb");

		try {
			assertThrows(
				IllegalStateException.class,
				() -> file.open("rb")
			);
		} finally {
			file.close();
		}
	}

	@Test
	public void testClose() throws Exception {
		FILE file = new FILE(tempFile.getPath());

		file.open("rb");
		assertTrue(file.isOpen());

		file.close();

		assertFalse(file.isOpen());
	}

	@Test
	public void testCloseWithoutOpen() throws Exception {
		FILE file = new FILE(tempFile.getPath());

		assertFalse(file.isOpen());

		file.close();

		assertFalse(file.isOpen());
	}

	@Test
	public void testCloseTwice() throws Exception {
		FILE file = new FILE(tempFile.getPath());

		file.open("rb");

		file.close();
		assertFalse(file.isOpen());

		file.close();
		assertFalse(file.isOpen());
	}

	@Test
	public void testReopenAfterClose() throws Exception {
		FILE file = new FILE(tempFile.getPath());

		file.open("rb");
		assertTrue(file.isOpen());

		file.close();
		assertFalse(file.isOpen());

		file.open("rb");
		assertTrue(file.isOpen());

		file.close();
		assertFalse(file.isOpen());
	}

	@Test
	public void testMultipleInstances() throws Exception {
		FILE file1 = new FILE(tempFile.getPath());
		FILE file2 = new FILE(tempFile.getPath());

		file1.open("rb");

		try {
			assertTrue(file1.isOpen());
			assertFalse(file2.isOpen());
		} finally {
			file1.close();
		}

		file2.open("rb");

		try {
			assertTrue(file2.isOpen());
			assertFalse(file1.isOpen());
		} finally {
			file2.close();
		}
	}

	@Test
	public void testNonexistentFile() {
		File missing = new File(
			tempFile.getParentFile(),
			"xjava-file-does-not-exist-" + System.nanoTime()
		);

		FILE file = new FILE(missing.getPath());

		file.open("rb");

		/*
		 * Current FILE.open() API does not throw IOException.
		 * fopen() failure therefore appears as !isOpen().
		 */
		assertFalse(file.isOpen());
	}

	@Test
	public void testWriteCreatesFile() throws Exception {
		File output = new File("/tmp", "created.bin");
		assertFalse(output.exists());
		FILE file = new FILE(output.getPath());
		try {
			file.open("wb");
			assertTrue(file.isOpen());
		} finally {
			file.close();
		}
		assertFalse(file.isOpen());
		assertTrue(output.exists());
	}
}
