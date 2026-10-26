package xjava.io;

import java.io.DataInputStream;
import java.io.InputStream;
import java.util.Objects;

public final class DataIStream extends DataInputStream {
	public DataIStream(InputStream in) {
		super(Objects.requireNonNull(in, "in"));
	}

	public DataIStream(IStream in) {
		this((InputStream) in);
	}
}
