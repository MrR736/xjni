package xjava.io;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;

import xjava.XJNILibrary;

public final class FILE extends File implements Closeable {
    static { XJNILibrary.ensureLoaded(); }

    private long streamHandle;

    public FILE(String parent, String child) {
        super(parent, child);
    }

    public FILE(String pathname) {
        super(pathname);
    }

    /**
     * Creates a FILE from parent/child and immediately opens it.
     */
    public FILE(String parent, String child, String mode) {
        super(parent, child);
        open(mode);
    }

    public static FILE fromFile(File file) {
        if (file == null) {
            throw new NullPointerException("file");
        }

        return new FILE(file.getPath());
    }

    public static File toFile(FILE file) {
        return file;
    }

    public void open(String mode) {
        if (mode == null) {
            throw new NullPointerException("mode");
        }

        if (isOpen()) {
            throw new IllegalStateException("FILE is already open");
        }

        open(this, mode);
    }

    @Override
    public void close() throws IOException {
        if (isOpen()) {
            close(this);
        }
    }

    public boolean isOpen() {
        return isOpen(this);
    }

    public native void open(FILE fp, String mode);
    public native void close(FILE fp);
    public native boolean isOpen(FILE fp);
}
