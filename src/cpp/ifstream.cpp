#include <jni.h>

#include "xjava.h"

extern "C" JNIEXPORT jlong JNICALL
Java_xjava_io_IFStream_nativeNew(JNIEnv* env,jclass /* clazz */,jstring buffer) {
	if (buffer == nullptr) return 0;
	const char* data = env->GetStringUTFChars(buffer, nullptr);
	if (data == nullptr) return 0;
	jni::filesystem::path p(data);
	jni::filesystem::ifstream* stream = new jni::filesystem::ifstream(p);
	env->ReleaseStringUTFChars(buffer, data);
	if (!stream->is_open()) {
		delete stream;
		return 0;
	}
	return reinterpret_cast<jlong>(stream);
}

extern "C" JNIEXPORT void JNICALL
Java_xjava_io_IFStream_nativeDelete(JNIEnv*,jclass,jlong streamHandle) {
	delete reinterpret_cast<jni::filesystem::ifstream*>(streamHandle);
}

extern "C" JNIEXPORT jint JNICALL
Java_xjava_io_IFStream_nativeRead__J(
	JNIEnv* env,
	jclass /* clazz */,
	jlong streamHandle
) {
	auto* stream = reinterpret_cast<jni::filesystem::ifstream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr) { return -1; }
	char c{};
	if (stream->get(c)) { return static_cast<unsigned char>(c); }
	return -1;
}

extern "C" JNIEXPORT jint JNICALL
Java_xjava_io_IFStream_nativeRead__J_3BII(JNIEnv* env,jclass /* clazz */,jlong streamHandle,jbyteArray buffer,jint offset,jint length) {
	auto* stream = reinterpret_cast<jni::filesystem::ifstream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr || buffer == nullptr) { return -1; }
	jbyte* data = env->GetByteArrayElements(buffer, nullptr);
	if (data == nullptr) { return -1; }
	stream->read(reinterpret_cast<char*>(data) + offset,length);
	const std::streamsize count = stream->gcount();
	env->ReleaseByteArrayElements(buffer, data, 0);
	if (count == 0) { return -1; }
	return static_cast<jint>(count);
}

extern "C" JNIEXPORT jint JNICALL
Java_xjava_io_IFStream_nativeAvailable(JNIEnv*,jclass,jlong streamHandle) {
	auto* stream = reinterpret_cast<jni::filesystem::ifstream*>(streamHandle);
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

extern "C" JNIEXPORT jlong JNICALL
Java_xjava_io_IFStream_nativeSkip(JNIEnv* /* env */,jclass /* clazz */,jlong streamHandle,jlong count) {
	auto* stream = reinterpret_cast<jni::filesystem::ifstream*>(static_cast<std::uintptr_t>(streamHandle));
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
