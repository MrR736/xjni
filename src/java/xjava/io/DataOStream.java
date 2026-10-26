package xjava.io;

import java.io.DataOutputStream;
import java.io.OutputStream;
import java.util.Objects;

public final class DataOStream extends DataOutputStream {
	public DataOStream(OutputStream out) {
		super(Objects.requireNonNull(out, "out"));
	}

	public DataOStream(OStream out) {
		this((OutputStream) out);
	}
}
