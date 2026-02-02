#include <xjni.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

JNIEXPORTC void JNICALL JSnPrintf(JNIEnv *env, char* s, size_t maxlen, jstring format, jargs_t args) {
	if (!s || maxlen == 0 || !format) return;

	const char *fmt = _GetStringUTFChars(env, format, NULL);
	size_t pos = 0;
	int argIndex = 0;
	jsize argc = args ? _GetArrayLength(env, args) : 0;
	const char *p = fmt;

	while (*p && pos < maxlen - 1) {
		if (*p == '%' && *(p+1) != '\0') {
			p++; // skip '%'

			// Handle literal %%
			if (*p == '%') { s[pos++] = '%'; p++; continue; }

			// --- Flags ---
			int left_align = 0, force_sign = 0, zero_pad = 0;
			while (*p == '-' || *p == '+' || *p == '0') {
				if (*p == '-') left_align = 1;
				else if (*p == '+') force_sign = 1;
				else if (*p == '0') zero_pad = 1;
				p++;
			}

			// --- Width ---
			int width = 0;
			if (*p >= '0' && *p <= '9') width = strtol(p, (char**)&p, 10);

			// --- Precision ---
			int precision = -1;
			if (*p == '.') { p++; precision = strtol(p, (char**)&p, 10); }

			char spec = *p++;

			// No more arguments
			if (argIndex >= argc) { s[pos++] = '?'; continue; }

			jobject obj = _GetObjectArrayElement(env, args, argIndex++);
			jclass cls = _GetObjectClass(env, obj);

			char temp[256]; // sufficient for most cases
			temp[0] = '\0';

			// --- Integer types ---
			if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Integer"))) {
				jint val = _CallIntMethod(env, obj, _GetMethodID(env, cls, "intValue", "()I"));
				char fmtstr[32];
				snprintf(fmtstr, sizeof(fmtstr), "%%%s%s%s%d%c",
						 left_align ? "-" : "",
						 force_sign ? "+" : "",
						 zero_pad && !left_align ? "0" : "",
						 width, spec);
				snprintf(temp, sizeof(temp), fmtstr, val);

			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Long"))) {
				jlong val = _CallLongMethod(env, obj, _GetMethodID(env, cls, "longValue", "()J"));
				if (spec == 'd' || spec == 'i') snprintf(temp, sizeof(temp), "%*s%" PRId64,width, "", (int64_t)val);
				else if (spec == 'x') snprintf(temp, sizeof(temp), "%0*"PRIx64, width ? width : 0, (uint64_t)val);
				else if (spec == 'X') snprintf(temp, sizeof(temp), "%0*"PRIX64, width ? width : 0, (uint64_t)val);
				else snprintf(temp, sizeof(temp), "%" PRId64, (int64_t)val);
			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Short"))) {
				jshort val = _CallShortMethod(env, obj, _GetMethodID(env, cls, "shortValue", "()S"));
				snprintf(temp, sizeof(temp), "%hd", val);
			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Byte"))) {
				jbyte val = _CallByteMethod(env, obj, _GetMethodID(env, cls, "byteValue", "()B"));
				snprintf(temp, sizeof(temp), "%hhd", val);
			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Float"))) {
				jfloat val = _CallFloatMethod(env, obj, _GetMethodID(env, cls, "floatValue", "()F"));
				if (precision >= 0) snprintf(temp, sizeof(temp), "%*.*f", width, precision, val);
				else snprintf(temp, sizeof(temp), "%*f", width, val);
			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Double"))) {
				jdouble val = _CallDoubleMethod(env, obj, _GetMethodID(env, cls, "doubleValue", "()D"));
				if (precision >= 0) snprintf(temp, sizeof(temp), "%*.*f", width, precision, val);
				else snprintf(temp, sizeof(temp), "%*f", width, val);
			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Boolean"))) {
				jboolean val = _CallBooleanMethod(env, obj, _GetMethodID(env, cls, "booleanValue", "()Z"));
				snprintf(temp, sizeof(temp), "%s", val ? "true" : "false");
			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/Character"))) {
				jchar val = _CallCharMethod(env, obj, _GetMethodID(env, cls, "charValue", "()C"));
				char buf[4];
				int len = xjni_tochar(val,buf,sizeof(buf));
				if (len > 0 && len < sizeof(buf)) buf[len] = '\0';
				else buf[0] = '\0';
				snprintf(temp, sizeof(temp), "%s", buf);
			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/lang/String"))) {
				const char *str = _GetStringUTFChars(env, (jstring)obj, NULL);
				snprintf(temp, sizeof(temp), "%s", str);
				_ReleaseStringUTFChars(env, (jstring)obj, str);

			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/math/BigInteger"))) {
				jstring strObj = (jstring)_CallObjectMethod(env, obj, _GetMethodID(env, cls, "toString", "()Ljava/lang/String;"));
				const char *str = _GetStringUTFChars(env, strObj, NULL);
				snprintf(temp, sizeof(temp), "%s", str);
				_ReleaseStringUTFChars(env, strObj, str);
				_DeleteLocalRef(env, strObj);

			} else if (_IsInstanceOf(env, obj, _FindClass(env, "java/math/BigDecimal"))) {
				jstring strObj = (jstring)_CallObjectMethod(env, obj, _GetMethodID(env, cls, "toPlainString", "()Ljava/lang/String;"));
				const char *str = _GetStringUTFChars(env, strObj, NULL);
				snprintf(temp, sizeof(temp), "%s", str);
				_ReleaseStringUTFChars(env, strObj, str);
				_DeleteLocalRef(env, strObj);

			} else snprintf(temp, sizeof(temp), "<?>");

			// Copy to output buffer safely
			size_t len = strlen(temp);
			if (pos + len >= maxlen - 1) len = maxlen - 1 - pos;
			jmemcpy(s + pos, temp, len);
			pos += len;

			_DeleteLocalRef(env, obj);

		} else {
			s[pos++] = *p++;
		}
	}

	s[pos] = '\0';
	_ReleaseStringUTFChars(env, format, fmt);
}

JNIEXPORTC void JNICALL JSPrintf(JNIEnv *env,char* s,jstring format,jargs_t args) {
	JSnPrintf(env,s,SIZE_MAX,format,args);
}

JNIEXPORTC void JNICALL JSnPrintfUTF(JNIEnv *env,char* s,size_t maxlen,const char* format,jargs_t args) {
	if (format == NULL) return;
	jstring jstr = _NewStringUTF(env,format);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JSnPrintf(env,s,maxlen,jstr,args);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JSPrintfUTF(JNIEnv *env,char* s,const char* format,jargs_t args) {
	JSnPrintfUTF(env,s,SIZE_MAX,format,args);
}
