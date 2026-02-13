#include <stdlib.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

#ifdef _WIN32
#define pthread_once(once_control, init_routine) InitOnceExecuteOnce(once_control, init_routine, NULL, NULL)
#define pthread_mutex_init(mutex, attr) InitializeCriticalSection(mutex)
#define pthread_mutex_lock(mutex) EnterCriticalSection(mutex)
#define pthread_mutex_unlock(mutex) LeaveCriticalSection(mutex)
#define pthread_mutex_destroy(mutex) DeleteCriticalSection(mutex)
#endif

static char version[16];  // Enough for "255.255.255\0"

static jclass ioExceptionCls = NULL;
static pthread_mutex_t ioExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass charConversionExceptionCls = NULL;
static pthread_mutex_t charConversionExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass eofExceptionCls = NULL;
static pthread_mutex_t eofExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass fileNotFoundExceptionCls = NULL;
static pthread_mutex_t fileNotFoundExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass interruptedIOExceptionCls = NULL;
static pthread_mutex_t interruptedIOExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass invalidClassExceptionCls = NULL;
static pthread_mutex_t invalidClassExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass invalidObjectExceptionCls = NULL;
static pthread_mutex_t invalidObjectExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass notActiveExceptionCls = NULL;
static pthread_mutex_t notActiveExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass notSerializableExceptionCls = NULL;
static pthread_mutex_t notSerializableExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass optionalDataExceptionCls = NULL;
static pthread_mutex_t optionalDataExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass streamCorruptedExceptionCls = NULL;
static pthread_mutex_t streamCorruptedExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass syncFailedExceptionCls = NULL;
static pthread_mutex_t syncFailedExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass utfDataFormatExceptionCls = NULL;
static pthread_mutex_t utfDataFormatExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass unsupportedEncodingExceptionCls = NULL;
static pthread_mutex_t unsupportedEncodingExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass writeAbortedExceptionCls = NULL;
static pthread_mutex_t writeAbortedExceptionMutex = PTHREAD_MUTEX_INITIALIZER;
static jclass outOfMemoryErrorCls = NULL;
static pthread_mutex_t outOfMemoryErrorMutex = PTHREAD_MUTEX_INITIALIZER;

#ifdef _WIN32
static BOOL CALLBACK init_version(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *Context) {
	(void)InitOnce;
	(void)Parameter;
	(void)Context;
	snprintf(version,sizeof(version),"%d.%d.%d",_XJNI_VERSION_MAJOR,_XJNI_VERSION_MINOR,_XJNI_VERSION_PATCH);
	return TRUE;
}
#else
static void init_version(void) {
	snprintf(version,sizeof(version),"%d.%d.%d",_XJNI_VERSION_MAJOR,_XJNI_VERSION_MINOR,_XJNI_VERSION_PATCH);
}
#endif

JNIEXPORTC const char* JNICALL xjni_version(void) {
	static pthread_once_t once = PTHREAD_ONCE_INIT;
	pthread_once(&once,init_version);
	return version;
}

static jclass getExceptionClass(JNIEnv* env,const char* cls_name,jclass* cache,pthread_mutex_t* mutex) {
	if (*cache != NULL) return *cache;
	if (mutex != NULL) pthread_mutex_lock(mutex);
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
	if (mutex != NULL) pthread_mutex_unlock(mutex);
	return *cache;
}

