package xjava.lang;

import java.io.InputStream;
import java.io.PrintStream;
import java.io.IOException;
import java.io.Console;
import java.nio.channels.Channel;
import java.util.Properties;
import java.util.Map;

public final class XSystem {

	private XSystem() { }

	public static final InputStream in = System.in;
	public static final PrintStream out = System.out;
	public static final PrintStream err = System.err;

	public static void setIn(InputStream in) {
		System.setIn(in);
	}

	public static void setOut(PrintStream out) {
		System.setOut(out);
	}


	public static void setErr(PrintStream err) {
		System.setErr(err);
	}


	public static Console console() {
		return System.console();
	}

	public static Channel inheritedChannel() throws IOException {
		return System.inheritedChannel();
	}

	public static long currentTimeMillis() {
		return System.currentTimeMillis();
	}

	public static long nanoTime() {
		return System.nanoTime();
	}

	public static void arraycopy(Object src,  int  srcPos, Object dest, int destPos, int length) {
		System.arraycopy(src,srcPos,dest,destPos,length);
	}

	public static int identityHashCode(Object x) {
		return System.identityHashCode(x);
	}

	public static Properties getProperties() {
		return System.getProperties();
	}

	public static void setProperties(Properties props) {
		System.setProperties(props);
	}

	public static String lineSeparator() {
		return System.lineSeparator();
	}

	public static String getProperty(String key) {
		return System.getProperty(key);
	}

	public static String getProperty(String key, String def) {
		return System.getProperty(key, def);
	}

	public static String setProperty(String key, String value) {
		return System.setProperty(key, value);
	}

	public static String clearProperty(String key) {
		return System.clearProperty(key);
	}

	public static String[] clearProperty(String... keys) {
		if (keys == null) {
			throw new NullPointerException("keys");
		}

		String[] oldValues = new String[keys.length];

		for (int i = 0; i < keys.length; i++) {
			oldValues[i] = System.clearProperty(keys[i]);
		}

		return oldValues;
	}

	public static String getenv(String name) {
		return System.getenv(name);
	}

	public static String[] getenv(String... names) {
		if (names == null) {
			throw new NullPointerException("names");
		}

		String[] values = new String[names.length];
		for (int i = 0; i < names.length; i++) {
			values[i] = System.getenv(names[i]);
		}

		return values;
	}


	public static Map<String,String> getenv() {
		return System.getenv();
	}

	public static void exit(int status) {
		System.exit(status);
	}

	public static void gc() {
		System.gc();
	}

	public static void load(String filename) {
		System.load(filename);
	}

	public static void load(String... filenames) {
		if (filenames == null) {
			throw new NullPointerException("filenames");
		}

		for (String filename : filenames) {
			System.load(filename);
		}
	}

	public static void loadLibrary(String libname) {
		System.loadLibrary(libname);
	}

	public static void loadLibrary(String... libnames) {
		if (libnames == null) {
			throw new NullPointerException("libnames");
		}

		for (String libname : libnames) {
			System.loadLibrary(libname);
		}
	}

	public static String mapLibraryName(String libname) {
		return System.mapLibraryName(libname);
	}

	public static String[] mapLibraryName(String... libnames) {
		if (libnames == null) {
			throw new NullPointerException("libnames");
		}

		String[] mappedNames = new String[libnames.length];

		for (int i = 0; i < libnames.length; i++) {
			mappedNames[i] = System.mapLibraryName(libnames[i]);
		}

		return mappedNames;
	}
}
