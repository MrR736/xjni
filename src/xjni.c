#include <stddef.h>
#include <stdlib.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

static char version[16];  // Enough for "255.255.255\0"

static jclass ioExceptionCls = NULL;
static jclass charConversionExceptionCls = NULL;
static jclass eofExceptionCls = NULL;
static jclass fileNotFoundExceptionCls = NULL;
static jclass interruptedIOExceptionCls = NULL;
static jclass invalidClassExceptionCls = NULL;
static jclass invalidObjectExceptionCls = NULL;
static jclass notActiveExceptionCls = NULL;
static jclass notSerializableExceptionCls = NULL;
static jclass optionalDataExceptionCls = NULL;
static jclass streamCorruptedExceptionCls = NULL;
static jclass syncFailedExceptionCls = NULL;
static jclass utfDataFormatExceptionCls = NULL;
static jclass unsupportedEncodingExceptionCls = NULL;
static jclass writeAbortedExceptionCls = NULL;
static jclass outOfMemoryErrorCls = NULL;

static void init_version(void) {
	snprintf(version,sizeof(version),"%d.%d.%d",_XJNI_VERSION_MAJOR,_XJNI_VERSION_MINOR,_XJNI_VERSION_PATCH);
}

JNIEXPORTC const char* JNICALL xjni_version(void) {
	static xjni_once_t once = XJNI_ONCE_INIT;
	xjni_once(&once,init_version);
	return version;
}

static jclass getExceptionClass(JNIEnv* env,const char* cls_name,jclass* cache) {
	if (*cache != NULL) return *cache;
	if (*cache == NULL) {
		jclass local = _FindClass(env,cls_name);
		if (!local) {
			_ExceptionClear(env);
		} else {
			if (*cache == NULL && local != NULL)
				*cache = _NewGlobalRef(env,local);
			_DeleteLocalRef(env,local);
		}
	}
	return *cache;
}

JNIEXPORTC void JNICALL throwJava(JNIEnv *env,const char* tag,const char* msg,const char* cls_name,jclass* cache) {
	jclass clas = getExceptionClass(env,cls_name,cache);
	if (!clas) {
		XJNI_LOGE(tag,"Unable to find exception class: %s\n",cls_name);
		_ExceptionClear(env);
		return;
	}
	_ThrowNew(env,clas,msg);
}

JNIEXPORTC void JNICALL FatalErrorV(JNIEnv *env,const char *msg,va_list __arg) {
	va_list aq;
	va_copy(aq,__arg);
#ifdef _MSC_VER
	int len = _vscprintf(msg,aq);
#else
	int len = vsnprintf(NULL,0,msg,aq);
#endif
	va_end(aq);
	if (len < 0) return;
	size_t clen = (size_t)len + 1;
	char *formattedMsg = (char*)malloc(clen);
	if (!formattedMsg) return;
	va_copy(aq,__arg);
	int ret = vsnprintf(formattedMsg,clen,msg,aq);
	va_end(aq);
	if (ret < 0) {
		free(formattedMsg);
	}
	_FatalError(env,formattedMsg);
	free(formattedMsg);
}

JNIEXPORTC void JNICALL FatalErrorF(JNIEnv *env,const char *msg,...) {
	va_list args;
	va_start(args,msg);
	FatalErrorV(env,msg,args);
	va_end(args);
}

JNIEXPORTC jint JNICALL ThrowNewV(JNIEnv *env,jclass clazz,const char *msg,va_list __arg) {
	va_list aq;
	va_copy(aq,__arg);
#ifdef _MSC_VER
	int len = _vscprintf(msg,aq);
#else
	int len = vsnprintf(NULL,0,msg,aq);
#endif
	va_end(aq);
	if (len < 0) return JNI_ERR;
	size_t clen = (size_t)len + 1;
	char *formattedMsg = (char*)malloc(clen);
	if (!formattedMsg) return JNI_ERR;
	va_copy(aq,__arg);
	int ret = vsnprintf(formattedMsg,clen,msg,aq);
	va_end(aq);
	if (ret < 0) {
		free(formattedMsg);
		return JNI_ERR;
	}
	if (_ThrowNew(env,clazz,formattedMsg) != JNI_OK) {
		free(formattedMsg);
		return JNI_EDETACHED;
	}
	free(formattedMsg);
	return JNI_OK;
}

JNIEXPORTC jint JNICALL ThrowNewF(JNIEnv *env,jclass clazz,const char *msg,...) {
	va_list args;
	va_start(args,msg);
	jint result = ThrowNewV(env,clazz,msg,args);
	va_end(args);
	return result;
}

JNIEXPORTC void JNICALL
throwJavaV(JNIEnv *env,const char* tag,const char* cls_name,jclass* cache,const char* msg,va_list ap) {
	jclass clas = getExceptionClass(env,cls_name,cache);
	if (!clas) {
		XJNI_LOGE(tag,"Unable to find exception class: %s\n",cls_name);
		_ExceptionClear(env);
		return;
	}
	ThrowNewV(env,clas,msg,ap);
}

JNIEXPORTC void JNICALL
throwJavaF(JNIEnv *env,const char* tag,const char* cls_name,jclass* cache,const char *msg,...) {
	va_list args;
	va_start(args,msg);
	throwJavaV(env,tag,cls_name,cache,msg,args);
	va_end(args);
}

typedef struct {
	const char* name;
	jclass* cache;
} ExceptionEntry;

