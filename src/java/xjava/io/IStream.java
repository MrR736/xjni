package xjava.io;

import java.io.IOException;
import java.io.InputStream;

import xjava.XJNILibrary;

public final class IStream extends InputStream {
	static { XJNILibrary.ensureLoaded(); }

	private long streamHandle;
	private boolean ownsStream;

	public IStream() throws IOException {
		this(new byte[0]);
	}

	public IStream(InputStream stream) throws IOException {
		this(stream.readAllBytes());
	}

	public IStream(byte[] buffer) {
		this.streamHandle = nativeNew(buffer);
		this.ownsStream = true;
	}

	public IStream(long streamHandle) {
		this.streamHandle = streamHandle;
		this.ownsStream = false;
	}

	@Override
	public int read() throws IOException {
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		return nativeRead(handle);
	}

	@Override
	public int read(byte[] buffer,int offset,int length) throws IOException {
		if (buffer == null) { throw new NullPointerException(); }
		if (offset < 0 || length < 0 || offset > buffer.length - length) { throw new IndexOutOfBoundsException(); }
		if (length == 0) { return 0; }
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		return nativeRead(handle,buffer,offset,length);
	}

	@Override
	public void close() {
		long handle = streamHandle;
		if (handle == 0) { return; }
		streamHandle = 0;
		if (ownsStream) {
			ownsStream = false;
			nativeDelete(handle);
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
		if (count < 0) { throw new IllegalArgumentException("count < 0"); }
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		return nativeSkip(handle, count);
	}

	@Override
	public synchronized void mark(int readLimit) {
		// Not supported by the native std::istream.
	}

	@Override
	public synchronized void reset() throws IOException {
		throw new IOException("Mark not supported");
	}

	@Override
	public boolean markSupported() {
		return false;
	}

	private static native long nativeNew(byte[] buffer);
	private static native void nativeDelete(long streamHandle);
	private static native int nativeRead(long streamHandle);
	private static native int nativeRead(long streamHandle,byte[] buffer,int offset,int length);
	private static native int nativeAvailable(long streamHandle);
	private static native long nativeSkip(long streamHandle,long count);
}
