#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

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

static void init_version(void) {
	snprintf(version, sizeof(version), "%d.%d.%d",_XJNI_VERSION_MAJOR, _XJNI_VERSION_MINOR, _XJNI_VERSION_PATCH);
}

JNIEXPORTC const char* xjni_version(void) {
	static pthread_once_t once = PTHREAD_ONCE_INIT;
	pthread_once(&once,init_version);
	return version;
}

JNIEXPORTC char xjni_tochar(const jchar c) {
	return *xjni_tostring(&c);
}

JNIEXPORTC jchar xjni_tojchar(const char c) {
	return (jchar)(unsigned char)c;
}

JNIEXPORTC char* xjni_tostring(const jchar* c) {
	if (!c) return NULL;

	size_t max_len = 0;
	for (size_t i = 0; c[i]; i++) {
		max_len += 3;
		if (c[i] >= 0xD800 && c[i] <= 0xDBFF) {
			max_len += 1;
			i++;
		}
	}
	char* out = (char*)malloc(max_len + 1);
	if (!out) return NULL;
	char* p = out;
	for (size_t i = 0; c[i]; i++) {
		uint32_t ch = c[i];
		if (ch <= 0x7F) {
			*p++ = ch;
		} else if (ch <= 0x7FF) {
			*p++ = 0xC0 | (ch >> 6);
			*p++ = 0x80 | (ch & 0x3F);
		} else if (ch >= 0xD800 && ch <= 0xDBFF) {
			uint32_t low = c[++i];
			uint32_t codepoint = 0x10000 + (((ch - 0xD800) << 10) | (low - 0xDC00));
			*p++ = 0xF0 | (codepoint >> 18);
			*p++ = 0x80 | ((codepoint >> 12) & 0x3F);
			*p++ = 0x80 | ((codepoint >> 6) & 0x3F);
			*p++ = 0x80 | (codepoint & 0x3F);
		} else {
			*p++ = 0xE0 | (ch >> 12);
			*p++ = 0x80 | ((ch >> 6) & 0x3F);
			*p++ = 0x80 | (ch & 0x3F);
		}
	}
	*p = '\0';
	return out;
}

JNIEXPORTC jchar* xjni_tojstring(const char* c) {
	if (!c) return NULL;

	// 1. Compute required length in jchar units
	size_t len = 0;
	for (size_t i = 0; c[i]; ) {
		unsigned char b = c[i];
		if (b <= 0x7F) {
			i += 1; len += 1;
		} else if ((b & 0xE0) == 0xC0) {
			i += 2; len += 1;
		} else if ((b & 0xF0) == 0xE0) {
			i += 3; len += 1;
		} else if ((b & 0xF8) == 0xF0) {
			i += 4; len += 2;
		} else {
			i += 1; len += 1;
		}
	}

	jchar* out = (jchar*)malloc((len + 1) * sizeof(jchar));
	if (!out) return NULL;

	size_t j = 0;
	for (size_t i = 0; c[i]; ) {
		uint32_t codepoint = 0;
		unsigned char b = c[i];
		if (b <= 0x7F) {
			codepoint = b; i += 1;
		} else if ((b & 0xE0) == 0xC0) {
			codepoint = ((c[i] & 0x1F) << 6) | (c[i+1] & 0x3F);
			i += 2;
		} else if ((b & 0xF0) == 0xE0) {
			codepoint = ((c[i] & 0x0F) << 12) | ((c[i+1] & 0x3F) << 6) | (c[i+2] & 0x3F);
			i += 3;
		} else if ((b & 0xF8) == 0xF0) {
			codepoint = ((c[i] & 0x07) << 18) | ((c[i+1] & 0x3F) << 12) | ((c[i+2] & 0x3F) << 6) | (c[i+3] & 0x3F);
			i += 4;
			codepoint -= 0x10000;
			out[j++] = 0xD800 | (codepoint >> 10);
			out[j++] = 0xDC00 | (codepoint & 0x3FF);
			continue;
		}
		out[j++] = (jchar)codepoint;
	}
	out[j] = 0;
	return out;
}

