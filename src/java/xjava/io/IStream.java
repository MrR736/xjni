package xjava.io;

import java.io.IOException;
import java.io.InputStream;

import xjava.XJNILibrary;

public final class IStream extends InputStream {
	static { XJNILibrary.ensureLoaded(); }

	private long streamHandle;

	public IStream(long streamHandle) {
		this.streamHandle = streamHandle;
	}

	@Override
	public int read() throws IOException {
		return nativeRead(streamHandle);
	}

	@Override
	public int read(byte[] buffer,int offset,int length) throws IOException {
		if (buffer == null) { throw new NullPointerException(); }
		if (offset < 0 || length < 0 || offset > buffer.length - length) { throw new IndexOutOfBoundsException(); }
		if (length == 0) { return 0; }
		return nativeRead(streamHandle,buffer,offset,length);
	}

	@Override
	public void close() {
		streamHandle = 0;
	}

	private static native int nativeRead(long streamHandle);
	private static native int nativeRead(long streamHandle,byte[] buffer,int offset,int length);
}
