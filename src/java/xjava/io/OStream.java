package xjava.io;

import java.io.IOException;
import java.io.ByteArrayOutputStream;
import java.io.OutputStream;

import xjava.XJNILibrary;

public final class OStream extends OutputStream {
	static { XJNILibrary.ensureLoaded(); }

	private long streamHandle;
	private boolean ownsStream;
	private ByteArrayOutputStream byteArrayStream;

	public OStream() throws IOException {
		this(new byte[0]);
	}

	public OStream(OutputStream stream) throws IOException {
		if (stream == null) { throw new NullPointerException("stream"); }
		if (stream instanceof ByteArrayOutputStream) { this.byteArrayStream = (ByteArrayOutputStream)stream; }
		else { this.byteArrayStream = new ByteArrayOutputStream(); }
		this.streamHandle = nativeNew(this.byteArrayStream.toByteArray());
		this.ownsStream = true;
	}

	public OStream(byte[] buffer) {
		this.streamHandle = nativeNew(buffer);
		this.ownsStream = true;
	}

	public OStream(long streamHandle) {
		this.streamHandle = streamHandle;
		this.ownsStream = false;
	}

	@Override
	public void write(int value) throws IOException {
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		nativeWrite(handle,value);
	}

	@Override
	public void write(byte[] buffer,int offset,int length) throws IOException {
		if (buffer == null) { throw new NullPointerException(); }
		if (offset < 0 || length < 0 || offset > buffer.length - length) { throw new IndexOutOfBoundsException(); }
		if (length == 0) { return; }
		long handle = streamHandle;
		if (handle == 0) { throw new IOException("Stream closed"); }
		nativeWrite(handle,buffer,offset,length);
	}

	@Override
	public void flush() throws IOException {
		nativeFlush(streamHandle);
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

	private static native long nativeNew(byte[] buffer);
	private static native void nativeDelete(long streamHandle);
	private static native void nativeWrite(long streamHandle,int value);
	private static native void nativeWrite(long streamHandle,byte[] buffer,int offset,int length);
	private static native void nativeFlush(long streamHandle);
}
