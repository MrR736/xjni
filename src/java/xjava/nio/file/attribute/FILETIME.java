package xjava.nio.file.attribute;

import java.lang.Math;
import java.nio.file.attribute.FileTime;
import java.time.Instant;

public final class FILETIME {
	private static final long WINDOWS_EPOCH_OFFSET_SECONDS = 11644473600L;
	private static final long FILETIME_TICKS_PER_SECOND = 10_000_000L;
	private static final long FILETIME_TICKS_TO_NANOS = 100L;

	public int dwLowDateTime;
	public int dwHighDateTime;

	public FILETIME() { }

	public FILETIME(int dwLowDateTime, int dwHighDateTime) {
		this.dwLowDateTime = dwLowDateTime;
		this.dwHighDateTime = dwHighDateTime;
	}

	public FILETIME(FileTime fileTime) {
		final Instant instant = fileTime.toInstant();
		final long seconds = instant.getEpochSecond() + WINDOWS_EPOCH_OFFSET_SECONDS;
		final long ticks = seconds * FILETIME_TICKS_PER_SECOND + instant.getNano() / FILETIME_TICKS_TO_NANOS;
		this.dwLowDateTime = (int) ticks;
		this.dwHighDateTime = (int) (ticks >>> 32);
	}

	public long toLong() {
		return (Integer.toUnsignedLong(dwHighDateTime) << 32) | Integer.toUnsignedLong(dwLowDateTime);
	}

	public void fromLong(long value) {
		dwLowDateTime = (int) value;
		dwHighDateTime = (int) (value >>> 32);
	}

	public FileTime toFileTime() {
		final long value = toLong();
		final long seconds = Math.floorDiv(value, FILETIME_TICKS_PER_SECOND) - WINDOWS_EPOCH_OFFSET_SECONDS;
		final long remainder = Math.floorMod(value, FILETIME_TICKS_PER_SECOND);
		return FileTime.from(Instant.ofEpochSecond(seconds,remainder * FILETIME_TICKS_TO_NANOS));
	}

	public static FILETIME fromFileTime(FileTime fileTime) {
		final Instant instant = fileTime.toInstant();
		final long seconds = instant.getEpochSecond() + WINDOWS_EPOCH_OFFSET_SECONDS;
		final long ticks = seconds * FILETIME_TICKS_PER_SECOND + instant.getNano() / FILETIME_TICKS_TO_NANOS;
		return new FILETIME((int) ticks,(int) (ticks >>> 32));
	}

	@Override
	public String toString() {
		return toFileTime().toString();
	}
}