static ExceptionEntry exceptions[] = {
	{ "java/io/IOException", &ioExceptionCls },
	{ "java/io/CharConversionException", &charConversionExceptionCls },
	{ "java/io/EOFException", &eofExceptionCls },
	{ "java/io/FileNotFoundException", &fileNotFoundExceptionCls },
	{ "java/io/InterruptedIOException", &interruptedIOExceptionCls },
	{ "java/io/InvalidClassException", &invalidClassExceptionCls },
	{ "java/io/InvalidObjectException", &invalidObjectExceptionCls },
	{ "java/io/NotActiveException", &notActiveExceptionCls },
	{ "java/io/NotSerializableException", &notSerializableExceptionCls },
	{ "java/io/OptionalDataException", &optionalDataExceptionCls },
	{ "java/io/StreamCorruptedException", &streamCorruptedExceptionCls },
	{ "java/io/SyncFailedException", &syncFailedExceptionCls },
	{ "java/io/UTFDataFormatException", &utfDataFormatExceptionCls },
	{ "java/io/UnsupportedEncodingException", &unsupportedEncodingExceptionCls },
	{ "java/io/WriteAbortedException", &writeAbortedExceptionCls },
	{ "java/lang/OutOfMemoryError", &outOfMemoryErrorCls }
};
static const size_t count = arrayof(exceptions);

JNIEXPORT jint JNICALL XJNI_OnLoad(JavaVM* vm, void* reserved, jint ver) {
	JNIEnv* env = NULL;
	if (_GetEnv(vm, (void**)&env, ver) != JNI_OK) return JNI_ERR;

	if (XJNI_New_OnLoad(vm, reserved, ver) != ver) return JNI_ERR;
	if (XJNI_Utily_OnLoad(vm, reserved, ver) != ver) return JNI_ERR;

	size_t i;
	for (i = 0; i < count; i++) {
		jclass local = _FindClass(env, exceptions[i].name);
		if (!local) {
			_ExceptionClear(env);
			goto fail;
		}
		*exceptions[i].cache = _NewGlobalRef(env, local);
		_DeleteLocalRef(env, local);
		if (!*exceptions[i].cache) goto fail;
	}
	return ver;

fail:
	for (size_t j = 0; j < i; j++) {
		if (*exceptions[j].cache) {
			_DeleteGlobalRef(env, *exceptions[j].cache);
			*exceptions[j].cache = NULL;
		}
	}
	return JNI_ERR;
}

// --- OnUnload ---
JNIEXPORT void JNICALL XJNI_OnUnload(JavaVM* vm, void* reserved, jint ver) {
	JNIEnv* env = NULL;
	if (_GetEnv(vm, (void**)&env, ver) != JNI_OK) return;

	XJNI_New_OnUnload(vm, reserved, ver);
	XJNI_Utily_OnUnload(vm, reserved, ver);

	for (size_t i = 0; i < count; i++) {
		if (*exceptions[i].cache) {
			_DeleteGlobalRef(env, *exceptions[i].cache);
			*exceptions[i].cache = NULL;
		}
	}
}

#define MAKE_THROW_FUN(NAME, CLS_STR, CLS_CACHE) \
JNIEXPORTC void JNICALL NAME(JNIEnv *env, const char* tag, const char* msg) { \
	throwJava(env, tag, msg, CLS_STR, &CLS_CACHE); \
} \
JNIEXPORTC void JNICALL NAME##V(JNIEnv *env, const char* tag, const char* msg, va_list ap) { \
	throwJavaV(env, tag, CLS_STR, &CLS_CACHE, msg, ap); \
} \
JNIEXPORTC void JNICALL NAME##F(JNIEnv *env, const char* tag, const char* msg, ...) { \
	va_list args; \
	va_start(args, msg); \
	throwJavaV(env, tag, CLS_STR, &CLS_CACHE, msg, args); \
	va_end(args); \
}

MAKE_THROW_FUN(throwIOException, "java/io/IOException", ioExceptionCls)
MAKE_THROW_FUN(throwCharConversionException, "java/io/CharConversionException", charConversionExceptionCls)
MAKE_THROW_FUN(throwEOFException, "java/io/EOFException", eofExceptionCls)
MAKE_THROW_FUN(throwFileNotFoundException, "java/io/FileNotFoundException", fileNotFoundExceptionCls)
MAKE_THROW_FUN(throwInterruptedIOException, "java/io/InterruptedIOException", interruptedIOExceptionCls)
MAKE_THROW_FUN(throwInvalidClassException, "java/io/InvalidClassException", invalidClassExceptionCls)
MAKE_THROW_FUN(throwInvalidObjectException, "java/io/InvalidObjectException", invalidObjectExceptionCls)
MAKE_THROW_FUN(throwNotActiveException, "java/io/NotActiveException", notActiveExceptionCls)
MAKE_THROW_FUN(throwNotSerializableException, "java/io/NotSerializableException", notSerializableExceptionCls)
MAKE_THROW_FUN(throwOptionalDataException, "java/io/OptionalDataException", optionalDataExceptionCls)
MAKE_THROW_FUN(throwStreamCorruptedException, "java/io/StreamCorruptedException", streamCorruptedExceptionCls)
MAKE_THROW_FUN(throwSyncFailedException, "java/io/SyncFailedException", syncFailedExceptionCls)
MAKE_THROW_FUN(throwUTFDataFormatException, "java/io/UTFDataFormatException", utfDataFormatExceptionCls)
MAKE_THROW_FUN(throwUnsupportedEncodingException, "java/io/UnsupportedEncodingException", unsupportedEncodingExceptionCls)
MAKE_THROW_FUN(throwWriteAbortedException, "java/io/WriteAbortedException", writeAbortedExceptionCls)
MAKE_THROW_FUN(throwOutOfMemoryError, "java/lang/OutOfMemoryError", outOfMemoryErrorCls)