bool xjni_fromstring(const char *src, jchar *dst, size_t *dstlen) {
    if (!src || !dst || !dstlen || *dstlen == 0)
        return false;

    size_t cap = *dstlen;
    size_t out = 0;

    const unsigned char *s = (const unsigned char *)src;

    while (*s && out + 1 < cap) {
        uint32_t codepoint;

        /* -------- UTF-8 decode -------- */
        if (*s < 0x80) {
            codepoint = *s++;
        } else if ((*s & 0xE0) == 0xC0) {
            if ((s[1] & 0xC0) != 0x80)
                return false;
            codepoint = ((*s & 0x1F) << 6) |
                        (s[1] & 0x3F);
            s += 2;
        } else if ((*s & 0xF0) == 0xE0) {
            if ((s[1] & 0xC0) != 0x80 ||
                (s[2] & 0xC0) != 0x80)
                return false;
            codepoint = ((*s & 0x0F) << 12) |
                        ((s[1] & 0x3F) << 6) |
                        (s[2] & 0x3F);
            s += 3;
        } else if ((*s & 0xF8) == 0xF0) {
            if ((s[1] & 0xC0) != 0x80 ||
                (s[2] & 0xC0) != 0x80 ||
                (s[3] & 0xC0) != 0x80)
                return false;
            codepoint = ((*s & 0x07) << 18) |
                        ((s[1] & 0x3F) << 12) |
                        ((s[2] & 0x3F) << 6) |
                        (s[3] & 0x3F);
            s += 4;
        } else {
            return false;
        }

        /* -------- UTF-16 encode -------- */
        if (codepoint <= 0xFFFF) {
            if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
                return false; /* surrogate invalid */
            dst[out++] = (jchar)codepoint;
        } else if (codepoint <= 0x10FFFF) {
            if (out + 2 >= cap)
                break;
            codepoint -= 0x10000;
            dst[out++] = (jchar)(0xD800 | (codepoint >> 10));
            dst[out++] = (jchar)(0xDC00 | (codepoint & 0x3FF));
        } else {
            return false;
        }
    }

    dst[out] = 0;
    *dstlen = out;

    return (*s == '\0');
}


