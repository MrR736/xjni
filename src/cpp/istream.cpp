#include <jni.h>

#include <cstdint>
#include <istream>

extern "C" JNIEXPORT jint JNICALL
Java_xjava_io_IStream_nativeRead__J(JNIEnv* env,jclass /* clazz */,jlong streamHandle){
	auto* stream = reinterpret_cast<std::istream*>(static_cast<std::uintptr_t>(streamHandle));
	if (stream == nullptr) { return -1; }
	char c{};
	if (stream->get(c)) {
		return static_cast<unsigned char>(c);
	}
	return -1;
}

extern "C" JNIEXPORT jint JNICALL
Java_xjava_io_IStream_nativeRead__J_3BII(JNIEnv* env,jclass /* clazz */,jlong streamHandle,jbyteArray buffer,jint offset,jint length) {
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
