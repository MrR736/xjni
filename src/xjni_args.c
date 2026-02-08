#include <string.h>
#include <xjni_log.h>
#include <xjni_args.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

typedef enum {
	JARGS_OP_APPEND,
	JARGS_OP_INSERT,
	JARGS_OP_REPLACE
} jargs_op_t;

static jclass gCharCls = NULL;
static jmethodID gCharCtor = NULL;
static jmethodID gCharValue = NULL;

static jclass gBooleanCls = NULL;
static jmethodID gBooleanCtor = NULL;
static jmethodID gBooleanValue = NULL;

static jclass gIntCls = NULL;
static jmethodID gIntCtor = NULL;
static jmethodID gIntValue = NULL;

static jclass gLongCls = NULL;
static jmethodID gLongCtor = NULL;
static jmethodID gLongValue = NULL;

static jclass gFloatCls = NULL;
static jmethodID gFloatCtor = NULL;
static jmethodID gFloatValue = NULL;

static jclass gDoubleCls = NULL;
static jmethodID gDoubleCtor = NULL;
static jmethodID gDoubleValue = NULL;

// Helpers to ensure classes are loaded
static jboolean ensureCharacterClass(JNIEnv *env) {
	if (gCharCls && gCharCtor && gCharValue) return JNI_TRUE;
	jclass local = _FindClass(env, "java/lang/Character");
	if (!local) return JNI_FALSE;
	gCharCls = (jclass)_NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	if (!gCharCls) return JNI_FALSE;
	gCharCtor = _GetMethodID(env, gCharCls, "<init>", "(C)V");
	gCharValue = _GetMethodID(env, gCharCls, "charValue", "()C");
	return gCharCtor && gCharValue;
}

static jboolean ensureBooleanClass(JNIEnv *env) {
	if (gBooleanCls && gBooleanCtor && gBooleanValue) return JNI_TRUE;
	jclass local = _FindClass(env, "java/lang/Boolean");
	if (!local) return JNI_FALSE;
	gBooleanCls = (jclass)_NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	if (!gBooleanCls) return JNI_FALSE;
	gBooleanCtor  = _GetMethodID(env, gBooleanCls, "<init>", "(Z)V");
	gBooleanValue = _GetMethodID(env, gBooleanCls, "booleanValue", "()Z");
	return gBooleanCtor && gBooleanValue;
}

static jboolean ensureIntegerClass(JNIEnv *env) {
	if (gIntCls && gIntCtor && gIntValue) return JNI_TRUE;
	jclass local = _FindClass(env, "java/lang/Integer");
	if (!local) return JNI_FALSE;
	gIntCls = (jclass)_NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	if (!gIntCls) return JNI_FALSE;
	gIntCtor  = _GetMethodID(env, gIntCls, "<init>", "(I)V");
	gIntValue = _GetMethodID(env, gIntCls, "intValue", "()I");
	return gIntCtor && gIntValue;
}

static jboolean ensureLongClass(JNIEnv *env) {
	if (gLongCls && gLongCtor && gLongValue) return JNI_TRUE;
	jclass local = _FindClass(env, "java/lang/Long");
	if (!local) return JNI_FALSE;
	gLongCls = (jclass)_NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	if (!gLongCls) return JNI_FALSE;
	gLongCtor  = _GetMethodID(env, gLongCls, "<init>", "(J)V");
	gLongValue = _GetMethodID(env, gLongCls, "longValue", "()J");
	return gLongCtor && gLongValue;
}

static jboolean ensureFloatClass(JNIEnv *env) {
	if (gFloatCls && gFloatCtor && gFloatValue) return JNI_TRUE;
	jclass local = _FindClass(env, "java/lang/Float");
	if (!local) return JNI_FALSE;
	gFloatCls = (jclass)_NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	if (!gFloatCls) return JNI_FALSE;
	gFloatCtor  = _GetMethodID(env, gFloatCls, "<init>", "(F)V");
	gFloatValue = _GetMethodID(env, gFloatCls, "floatValue", "()F");
	return gFloatCtor && gFloatValue;
}

static jboolean ensureDoubleClass(JNIEnv *env) {
	if (gDoubleCls && gDoubleCtor && gDoubleValue) return JNI_TRUE;
	jclass local = _FindClass(env, "java/lang/Double");
	if (!local) return JNI_FALSE;
	gDoubleCls = (jclass)_NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	if (!gDoubleCls) return JNI_FALSE;
	gDoubleCtor  = _GetMethodID(env, gDoubleCls, "<init>", "(D)V");
	gDoubleValue = _GetMethodID(env, gDoubleCls, "doubleValue", "()D");
	return gDoubleCtor && gDoubleValue;
}

