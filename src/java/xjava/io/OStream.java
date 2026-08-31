package xjava.io;

import java.io.IOException;
import java.io.OutputStream;

import xjava.XJNILibrary;

public final class OStream extends OutputStream {
	static { XJNILibrary.ensureLoaded(); }

	private long streamHandle;

	public OStream(long streamHandle) {
		this.streamHandle = streamHandle;
	}

	@Override
	public void write(int value) throws IOException {
		nativeWrite(streamHandle,value);
	}

	@Override
	public void write(byte[] buffer,int offset,int length) throws IOException {
		if (buffer == null) { throw new NullPointerException(); }
		if (offset < 0 || length < 0 || offset > buffer.length - length) { throw new IndexOutOfBoundsException(); }
		if (length == 0) { return; }
		nativeWrite(streamHandle,buffer,offset,length);
	}

	@Override
	public void flush() throws IOException {
		nativeFlush(streamHandle);
	}

	@Override
	public void close() {
		streamHandle = 0;
	}

	private static native void nativeWrite(long streamHandle,int value);
	private static native void nativeWrite(long streamHandle,byte[] buffer,int offset,int length);
	private static native void nativeFlush(long streamHandle);
}
