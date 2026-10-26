#include "base-jni.h"

#include "xjava.h"

JNIEXPORTC jlong JNICALL
Java_xjava_io_OStream_nativeNew(
	JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jbyteArray buffer
) {
	if (buffer == nullptr) return 0;
	const jsize length = _GetArrayLength(env,buffer);
	jbyte* data = _GetByteArrayElements(env,buffer, nullptr);
	if (data == nullptr) return 0;
	jni::u8_string str(reinterpret_cast<const uint8_t*>(data),static_cast<size_t>(length));
	_ReleaseByteArrayElements(env,buffer, data, JNI_ABORT);
	auto* stream = new jni::u8ostringstream(std::move(str));
	return reinterpret_cast<jlong>(stream);
}

JNIEXPORTC void JNICALL
Java_xjava_io_OStream_nativeDelete(
	XJNI_MAYBE_UNUSED JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle
) {
	delete reinterpret_cast<jni::u8ostringstream*>(streamHandle);
}

JNIEXPORTC void JNICALL
Java_xjava_io_OStream_nativeWrite__JI(
	XJNI_MAYBE_UNUSED JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle,
	jint value
) {
	auto* stream = reinterpret_cast<std::ostream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr) return;
	stream->put(static_cast<char>(static_cast<unsigned char>(value)));
}

JNIEXPORTC void JNICALL
Java_xjava_io_OStream_nativeWrite__J_3BII(
	JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle,
	jbyteArray buffer,
	jint offset,
	jint length
) {
	auto* stream = reinterpret_cast<std::ostream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr || buffer == nullptr) return;
	jbyte* data = _GetByteArrayElements(env,buffer, nullptr);
	if (data == nullptr) return;
	stream->write(reinterpret_cast<const char*>(data) + offset,static_cast<std::streamsize>(length));
	_ReleaseByteArrayElements(env,buffer,data,JNI_ABORT);
}

JNIEXPORTC void JNICALL
Java_xjava_io_OStream_nativeFlush(
	XJNI_MAYBE_UNUSED JNIEnv* env,
	XJNI_MAYBE_UNUSED jclass clazz,
	jlong streamHandle
) {
	auto* stream = reinterpret_cast<std::ostream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream != nullptr) stream->flush();
}