static void jargs_handle_object(JNIEnv *env, jargs_t args, jobject obj, jargs_op_t op, jsize index) {
	if (!env || !args || !obj) return;

	jsize len = _GetArrayLength(env, args);

	if (op == JARGS_OP_APPEND) {
		// append to first NULL slot
		for (jsize i = 0; i < len; i++) {
			jobject cur = _GetObjectArrayElement(env, args, i);
			if (!cur) {
				_SetObjectArrayElement(env, args, i, obj);
				return;
			}
			_DeleteLocalRef(env, cur);
		}
		XJNI_LOGE("jargs_handle_object", "jobjectArray full, cannot append");
		_DeleteLocalRef(env, obj);
	} else if (op == JARGS_OP_INSERT || op == JARGS_OP_REPLACE) {
		if (index < 0 || index >= len) {
			XJNI_LOGE("jargs_handle_object", "Index out of bounds");
			_DeleteLocalRef(env, obj);
			return;
		}
		if (op == JARGS_OP_INSERT) {
			// shift elements right
			for (jsize i = len - 1; i > index; i--) {
				jobject tmp = _GetObjectArrayElement(env, args, i - 1);
				_SetObjectArrayElement(env, args, i, tmp);
				if (tmp) _DeleteLocalRef(env, tmp);
			}
		} else {
			// REPLACE: delete old element
			jobject old = _GetObjectArrayElement(env, args, index);
			if (old) _DeleteLocalRef(env, old);
		}
		_SetObjectArrayElement(env, args, index, obj);
	}
}

static jobject jargs_box(JNIEnv *env, jclass cls, jmethodID ctor, ...) {
	if (!cls || !ctor) return NULL;
	va_list ap;
	va_start(ap, ctor);
	jobject obj = _NewObjectV(env, cls, ctor, ap);
	va_end(ap);
	if (_ExceptionCheck(env)) { _ExceptionClear(env); return NULL; }
	return obj;
}

#define DEFINE_JARGS_FUNCS(type, jtype, cls_func) \
JNIEXPORTC void JNICALL JArgsAppend##type(JNIEnv *env, jargs_t args, jtype val) { \
	if (!cls_func(env)) return; \
	jobject obj = _NewObject(env, g##type##Cls, g##type##Ctor, val); \
	if (!obj) return; \
	jargs_handle_object(env, args, obj, JARGS_OP_APPEND, 0); \
} \
\
JNIEXPORTC void JNICALL JArgsInsert##type(JNIEnv *env, jargs_t args, jtype val, jsize index) { \
	if (!cls_func(env)) return; \
	jobject obj = _NewObject(env, g##type##Cls, g##type##Ctor, val); \
	if (!obj) return; \
	jargs_handle_object(env, args, obj, JARGS_OP_INSERT, index); \
} \
\
JNIEXPORTC void JNICALL JArgsReplace##type(JNIEnv *env, jargs_t args, jtype val, jsize index) { \
	if (!cls_func(env)) return; \
	jobject obj = _NewObject(env, g##type##Cls, g##type##Ctor, val); \
	if (!obj) return; \
	jargs_handle_object(env, args, obj, JARGS_OP_REPLACE, index); \
}

DEFINE_JARGS_FUNCS(Char, jchar, ensureCharacterClass)
DEFINE_JARGS_FUNCS(Boolean, jboolean, ensureBooleanClass)
DEFINE_JARGS_FUNCS(Int, jint, ensureIntegerClass)
DEFINE_JARGS_FUNCS(Long, jlong, ensureLongClass)
DEFINE_JARGS_FUNCS(Float, jfloat, ensureFloatClass)
DEFINE_JARGS_FUNCS(Double, jdouble, ensureDoubleClass)

JNIEXPORTC jargs_t JNICALL NewJArgs(JNIEnv *env, jsize index,jclass cls,jobject init) {
	return _NewObjectArray(env,index,cls,init);
}

// Helper to append object to first NULL slot
JNIEXPORTC void JNICALL JArgsAppendObject(JNIEnv *env, jargs_t args, jobject obj) {
	if (!env || !args || !obj) return;
	jsize len = _GetArrayLength(env, args);
	for (jsize i = 0; i < len; i++) {
		jobject current = _GetObjectArrayElement(env, args, i);
		if (!current) {
			_SetObjectArrayElement(env, args, i, obj);
			return;
		}
		_DeleteLocalRef(env, current);
	}
	XJNI_LOGE("JArgsAppendObject", "jobjectArray is full, cannot append element");
	_DeleteLocalRef(env, obj);
}

// Append functions
JNIEXPORTC void JNICALL JArgsAppendString(JNIEnv *env, jargs_t args, jstring obj) {
	JArgsAppendObject(env, args, obj);
}