JNIEXPORTC void JNICALL throwJava(JNIEnv *env,const char* tag,const char* msg,const char* cls_name,jclass* cache,pthread_mutex_t* mutex) {
	jclass clas = getExceptionClass(env,cls_name,cache,mutex);
	if (!clas) {
		BASE_TAG_LOGE(tag,"Unable to find exception class: %s\n",cls_name);
#ifndef __ANDROID__
		fflush(stderr);
#endif
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
throwJavaV(JNIEnv *env,const char* tag,const char* cls_name,jclass* cache,pthread_mutex_t* mutex,const char* msg,va_list ap) {
	jclass clas = getExceptionClass(env,cls_name,cache,mutex);
	if (!clas) {
		BASE_TAG_LOGE(tag,"Unable to find exception class: %s\n",cls_name);
#ifndef __ANDROID__
		fflush(stderr);
#endif
		_ExceptionClear(env);
		return;
	}
	ThrowNewV(env,clas,msg,ap);
}

JNIEXPORTC void JNICALL
throwJavaF(JNIEnv *env,const char* tag,const char* cls_name,jclass* cache,pthread_mutex_t* mutex,const char *msg,...) {
	va_list args;
	va_start(args,msg);
	throwJavaV(env,tag,cls_name,cache,mutex,msg,args);
	va_end(args);
}

JNIEXPORTC jint JNICALL XJNI_OnLoad(JavaVM* vm,void* reserved,jint ver) {
	JNIEnv* env = NULL;

	if (_GetEnv(vm,(void**)&env,ver) != JNI_OK)
		return JNI_ERR;

	if (XJNI_New_OnLoad(vm,reserved,ver) != ver)
		return JNI_ERR;

	struct {
		const char* name;
		jclass* cache;
	} exceptions[] = {
		{ "java/io/IOException",&ioExceptionCls },
		{ "java/io/CharConversionException",&charConversionExceptionCls },
		{ "java/io/EOFException",&eofExceptionCls },
		{ "java/io/FileNotFoundException",&fileNotFoundExceptionCls },
		{ "java/io/InterruptedIOException",&interruptedIOExceptionCls },
		{ "java/io/InvalidClassException",&invalidClassExceptionCls },
		{ "java/io/InvalidObjectException",&invalidObjectExceptionCls },
		{ "java/io/NotActiveException",&notActiveExceptionCls },
		{ "java/io/NotSerializableException",&notSerializableExceptionCls },
		{ "java/io/OptionalDataException",&optionalDataExceptionCls },
		{ "java/io/StreamCorruptedException",&streamCorruptedExceptionCls },
		{ "java/io/SyncFailedException",&syncFailedExceptionCls },
		{ "java/io/UTFDataFormatException",&utfDataFormatExceptionCls },
		{ "java/io/UnsupportedEncodingException",&unsupportedEncodingExceptionCls },
		{ "java/io/WriteAbortedException",&writeAbortedExceptionCls },
		{ "java/lang/OutOfMemoryError",&outOfMemoryErrorCls },
	};
	size_t count = sizeof(exceptions) / sizeof(exceptions[0]);
	size_t i;

	for (i = 0; i < count; i++) {
		jclass local = _FindClass(env, exceptions[i].name);
		if (!local) {
			_ExceptionClear(env);
			goto fail;
		}

		*exceptions[i].cache = _NewGlobalRef(env, local);
		_DeleteLocalRef(env, local);

		if (!*exceptions[i].cache)
			goto fail;
	}

	return ver;

	fail:
	while (i-- > 0) {
		if (*exceptions[i].cache) {
			_DeleteGlobalRef(env, *exceptions[i].cache);
			*exceptions[i].cache = NULL;
		}
	}
	return JNI_ERR;
}

#define class_free(env, cls, mtx)\
	do {\
		pthread_mutex_lock(&(mtx));\
		if (cls) { _DeleteGlobalRef(env, cls); cls = NULL; }\
		pthread_mutex_unlock(&(mtx));\
		pthread_mutex_destroy(&(mtx));\
	} while (0)


JNIEXPORTC void JNICALL XJNI_OnUnload(JavaVM* vm,void* reserved,jint ver) {
	JNIEnv* env = NULL;
	if (_GetEnv(vm, (void**)&env, ver) != JNI_OK)
		return;
	XJNI_New_OnUnload(vm,reserved,ver);
	class_free(env,ioExceptionCls,ioExceptionMutex);
	class_free(env,charConversionExceptionCls,charConversionExceptionMutex);
	class_free(env,eofExceptionCls,eofExceptionMutex);
	class_free(env,fileNotFoundExceptionCls,fileNotFoundExceptionMutex);
	class_free(env,interruptedIOExceptionCls,interruptedIOExceptionMutex);
	class_free(env,invalidClassExceptionCls,invalidClassExceptionMutex);
	class_free(env,invalidObjectExceptionCls,invalidObjectExceptionMutex);
	class_free(env,notActiveExceptionCls,notActiveExceptionMutex);
	class_free(env,notSerializableExceptionCls,notSerializableExceptionMutex);
	class_free(env,optionalDataExceptionCls,optionalDataExceptionMutex);
	class_free(env,streamCorruptedExceptionCls,streamCorruptedExceptionMutex);
	class_free(env,syncFailedExceptionCls,syncFailedExceptionMutex);
	class_free(env,utfDataFormatExceptionCls,utfDataFormatExceptionMutex);
	class_free(env,unsupportedEncodingExceptionCls,unsupportedEncodingExceptionMutex);
	class_free(env,writeAbortedExceptionCls,writeAbortedExceptionMutex);
	class_free(env,outOfMemoryErrorCls,outOfMemoryErrorMutex);
}

JNIEXPORTC void JNICALL throwIOException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/IOException",&ioExceptionCls,&ioExceptionMutex);
}

JNIEXPORTC void JNICALL throwCharConversionException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/CharConversionException",&charConversionExceptionCls,&charConversionExceptionMutex);
}

JNIEXPORTC void JNICALL throwEOFException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/EOFException",&eofExceptionCls,&eofExceptionMutex);
}

JNIEXPORTC void JNICALL throwFileNotFoundException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/FileNotFoundException",&fileNotFoundExceptionCls,&fileNotFoundExceptionMutex);
}

JNIEXPORTC void JNICALL throwInterruptedIOException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InterruptedIOException",&interruptedIOExceptionCls,&interruptedIOExceptionMutex);
}

