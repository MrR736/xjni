#include <jni.h>

#include <cstdint>
#include <ostream>

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