JNIEXPORTC void JNICALL JArgsAppendStringUTF(JNIEnv *env, jargs_t args,const char* fmt) {
	if (fmt == NULL) return;
	jstring jstr = _NewStringUTF(env,fmt);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JArgsAppendString(env,args,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JArgsInsertObject(JNIEnv *env, jargs_t args, jobject obj, jsize index) {
	if (!env || !args || !obj) return;
	jsize len = _GetArrayLength(env, args);
	if (index < 0 || index >= len) {
		XJNI_LOGE("JArgsInsertObject", "Index out of bounds");
		return;
	}
	jobject old = _GetObjectArrayElement(env, args, index);
	if (old) _DeleteLocalRef(env, old);
	_SetObjectArrayElement(env, args, index, obj);
}

JNIEXPORTC void JNICALL JArgsInsertString(JNIEnv *env, jargs_t args, jstring obj, jsize index) {
	JArgsInsertObject(env, args, obj, index);
}

JNIEXPORTC void JNICALL JArgsInsertStringUTF(JNIEnv *env, jargs_t args,const char* fmt, jsize index) {
	if (fmt == NULL) return;
	jstring jstr = _NewStringUTF(env,fmt);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JArgsInsertString(env,args,jstr,index);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JArgsReplaceObject(JNIEnv *env, jargs_t args, jobject obj, jsize index) {
	if (!env || !args || !obj) return;
	jsize len = _GetArrayLength(env, args);
	if (index < 0 || index >= len) {
		XJNI_LOGE("JArgsReplaceObject", "Index out of bounds");
		return;
	}
	jobject old = _GetObjectArrayElement(env, args, index);
	if (old) _DeleteLocalRef(env, old);  // release old element
	_SetObjectArrayElement(env, args, index, obj);
}

JNIEXPORTC void JNICALL JArgsReplaceString(JNIEnv *env, jargs_t args, jstring obj, jsize index) {
	JArgsReplaceObject(env, args, obj, index);
}

JNIEXPORTC void JNICALL JArgsReplaceStringUTF(JNIEnv *env, jargs_t args,const char* fmt, jsize index) {
	if (fmt == NULL) return;
	jstring jstr = _NewStringUTF(env,fmt);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JArgsReplaceString(env,args,jstr,index);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JArgsDelete(JNIEnv *env, jargs_t args, jsize index) {
	if (!env || !args) return;
	jsize len = _GetArrayLength(env, args);
	if (index < 0 || index >= len) {
		XJNI_LOGE("JArgsDelete", "Index out of bounds");
		return;
	}
	jobject old = _GetObjectArrayElement(env, args, index);
	if (old) _DeleteLocalRef(env, old);
	for (jsize i = index; i < len - 1; i++) {
		jobject next = _GetObjectArrayElement(env, args, i + 1);
		_SetObjectArrayElement(env, args, i, next);
		if (next) _DeleteLocalRef(env, next);
	}
	_SetObjectArrayElement(env, args, len - 1, NULL);
}

JNIEXPORTC jobject JNICALL GetJArgs(JNIEnv *env, jargs_t args, jsize index) {
	return _GetObjectArrayElement(env, args, index);
}

JNIEXPORTC jstring JNICALL GetJArgsString(JNIEnv *env, jargs_t args, jsize index) {
	return (jstring)GetJArgs(env, args, index);
}

JNIEXPORTC char* JNICALL GetJArgsStringUTF(JNIEnv *env, jargs_t args, jsize index) {
    jstring jstr = GetJArgsString(env, args, index);
    if (!jstr) return NULL;
    const char* str = _GetStringUTFChars(env, jstr, NULL);
    if (!str) {
        _DeleteLocalRef(env, jstr);
        return NULL;
    }
    char* ret = strdup(str);
    _ReleaseStringUTFChars(env, jstr, str);
    _DeleteLocalRef(env, jstr);
    return ret;
}

JNIEXPORTC jchar JNICALL GetJArgsChar(JNIEnv *env, jargs_t args, jsize index) {
	if (!ensureCharacterClass(env)) return '\0';
	jobject obj = GetJArgs(env, args, index);
	jchar value = _CallCharMethod(env, obj, gCharValue);
	_DeleteLocalRef(env, obj);
	return value;
}

JNIEXPORTC jboolean JNICALL GetJArgsBoolean(JNIEnv *env, jargs_t args, jsize index) {
	if (!ensureBooleanClass(env)) return JNI_FALSE;
	jobject obj = GetJArgs(env, args, index);
	jboolean value = _CallBooleanMethod(env, obj, gBooleanValue);
	_DeleteLocalRef(env, obj);
	return value;
}

JNIEXPORTC jlong JNICALL GetJArgsLong(JNIEnv *env, jargs_t args, jsize index) {
	if (!ensureLongClass(env)) { return 0; }
	jobject obj = GetJArgs(env, args, index);
	jlong value = _CallLongMethod(env, obj, gLongValue);
	_DeleteLocalRef(env, obj);
	return value;
}

JNIEXPORTC jfloat JNICALL GetJArgsFloat(JNIEnv *env, jargs_t args, jsize index) {
	if (!ensureFloatClass(env)) { return 0.0; }
	jobject obj = GetJArgs(env, args, index);
	jfloat value = _CallFloatMethod(env, obj, gFloatValue);
	_DeleteLocalRef(env, obj);
	return value;
}

JNIEXPORTC jdouble JNICALL GetJArgsDouble(JNIEnv *env, jargs_t args, jsize index) {
	if (!ensureDoubleClass(env)) { return 0.0; }
	jobject obj = GetJArgs(env, args, index);
	jdouble value = _CallDoubleMethod(env, obj, gDoubleValue);
	_DeleteLocalRef(env, obj);
	return value;
}
