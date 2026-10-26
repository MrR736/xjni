#include "base-jni.h"

#include "xjava.h"

JNIEXPORTC jlong JNICALL
Java_xjava_io_OFStream_nativeNew(
	JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jstring buffer,
	jboolean append
) {
	if (buffer == nullptr) return 0;
	const char* data = _GetStringUTFChars(env,buffer, nullptr);
	if (data == nullptr) return 0;
	jlong result = 0;
	try {
		jni::filesystem::path p(data);
		std::ios_base::openmode mode = std::ios_base::out;
		if (append == JNI_TRUE) { mode |= std::ios_base::app; }
		jni::filesystem::ofstream* stream =
		new jni::filesystem::ofstream(p, mode);
		if (stream->is_open()) { result = reinterpret_cast<jlong>(stream); }
		else { delete stream; }
	} catch (...) { result = 0; }
	_ReleaseStringUTFChars(env,buffer, data);
	return result;
}

JNIEXPORTC void JNICALL
Java_xjava_io_OFStream_nativeDelete(
	XJNI_MAYBE_UNUSED JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle
) {
	delete reinterpret_cast<jni::filesystem::ofstream*>(streamHandle);
}

JNIEXPORTC void JNICALL
Java_xjava_io_OFStream_nativeWrite__JI(
	XJNI_MAYBE_UNUSED JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle,
	jint value
) {
	auto* stream = reinterpret_cast<jni::filesystem::ofstream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr) return;
	stream->put(static_cast<char>(static_cast<unsigned char>(value)));
}

JNIEXPORTC void JNICALL
Java_xjava_io_OFStream_nativeWrite__J_3BII(
	JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle,
	jbyteArray buffer,
	jint offset,
	jint length
) {
	auto* stream = reinterpret_cast<jni::filesystem::ofstream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr || buffer == nullptr) return;
	jbyte* data = _GetByteArrayElements(env,buffer, nullptr);
	if (data == nullptr) return;
	stream->write(reinterpret_cast<const char*>(data) + offset,static_cast<std::streamsize>(length));
	_ReleaseByteArrayElements(env,buffer,data,JNI_ABORT);
}

JNIEXPORTC void JNICALL
Java_xjava_io_OFStream_nativeFlush(
	XJNI_MAYBE_UNUSED JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle
) {
	auto* stream = reinterpret_cast<jni::filesystem::ofstream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream != nullptr) stream->flush();
}
