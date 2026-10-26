package xjava.io;

import java.io.Closeable;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import xjava.XJNILibrary;

public final class OFStream extends FileOutputStream implements Closeable {
	static { XJNILibrary.ensureLoaded(); }

	private long streamHandle;

	public OFStream(String name, boolean append) throws FileNotFoundException {
		super(name,append);
		long handle = nativeNew(name, append);
		if (handle == 0) {
			try {
				super.close();
			} catch (IOException e) {
				// Preserve the original FileNotFoundException.
			}
			throw new FileNotFoundException(name);
		}
		this.streamHandle = handle;
	}

	public OFStream(String name) throws FileNotFoundException {
		this(name, false);
	}

	public OFStream(File file, boolean append) throws FileNotFoundException {
		this(file.getPath(), append);
	}

	public OFStream(File file) throws FileNotFoundException {
		this(file.getPath(), false);
	}

	@Override
	public void write(int value) throws IOException {
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		nativeWrite(handle, value);
	}

	@Override
	public void write(byte[] buffer,int offset,int length) throws IOException {
		if (buffer == null) { throw new NullPointerException("buffer"); }
		if (offset < 0 || length < 0 || offset > buffer.length - length) { throw new IndexOutOfBoundsException(); }
		if (length == 0) { return; }
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		nativeWrite(handle,buffer,offset,length);
	}

	@Override
	public void flush() throws IOException {
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		nativeFlush(handle);
	}

	@Override
	public void close() throws IOException {
		long handle = streamHandle;
		if (handle == 0) { return; }
		streamHandle = 0;
		try {
			nativeDelete(handle);
		} finally {
			super.close();
		}
	}

	private static native long nativeNew(String buffer,boolean append);
	private static native void nativeDelete(long streamHandle);
	private static native void nativeWrite(long streamHandle,int value);
	private static native void nativeWrite(long streamHandle,byte[] buffer,int offset,int length);
	private static native void nativeFlush(long streamHandle);
}
