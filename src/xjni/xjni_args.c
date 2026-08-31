#include <string.h>
#include <xjni_log.h>
#include <xjni_args.h>
#include <xjni_utils.h>
#include <xjni_string.h>

#include "base-jni.h"

#define JLSTR "java/lang/String"
#define JLSTR_LEN (sizeof(JLSTR) - 1)
#define CLASSNAME_LEN 4096

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

static jclass gByteCls = NULL;
static jmethodID gByteCtor = NULL;
static jmethodID gByteValue = NULL;

static jclass gShortCls = NULL;
static jmethodID gShortCtor = NULL;
static jmethodID gShortValue = NULL;

// Helpers to ensure classes are loaded
static jboolean ensureCharacterClass(JNIEnv *env) {
	if (gCharCls && gCharCtor && gCharValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Character");
	if (!local) return JNI_FALSE;
	gCharCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gCharCls) return JNI_FALSE;
	gCharCtor = _GetMethodID(env,gCharCls,"<init>","(C)V");
	gCharValue = _GetMethodID(env,gCharCls,"charValue","()C");
	return gCharCtor && gCharValue;
}

static jboolean ensureBooleanClass(JNIEnv *env) {
	if (gBooleanCls && gBooleanCtor && gBooleanValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Boolean");
	if (!local) return JNI_FALSE;
	gBooleanCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gBooleanCls) return JNI_FALSE;
	gBooleanCtor  = _GetMethodID(env,gBooleanCls,"<init>","(Z)V");
	gBooleanValue = _GetMethodID(env,gBooleanCls,"booleanValue","()Z");
	return gBooleanCtor && gBooleanValue;
}

static jboolean ensureIntegerClass(JNIEnv *env) {
	if (gIntCls && gIntCtor && gIntValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Integer");
	if (!local) return JNI_FALSE;
	gIntCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gIntCls) return JNI_FALSE;
	gIntCtor  = _GetMethodID(env,gIntCls,"<init>","(I)V");
	gIntValue = _GetMethodID(env,gIntCls,"intValue","()I");
	return gIntCtor && gIntValue;
}

static jboolean ensureLongClass(JNIEnv *env) {
	if (gLongCls && gLongCtor && gLongValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Long");
	if (!local) return JNI_FALSE;
	gLongCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gLongCls) return JNI_FALSE;
	gLongCtor  = _GetMethodID(env,gLongCls,"<init>","(J)V");
	gLongValue = _GetMethodID(env,gLongCls,"longValue","()J");
	return gLongCtor && gLongValue;
}

static jboolean ensureFloatClass(JNIEnv *env) {
	if (gFloatCls && gFloatCtor && gFloatValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Float");
	if (!local) return JNI_FALSE;
	gFloatCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gFloatCls) return JNI_FALSE;
	gFloatCtor  = _GetMethodID(env,gFloatCls,"<init>","(F)V");
	gFloatValue = _GetMethodID(env,gFloatCls,"floatValue","()F");
	return gFloatCtor && gFloatValue;
}

static jboolean ensureDoubleClass(JNIEnv *env) {
	if (gDoubleCls && gDoubleCtor && gDoubleValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Double");
	if (!local) return JNI_FALSE;
	gDoubleCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gDoubleCls) return JNI_FALSE;
	gDoubleCtor  = _GetMethodID(env,gDoubleCls,"<init>","(D)V");
	gDoubleValue = _GetMethodID(env,gDoubleCls,"doubleValue","()D");
	return gDoubleCtor && gDoubleValue;
}

static jboolean ensureByteClass(JNIEnv *env) {
	if (gByteCls && gByteCtor && gByteValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Byte");
	if (!local) return JNI_FALSE;
	gByteCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gByteCls) return JNI_FALSE;
	gByteCtor  = _GetMethodID(env,gByteCls,"<init>","(B)V");
	gByteValue = _GetMethodID(env,gByteCls,"byteValue","()B");
	return gByteCtor && gByteValue;
}

static jboolean ensureShortClass(JNIEnv *env) {
	if (gShortCls && gShortCtor && gShortValue) return JNI_TRUE;
	jclass local = _FindClass(env,"java/lang/Short");
	if (!local) return JNI_FALSE;
	gShortCls = (jclass)_NewGlobalRef(env,local);
	_DeleteLocalRef(env,local);
	if (!gShortCls) return JNI_FALSE;
	gShortCtor  = _GetMethodID(env,gShortCls,"<init>","(S)V");
	gShortValue = _GetMethodID(env,gShortCls,"shortValue","()S");
	return gShortCtor && gShortValue;
}

