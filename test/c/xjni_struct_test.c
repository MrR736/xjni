#include <stdlib.h>
#include <string.h>
#include <xjni_struct.h>

typedef struct {
	jint field;
	jdouble value;
} MyStruct;

/* ==========================================================
 * Callback Implementations
 * ========================================================== */

static void* my_alloc(JNIEnv *env, void* data, size_t size) {
	void* ptr = malloc(size);
	if (!ptr) return NULL;
	if (data) memcpy(ptr, data, size);
	else memset(ptr, 0, size);
	return ptr;
}

static void my_free(void* ptr) {
	free(ptr);
}

static jobject my_jalloc(JNIEnv *env, size_t size) {
	jclass cls = (*env)->FindClass(env, "MyStructObject");
	if (!cls) return NULL;

	jmethodID ctor = (*env)->GetMethodID(env, cls, "<init>", "()V");
	if (!ctor) return NULL;

	return (*env)->NewObject(env, cls, ctor);
}

static void my_set(JNIEnv *env, void* in, jobject out) {
	MyStruct* data = (MyStruct*)in;

	jclass cls = (*env)->GetObjectClass(env, out);

	jfieldID fieldID = (*env)->GetFieldID(env, cls, "field", "I");
	jfieldID valueID = (*env)->GetFieldID(env, cls, "value", "D");

	(*env)->SetIntField(env, out, fieldID, data->field);
	(*env)->SetDoubleField(env, out, valueID, data->value);
}

static void* my_get(JNIEnv *env, jobject in) {
	MyStruct* data = malloc(sizeof(MyStruct));
	if (!data) return NULL;

	jclass cls = (*env)->GetObjectClass(env, in);

	jfieldID fieldID = (*env)->GetFieldID(env, cls, "field", "I");
	jfieldID valueID = (*env)->GetFieldID(env, cls, "value", "D");

	data->field = (*env)->GetIntField(env, in, fieldID);
	data->value = (*env)->GetDoubleField(env, in, valueID);

	return data;
}

/* ==========================================================
 * Context
 * ========================================================== */
static xjni_struct_ctx_t ctx = {
	.set = my_set,
	.get = my_get,
	.freef = my_free,
	.alloc = my_alloc,
	.jalloc = my_jalloc,
	.flags = 0
};

/* ==========================================================
 * Native Methods
 * ========================================================== */

JNIEXPORT jobject JNICALL
Java_MyStructObject_create(JNIEnv *env, jclass cls) {

	MyStruct init = { 42, 3.14 };

	return xjni_struct_new(&ctx, env, &init, sizeof(MyStruct));
}

JNIEXPORT void JNICALL
Java_MyStructObject_modify(JNIEnv *env, jclass cls, jobject obj) {
	MyStruct* data = (MyStruct*)xjni_struct_get(&ctx, env, obj);

	if (!data) return;

	data->field += 10;
	data->value *= 2.0;

	xjni_struct_release(&ctx, env, obj, data, 0);
}
