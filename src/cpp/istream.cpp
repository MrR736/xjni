#include "base-jni.h"

#include "xjava.h"

JNIEXPORTC jlong JNICALL
Java_xjava_io_IStream_nativeNew(JNIEnv* env,jclass /* clazz */,jbyteArray buffer) {
	if (buffer == nullptr) return 0;
	const jsize length = env->GetArrayLength(buffer);
	jbyte* data = env->GetByteArrayElements(buffer, nullptr);
	if (data == nullptr) return 0;
	u8_string str(reinterpret_cast<const uint8_t*>(data),static_cast<size_t>(length));
	env->ReleaseByteArrayElements(buffer, data, JNI_ABORT);
	auto* stream = new u8istringstream(std::move(str));
	return reinterpret_cast<jlong>(stream);
}

JNIEXPORTC void JNICALL
Java_xjava_io_IStream_nativeDelete(JNIEnv*,jclass,jlong streamHandle) {
	delete reinterpret_cast<u8istringstream*>(streamHandle);
}

JNIEXPORTC jint JNICALL
Java_xjava_io_IStream_nativeRead__J(
	JNIEnv* env,
	jclass /* clazz */,
	jlong streamHandle
) {
	auto* stream = reinterpret_cast<std::istream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr) { return -1; }
	char c{};
	if (stream->get(c)) {
		return static_cast<unsigned char>(c);
	}
	return -1;
}

JNIEXPORTC jint JNICALL
Java_xjava_io_IStream_nativeRead__J_3BII(
	JNIEnv* env,
	jclass /* clazz */,
	jlong streamHandle,
	jbyteArray buffer,
	jint offset,
	jint length
) {
	auto* stream = reinterpret_cast<std::istream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr || buffer == nullptr) { return -1; }
	jbyte* data = env->GetByteArrayElements(buffer, nullptr);
	if (data == nullptr) { return -1; }
	stream->read(reinterpret_cast<char*>(data) + offset,length);
	const std::streamsize count = stream->gcount();
	env->ReleaseByteArrayElements(buffer, data, 0);
	if (count == 0) { return -1; }
	return static_cast<jint>(count);
}

JNIEXPORTC jint JNICALL
Java_xjava_io_IStream_nativeAvailable(JNIEnv*,jclass,jlong streamHandle) {
	auto* stream = reinterpret_cast<std::istream*>(streamHandle);
	if (stream == nullptr) return 0;
	std::streampos current = stream->tellg();
	if (current == std::streampos(-1)) return 0;
	stream->seekg(0, std::ios::end);
	std::streampos end = stream->tellg();
	stream->seekg(current);
	if (end == std::streampos(-1) || end < current) { return 0; }
	std::streamoff available = end - current;
	if (available > static_cast<std::streamoff>(
		std::numeric_limits<jint>::max())) {
		return std::numeric_limits<jint>::max();
		}
		return static_cast<jint>(available);
}

JNIEXPORTC jlong JNICALL
Java_xjava_io_IStream_nativeSkip(JNIEnv* /* env */,jclass /* clazz */,jlong streamHandle,jlong count) {
	auto* stream = reinterpret_cast<std::istream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr || count <= 0) { return 0; }
	stream->clear();
	const std::streampos before = stream->tellg();
	if (before == std::streampos(-1)) { return 0; }
	stream->seekg(0, std::ios::end);
	if (stream->fail()) {
		stream->clear();
		return 0;
	}
	const std::streampos end = stream->tellg();
	if (end == std::streampos(-1) || end < before) {
		stream->clear();
		stream->seekg(before);
		return 0;
	}
	const std::streamoff remaining = end - before;
	std::streamoff requested;
	if (static_cast<unsigned long long>(count)>
		static_cast<unsigned long long>(std::numeric_limits<std::streamoff>::max())) {
		requested = std::numeric_limits<std::streamoff>::max();
		} else { requested = static_cast<std::streamoff>(count); }
		const std::streamoff skipped = (requested < remaining) ? requested : remaining;
	stream->clear();
	stream->seekg(before + skipped,std::ios::beg);
	if (stream->fail()) {
		stream->clear();
		stream->seekg(before);
		return 0;
	}
	return static_cast<jlong>(skipped);
}