static void jargs_handle_object(JNIEnv *env,jargs_t args,jobject obj,jargs_op_t op,jsize index) {
	if (!env || !args || !obj) return;

	jsize len = _GetArrayLength(env,args);

	if (op == JARGS_OP_APPEND) {
		for (jsize i = 0; i < len; i++) {
			jobject cur = _GetObjectArrayElement(env,args,i);
			if (!cur) {
				_SetObjectArrayElement(env,args,i,obj);
				_DeleteLocalRef(env,obj);
				return;
			}
			_DeleteLocalRef(env,cur);
		}
		XJNI_LOGE("jargs_handle_object","jobjectArray full,cannot append");
		_DeleteLocalRef(env,obj);
	}
	else if (op == JARGS_OP_INSERT || op == JARGS_OP_REPLACE) {

		if (index < 0 || index >= len) {
			XJNI_LOGE("jargs_handle_object","Index out of bounds");
			_DeleteLocalRef(env,obj);
			return;
		}

		if (op == JARGS_OP_INSERT) {
			for (jsize i = len - 1; i > index; i--) {
				jobject tmp = _GetObjectArrayElement(env,args,i - 1);
				_SetObjectArrayElement(env,args,i,tmp);
				if (tmp) _DeleteLocalRef(env,tmp);
			}
		} else {
			jobject old = _GetObjectArrayElement(env,args,index);
			if (old) _DeleteLocalRef(env,old);
		}

		_SetObjectArrayElement(env,args,index,obj);
		_DeleteLocalRef(env,obj);
	}
}

static jobject jargs_box(JNIEnv *env,jclass cls,jmethodID ctor,...) {
	if (!cls || !ctor) return NULL;
	va_list ap;
	va_start(ap,ctor);
	jobject obj = _NewObjectV(env,cls,ctor,ap);
	va_end(ap);
	if (_ExceptionCheck(env)) { _ExceptionClear(env); return NULL; }
	return obj;
}

#define DEFINE_JARGS_FUNCS(type,jtype,cls_func) \
JNIEXPORTC void JNICALL JArgsAppend##type(JNIEnv *env,jargs_t args,jtype val) { \
	if (!cls_func(env)) return; \
	jobject obj = _NewObject(env,g##type##Cls,g##type##Ctor,val); \
	if (!obj) return; \
	jargs_handle_object(env,args,obj,JARGS_OP_APPEND,0); \
} \
\
JNIEXPORTC void JNICALL JArgsInsert##type(JNIEnv *env,jargs_t args,jtype val,jsize index) { \
	if (!cls_func(env)) return; \
	jobject obj = _NewObject(env,g##type##Cls,g##type##Ctor,val); \
	if (!obj) return; \
	jargs_handle_object(env,args,obj,JARGS_OP_INSERT,index); \
} \
\
JNIEXPORTC void JNICALL JArgsReplace##type(JNIEnv *env,jargs_t args,jtype val,jsize index) { \
	if (!cls_func(env)) return; \
	jobject obj = _NewObject(env,g##type##Cls,g##type##Ctor,val); \
	if (!obj) return; \
	jargs_handle_object(env,args,obj,JARGS_OP_REPLACE,index); \
}

DEFINE_JARGS_FUNCS(Char,jchar,ensureCharacterClass)
DEFINE_JARGS_FUNCS(Boolean,jboolean,ensureBooleanClass)
DEFINE_JARGS_FUNCS(Int,jint,ensureIntegerClass)
DEFINE_JARGS_FUNCS(Long,jlong,ensureLongClass)
DEFINE_JARGS_FUNCS(Float,jfloat,ensureFloatClass)
DEFINE_JARGS_FUNCS(Double,jdouble,ensureDoubleClass)
DEFINE_JARGS_FUNCS(Byte,jbyte,ensureByteClass)
DEFINE_JARGS_FUNCS(Short,jshort,ensureShortClass)

JNIEXPORTC jargs_t JNICALL NewJArgs(JNIEnv *env,jsize index,jclass cls,jobject init) {
	return _NewObjectArray(env,index,cls,init);
}

