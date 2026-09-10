package xjava.io;

import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import xjava.XJNILibrary;

public final class IFStream extends FileInputStream implements Closeable {
	static { XJNILibrary.ensureLoaded(); }

	private long streamHandle;

	public IFStream(String name) throws FileNotFoundException {
		super(name);
		long handle = nativeNew(name);
		if (handle == 0) {
			try {
				super.close();
			} catch (IOException e) {
				// Preserve the original FileNotFoundException.
			}
			throw new FileNotFoundException(name);
		}
		streamHandle = handle;
	}

	public IFStream(File file) throws FileNotFoundException {
		super(file);
		long handle = nativeNew(file.getPath());
		if (handle == 0) {
			try {
				super.close();
			} catch (IOException e) {
				// Preserve the original FileNotFoundException.
			}
			throw new FileNotFoundException(file.getPath());
		}
		streamHandle = handle;
	}

	@Override
	public int read() throws IOException {
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		return nativeRead(handle);
	}

	@Override
	public int read(byte[] buffer,int offset,int length) throws IOException {
		if (buffer == null) { throw new NullPointerException("buffer"); }
		if (offset < 0 || length < 0 || offset > buffer.length - length) { throw new IndexOutOfBoundsException(); }
		if (length == 0) { return 0; }
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		return nativeRead(handle,buffer,offset,length);
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

	@Override
	public int available() throws IOException {
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		return nativeAvailable(handle);
	}

	@Override
	public long skip(long count) throws IOException {
		if (count <= 0) { return 0; }
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		return nativeSkip(handle, count);
	}

	@Override
	public synchronized void mark(int readLimit) {
		// Native std::ifstream does not support Java mark/reset semantics.
	}

	@Override
	public synchronized void reset() throws IOException {
		throw new IOException("Mark not supported");
	}

	@Override
	public boolean markSupported() {
		return false;
	}

	private static native long nativeNew(String name);
	private static native void nativeDelete(long streamHandle);
	private static native int nativeRead(long streamHandle);
	private static native int nativeRead(long streamHandle,byte[] buffer,int offset,int length);
	private static native int nativeAvailable(long streamHandle);
	private static native long nativeSkip(long streamHandle,long count);
}