JNIEXPORTC void JNICALL throwInvalidClassException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InvalidClassException",&invalidClassExceptionCls,&invalidClassExceptionMutex);
}

JNIEXPORTC void JNICALL throwInvalidObjectException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InvalidObjectException",&invalidObjectExceptionCls,&invalidObjectExceptionMutex);
}

JNIEXPORTC void JNICALL throwNotActiveException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/NotActiveException",&notActiveExceptionCls,&notActiveExceptionMutex);
}

JNIEXPORTC void JNICALL throwNotSerializableException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/NotSerializableException",&notSerializableExceptionCls,&notSerializableExceptionMutex);
}

JNIEXPORTC void JNICALL throwOptionalDataException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/OptionalDataException",&optionalDataExceptionCls,&optionalDataExceptionMutex);
}

JNIEXPORTC void JNICALL throwStreamCorruptedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/StreamCorruptedException",&streamCorruptedExceptionCls,&streamCorruptedExceptionMutex);
}

JNIEXPORTC void JNICALL throwSyncFailedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/SyncFailedException",&syncFailedExceptionCls,&syncFailedExceptionMutex);
}

JNIEXPORTC void JNICALL throwUTFDataFormatException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/UTFDataFormatException",&utfDataFormatExceptionCls,&utfDataFormatExceptionMutex);
}

JNIEXPORTC void JNICALL throwUnsupportedEncodingException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/UnsupportedEncodingException",&unsupportedEncodingExceptionCls,&unsupportedEncodingExceptionMutex);
}

JNIEXPORTC void JNICALL throwWriteAbortedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/WriteAbortedException",&writeAbortedExceptionCls,&writeAbortedExceptionMutex);
}

JNIEXPORTC void JNICALL throwOutOfMemoryError(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/lang/OutOfMemoryError",&outOfMemoryErrorCls,&outOfMemoryErrorMutex);
}



JNIEXPORTC void JNICALL throwIOExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/IOException",&ioExceptionCls,&ioExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwCharConversionExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/CharConversionException",&charConversionExceptionCls,&charConversionExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwEOFExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/EOFException",&eofExceptionCls,&eofExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwFileNotFoundExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/FileNotFoundException",&fileNotFoundExceptionCls,&fileNotFoundExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwInterruptedIOExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/InterruptedIOException",&interruptedIOExceptionCls,&interruptedIOExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwInvalidClassExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/InvalidClassException",&invalidClassExceptionCls,&invalidClassExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwInvalidObjectExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/InvalidObjectException",&invalidObjectExceptionCls,&invalidObjectExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwNotActiveExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/NotActiveException",&notActiveExceptionCls,&notActiveExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwNotSerializableExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/NotSerializableException",&notSerializableExceptionCls,&notSerializableExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwOptionalDataExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/OptionalDataException",&optionalDataExceptionCls,&optionalDataExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwStreamCorruptedExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/StreamCorruptedException",&streamCorruptedExceptionCls,&streamCorruptedExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwSyncFailedExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/SyncFailedException",&syncFailedExceptionCls,&syncFailedExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwUTFDataFormatExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/UTFDataFormatException",&utfDataFormatExceptionCls,&utfDataFormatExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwUnsupportedEncodingExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/UnsupportedEncodingException",&unsupportedEncodingExceptionCls,&unsupportedEncodingExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwWriteAbortedExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/io/WriteAbortedException",&writeAbortedExceptionCls,&writeAbortedExceptionMutex,msg,ap);
}

JNIEXPORTC void JNICALL throwOutOfMemoryErrorV(JNIEnv *env,const char* tag,const char* msg,va_list ap) {
	throwJavaV(env,tag,"java/lang/OutOfMemoryError",&outOfMemoryErrorCls,&outOfMemoryErrorMutex,msg,ap);
}

#define makeThrowF(name) \
JNIEXPORTC void JNICALL name##F(JNIEnv *env,const char* tag,const char* msg,...) {\
	va_list args;\
	va_start(args,msg);\
	name##V(env,tag,msg,args);\
	va_end(args);\
}

makeThrowF(throwIOException)
makeThrowF(throwCharConversionException)
makeThrowF(throwEOFException)
makeThrowF(throwFileNotFoundException)
makeThrowF(throwInterruptedIOException)
makeThrowF(throwInvalidClassException)
makeThrowF(throwInvalidObjectException)
makeThrowF(throwNotActiveException)
makeThrowF(throwNotSerializableException)
makeThrowF(throwOptionalDataException)
makeThrowF(throwStreamCorruptedException)
makeThrowF(throwSyncFailedException)
makeThrowF(throwUTFDataFormatException)
makeThrowF(throwUnsupportedEncodingException)
makeThrowF(throwWriteAbortedException)
makeThrowF(throwOutOfMemoryError)