JNIEXPORTC jargs_t JNICALL JArgsStartV(JNIEnv *env,jsize index,const char* sig,va_list ap) {
	jargs_t args = NewJArgs(env,index,_FindClass(env,"java/lang/Object"),NULL);
	const char* p = sig;
	while (*p) {
		switch (*p) {
			case 'L': {
				const char* start = ++p;
				while (*p && *p != ';') p++;
				if (*p != ';') return args;
				size_t len = (size_t)(p - start);
				if (len == JLSTR_LEN && jmemcmp(start, JLSTR, JLSTR_LEN) == 0) {
					jstring str = va_arg(ap, jstring);
					JArgsAppendString(env, args, str);
				} else {
					if (len > 0 && len < CLASSNAME_LEN) {
						char className[CLASSNAME_LEN];
						jmemcpy(className, start, len);
						className[len] = '\0';
						jobject obj = va_arg(ap, jobject);
						if (obj && IsSameObjectChars(env, className, obj))
							JArgsAppendObject(env, args, obj);
					} else {
						(void)va_arg(ap, jobject);
					}
				}

				p++; /* skip ';' */
				break;
			}
			case 'I': {
				jint v = va_arg(ap,jint);
				JArgsAppendInt(env,args,v);
				p++;
				break;
			}
			case 'J': {
				jlong v = va_arg(ap,jlong);
				JArgsAppendLong(env,args,v);
				p++;
				break;
			}
			case 'Z': {
				int v = va_arg(ap,int);
				JArgsAppendBoolean(env,args,(jboolean)v);
				p++;
				break;
			}
			case 'F': {
				double v = va_arg(ap,double);
				JArgsAppendFloat(env,args,(jfloat)v);
				p++;
				break;
			}
			case 'D': {
				jdouble v = va_arg(ap,jdouble);
				JArgsAppendDouble(env,args,v);
				p++;
				break;
			}
			case 'C': {
				int v = va_arg(ap,int);
				JArgsAppendChar(env,args,(jchar)v);
				p++;
				break;
			}
			case 'S': {
				int v = va_arg(ap,int);
				JArgsAppendShort(env,args,(jshort)v);
				p++;
				break;
			}
			case 'B': {
				int v = va_arg(ap,int);
				JArgsAppendByte(env,args,(jbyte)v);
				p++;
				break;
			}
			default:
				p++;
				break;
		}
	}
	return args;
}

JNIEXPORTC jargs_t JNICALL JArgsStart(JNIEnv *env,jsize index,const char* sig,...) {
	va_list ap;
	va_start(ap,sig);
	jargs_t args = JArgsStartV(env,index,sig,ap);
	va_end(ap);
	return args;
}

// Helper to append object to first NULL slot
JNIEXPORTC void JNICALL JArgsAppendObject(JNIEnv *env,jargs_t args,jobject obj) {
	if (!env || !args || !obj) return;
	jsize len = _GetArrayLength(env,args);
	for (jsize i = 0; i < len; i++) {
		jobject cur = _GetObjectArrayElement(env,args,i);
		if (!cur) {
			_SetObjectArrayElement(env,args,i,obj);
			_DeleteLocalRef(env,obj);
			return;
		}
		_DeleteLocalRef(env,cur);
	}
	XJNI_LOGE("JArgsAppendObject","jobjectArray is full,cannot append element");
	_DeleteLocalRef(env,obj);
}

// Append functions
JNIEXPORTC void JNICALL JArgsAppendString(JNIEnv *env,jargs_t args,jstring obj) {
	JArgsAppendObject(env,args,(jobject)obj);
}

