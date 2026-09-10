#include <jni.h>

#include "xjava.h"

extern "C" JNIEXPORT jlong JNICALL
Java_xjava_io_OStream_nativeNew(JNIEnv* env,jclass /* clazz */,jbyteArray buffer) {
	if (buffer == nullptr) return 0;
	const jsize length = env->GetArrayLength(buffer);
	jbyte* data = env->GetByteArrayElements(buffer, nullptr);
	if (data == nullptr) return 0;
	u8_string str(reinterpret_cast<const uint8_t*>(data),static_cast<size_t>(length));
	env->ReleaseByteArrayElements(buffer, data, JNI_ABORT);
	auto* stream = new u8ostringstream(std::move(str));
	return reinterpret_cast<jlong>(stream);
}

extern "C" JNIEXPORT void JNICALL
Java_xjava_io_OStream_nativeDelete(JNIEnv*,jclass,jlong streamHandle) {
	delete reinterpret_cast<u8ostringstream*>(streamHandle);
}

extern "C" JNIEXPORT void JNICALL
Java_xjava_io_OStream_nativeWrite__JI(JNIEnv* /* env */,jclass /* clazz */,jlong streamHandle,jint value) {
	auto* stream = reinterpret_cast<std::ostream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr) return;
	stream->put(static_cast<char>(static_cast<unsigned char>(value)));
}

extern "C" JNIEXPORT void JNICALL
Java_xjava_io_OStream_nativeWrite__J_3BII(JNIEnv* env,jclass /* clazz */,jlong streamHandle,jbyteArray buffer,jint offset,jint length) {
	auto* stream = reinterpret_cast<std::ostream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr || buffer == nullptr) return;
	jbyte* data = env->GetByteArrayElements(buffer, nullptr);
	if (data == nullptr) return;
	stream->write(reinterpret_cast<const char*>(data) + offset,static_cast<std::streamsize>(length));
	env->ReleaseByteArrayElements(buffer,data,JNI_ABORT);
}

extern "C" JNIEXPORT void JNICALL
Java_xjava_io_OStream_nativeFlush(JNIEnv* /* env */,jclass /* clazz */,jlong streamHandle) {
	auto* stream = reinterpret_cast<std::ostream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream != nullptr) stream->flush();
}