bool xjni_fromjstring(const jchar *src, char *dst, size_t *dstlen) {
    if (!src || !dst || !dstlen || *dstlen == 0)
        return false;

    size_t cap = *dstlen;
    size_t out = 0;

    while (*src) {
        uint32_t codepoint;

        /* -------- UTF-16 decode -------- */
        jchar wc = *src++;

        if (wc >= 0xD800 && wc <= 0xDBFF) {
            /* High surrogate */
            jchar wc2 = *src++;
            if (wc2 < 0xDC00 || wc2 > 0xDFFF)
                return false;
            codepoint = 0x10000 +
                        (((wc - 0xD800) << 10) |
                         (wc2 - 0xDC00));
        } else if (wc >= 0xDC00 && wc <= 0xDFFF) {
            /* Lone low surrogate */
            return false;
        } else {
            codepoint = wc;
        }

        /* -------- UTF-8 encode -------- */
        if (codepoint <= 0x7F) {
            if (out + 1 >= cap) break;
            dst[out++] = (char)codepoint;
        } else if (codepoint <= 0x7FF) {
            if (out + 2 >= cap) break;
            dst[out++] = (char)(0xC0 | (codepoint >> 6));
            dst[out++] = (char)(0x80 | (codepoint & 0x3F));
        } else if (codepoint <= 0xFFFF) {
            if (out + 3 >= cap) break;
            dst[out++] = (char)(0xE0 | (codepoint >> 12));
            dst[out++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
            dst[out++] = (char)(0x80 | (codepoint & 0x3F));
        } else if (codepoint <= 0x10FFFF) {
            if (out + 4 >= cap) break;
            dst[out++] = (char)(0xF0 | (codepoint >> 18));
            dst[out++] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
            dst[out++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
            dst[out++] = (char)(0x80 | (codepoint & 0x3F));
        } else {
            return false;
        }
    }

    dst[out] = '\0';
    *dstlen = out;

    return (*src == 0);
}

static jclass getExceptionClass(JNIEnv* env, const char* cls_name, jclass* cache,pthread_mutex_t* mutex) {
	if (*cache != NULL) return *cache;
	pthread_mutex_lock(mutex);
	jclass local = _FindClass(env, cls_name);
	if (!local) {
		_ExceptionClear(env);
		pthread_mutex_unlock(mutex);
		return NULL;
	}
	*cache = _NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	pthread_mutex_unlock(mutex);
	return *cache;
}

JNIEXPORTC void throwJava(JNIEnv *env,const char* tag,const char* msg,const char* cls_name,jclass* cache,pthread_mutex_t* mutex) {
	jclass clas = getExceptionClass(env, cls_name, cache,mutex);
	if (!clas) {
#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_ERROR, tag, "Unable to find exception class: %s", cls_name);
#else
		fprintf(stderr, "[%s] ERROR: Unable to find exception class: %s\n", tag, cls_name);
		fflush(stderr);
#endif
		_ExceptionClear(env);
		return;
	}

	_ThrowNew(env, clas, msg);
}

#define class_free(env,cls,tex) \
	do { \
		pthread_mutex_lock(&tex); \
		if (cls) _DeleteGlobalRef(env,cls); \
		cls = NULL; \
		pthread_mutex_unlock(&tex); \
		pthread_mutex_destroy(&tex); \
	} while (0)


JNIEXPORTC void JNI_OnUnload(JavaVM* vm, void* reserved) {
	JNIEnv* env;
	_GetEnv(vm,env,JNI_VERSION_1_6);
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
}

JNIEXPORTC void throwIOException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/IOException",&ioExceptionCls,&ioExceptionMutex);
}

JNIEXPORTC void throwCharConversionException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/CharConversionException",&charConversionExceptionCls,&charConversionExceptionMutex);
}

JNIEXPORTC void throwEOFException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/EOFException",&eofExceptionCls,&eofExceptionMutex);
}


JNIEXPORTC void throwFileNotFoundException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/FileNotFoundException",&fileNotFoundExceptionCls,&fileNotFoundExceptionMutex);
}

JNIEXPORTC void throwInterruptedIOException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InterruptedIOException",&interruptedIOExceptionCls,&interruptedIOExceptionMutex);
}

JNIEXPORTC void throwInvalidClassException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InvalidClassException",&invalidClassExceptionCls,&invalidClassExceptionMutex);
}

JNIEXPORTC void throwInvalidObjectException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InvalidObjectException",&invalidObjectExceptionCls,&invalidObjectExceptionMutex);
}

JNIEXPORTC void throwNotActiveException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/NotActiveException",&notActiveExceptionCls,&notActiveExceptionMutex);
}

JNIEXPORTC void throwNotSerializableException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/NotSerializableException",&notSerializableExceptionCls,&notSerializableExceptionMutex);
}

JNIEXPORTC void throwOptionalDataException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/OptionalDataException",&optionalDataExceptionCls,&optionalDataExceptionMutex);
}

JNIEXPORTC void throwStreamCorruptedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/StreamCorruptedException",&streamCorruptedExceptionCls,&streamCorruptedExceptionMutex);
}

JNIEXPORTC void throwSyncFailedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/SyncFailedException",&syncFailedExceptionCls,&syncFailedExceptionMutex);
}

JNIEXPORTC void throwUTFDataFormatException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/UTFDataFormatException",&utfDataFormatExceptionCls,&utfDataFormatExceptionMutex);
}

JNIEXPORTC void throwUnsupportedEncodingException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/UnsupportedEncodingException",&unsupportedEncodingExceptionCls,&unsupportedEncodingExceptionMutex);
}

JNIEXPORTC void throwWriteAbortedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/WriteAbortedException",&writeAbortedExceptionCls,&writeAbortedExceptionMutex);
}