JNIEXPORTC void JNICALL JArgsAppendStringUTF(JNIEnv *env,jargs_t args,const char* fmt) {
	if (fmt == NULL) return;
	jstring jstr = _NewStringUTF(env,fmt);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JArgsAppendString(env,args,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JArgsInsertObject(JNIEnv *env,jargs_t args,jobject obj,jsize index) {
	jargs_handle_object(env,args,obj,JARGS_OP_INSERT,index);
}

JNIEXPORTC void JNICALL JArgsInsertString(JNIEnv *env,jargs_t args,jstring obj,jsize index) {
	JArgsInsertObject(env,args,obj,index);
}

JNIEXPORTC void JNICALL JArgsInsertStringUTF(JNIEnv *env,jargs_t args,const char* fmt,jsize index) {
	if (fmt == NULL) return;
	jstring jstr = _NewStringUTF(env,fmt);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JArgsInsertString(env,args,jstr,index);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JArgsReplaceObject(JNIEnv *env,jargs_t args,jobject obj,jsize index) {
	jargs_handle_object(env,args,obj,JARGS_OP_REPLACE,index);
}

JNIEXPORTC void JNICALL JArgsReplaceString(JNIEnv *env,jargs_t args,jstring obj,jsize index) {
	JArgsReplaceObject(env,args,obj,index);
}

JNIEXPORTC void JNICALL JArgsReplaceStringUTF(JNIEnv *env,jargs_t args,const char* fmt,jsize index) {
	if (fmt == NULL) return;
	jstring jstr = _NewStringUTF(env,fmt);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JArgsReplaceString(env,args,jstr,index);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JArgsDelete(JNIEnv *env,jargs_t args,jsize index) {
	if (!env || !args) return;
	jsize len = _GetArrayLength(env,args);
	if (index < 0 || index >= len) {
		XJNI_LOGE("JArgsDelete","Index out of bounds");
		return;
	}
	jobject old = _GetObjectArrayElement(env,args,index);
	if (old) _DeleteLocalRef(env,old);
	for (jsize i = index; i < len - 1; i++) {
		jobject next = _GetObjectArrayElement(env,args,i + 1);
		_SetObjectArrayElement(env,args,i,next);
		if (next) _DeleteLocalRef(env,next);
	}
	_SetObjectArrayElement(env,args,len - 1,NULL);
}

JNIEXPORTC jobject JNICALL GetJArgs(JNIEnv *env,jargs_t args,jsize index) {
	return _GetObjectArrayElement(env,args,index);
}

JNIEXPORTC jstring JNICALL GetJArgsString(JNIEnv *env,jargs_t args,jsize index) {
	return (jstring)GetJArgs(env,args,index);
}

JNIEXPORTC char* JNICALL GetJArgsStringUTF(JNIEnv *env,jargs_t args,jsize index) {
	jstring jstr = GetJArgsString(env,args,index);
	if (!jstr) return NULL;
	const char* str = _GetStringUTFChars(env,jstr,NULL);
	if (!str) {
		_DeleteLocalRef(env,jstr);
		return NULL;
	}
	char* ret = strdup(str);
	_ReleaseStringUTFChars(env,jstr,str);
	_DeleteLocalRef(env,jstr);
	return ret;
}

#define MAKE_GETJARGS(NAME,TYPE,CALL,ENSURE,VALUE,RET) \
JNIEXPORTC TYPE JNICALL NAME(JNIEnv *env,jargs_t args,jsize index) {\
	if (!ENSURE(env)) return RET;\
	jobject obj = GetJArgs(env,args,index);\
	if (!obj) return RET; \
	TYPE value = CALL(env,obj,VALUE);\
	if (_ExceptionCheck(env)) {\
		_DeleteLocalRef(env, obj);\
		return RET;\
	}\
	_DeleteLocalRef(env,obj);\
	return value;\
}

MAKE_GETJARGS(GetJArgsChar,jchar,_CallCharMethod,ensureCharacterClass,gCharValue,'\0')
MAKE_GETJARGS(GetJArgsBoolean,jboolean,_CallBooleanMethod,ensureBooleanClass,gBooleanValue,JNI_FALSE)
MAKE_GETJARGS(GetJArgsLong,jlong,_CallLongMethod,ensureLongClass,gLongValue,0)
MAKE_GETJARGS(GetJArgsFloat,jfloat,_CallFloatMethod,ensureFloatClass,gFloatValue,0.0)
MAKE_GETJARGS(GetJArgsDouble,jdouble,_CallDoubleMethod,ensureDoubleClass,gDoubleValue,0.0)
MAKE_GETJARGS(GetJArgsByte,jbyte,_CallByteMethod,ensureByteClass,gByteValue,0x00)
MAKE_GETJARGS(GetJArgsShort,jshort,_CallShortMethod,ensureShortClass,gShortValue,0)

JNIEXPORTC void JNICALL JArgsPopBack(JNIEnv *env,jargs_t args) {
	jsize len = _GetArrayLength(env, args);
	if (len <= 0) return;
	JArgsDelete(env, args, len - 1);
}
