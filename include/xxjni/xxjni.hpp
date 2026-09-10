/**
 * @file xxjni.hpp
 * @brief Extern JNI Utility in C++
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __XXJNI_HPP__
#define __XXJNI_HPP__

#include <string>
#include <xjni.h>
#include <xjni_hashmap.h>
#include <xjni_pool.h>

#include "jni.hpp"

/** @defgroup XXJNI_VERSION Version Macros
 *  @brief Version information for XJNI
 *  @{
 */
#ifndef _XXJNI_VERSION_MAJOR
#define _XXJNI_VERSION_MAJOR	1
#endif
#ifndef _XXJNI_VERSION_MINOR
#define _XXJNI_VERSION_MINOR	0
#endif
#ifndef _XXJNI_VERSION_PATCH
#define _XXJNI_VERSION_PATCH	1
#endif

#define _XXJNI_VERSION		((_XXJNI_VERSION_MAJOR * 1000) + (_XXJNI_VERSION_MINOR * 100) + _XXJNI_VERSION_PATCH)

#define __XXJNI_VERSION(major,minor,patch) \
	(((major * 1000) + (minor * 100) + patch) == _XXJNI_VERSION)
/** @} */

#define __MAKE_XXJNI_THROW_FUN(NAME) \
using ::NAME; \
using ::NAME##V; \
using ::NAME##F;

#define __MAKE_XXJNI_CLASS_THROW_FUN(NAME)\
void NAME(const char* tag, const char* msg) { \
	::NAME(_env,tag,msg); \
} \
void NAME##V(const char* tag, const char* msg, va_list ap) { \
	::NAME##V(_env, tag, msg, ap); \
} \
void NAME##F(const char* tag, const char* msg, ...) { \
	va_list args; \
	va_start(args, msg); \
	::NAME##V(_env, tag, msg, args); \
	va_end(args); \
}

namespace xxjni {

	class xxjni_load : public jni::XJavaVM {
	private:
		JavaVM* _vm = nullptr;
		jint _ver = 0;
		bool _ver_ok = false;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_load() noexcept = default;
#endif
		explicit xxjni_load(JavaVM* vm,jint ver) noexcept : _vm(vm), _ver(ver), XJavaVM(vm), _ver_ok(SupportsVersion(_ver)) { }

		explicit constexpr operator bool() const noexcept {
			return _vm != nullptr && _ver_ok;
		}

		JavaVM* get() const noexcept {
			return _vm;
		}

		jint getVersion() const noexcept {
			return _ver;
		}

		bool isVersionSupported() const noexcept {
			return _ver_ok;
		}

		enum XXJNI_TYPE { ALL, NEW, UTILY };

		jint OnLoad(void* reserved, XXJNI_TYPE jt) {
			if (jt == NEW) return ::XJNI_New_OnLoad(_vm,reserved,_ver);
			else if (jt == UTILY) return ::XJNI_Utily_OnLoad(_vm,reserved,_ver);
			else return ::XJNI_OnLoad(_vm,reserved,_ver);
		}

		void OnUnload(void* reserved, XXJNI_TYPE jt) {
			if (jt == NEW) ::XJNI_New_OnUnload(_vm,reserved,_ver);
			else if (jt == UTILY) ::XJNI_Utily_OnUnload(_vm,reserved,_ver);
			else ::XJNI_OnUnload(_vm,reserved,_ver);
		}
	};

	class xxjni_new {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_new() noexcept = default;
#endif
		explicit constexpr xxjni_new(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		jobjectArray NewObjectArray(jclass clz, jsize len) {
			return ::xjni_NewObjectArray(_env,clz,len);
		}

		jbyteArray NewByteArray(const jbyte* data, jsize len) {
			return ::xjni_NewByteArray(_env,data,len);
		}

		jintArray NewIntArray(const jint* data, jsize len) {
			return ::xjni_NewIntArray(_env,data,len);
		}

		jlongArray NewLongArray(const jlong* data, jsize len) {
			return ::xjni_NewLongArray(_env,data,len);
		}

		jfloatArray NewFloatArray(const jfloat* data, jsize len) {
			return ::xjni_NewFloatArray(_env,data,len);
		}

		jdoubleArray NewDoubleArray(const jdouble* data, jsize len) {
			return ::xjni_NewDoubleArray(_env,data,len);
		}

		jobject NewObjectBuilderV(const char* className, const char* sig, va_list ap) {
			return ::NewObjectBuilderV(_env,className,sig,ap);
		}

		jobject NewObjectBuilder(const char* className, const char* sig, ...) {
			va_list args;
			va_start(args, sig);
			jobject ret = ::NewObjectBuilderV(_env, className, sig, args);
			va_end(args);
			return ret;
		}
	};

	class xxjni_utils {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_utils() noexcept = default;
#endif
		explicit constexpr xxjni_utils(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		jboolean IsSameObjectChars(const char* className, jobject obj2) {
			return ::IsSameObjectChars(_env,className,obj2);
		}

		jfieldID GetFieldIDByName(const char* className,const char* fieldName,const char* sig) {
			return ::GetFieldIDByName(_env,className,fieldName,sig);
		}

		jfieldID GetFieldIDByObject(jobject obj,const char* fieldName,const char* sig) {
			return ::GetFieldIDByObject(_env,obj,fieldName,sig);
		}

		jstring GetObjectName(jobject obj) {
			return ::GetObjectName(_env,obj);
		}

		char* GetObjectNameChars(jobject obj) {
			return ::GetObjectNameChars(_env,obj);
		}

		std::string GetObjectNameString(jobject obj) {
			char* cobj = GetObjectNameChars(obj);
			if (!cobj) return {};
			std::string s(cobj);
			std::free(cobj);
			return s;
		}
	};

	class xxjni_args {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_args() noexcept = default;
#endif
		explicit constexpr xxjni_args(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		jargs_t NewJArgs(jsize index, jclass cls, jobject init) {
			return ::NewJArgs(_env, index, cls, init);
		}

		void push_back(jargs_t args, jobject obj) {
			::JArgsPushBackObject(_env, args, obj);
		}

		void push_back(jargs_t args, jstring obj) {
			::JArgsPushBackString(_env, args, obj);
		}

		void push_back(jargs_t args, const char* str) {
			::JArgsPushBackStringUTF(_env, args, str);
		}

		void push_back(jargs_t args, jchar obj) {
			::JArgsPushBackChar(_env, args, obj);
		}

		void push_back(jargs_t args, jboolean obj) {
			::JArgsPushBackBoolean(_env, args, obj);
		}

		void push_back(jargs_t args, jint obj) {
			::JArgsPushBackInt(_env, args, obj);
		}

		void push_back(jargs_t args, jlong obj) {
			::JArgsPushBackLong(_env, args, obj);
		}

		void push_back(jargs_t args, jfloat obj) {
			::JArgsPushBackFloat(_env, args, obj);
		}

		void push_back(jargs_t args, jdouble obj) {
			::JArgsPushBackDouble(_env, args, obj);
		}

		void push_back(jargs_t args, jbyte obj) {
			::JArgsPushBackByte(_env, args, obj);
		}

		void push_back(jargs_t args, jshort obj) {
			::JArgsPushBackShort(_env, args, obj);
		}

		void insert(jargs_t args, jobject obj, jsize index) {
			::JArgsInsertObject(_env, args, obj, index);
		}

		void insert(jargs_t args, jstring obj, jsize index) {
			::JArgsInsertString(_env, args, obj, index);
		}

		void insert(jargs_t args, const char* str, jsize index) {
			::JArgsInsertStringUTF(_env, args, str, index);
		}

		void insert(jargs_t args, jchar obj, jsize index) {
			::JArgsInsertChar(_env, args, obj, index);
		}

		void insert(jargs_t args, jboolean obj, jsize index) {
			::JArgsInsertBoolean(_env, args, obj, index);
		}

		void insert(jargs_t args, jint obj, jsize index) {
			::JArgsInsertInt(_env, args, obj, index);
		}

		void insert(jargs_t args, jlong obj, jsize index) {
			::JArgsInsertLong(_env, args, obj, index);
		}

		void insert(jargs_t args, jfloat obj, jsize index) {
			::JArgsInsertFloat(_env, args, obj, index);
		}

		void insert(jargs_t args, jdouble obj, jsize index) {
			::JArgsInsertDouble(_env, args, obj, index);
		}

		void insert(jargs_t args, jbyte obj, jsize index) {
			::JArgsInsertByte(_env, args, obj, index);
		}

		void insert(jargs_t args, jshort obj, jsize index) {
			::JArgsInsertShort(_env, args, obj, index);
		}

		void replace(jargs_t args, jobject obj, jsize index) {
			::JArgsReplaceObject(_env, args, obj, index);
		}

		void replace(jargs_t args, jstring obj, jsize index) {
			::JArgsReplaceString(_env, args, obj, index);
		}

		void replace(jargs_t args, const char* str, jsize index) {
			::JArgsReplaceStringUTF(_env, args, str, index);
		}

		void replace(jargs_t args, jchar obj, jsize index) {
			::JArgsReplaceChar(_env, args, obj, index);
		}

		void replace(jargs_t args, jboolean obj, jsize index) {
			::JArgsReplaceBoolean(_env, args, obj, index);
		}

		void replace(jargs_t args, jint obj, jsize index) {
			::JArgsReplaceInt(_env, args, obj, index);
		}

		void replace(jargs_t args, jlong obj, jsize index) {
			::JArgsReplaceLong(_env, args, obj, index);
		}

		void replace(jargs_t args, jfloat obj, jsize index) {
			::JArgsReplaceFloat(_env, args, obj, index);
		}

		void replace(jargs_t args, jdouble obj, jsize index) {
			::JArgsReplaceDouble(_env, args, obj, index);
		}

		void replace(jargs_t args, jbyte obj, jsize index) {
			::JArgsReplaceByte(_env, args, obj, index);
		}

		void replace(jargs_t args, jshort obj, jsize index) {
			::JArgsReplaceShort(_env, args, obj, index);
		}

		void erase(jargs_t args, jsize index) {
			::JArgsDelete(_env, args, index);
		}

		jobject get(jargs_t args, jsize index) {
			return ::GetJArgs(_env, args, index);
		}

		jstring getString(jargs_t args, jsize index) {
			return ::GetJArgsString(_env, args, index);
		}

		char* getStringUTF(jargs_t args, jsize index) {
			return ::GetJArgsStringUTF(_env, args, index);
		}

		jchar getChar(jargs_t args, jsize index) {
			return ::GetJArgsChar(_env, args, index);
		}

		jboolean getBoolean(jargs_t args, jsize index) {
			return ::GetJArgsBoolean(_env, args, index);
		}

		jlong getLong(jargs_t args, jsize index) {
			return ::GetJArgsLong(_env, args, index);
		}

		jfloat getFloat(jargs_t args, jsize index) {
			return ::GetJArgsFloat(_env, args, index);
		}

		jdouble getDouble(jargs_t args, jsize index) {
			return ::GetJArgsDouble(_env, args, index);
		}

		jbyte getByte(jargs_t args, jsize index) {
			return ::GetJArgsByte(_env, args, index);
		}

		jshort getShort(jargs_t args, jsize index) {
			return ::GetJArgsShort(_env, args, index);
		}

		jargs_t startV(jsize index, const char* sig, va_list ap) {
			return ::JArgsStartV(_env, index, sig, ap);
		}

		jargs_t start(jsize index, const char* sig, ...) {
			va_list args;
			va_start(args, sig);
			jargs_t ret = ::JArgsStartV(_env, index, sig, args);
			va_end(args);
			return ret;
		}

		void pop_back(jargs_t args) {
			::JArgsPopBack(_env, args);
		}
	};

	class xxjni_arrayfield {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_arrayfield() noexcept = default;
#endif
		explicit constexpr xxjni_arrayfield(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		jobjectArray GetObjectArrayField(jobject obj, jfieldID fid) {
			return ::GetObjectArrayField(_env, obj, fid);
		}

		jobjectArray GetStaticObjectArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticObjectArrayField(_env, cls, fid);
		}

		void SetObjectArrayField(jobject obj, jfieldID fid, jobjectArray value) {
			::SetObjectArrayField(_env, obj, fid, value);
		}

		void SetStaticObjectArrayField(jclass cls, jfieldID fid, jobjectArray value) {
			::SetStaticObjectArrayField(_env, cls, fid, value);
		}

		jarray GetArrayField(jobject obj, jfieldID fid) {
			return ::GetArrayField(_env, obj, fid);
		}

		jarray GetStaticArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticArrayField(_env, cls, fid);
		}

		void SetArrayField(jobject obj, jfieldID fid, jarray value) {
			::SetArrayField(_env, obj, fid, value);
		}

		void SetStaticArrayField(jclass cls, jfieldID fid, jarray value) {
			::SetStaticArrayField(_env, cls, fid, value);
		}

		jbyteArray GetByteArrayField(jobject obj, jfieldID fid) {
			return ::GetByteArrayField(_env, obj, fid);
		}

		jbyteArray GetStaticByteArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticByteArrayField(_env, cls, fid);
		}

		void SetByteArrayField(jobject obj, jfieldID fid, jbyteArray value) {
			::SetByteArrayField(_env, obj, fid, value);
		}

		void SetStaticByteArrayField(jclass cls, jfieldID fid, jbyteArray value) {
			::SetStaticByteArrayField(_env, cls, fid, value);
		}

		jintArray GetIntArrayField(jobject obj, jfieldID fid) {
			return ::GetIntArrayField(_env, obj, fid);
		}

		jintArray GetStaticIntArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticIntArrayField(_env, cls, fid);
		}

		void SetIntArrayField(jobject obj, jfieldID fid, jintArray value) {
			::SetIntArrayField(_env, obj, fid, value);
		}

		void SetStaticIntArrayField(jclass cls, jfieldID fid, jintArray value) {
			::SetStaticIntArrayField(_env, cls, fid, value);
		}

		jlongArray GetLongArrayField(jobject obj, jfieldID fid) {
			return ::GetLongArrayField(_env, obj, fid);
		}

		jlongArray GetStaticLongArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticLongArrayField(_env, cls, fid);
		}

		void SetLongArrayField(jobject obj, jfieldID fid, jlongArray value) {
			::SetLongArrayField(_env, obj, fid, value);
		}

		void SetStaticLongArrayField(jclass cls, jfieldID fid, jlongArray value) {
			::SetStaticLongArrayField(_env, cls, fid, value);
		}

		jfloatArray GetFloatArrayField(jobject obj, jfieldID fid) {
			return ::GetFloatArrayField(_env, obj, fid);
		}

		jfloatArray GetStaticFloatArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticFloatArrayField(_env, cls, fid);
		}

		void SetFloatArrayField(jobject obj, jfieldID fid, jfloatArray value) {
			::SetFloatArrayField(_env, obj, fid, value);
		}

		void SetStaticFloatArrayField(jclass cls, jfieldID fid, jfloatArray value) {
			::SetStaticFloatArrayField(_env, cls, fid, value);
		}

		jdoubleArray GetDoubleArrayField(jobject obj, jfieldID fid) {
			return ::GetDoubleArrayField(_env, obj, fid);
		}

		jdoubleArray GetStaticDoubleArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticDoubleArrayField(_env, cls, fid);
		}

		void SetDoubleArrayField(jobject obj, jfieldID fid, jdoubleArray value) {
			::SetDoubleArrayField(_env, obj, fid, value);
		}

		void SetStaticDoubleArrayField(jclass cls, jfieldID fid, jdoubleArray value) {
			::SetStaticDoubleArrayField(_env, cls, fid, value);
		}

		jbooleanArray GetBooleanArrayField(jobject obj, jfieldID fid) {
			return ::GetBooleanArrayField(_env, obj, fid);
		}

		jbooleanArray GetStaticBooleanArrayField(jclass cls, jfieldID fid) {
			return ::GetStaticBooleanArrayField(_env, cls, fid);
		}

		void SetBooleanArrayField(jobject obj, jfieldID fid, jbooleanArray value) {
			::SetBooleanArrayField(_env, obj, fid, value);
		}

		void SetStaticBooleanArrayField(jclass cls, jfieldID fid, jbooleanArray value) {
			::SetStaticBooleanArrayField(_env, cls, fid, value);
		}
	};

	class xxjni_classcache {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_classcache() noexcept = default;
#endif
		explicit constexpr xxjni_classcache(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		jboolean init() {
			return ::xjni_classcache_init();
		}

		jclass get(const char *className) {
			return ::xjni_classcache_get(_env,className);
		}

		jclass insert(const char *className,jclass cls_local) {
			return ::xjni_classcache_insert(_env,className,cls_local);
		}

		void clear() {
			::xjni_classcache_clear(_env);
		}
	};

	class xxjni_hashmap {
	private:
		XJNI_HashMap _map{};
		bool _initialized{false};
		bool _mutex{false};

	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_classcache() noexcept = default;
#endif
		explicit
#if CPLUSPLUS_GE(STDC14PP)
		constexpr
#endif
		xxjni_hashmap(size_t capacity, bool mutex) noexcept : _map{}, _initialized(false), _mutex(mutex) {
			if (_mutex) _initialized = ::XJNI_HashMapInit(&_map, capacity) != 0;
			else _initialized = ::XJNI_HashMapInitNoMutex(&_map, capacity) != 0;
		}

		~xxjni_hashmap() noexcept {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapFree(&_map);
			else ::XJNI_HashMapFreeNoLock(&_map);
			_initialized = false;
		}

		explicit constexpr operator bool() const noexcept {
			return _initialized;
		}

		/* Non-copyable */
		xxjni_hashmap(const xxjni_hashmap&) = delete;
		xxjni_hashmap& operator=(const xxjni_hashmap&) = delete;

		/* Non-movable */
		xxjni_hashmap(xxjni_hashmap&&) = delete;
		xxjni_hashmap& operator=(xxjni_hashmap&&) = delete;

		/* Status */
		bool initialized() const {
			return _initialized;
		}

		bool mutexEnabled() const {
			return _mutex;
		}

		/* Access */
		XJNI_HashMap* get() {
			return &_map;
		}

		const XJNI_HashMap* get() const {
			return &_map;
		}

		XJNI_HashMap& operator*() {
			return _map;
		}

		const XJNI_HashMap& operator*() const {
			return _map;
		}

		XJNI_HashMap* operator->() {
			return &_map;
		}

		const XJNI_HashMap* operator->() const {
			return &_map;
		}

		/* Memory management */
		void free() {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapFree(&_map);
			else ::XJNI_HashMapFreeNoLock(&_map);
			_initialized = false;
		}

		void clear() {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapClear(&_map);
			else ::XJNI_HashMapClearNoLock(&_map);
		}

		/* Insert */
		void insert(const char* key, XJNI_HashValue_t value) {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapInsert(&_map, key, value);
			else ::XJNI_HashMapInsertNoLock(&_map, key, value);
		}

		/* Replace */
		int replace(const char* key,XJNI_HashValue_t value) {
			if (!_initialized) return 0;
			if (_mutex) return ::XJNI_HashMapReplace(&_map, key, value);
			return ::XJNI_HashMapReplaceNoLock(&_map, key, value);
		}

		int replaceGetOld(const char* key,XJNI_HashValue_t newValue,XJNI_HashValue_t* oldValue) {
			if (!_initialized) return 0;
			if (_mutex) return ::XJNI_HashMapReplaceGetOld(&_map, key, newValue, oldValue);
			return ::XJNI_HashMapReplaceGetOldNoLock(&_map, key, newValue, oldValue);
		}

		/* Compare and replace */
		int compareReplace(const char* key,XJNI_HashValue_t expected,XJNI_HashValue_t desired) {
			if (!_initialized) return 0;
			if (_mutex) return ::XJNI_HashMapCompareReplace(&_map, key, expected, desired);
			return ::XJNI_HashMapCompareReplaceNoLock(&_map, key, expected, desired);
		}

		/* Replace or insert */
		int replaceOrInsert(const char* key,XJNI_HashValue_t value) {
			if (!_initialized) return 0;
			if (_mutex) return ::XJNI_HashMapReplaceOrInsert(&_map, key, value);
			return ::XJNI_HashMapReplaceOrInsertNoLock(&_map, key, value);
		}

		/* Add */
		void add(const char* key,XJNI_HashValue_t delta) {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapAdd(&_map, key, delta);
			else ::XJNI_HashMapAddNoLock(&_map, key, delta);
		}

		/* Convenience */
		void append(const char* key) {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapAppend(&_map, key);
			else ::XJNI_HashMapAppendNoLock(&_map, key);
		}

		void increment(const char* key) {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapIncrement(&_map, key);
			else ::XJNI_HashMapIncrementNoLock(&_map, key);
		}

		/* Lookup */
		int tryGet(const char* key,XJNI_HashValue_t* out) {
			if (!_initialized) return 0;
			if (_mutex) return ::XJNI_HashMapTryGet(&_map, key, out);
			return ::XJNI_HashMapTryGetNoLock(&_map, key, out);
		}

		XJNI_HashValue_t search(const char* key) {
			if (!_initialized) return XJNI_HASHMAP_NOT_FOUND;
			if (_mutex) return ::XJNI_HashMapSearch(&_map, key);
			return ::XJNI_HashMapSearchNoLock(&_map, key);
		}

		/* Delete */
		void erase(const char* key) {
			if (!_initialized) return;
			if (_mutex) ::XJNI_HashMapDelete(&_map, key);
			else ::XJNI_HashMapDeleteNoLock(&_map, key);
		}

		/* Validation */
		int checkInvariant() {
			if (!_initialized) return 0;

			if (_mutex) return ::XJNI_HashMapCheckInvariant(&_map);

			return ::XJNI_HashMapCheckInvariantNoLock(&_map);
		}
	};

	class xxjni_pool {
	private:
		xjni_pool* _pool{nullptr};
		bool _initialized{false};
		bool _mutex{false};

	public:
		xxjni_pool(size_t capacity,size_t element_size,bool mutex)
		: _pool(nullptr), _initialized(false), _mutex(mutex) {
			_pool = ::xjni_pool_Create(capacity,element_size,_mutex ? 1 : 0);
			_initialized = (_pool != nullptr);
		}

		~xxjni_pool() {
			if (!_initialized) return;
			::xjni_pool_Free(_pool);
			_pool = nullptr;
			_initialized = false;
		}

		/* Non-copyable */
		xxjni_pool(const xxjni_pool&) = delete;
		xxjni_pool& operator=(const xxjni_pool&) = delete;

		/* Non-movable */
		xxjni_pool(xxjni_pool&&) = delete;
		xxjni_pool& operator=(xxjni_pool&&) = delete;

		/* Status */
		bool initialized() const {
			return _initialized;
		}

		bool mutexEnabled() const {
			return _mutex;
		}

		explicit operator bool() const {
			return _initialized;
		}

		/* Access */
		xjni_pool* get() {
			return _pool;
		}

		const xjni_pool* get() const {
			return _pool;
		}

		xjni_pool& operator*() {
			return *_pool;
		}

		const xjni_pool& operator*() const {
			return *_pool;
		}

		xjni_pool* operator->() {
			return _pool;
		}

		const xjni_pool* operator->() const {
			return _pool;
		}

		/* Memory */
		void* alloc() {
			if (!_initialized) return nullptr;
			return ::xjni_pool_AllocFromPool(_pool);
		}

		void freeElement(void* elem) {
			if (!_initialized) return;
			::xjni_pool_FreeElement(_pool, elem);
		}

		/* Free */

		int free() {
			if (!_initialized) return XJNI_POOL_ERROR_NULL;
			int result = ::xjni_pool_Free(_pool);
			if (result == XJNI_POOL_OK) {_pool = nullptr;_initialized = false;
			}
			return result;
		}

#ifdef DEBUG
		/* Debug */
		size_t usedCount() {
			if (!_initialized) return 0;
			return ::xjni_pool_UsedCount(_pool);
		}
		size_t freeCount() {
			if (!_initialized) return 0;
			return ::xjni_pool_FreeCount(_pool);
		}
		size_t capacity() {
			if (!_initialized) return 0;
			return ::xjni_pool_Capacity(_pool);
		}
#endif
	};

	class xxjni2d {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni2d() noexcept = default;
#endif
		explicit constexpr xxjni2d(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		/* Byte */
		jobjectArray newByteArray(jsize row,jsize col) {
			return ::NewByte2DArray(_env, row, col);
		}
		jbyte** getByteElements(jobjectArray array,jboolean* isCopy) {
			return ::GetByte2DArrayElements(_env, array, isCopy);
		}
		void release(jobjectArray array,jbyte** elements,jint mode) {
			::ReleaseByte2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jbyte** buf) {
			::SetByte2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jbyte** buf) {
			::GetByte2DArrayRegion(_env, array, start, len, buf);
		}

		/* Int */

		jobjectArray newIntArray(jsize row,jsize col) {
			return ::NewInt2DArray(_env, row, col);
		}

		jint** getIntElements(jobjectArray array,jboolean* isCopy) {
			return ::GetInt2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,jint** elements,jint mode) {
			::ReleaseInt2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jint** buf) {
			::SetInt2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jint** buf) {
			::GetInt2DArrayRegion(_env, array, start, len, buf);
		}

		/* Long */
		jobjectArray newLongArray(jsize row,jsize col) {
			return ::NewLong2DArray(_env, row, col);
		}

		jlong** getLongElements(jobjectArray array,jboolean* isCopy) {
			return ::GetLong2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,jlong** elements,jint mode) {
			::ReleaseLong2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jlong** buf) {
			::SetLong2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jlong** buf) {
			::GetLong2DArrayRegion(_env, array, start, len, buf);
		}

		/* Float */
		jobjectArray newFloatArray(jsize row,jsize col) {
			return ::NewFloat2DArray(_env, row, col);
		}

		jfloat** getFloatElements(jobjectArray array,jboolean* isCopy) {
			return ::GetFloat2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,jfloat** elements,jint mode) {
			::ReleaseFloat2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jfloat** buf) {
			::SetFloat2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jfloat** buf) {
			::GetFloat2DArrayRegion(_env, array, start, len, buf);
		}

		/* Double */

		jobjectArray newDoubleArray(jsize row,jsize col) {
			return ::NewDouble2DArray(_env, row, col);
		}

		jdouble** getDoubleElements(jobjectArray array,jboolean* isCopy) {
			return ::GetDouble2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,jdouble** elements,jint mode) {
			::ReleaseDouble2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jdouble** buf) {
			::SetDouble2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jdouble** buf) {
			::GetDouble2DArrayRegion(_env, array, start, len, buf);
		}

		/* Short */

		jobjectArray newShortArray(jsize row,jsize col) {
			return ::NewShort2DArray(_env, row, col);
		}

		jshort** getShortElements(jobjectArray array,jboolean* isCopy) {
			return ::GetShort2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,jshort** elements,jint mode) {
			::ReleaseShort2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jshort** buf) {
			::SetShort2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jshort** buf) {
			::GetShort2DArrayRegion(_env, array, start, len, buf);
		}

		/* Char */

		jobjectArray newCharArray(jsize row,jsize col) {
			return ::NewChar2DArray(_env, row, col);
		}

		jchar** getCharElements(jobjectArray array,jboolean* isCopy) {
			return ::GetChar2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,jchar** elements,jint mode) {
			::ReleaseChar2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jchar** buf) {
			::SetChar2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jchar** buf) {
			::GetChar2DArrayRegion(_env, array, start, len, buf);
		}

		/* Boolean */
		jobjectArray newBooleanArray(jsize row,jsize col) {
			return ::NewBoolean2DArray(_env, row, col);
		}

		jboolean** getBooleanElements(jobjectArray array,jboolean* isCopy) {
			return ::GetBoolean2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,jboolean** elements,jint mode) {
			::ReleaseBoolean2DArrayElements(_env, array, elements, mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jboolean** buf) {
			::SetBoolean2DArrayRegion(_env, array, start, len, buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jboolean** buf) {
			::GetBoolean2DArrayRegion(_env, array, start, len, buf);
		}

		/* String UTF */
		jobjectArray newStringUTFArray(const char*** utf,jsize row,jsize col) {
			return ::NewStringUTF2DArray(_env, utf, row, col);
		}

		const char*** getStringUTFElements(jobjectArray array,jboolean* isCopy) {
			return ::GetStringUTF2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,const char*** elements,jint mode) {
			::ReleaseStringUTF2DArrayChars(_env,array,elements,mode);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const char*** buf) {
			::SetStringUTF2DArrayRegion(_env,array,start,len,buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,char*** buf) {
			::GetStringUTF2DArrayRegion(_env,array,start,len,buf);
		}

		/* String */
		jobjectArray newStringArray(const jchar*** utf,jsize row,jsize col) {
			return ::NewString2DArray(_env, utf, row, col);
		}

		const jchar*** getStringElements(jobjectArray array,jboolean* isCopy) {
			return ::GetString2DArrayElements(_env, array, isCopy);
		}

		void release(jobjectArray array,const jchar*** elements) {
			::ReleaseString2DArrayChars(_env,array,elements);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jchar*** buf) {
			::SetString2DArrayRegion(_env,array,start,len,buf);
		}

		void getRegion(jobjectArray array,jsize start,jsize len,jchar*** buf) {
			::GetString2DArrayRegion(_env,array,start,len,buf);
		}
	};

	class xxjni_va_list {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_va_list() noexcept = default;
#endif
		explicit constexpr xxjni_va_list(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		/* Buffer */
		void snprintf(char* s,size_t maxlen,jstring format,jargs_t args) {
			::JSnPrintf(_env,s,maxlen,format,args);
		}

		void snprintf(char* s,size_t maxlen,const char* format,jargs_t args) {
			::JSnPrintfUTF(_env,s,maxlen,format,args);
		}

		void sprintf(char* s,jstring format,jargs_t args) {
			::JSPrintf(_env,s,format,args);
		}

		void sprintf(char* s,const char* format,jargs_t args) {
			::JSPrintfUTF(_env,s,format,args);
		}

		/* FILE */
		void fprintf(FILE* fp,jstring format,jargs_t args) {
			::JFPrintf(_env,fp,format,args);
		}

		void fprintf(FILE* fp,const char* format,jargs_t args) {
			::JFPrintfUTF(_env,fp,format,args);
		}

		/* File descriptor */
		void dprintf(int fd,jstring format,jargs_t args) {
			::JDPrintf(_env,fd,format,args);
		}

		void dprintf(int fd,const char* format,jargs_t args) {
			::JDPrintfUTF(_env,fd,format,args);
		}

		/* Standard output */
		void printf(jstring format,jargs_t args) {
			::JPrintf(_env,format,args);
		}

		void printf(const char* format,jargs_t args) {
			::JPrintfUTF(_env,format,args);
		}
	};

	class xxjni_printf {
	public:
		/* Buffer */
		int vsprintf(jchar* s,const jchar* format,va_list arg) {
			return ::vsjprintf(s, format, arg);
		}

		int sprintf(jchar* s,const jchar* format,...) {
			va_list arg;
			va_start(arg, format);
			int result = ::vsjprintf(s, format, arg);
			va_end(arg);
			return result;
		}

		int vsnprintf(jchar* s,size_t maxlen,const jchar* format,va_list arg) {
			return ::vsnjprintf(s, maxlen, format, arg);
		}

		int snprintf(jchar* s,size_t maxlen,const jchar* format,...) {
			va_list arg;
			va_start(arg, format);
			int result = ::vsnjprintf(s, maxlen, format, arg);
			va_end(arg);
			return result;
		}

		/* FILE */
		int vfprintf(FILE* stream,const jchar* format,va_list arg) {
			return ::vjfprintf(stream, format, arg);
		}

		int fprintf(FILE* stream,const jchar* format,...) {
			va_list arg;
			va_start(arg, format);
			int result = ::vjfprintf(stream, format, arg);
			va_end(arg);
			return result;
		}

		/* File descriptor */
		int vdprintf(int fd,const jchar* format,va_list arg) {
			return ::vjdprintf(fd, format, arg);
		}

		int dprintf(int fd,const jchar* format,...) {
			va_list arg;
			va_start(arg, format);
			int result = ::vjdprintf(fd, format, arg);
			va_end(arg);
			return result;
		}

		/* Standard output */
		int vprintf(const jchar* format,va_list arg) {
			return ::vjprintf(format, arg);
		}

		int printf(const jchar* format,...) {
			va_list arg;
			va_start(arg, format);
			int result = ::vjprintf(format, arg);
			va_end(arg);
			return result;
		}
	};

	class xxjni_stringarray {
	private:
		JNIEnv *_env = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_stringarray() noexcept = default;
#endif
		explicit constexpr xxjni_stringarray(JNIEnv *env) noexcept : _env(env) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		/* String UTF-8 */
		jobjectArray newStringUTFArray(const char** utf,jsize count) {
			return ::NewStringUTFArray(_env, utf, count);
		}

		const char** getStringUTFArrayChars(jobjectArray array,jboolean* isCopy) {
			return ::GetStringUTFArrayChars(_env, array, isCopy);
		}

		void release(jobjectArray array,const char** elements,jint mode) {
			::ReleaseStringUTFArrayChars(_env,array,elements,mode);
		}

		void getRegion(jobjectArray str,jsize start,jsize len,char** buf) {
			::GetStringUTFArrayRegion(_env,str,start,len,buf);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const char** buf) {
			::SetStringUTFArrayRegion(_env,array,start,len,buf);
		}

		jsize getStringUTFArrayLength(jobjectArray str) {
			return ::GetStringUTFArrayLength(_env, str);
		}

		/* String Unicode */
		jobjectArray newStringArray(const jchar** unicode,jsize len,jsize n) {
			return ::NewStringArray(_env,unicode,len,n);
		}

		jsize getStringArrayLength(jobjectArray str) {
			return ::GetStringArrayLength(_env, str);
		}

		const jchar** getStringArrayChars(jobjectArray str,jboolean* isCopy) {
			return ::GetStringArrayChars(_env,str,isCopy);
		}

		void release(jobjectArray str,const jchar** chars) {
			::ReleaseStringArrayChars(_env,str,chars);
		}

		void getRegion(jobjectArray str,jsize start,jsize len,jchar** buf) {
			::GetStringArrayRegion(_env,str,start,len,buf);
		}

		void setRegion(jobjectArray array,jsize start,jsize len,const jchar** buf) {
			::SetStringArrayRegion(_env,array,start,len,buf);
		}
	};

	class xxjni_struct {
	private:
		JNIEnv* _env;
		xjni_struct_ctx_t* _ctx;

	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_struct() noexcept = default;
#endif
		explicit constexpr xxjni_struct(JNIEnv *env,xjni_struct_ctx_t* ctx) noexcept : _env(env), _ctx(ctx) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr && _ctx != nullptr;
		}

		JNIEnv* get() const noexcept {
			return _env;
		}

		xjni_struct_ctx_t* getContext() const noexcept {
			return _ctx;
		}

		/* Context */
		void ctxInit(xjni_struct_set_t set,
			xjni_struct_get_t get,
			xjni_struct_free_t freef,
			xjni_struct_alloc_t alloc,
			xjni_struct_jalloc_t jalloc,
			uint32_t flags) {
			::xjni_struct_ctx_init(_ctx,set,get,freef,alloc,jalloc,flags);
		}

		/* Struct */
		jobject newStruct(void* data,size_t size) {
			return ::xjni_struct_new(_ctx,_env,data,size);
		}

		void* get(jobject obj) {
			return ::xjni_struct_get(_ctx,_env,obj);
		}

		void release(jobject obj,void* elements,jint mode) {
			::xjni_struct_release(_ctx,_env,obj,elements,mode);
		}

		void getRegion(jobject obj,size_t size,void* buf) {
			::xjni_struct_get_region(_ctx,_env,obj,size,buf);
		}

		void setRegion(jobject obj,void* data,size_t size,const void* buf) {
			::xjni_struct_set_region(_ctx,_env,obj,data,size,buf);
		}
	};

	class xxjni_stringbuffer {
	private:
		JNIEnv *_env = nullptr;
		jstringBuffer _sb = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_stringbuffer() noexcept = default;
#endif
		explicit xxjni_stringbuffer(JNIEnv *env) noexcept : _env(env), _sb(::NewStringBuffer(env)) { }

		/** @brief Construct a wrapper around an existing StringBuffer. */
		explicit xxjni_stringbuffer(JNIEnv *env, jstringBuffer sb) noexcept : _env(env), _sb(sb) { }

		/** @brief Construct with an initial capacity. */
		explicit xxjni_stringbuffer(JNIEnv *env, jint capacity) : _env(env), _sb(::NewStringBufferCapacity(env, capacity)) { }

		/** @brief Construct from a Java String. */
		explicit xxjni_stringbuffer(JNIEnv *env, jstring str) : _env(env), _sb(::NewStringBufferString(env, str)) { }

		/** @brief Construct from a UTF-8 C string. */
		explicit xxjni_stringbuffer(JNIEnv *env, const char *str) : _env(env), _sb(::NewStringBufferStringUTF(env, str)) { }

		/** @brief Construct from a C++ string. */
		explicit xxjni_stringbuffer(JNIEnv *env, const std::string &str) : _env(env), _sb(::NewStringBufferStringUTF(env,str.c_str())) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr && _sb != nullptr;
		}

		operator jstringBuffer() const noexcept {
			return _sb;
		}

		/* Access */
		JNIEnv *get() const {
			return _env;
		}

		jstringBuffer getStringBuffer() const {
			return _sb;
		}

		/* Conversion */
		jstring toString() {
			return ::StringBufferToString(_env, _sb);
		}

		char *toStringUTF() {
			return ::StringBufferToStringUTF(_env, _sb);
		}

		std::string toStringC() {
			char *cs = ::StringBufferToStringUTF(_env, _sb);
			std::string s(cs);
			std::free(cs);
			return s;
		}

		/* Substrings */
		jstring substring(jint start) {
			return ::StringBufferSubString(_env, _sb, start);
		}

		jstring substring(jint start, jint end) {
			return ::StringBufferSubStringIntInt(_env, _sb, start, end);
		}

		jobject subSequence(jint start, jint end) {
			return ::StringBufferSubSequence(_env, _sb, start, end);
		}

		/* Append */
		void appendObject(jobject obj) {
			::StringBufferAppendObject(_env, _sb, obj);
		}

		void append(jstring str) {
			::StringBufferAppendString(_env, _sb, str);
		}

		void append(const char *str) {
			::StringBufferAppendStringUTF(_env, _sb, str);
		}

		void append(jstringBuffer obj) {
			::StringBufferAppendStringBuffer(_env, _sb, obj);
		}

		void append(jboolean value) {
			::StringBufferAppendBoolean(_env, _sb, value);
		}

		void append(jchar value) {
			::StringBufferAppendChar(_env, _sb, value);
		}

		void append(jcharArray value) {
			::StringBufferAppendCharArray(_env, _sb, value);
		}

		void append(jint value) {
			::StringBufferAppendInt(_env, _sb, value);
		}

		void append(jlong value) {
			::StringBufferAppendLong(_env, _sb, value);
		}

		void append(jfloat value) {
			::StringBufferAppendFloat(_env, _sb, value);
		}

		void append(jdouble value) {
			::StringBufferAppendDouble(_env, _sb, value);
		}

		void append(jcharArray value, jint offset, jint len) {
			::StringBufferAppendCharArrayIntInt(_env, _sb, value, offset, len);
		}

		void appendCodePoint(jint codePoint) {
			::StringBufferAppendCodePoint(_env, _sb, codePoint);
		}

		/* Size */
		jint length() {
			return ::StringBufferLength(_env, _sb);
		}

		jint capacity() {
			return ::StringBufferCapacity(_env, _sb);
		}

		void ensureCapacity(jint minimumCapacity) {
			::StringBufferEnsureCapacity(_env, _sb, minimumCapacity);
		}

		void setLength(jint newLength) {
			::StringBufferSetLength(_env, _sb, newLength);
		}

		/* Characters */
		jchar charAt(jint index) const {
			return ::StringBufferCharAt(_env, _sb, index);
		}

		void setCharAt(jint index, jchar ch) {
			::StringBufferSetCharAt(_env, _sb, index, ch);
		}

		/* Unicode code points */
		jint codePointAt(jint index) const {
			return ::StringBufferCodePointAt(_env, _sb, index);
		}

		jint codePointBefore(jint index) const {
			return ::StringBufferCodePointBefore(_env, _sb, index);
		}

		jint codePointCount(jint beginIndex, jint endIndex) const {
			return ::StringBufferCodePointCount(_env, _sb, beginIndex, endIndex);
		}

		jint offsetByCodePoints(jint index,jint codePointOffset) const {
			return ::StringBufferOffsetByCodePoints(_env,_sb,index,codePointOffset);
		}

		/* Modification */
		void trimToSize() {
			::StringBufferTrimToSize(_env, _sb);
		}

		void getChars(jint srcBegin,jint srcEnd,jcharArray dst,jint dstBegin) {
			::StringBufferGetChars(_env,_sb,srcBegin,srcEnd,dst,dstBegin);
		}

		void erase(jint start, jint end) {
			::StringBufferDelete(_env, _sb, start, end);
		}

		void deleteCharAt(jint index) {
			::StringBufferDeleteCharAt(_env, _sb, index);
		}

		void replace(jint start,jint end,jstring str) {
			::StringBufferReplace(_env, _sb, start, end, str);
		}

		void replace(jint start,jint end,const char *str) {
			::StringBufferReplaceUTF(_env, _sb, start, end, str);
		}

		/* Insert */
		void insert(jint index,jstring str) {
			::StringBufferInsertString(_env, _sb, index, str);
		}

		void insert(jint index,const char *str) {
			::StringBufferInsertStringUTF(_env, _sb, index, str);
		}

		void insert(jint index,jobject obj) {
			::StringBufferInsertObject(_env, _sb, index, obj);
		}

		void insert(jint index,jcharArray obj) {
			::StringBufferInsertCharArray(_env, _sb, index, obj);
		}

		void insert(jint index,jboolean value) {
			::StringBufferInsertBoolean(_env, _sb, index, value);
		}

		void insert(jint index,jchar value) {
			::StringBufferInsertChar(_env, _sb, index, value);
		}

		void insert(jint index,jint value) {
			::StringBufferInsertInt(_env, _sb, index, value);
		}

		void insert(jint index,jlong value) {
			::StringBufferInsertLong(_env, _sb, index, value);
		}

		void insert(jint index,jfloat value) {
			::StringBufferInsertFloat(_env, _sb, index, value);
		}

		void insert(jint index,jdouble value) {
			::StringBufferInsertDouble(_env, _sb, index, value);
		}

		void insert(jint index,jcharArray str,jint offset,jint len) {
			::StringBufferInsertCharArrayIntInt(_env,_sb,index,str,offset,len);
		}

		/* Search	 */
		jint indexOf(jstring str) const {
			return ::StringBufferIndexOf(_env, _sb, str);
		}

		jint indexOf(const char *str) const {
			return ::StringBufferIndexOfUTF(_env, _sb, str);
		}

		jint indexOf(jstring str, jint fromIndex) const {
			return ::StringBufferIndexOfI(_env, _sb, str, fromIndex);
		}

		jint indexOf(const char *str,jint fromIndex) const {
			return ::StringBufferIndexOfIUTF(_env, _sb, str, fromIndex);
		}

		jint lastIndexOf(jstring str) const {
			return ::StringBufferLastIndexOf(_env, _sb, str);
		}

		jint lastIndexOf(const char *str) const {
			return ::StringBufferLastIndexOfUTF(_env, _sb, str);
		}

		jint lastIndexOf(jstring str,jint fromIndex) const {
			return ::StringBufferLastIndexOfI(_env, _sb, str, fromIndex);
		}

		jint lastIndexOf(const char *str,jint fromIndex) const {
			return ::StringBufferLastIndexOfIUTF(_env, _sb, str, fromIndex);
		}

		/* Reverse */
		void reverse() {
			::StringBufferReverse(_env, _sb);
		}
	};

	class xxjni_stringbuilder {
	private:
		JNIEnv *_env = nullptr;
		jstringBuilder _sb = nullptr;

	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_stringbuilder() noexcept = default;
#endif
		explicit xxjni_stringbuilder(JNIEnv *env) noexcept : _env(env), _sb(::NewStringBuilder(env)) { }

		/** @brief Construct a wrapper around an existing StringBuilder. */
		explicit xxjni_stringbuilder(JNIEnv *env, jstringBuilder sb) noexcept : _env(env), _sb(sb) { }

		/** @brief Construct with an initial capacity. */
		explicit xxjni_stringbuilder(JNIEnv *env, jint capacity) : _env(env), _sb(::NewStringBuilderCapacity(env, capacity)) { }

		/** @brief Construct from a Java String. */
		explicit xxjni_stringbuilder(JNIEnv *env, jstring str) : _env(env), _sb(::NewStringBuilderString(env, str)) { }

		/** @brief Construct from a UTF-8 C string. */
		explicit xxjni_stringbuilder(JNIEnv *env, const char *str) : _env(env), _sb(::NewStringBuilderStringUTF(env, str)) { }

		/** @brief Construct from a C++ string. */
		explicit xxjni_stringbuilder(JNIEnv *env, const std::string &str) : _env(env), _sb(::NewStringBuilderStringUTF(env,str.c_str())) { }

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr && _sb != nullptr;
		}

		operator jstringBuffer() const noexcept {
			return _sb;
		}

		/* Access */
		JNIEnv *get() const noexcept {
			return _env;
		}

		jstringBuffer getStringBuilder() const noexcept {
			return _sb;
		}

		/* Conversion */
		jstring toString() {
			return ::StringBuilderToString(_env,_sb);
		}

		char *toStringUTF() {
			return ::StringBuilderToStringUTF(_env,_sb);
		}

		std::string toStringC() {
			char *cs = ::StringBuilderToStringUTF(_env, _sb);
			std::string s(cs);
			std::free(cs);
			return s;
		}

		/* Append */
		void append(jstring obj) {
			::StringBuilderAppendString(_env,_sb,obj);
		}

		void append(const char *str) {
			::StringBuilderAppendStringUTF(_env,_sb,str);
		}

		void appendObject(jobject obj) {
			::StringBuilderAppendObject(_env,_sb,obj);
		}

		void append(xxjni::xxjni_stringbuffer obj) {
			::StringBuilderAppendStringBuffer(_env,_sb,obj.getStringBuffer());
		}

		void append(jboolean value) {
			::StringBuilderAppendBoolean(_env,_sb,value);
		}

		void append(jchar value) {
			::StringBuilderAppendChar(_env,_sb,value);
		}

		void append(jint value) {
			::StringBuilderAppendInt(_env,_sb,value);
		}

		void append(jlong value) {
			::StringBuilderAppendLong(_env,_sb,value);
		}

		void append(jfloat value) {
			::StringBuilderAppendFloat(_env,_sb,value);
		}

		void append(jdouble value) {
			::StringBuilderAppendDouble(_env,_sb,value);
		}

		void append(jcharArray obj,jint offset,jint len) {
			::StringBuilderAppendCharArrayIntInt(_env,_sb,obj,offset,len);
		}

		void appendCodePoint(jint codePoint) {
			::StringBuilderAppendCodePoint(_env,_sb,codePoint);
		}

		/* Delete */
		void erase(jint start,jint end) {
			::StringBuilderDelete(_env,_sb,start,end);
		}

		void deleteCharAt(jint index) {
			::StringBuilderDeleteCharAt(_env,_sb,index);
		}

		/* Replace */
		void replace(jint start,jint end,jstring str) {
			::StringBuilderReplace(_env,_sb,start,end,str);
		}
		void replace(jint start,jint end,const char *str) {
			::StringBuilderReplaceUTF(_env,_sb,start,end,str);
		}

		/* Insert */
		void insert(jint index,jstring obj) {
			::StringBuilderInsertString(_env,_sb,index,obj);
		}

		void insert(jint index,const char *str) {
			::StringBuilderInsertStringUTF(_env,_sb,index,str);
		}

		void insert(jint index,jobject obj) {
			::StringBuilderInsertObject(_env,_sb,index,obj);
		}

		void insert(jint index,jcharArray obj) {
			::StringBuilderInsertCharArray(_env,_sb,index,obj);
		}

		void insert(jint index,jboolean value) {
			::StringBuilderInsertBoolean(_env,_sb,index,value);
		}

		void insert(jint index,jchar value) {
			::StringBuilderInsertChar(_env,_sb,index,value);
		}

		void insert(jint index,jint value) {
			::StringBuilderInsertInt(_env,_sb,index,value);
		}

		void insert(jint index,jlong value) {
			::StringBuilderInsertLong(_env,_sb,index,value);
		}

		void insert(jint index,jfloat value) {
			::StringBuilderInsertFloat(_env,_sb,index,value);
		}

		void insert(jint index,jdouble value) {
			::StringBuilderInsertDouble(_env,_sb,index,value);
		}

		void insert(jint index,jcharArray obj,jint offset,jint len) {
			::StringBuilderInsertCharArrayIntInt(_env,_sb,index,obj,offset,len);
		}

		/* Search */
		jint indexOf(jstring str) const {
			return ::StringBuilderIndexOf(_env,_sb,str);
		}

		jint indexOf(const char *str) const {
			return ::StringBuilderIndexOfUTF(_env,_sb,str);
		}

		jint indexOf(jstring str,jint fromIndex) const {
			return ::StringBuilderIndexOfI(_env,_sb,str,fromIndex);
		}

		jint indexOf(const char *str,jint fromIndex) const {
			return ::StringBuilderIndexOfIUTF(_env,_sb,str,fromIndex);
		}

		jint lastIndexOf(jstring str) const {
			return ::StringBuilderLastIndexOf(_env,_sb,str);
		}

		jint lastIndexOf(const char *str) const {
			return ::StringBuilderLastIndexOfUTF(_env,_sb,str);
		}

		jint lastIndexOf(jstring str,jint fromIndex) const {
			return ::StringBuilderLastIndexOfI(_env,_sb,str,fromIndex);
		}

		jint lastIndexOf(const char *str,jint fromIndex) const {
			return ::StringBuilderLastIndexOfIUTF(_env,_sb,str,fromIndex);
		}

		/* Reverse  */
		void reverse() {
			::StringBuilderReverse(_env,_sb);
		}
	};

	class xxjni_stringwriter {
	private:
		JNIEnv* _env = nullptr;
		jstringWriter _sw = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_stringwriter() noexcept = default;
#endif
		/* Constructors */
		explicit xxjni_stringwriter(JNIEnv *env) noexcept : _env(env), _sw(::NewStringWriter(env)) { }
		explicit xxjni_stringwriter(JNIEnv* env, jint initialSize) noexcept : _env(env), _sw(NewStringWriterInitialSize(env, initialSize)) {}

		/* Existing Java StringWriter */
		explicit xxjni_stringwriter(JNIEnv* env, jstringWriter writer) noexcept : _env(env), _sw(writer) {}

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr && _sw != nullptr;
		}

		/* Access */
		JNIEnv* get() const noexcept {
			return _env;
		}

		jstringWriter getStringWriter() const noexcept {
			return _sw;
		}

		operator jstringWriter() const noexcept {
			return _sw;
		}

		/* Conversion */
		jstring toString() {
			return StringWriterToString(_env, _sw);
		}

		char* toStringUTF() {
			return StringWriterToStringUTF(_env, _sw);
		}

		std::string toStringC() {
			char *cs = ::StringWriterToStringUTF(_env, _sw);
			std::string s(cs);
			std::free(cs);
			return s;
		}

		/* Writing */
		void write(jcharArray obj, jint offset, jint len) {
			StringWriterWriteCharArrayIntInt(_env, _sw, obj, offset, len);
		}

		void write(jint value) {
			StringWriterWriteInt(_env, _sw, value);
		}

		void write(jstring str) {
			StringWriterWriteString(_env, _sw, str);
		}

		void write(jstring str, jint offset, jint len) {
			StringWriterWriteStringIntInt(_env, _sw, str, offset, len);
		}

		/* Flush */
		void flush() {
			StringWriterFlush(_env, _sw);
		}

		/* Append */
		void append(jchar ch) {
			StringWriterAppendChar(_env, _sw, ch);
		}
	};

	class xxjni_stringreader {
	private:
		JNIEnv* _env = nullptr;
		jstringReader _sr = nullptr;

	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr xxjni_stringwriter() noexcept = default;
#endif
		/* Constructors */
		explicit xxjni_stringreader(JNIEnv* env, jstring str) : _env(env), _sr(NewStringReader(env, str)) {}
		explicit xxjni_stringreader(JNIEnv* env, const char* str) : _env(env), _sr(NewStringReaderUTF(env, str)) {}
		explicit xxjni_stringreader(JNIEnv* env, const std::string &str) : _env(env), _sr(NewStringReaderUTF(env, str.c_str())) {}
		explicit xxjni_stringreader(JNIEnv* env, jstringReader reader) : _env(env),_sr(reader) {}

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr && _sr != nullptr;
		}

		/* Accessors */
		JNIEnv* get() const noexcept {
			return _env;
		}

		jstringReader getStringReader() const noexcept {
			return _sr;
		}

		operator jstringReader() const noexcept{
			return _sr;
		}

		/* Conversion */
		jstring toString() {
			return StringReaderToString(_env, _sr);
		}

		char* toStringUTF() {
			return StringReaderToStringUTF(_env, _sr);
		}

		std::string toStringC() {
			char *cs = StringReaderToStringUTF(_env, _sr);
			std::string s(cs);
			std::free(cs);
			return s;
		}

		/* Stream Operations */
		void ensureOpen() {
			StringReaderEnsureOpen(_env, _sr);
		}

		jint read() {
			return StringReaderRead(_env, _sr);
		}

		jint read(jcharArray obj, jint offset, jint len) {
			return StringReaderReadCharArrayIntInt(_env,_sr,obj,offset,len);
		}

		jlong skip(jlong ns) {
			return StringReaderReadSkip(_env, _sr, ns);
		}

		jboolean ready() {
			return StringReaderReadReady(_env, _sr);
		}

		jboolean markSupported() {
			return StringReaderReadMarkSupported(_env, _sr);
		}

		void mark(jint readAheadLimit) {
			StringReaderMark(_env,_sr,readAheadLimit);
		}

		void reset() {
			StringReaderReset(_env, _sr);
		}

		void close() {
			StringReaderClose(_env, _sr);
		}
	};

	class xxjni_thread {
	public:
		using thread_t  = xjni_thread_t;
		using result_t  = xjni_thread_result_t;
		using start_t   = void* (*)(void*);

		/**
		 * @brief Construct an empty thread object.
		 *
		 * No thread is created until start() is called.
		 */
		explicit constexpr xxjni_thread() noexcept : _thread{}, _running{false}, _detached{false} { }

		/**
		 * @brief Destructor.
		 *
		 * If the thread is still joinable, it is joined automatically.
		 */
		~xxjni_thread() noexcept{
			join();
		}

		xxjni_thread(const xxjni_thread&) = delete;
		xxjni_thread& operator=(const xxjni_thread&) = delete;

		/**
		 * @brief Move constructor.
		 */
		xxjni_thread(xxjni_thread&& other) noexcept : _thread{other._thread}, _running{other._running}, _detached{other._detached} {
			other._running = false;
			other._detached = false;
		}

		/** @brief Move assignment. */
		xxjni_thread& operator=(xxjni_thread&& other) noexcept {
			if (this != &other) {
				join();
				_thread   = other._thread;
				_running  = other._running;
				_detached = other._detached;
				other._running = false;
				other._detached = false;
			}
			return *this;
		}

		/**
		 * @brief Start a new thread.
		 * @param routine Thread entry function.
		 * @param arg User supplied argument.
		 * @return Platform-specific result code.
		 */
		result_t start(start_t routine, void* arg = nullptr) noexcept {
			if (_running || _detached || routine == nullptr) return static_cast<result_t>(-1);
			result_t result = xjni_create(&_thread, routine, arg);
			if (result == 0) _running = true;
			return result;
		}

		/**
		 * @brief Join the thread.
		 * @param retval Optional pointer receiving the thread return value.
		 * @return Platform-specific result code.
		 */
		result_t join(void** retval = nullptr) noexcept {
			if (!_running) return 0;
			result_t result = xjni_join(_thread, retval);
			if (result == 0) _running = false;
			return result;
		}

		/**
		 * @brief Detach the thread.
		 * After detaching, the thread manages its own resources.
		 * @return Platform-specific result code.
		 */
		result_t detach() noexcept {
			if (!_running) return 0;
			result_t result = xjni_detach(_thread);
			if (result == 0) {
				_running = false;
				_detached = true;
			}
			return result;
		}

		/** @brief Check whether the thread is currently joinable. */
		bool joinable() const noexcept {
			return _running && !_detached;
		}

		/** @brief Check whether the thread has been started. */
		bool running() const noexcept {
			return _running;
		}

		/** @brief Get the native thread handle. */
		thread_t native_handle() const noexcept {
			return _thread;
		}

		/** @brief Get the current thread handle. */
		static thread_t self() noexcept {
			return xjni_self();
		}

		/** @brief Compare two thread handles. */
		static bool equal(thread_t t1, thread_t t2) noexcept {
			return xjni_equal(t1, t2) != 0;
		}

		/** @brief Check whether this object represents the current thread. */
		bool is_current() const noexcept {
			return equal(_thread, self());
		}

		/**
		 * @brief Request cancellation of this thread.
		 *
		 * @return Platform-specific result code.
		 */
		result_t cancel() noexcept {
			if (!_running) return static_cast<result_t>(-1);
			return xjni_cancel(_thread);
		}

		/**
		 * @brief Set cancellation type for the current thread.
		 *
		 * @param type XJNI_CANCEL_ENABLE or XJNI_CANCEL_DISABLE.
		 * @param oldtype Optional previous cancellation type.
		 */
		static int set_cancel_type(int type, int* oldtype = nullptr) noexcept {
			return xjni_setcanceltype(type, oldtype);
		}

		/**
		 * @brief Set cancellation state for the current thread.
		 *
		 * @param state XJNI_CANCEL_ENABLE or XJNI_CANCEL_DISABLE.
		 * @param oldstate Optional previous cancellation state.
		 */
		static int set_cancel_state(int state, int* oldstate = nullptr) noexcept {
			return xjni_setcancelstate(state, oldstate);
		}

	private:
		thread_t _thread;
		bool _running;
		bool _detached;
	};


	/**
	 * @brief RAII wrapper around xjni_mutex_t.
	 */
	class xxjni_mutex {
	public:
		explicit xxjni_mutex() noexcept : _initialized{false} {
			if (xjni_mutex_init(&_mutex) == 0) _initialized = true;
		}

		explicit xxjni_mutex(bool initialize) noexcept : _initialized{false} {
			if (initialize && xjni_mutex_init(&_mutex) == 0) _initialized = true;
		}

		~xxjni_mutex() noexcept {
			destroy();
		}

		xxjni_mutex(const xxjni_mutex&) = delete;
		xxjni_mutex& operator=(const xxjni_mutex&) = delete;

		xxjni_mutex(xxjni_mutex&& other) noexcept : _mutex{other._mutex}, _initialized{other._initialized} {
			other._initialized = false;
		}

		xxjni_mutex& operator=(xxjni_mutex&& other) noexcept {
			if (this != &other) {
				destroy();
				_mutex = other._mutex;
				_initialized = other._initialized;
				other._initialized = false;
			}
			return *this;
		}

		/** @brief Initialize the mutex. */
		int init() noexcept {
			if (_initialized) return 0;
			int result = xjni_mutex_init(&_mutex);
			if (result == 0) _initialized = true;
			return result;
		}

		/** @brief Destroy the mutex. */
		int destroy() noexcept {
			if (!_initialized) return 0;
			int result = xjni_mutex_destroy(&_mutex);
			if (result == 0) _initialized = false;
			return result;
		}

		/** @brief Lock the mutex. */
		int lock() noexcept {
			return xjni_mutex_lock(&_mutex);
		}

		/** @brief Unlock the mutex. */
		int unlock() noexcept {
			return xjni_mutex_unlock(&_mutex);
		}

		/** @brief Try to lock the mutex. */
		int try_lock() noexcept {
			return xjni_mutex_trylock(&_mutex);
		}

		/** @brief Lock the mutex with an absolute timeout. */
		int timed_lock(const struct timespec* abstime) noexcept {
			return xjni_mutex_timedlock(&_mutex, abstime);
		}

		/** @brief Get the native mutex. */
		xjni_mutex_t* native_handle() noexcept {
			return &_mutex;
		}

		const xjni_mutex_t* native_handle() const noexcept {
			return &_mutex;
		}

		/** @brief Check whether the mutex has been initialized. */
		bool initialized() const noexcept {
			return _initialized;
		}
	private:
		xjni_mutex_t _mutex;
		bool _initialized;
	};


	/** @brief RAII lock guard for xxjni_mutex. */
	class xxjni_lock_guard {
	public:
		explicit xxjni_lock_guard(xxjni_mutex& mutex) noexcept : mutex_{mutex}, _locked{false} {
			if (mutex_.lock() == 0) _locked = true;
		}

		~xxjni_lock_guard() noexcept {
			if (_locked) mutex_.unlock();
		}

		xxjni_lock_guard(const xxjni_lock_guard&) = delete;
		xxjni_lock_guard& operator=(const xxjni_lock_guard&) = delete;

		bool locked() const noexcept {
			return _locked;
		}
	private:
		xxjni_mutex& mutex_;
		bool _locked;
	};


	/** @brief One-time initialization wrapper. */
	class xxjni_once {
	public:
		/** @brief Construct a one-time initialization object. */
		xxjni_once() noexcept : _once{XJNI_ONCE_INIT} { }
		xxjni_once(const xxjni_once&) = delete;
		xxjni_once& operator=(const xxjni_once&) = delete;

		/**
		 * @brief Execute a function exactly once.
		 * @param routine Initialization function.
		 * @return Platform-specific result code.
		 */
		int call(void (*routine)(void)) noexcept {
			if (routine == nullptr) return -1;
			return xjni_once(&_once, routine);
		}

		/** @brief Get the underlying once control object. */
		xjni_once_t* native_handle() noexcept {
			return &_once;
		}
	private:
		xjni_once_t _once;
	};


	/**
	 * @brief Get the current realtime timestamp.
	 */
	class xxjni_clock {
	public:
		/** @brief Get current realtime. */
		static struct timespec realtime() noexcept {
			struct timespec ts{};
			xjni_get_realtime(&ts);
			return ts;
		}

		/** @brief Store current realtime in a caller supplied timespec. */
		static void realtime(struct timespec* ts) noexcept {
			if (ts != nullptr) xjni_get_realtime(ts);
		}
	};

	class xxjni_class {
	public:
		using set_type	= xjni_class_set_t;
		using get_type	= xjni_class_get_t;
		using free_type   = xjni_class_free_t;
		using alloc_type  = xjni_class_alloc_t;
		using jalloc_type = xjni_class_jalloc_t;

		xxjni_class() noexcept : m_ctx{} { }

		xxjni_class(set_type set,get_type get,free_type freef,
			alloc_type alloc,jalloc_type jalloc,uint32_t flags = 0) noexcept : m_ctx{} {
			init(set,get,freef,alloc,jalloc,flags);
		}

		xxjni_class(const xxjni_class&) = delete;
		xxjni_class& operator=(const xxjni_class&) = delete;

		xxjni_class(xxjni_class&& other) noexcept : m_ctx(other.m_ctx) { other.m_ctx = {}; }

		xxjni_class& operator=(xxjni_class&& other) noexcept {
			if (this != &other) {
				m_ctx = other.m_ctx;
				other.m_ctx = {};
			}
			return *this;
		}

		~xxjni_class() = default;

		void init(set_type set,get_type get,free_type freef,alloc_type alloc,jalloc_type jalloc,uint32_t flags = 0) noexcept {
			xjni_class_ctx_init(&m_ctx,set,get,freef,alloc,jalloc,flags);
		}

		xjni_class_ctx_t* context() noexcept {
			return &m_ctx;
		}

		const xjni_class_ctx_t* context() const noexcept {
			return &m_ctx;
		}

		/**
		 * @brief Check whether the context has been initialized.
		 */
		bool valid() const noexcept {
			return
				m_ctx.set != nullptr ||
				m_ctx.get != nullptr ||
				m_ctx.freef != nullptr ||
				m_ctx.alloc != nullptr ||
				m_ctx.jalloc != nullptr;
		}

		void set_flags(uint32_t flags) noexcept {
			m_ctx.flags = flags;
		}

		uint32_t flags() const noexcept {
			return m_ctx.flags;
		}

		bool should_free() const noexcept {
			return XJNI_CLASS_SHOULD_FREE(&m_ctx) != 0;
		}

		bool should_thread() const noexcept {
			return XJNI_CLASS_SHOULD_THREAD(&m_ctx) != 0;
		}

		jobject create(JNIEnv* env,const void* data = nullptr,size_t size = 0) const noexcept{
			return xjni_class_new(const_cast<xjni_class_ctx_t*>(&m_ctx),env,data,size);
		}

		template<class T>
		jobject create(JNIEnv* env,const T& object) const noexcept {
			return create(env,&object,sizeof(T));
		}

		void* get(JNIEnv* env,jobject obj) const noexcept {
			return xjni_class_get(const_cast<xjni_class_ctx_t*>(&m_ctx),env,obj);
		}

		template<class T>
		T* get(JNIEnv* env,jobject obj) const noexcept {
			return static_cast<T*>(get(env, obj));
		}

		void release(JNIEnv* env,jobject obj,void* object,jint mode = 0) const noexcept {
			xjni_class_release(
				const_cast<xjni_class_ctx_t*>(&m_ctx),env,obj,object,mode);
		}

		template<class T>
		void release(JNIEnv* env,jobject obj,T* object,jint mode = 0) const noexcept {
			release(env,obj,static_cast<void*>(object),mode);
		}

		void get_region(JNIEnv* env,jobject obj,size_t size,void* buf) const noexcept {
			xjni_class_get_region(const_cast<xjni_class_ctx_t*>(&m_ctx),env,obj,size,buf);
		}

		template<class T>
		void get_region(JNIEnv* env,jobject obj,T& value) const noexcept{
			get_region(env,obj,sizeof(T),&value);
		}

		void set_region(JNIEnv* env,jobject obj,const void* data,size_t size,const void* buf) const noexcept {
			xjni_class_set_region(const_cast<xjni_class_ctx_t*>(&m_ctx),env,obj,data,size,buf);
		}

		template<class T>
		void set_region(JNIEnv* env,jobject obj,const T& value) const noexcept {
			set_region(env,obj,nullptr,sizeof(T),&value);
		}

	private:
		xjni_class_ctx_t m_ctx;
	};

	template<class T>
	class xxjni_class_object {
	public:
		xxjni_class_object() noexcept
		: m_owner(nullptr),m_object(nullptr),m_env(nullptr),m_java_object(nullptr),m_mode(JNI_ABORT) { }

		xxjni_class_object(const xxjni_class& owner,JNIEnv* env,jobject java_object,T* object) noexcept
		: m_owner(&owner),m_object(object),m_env(env),m_java_object(java_object),m_mode(JNI_ABORT) { }
		xxjni_class_object(const xxjni_class_object&) = delete;
		xxjni_class_object& operator=(const xxjni_class_object&) = delete;
		xxjni_class_object(xxjni_class_object&& other) noexcept
		: m_owner(other.m_owner),m_object(other.m_object),m_env(other.m_env),
		m_java_object(other.m_java_object),m_mode(other.m_mode) {
			other.m_owner = nullptr;
			other.m_object = nullptr;
			other.m_env = nullptr;
			other.m_java_object = nullptr;
		}

		xxjni_class_object& operator=(xxjni_class_object&& other) noexcept {
			if (this != &other) {
				reset();
				m_owner = other.m_owner;
				m_object = other.m_object;
				m_env = other.m_env;
				m_java_object = other.m_java_object;
				m_mode = other.m_mode;
				other.m_owner = nullptr;
				other.m_object = nullptr;
				other.m_env = nullptr;
				other.m_java_object = nullptr;
			}
			return *this;
		}

		~xxjni_class_object() {
			reset();
		}

		T* get() noexcept {
			return m_object;
		}

		const T* get() const noexcept {
			return m_object;
		}

		T* operator->() noexcept {
			return m_object;
		}

		const T* operator->() const noexcept {
			return m_object;
		}

		T& operator*() noexcept {
			return *m_object;
		}

		const T& operator*() const noexcept {
			return *m_object;
		}

		explicit operator bool() const noexcept {
			return m_object != nullptr;
		}

		/**
		 * @brief Commit changes to Java and destroy native object.
		 */
		void commit() noexcept {
			if (m_owner == nullptr ||
				m_object == nullptr) {
				return;
				}
				m_owner->release(m_env,m_java_object,m_object,0);
			m_object = nullptr;
			m_owner = nullptr;
		}

		/**
		 * @brief Commit changes but keep native object.
		 */
		void commit_keep() noexcept {
			if (m_owner == nullptr ||
				m_object == nullptr) {
				return;
				}
				m_owner->release(m_env,m_java_object,m_object,JNI_COMMIT);
		}

		/**
		 * @brief Discard changes and destroy native object.
		 */
		void abort() noexcept {
			if (m_owner == nullptr ||
				m_object == nullptr) {
				return;
				}
				m_owner->release(m_env,m_java_object,m_object,JNI_ABORT);
			m_object = nullptr;
			m_owner = nullptr;
		}

		/**
		 * @brief Release the native object.
		 */
		void reset() noexcept {
			if (m_owner == nullptr ||
				m_object == nullptr) {
				return;
				}
				m_owner->release(m_env,m_java_object,m_object,m_mode);
			m_object = nullptr;
			m_owner = nullptr;
		}

		/**
		 * @brief Prevent automatic release.
		 *
		 * Ownership is returned to the caller.
		 */
		T* detach() noexcept {
			T* result = m_object;
			m_object = nullptr;
			m_owner = nullptr;
			return result;
		}
	private:
		const xxjni_class* m_owner;
		T* m_object;
		JNIEnv* m_env;
		jobject m_java_object;
		jint m_mode;
	};

	template<class T>
	xxjni_class_object<T>
	xxjni_class_get_object(const xxjni_class& cls,JNIEnv* env,jobject obj) {
		T* object = cls.get<T>(env,obj);
		return xxjni_class_object<T>(cls,env,obj,object);
	}

	class xxjni : public xxjni_new, public xxjni_utils,
				  public xxjni_args, public xxjni_arrayfield,
				  public xxjni_classcache, public xxjni2d,
				  public xxjni_va_list, public xxjni_stringarray,
				  public jni::XJNIEnv {
	private:
		JNIEnv *_env;
	public:
		explicit constexpr xxjni(JNIEnv *env) noexcept
			: xxjni_new(env), xxjni_utils(env),
			  xxjni_args(env), xxjni_arrayfield(env),
			  xxjni_classcache(env), xxjni2d(env),
			  xxjni_va_list(env), xxjni_stringarray(env),
			  jni::XJNIEnv(env) , _env(env) { }

		JNIEnv* get() const noexcept {
			return _env;
		}

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		void FatalErrorV(const char *msg, va_list __arg) {
			::FatalErrorV(_env, msg, __arg);
		}

		void FatalErrorF(const char *msg, ...) {
			va_list args;
			va_start(args, msg);
			::FatalErrorV(_env, msg, args);
			va_end(args);
		}

		jint ThrowNewV(jclass clazz, const char *msg, va_list __arg) {
			return ::ThrowNewV(_env, clazz,msg,__arg);
		}

		jint ThrowNewF(jclass clazz, const char *msg, ...) {
			va_list args;
			va_start(args, msg);
			jint ret = ::ThrowNewV(_env, clazz,msg,args);
			va_end(args);
			return ret;
		}

		void throwJava(const char* tag,const char* msg, const char* cls_name, jclass* cache) {
			::throwJava(_env, tag, msg,cls_name,cache);
		}

		void throwJavaV(const char* tag,const char* clsName,jclass* cache,const char* msg,va_list ap) {
			::throwJavaV(_env, tag,clsName,cache, msg, ap);
		}

		void throwJavaF(const char* tag,const char* cls_name,jclass* cache,const char* msg,...) {
			va_list args;
			va_start(args, msg);
			::throwJavaV(_env, tag,cls_name,cache, msg, args);
			va_end(args);
		}

		__MAKE_XXJNI_CLASS_THROW_FUN(throwIOException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwCharConversionException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwEOFException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwFileNotFoundException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwInterruptedIOException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwInvalidClassException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwInvalidObjectException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwNotActiveException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwNotSerializableException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwOptionalDataException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwStreamCorruptedException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwSyncFailedException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwUTFDataFormatException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwUnsupportedEncodingException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwWriteAbortedException)
		__MAKE_XXJNI_CLASS_THROW_FUN(throwOutOfMemoryError)
	};

	static const std::string xxjni_version(void) {
		return std::to_string(_XXJNI_VERSION_MAJOR) + "." +
		std::to_string(_XXJNI_VERSION_MINOR) + "." +
		std::to_string(_XXJNI_VERSION_PATCH);
	}

//  xjni.h
	using ::xjni_version;

	using ::XJNI_OnLoad;
	using ::XJNI_OnUnload;


	using ::FatalErrorV;
	using ::FatalErrorF;

	using ::ThrowNewV;
	using ::ThrowNewF;

	using ::throwJava;
	using ::throwJavaV;
	using ::throwJavaF;

	__MAKE_XXJNI_THROW_FUN(throwIOException)
	__MAKE_XXJNI_THROW_FUN(throwCharConversionException)
	__MAKE_XXJNI_THROW_FUN(throwEOFException)
	__MAKE_XXJNI_THROW_FUN(throwFileNotFoundException)
	__MAKE_XXJNI_THROW_FUN(throwInterruptedIOException)
	__MAKE_XXJNI_THROW_FUN(throwInvalidClassException)
	__MAKE_XXJNI_THROW_FUN(throwInvalidObjectException)
	__MAKE_XXJNI_THROW_FUN(throwNotActiveException)
	__MAKE_XXJNI_THROW_FUN(throwNotSerializableException)
	__MAKE_XXJNI_THROW_FUN(throwOptionalDataException)
	__MAKE_XXJNI_THROW_FUN(throwStreamCorruptedException)
	__MAKE_XXJNI_THROW_FUN(throwSyncFailedException)
	__MAKE_XXJNI_THROW_FUN(throwUTFDataFormatException)
	__MAKE_XXJNI_THROW_FUN(throwUnsupportedEncodingException)
	__MAKE_XXJNI_THROW_FUN(throwWriteAbortedException)
	__MAKE_XXJNI_THROW_FUN(throwOutOfMemoryError)

//  xjni_new.h
	using ::XJNI_New_OnLoad;
	using ::XJNI_New_OnUnload;

	using ::xjni_NewObjectArray;
	using ::xjni_NewByteArray;
	using ::xjni_NewIntArray;
	using ::xjni_NewLongArray;
	using ::xjni_NewFloatArray;
	using ::xjni_NewDoubleArray;
	using ::NewObjectBuilderV;
	using ::NewObjectBuilder;

//  xjni_utils.h
	using ::XJNI_Utily_OnLoad;
	using ::XJNI_Utily_OnUnload;

	using ::IsSameObjectChars;
	using ::GetFieldIDByName;
	using ::GetFieldIDByObject;
	using ::GetObjectName;
	using ::GetObjectNameChars;

//  xjni_args.h

	using ::NewJArgs;

	using ::JArgsAppendObject;
	using ::JArgsAppendString;
	using ::JArgsAppendStringUTF;
	using ::JArgsAppendChar;
	using ::JArgsAppendBoolean;
	using ::JArgsAppendInt;
	using ::JArgsAppendLong;
	using ::JArgsAppendFloat;
	using ::JArgsAppendDouble;
	using ::JArgsAppendByte;
	using ::JArgsAppendShort;

	using ::JArgsInsertObject;
	using ::JArgsInsertString;
	using ::JArgsInsertStringUTF;
	using ::JArgsInsertChar;
	using ::JArgsInsertBoolean;
	using ::JArgsInsertInt;
	using ::JArgsInsertLong;
	using ::JArgsInsertFloat;
	using ::JArgsInsertDouble;
	using ::JArgsInsertByte;
	using ::JArgsInsertShort;

	using ::JArgsReplaceObject;
	using ::JArgsReplaceString;
	using ::JArgsReplaceStringUTF;
	using ::JArgsReplaceChar;
	using ::JArgsReplaceBoolean;
	using ::JArgsReplaceInt;
	using ::JArgsReplaceLong;
	using ::JArgsReplaceFloat;
	using ::JArgsReplaceDouble;
	using ::JArgsReplaceByte;
	using ::JArgsReplaceShort;

	using ::JArgsDelete;

	using ::GetJArgs;
	using ::GetJArgsString;
	using ::GetJArgsStringUTF;
	using ::GetJArgsChar;
	using ::GetJArgsBoolean;
	using ::GetJArgsLong;
	using ::GetJArgsFloat;
	using ::GetJArgsDouble;
	using ::GetJArgsByte;
	using ::GetJArgsShort;

	using ::JArgsStartV;
	using ::JArgsStart;

	using ::JArgsPopBack;

//  xjni_arrayfield.h

	using ::GetObjectArrayField;
	using ::GetStaticObjectArrayField;
	using ::SetObjectArrayField;
	using ::SetStaticObjectArrayField;

	using ::GetArrayField;
	using ::GetStaticArrayField;
	using ::SetArrayField;
	using ::SetStaticArrayField;

	using ::GetByteArrayField;
	using ::GetStaticByteArrayField;
	using ::SetByteArrayField;
	using ::SetStaticByteArrayField;

	using ::GetIntArrayField;
	using ::GetStaticIntArrayField;
	using ::SetIntArrayField;
	using ::SetStaticIntArrayField;

	using ::GetLongArrayField;
	using ::GetStaticLongArrayField;
	using ::SetLongArrayField;
	using ::SetStaticLongArrayField;

	using ::GetFloatArrayField;
	using ::GetStaticFloatArrayField;
	using ::SetFloatArrayField;
	using ::SetStaticFloatArrayField;

	using ::GetDoubleArrayField;
	using ::GetStaticDoubleArrayField;
	using ::SetDoubleArrayField;
	using ::SetStaticDoubleArrayField;

	using ::GetBooleanArrayField;
	using ::GetStaticBooleanArrayField;
	using ::SetBooleanArrayField;
	using ::SetStaticBooleanArrayField;

//  xjni_classcache.h

	using ::xjni_classcache_init;
	using ::xjni_classcache_get;
	using ::xjni_classcache_insert;
	using ::xjni_classcache_clear;

//  xjni_hashmap.h

	using ::XJNI_HashMap;
	using ::XJNI_HashMapKeyValue;
	using ::XJNI_HashSlotState;
	using ::XJNI_HashValue_t;

	using ::XJNI_HashMapInitNoMutex;
	using ::XJNI_HashMapInit;

	using ::XJNI_HashMapFreeNoLock;
	using ::XJNI_HashMapFree;

	using ::XJNI_HashMapClearNoLock;
	using ::XJNI_HashMapClear;

	using ::XJNI_HashMapInsertNoLock;
	using ::XJNI_HashMapInsert;

	using ::XJNI_HashMapReplaceNoLock;
	using ::XJNI_HashMapReplace;

	using ::XJNI_HashMapReplaceGetOldNoLock;
	using ::XJNI_HashMapReplaceGetOld;

	using ::XJNI_HashMapCompareReplaceNoLock;
	using ::XJNI_HashMapCompareReplace;

	using ::XJNI_HashMapReplaceOrInsertNoLock;
	using ::XJNI_HashMapReplaceOrInsert;

	using ::XJNI_HashMapAddNoLock;
	using ::XJNI_HashMapAdd;

	using ::XJNI_HashMapTryGetNoLock;
	using ::XJNI_HashMapTryGet;

	using ::XJNI_HashMapSearchNoLock;
	using ::XJNI_HashMapSearch;

	using ::XJNI_HashMapDeleteNoLock;
	using ::XJNI_HashMapDelete;

	using ::XJNI_HashMapCheckInvariantNoLock;
	using ::XJNI_HashMapCheckInvariant;

//  xjni_log.h

	using ::xjni_LogPriority;
	using ::XJNI_LOG_UNKNOWN;
	using ::XJNI_LOG_DEFAULT;
	using ::XJNI_LOG_VERBOSE;
	using ::XJNI_LOG_DEBUG;
	using ::XJNI_LOG_INFO;
	using ::XJNI_LOG_WARN;
	using ::XJNI_LOG_ERROR;
	using ::XJNI_LOG_FATAL;
	using ::XJNI_LOG_SILENT;

	using ::xjni_log_vcprint;
	using ::xjni_log_cprint;;

#ifndef ANDROID
	using ::xjni_log_vccprint;
	using ::xjni_log_ccprint;
#endif

//  xjni_hashmap.h

	using ::xjni_pool;
	using ::xjni_pool_result;

	using ::xjni_pool_Create;
	using ::xjni_pool_AllocFromPool;
	using ::xjni_pool_FreeElement;
	using ::xjni_pool_Free;
#ifdef DEBUG
	using ::xjni_pool_UsedCount;
	using ::xjni_pool_FreeCount;
	using ::xjni_pool_Capacity;
#endif

//  xjni2d.h

	using ::NewByte2DArray;
	using ::GetByte2DArrayElements;
	using ::ReleaseByte2DArrayElements;
	using ::SetByte2DArrayRegion;
	using ::GetByte2DArrayRegion;

	using ::NewInt2DArray;
	using ::GetInt2DArrayElements;
	using ::ReleaseInt2DArrayElements;
	using ::SetInt2DArrayRegion;
	using ::GetInt2DArrayRegion;

	using ::NewLong2DArray;
	using ::GetLong2DArrayElements;
	using ::ReleaseLong2DArrayElements;
	using ::SetLong2DArrayRegion;
	using ::GetLong2DArrayRegion;

	using ::NewFloat2DArray;
	using ::GetFloat2DArrayElements;
	using ::ReleaseFloat2DArrayElements;
	using ::SetFloat2DArrayRegion;
	using ::GetFloat2DArrayRegion;

	using ::NewDouble2DArray;
	using ::GetDouble2DArrayElements;
	using ::ReleaseDouble2DArrayElements;
	using ::SetDouble2DArrayRegion;
	using ::GetDouble2DArrayRegion;

	using ::NewShort2DArray;
	using ::GetShort2DArrayElements;
	using ::ReleaseShort2DArrayElements;
	using ::SetShort2DArrayRegion;
	using ::GetShort2DArrayRegion;

	using ::NewChar2DArray;
	using ::GetChar2DArrayElements;
	using ::ReleaseChar2DArrayElements;
	using ::SetChar2DArrayRegion;
	using ::GetChar2DArrayRegion;

	using ::NewBoolean2DArray;
	using ::GetBoolean2DArrayElements;
	using ::ReleaseBoolean2DArrayElements;
	using ::SetBoolean2DArrayRegion;
	using ::GetBoolean2DArrayRegion;

	using ::NewStringUTF2DArray;
	using ::GetStringUTF2DArrayElements;
	using ::ReleaseStringUTF2DArrayChars;
	using ::SetStringUTF2DArrayRegion;
	using ::GetStringUTF2DArrayRegion;

	using ::NewString2DArray;
	using ::GetString2DArrayElements;
	using ::ReleaseString2DArrayChars;
	using ::SetString2DArrayRegion;
	using ::GetString2DArrayRegion;
	using ::GetString2DArrayRegion;

//  xjni_va_list.h

	using ::JSnPrintf;
	using ::JSnPrintfUTF;
	using ::JSPrintf;
	using ::JSPrintfUTF;
	using ::JFPrintf;
	using ::JFPrintfUTF;
	using ::JDPrintf;
	using ::JDPrintfUTF;
	using ::JPrintf;
	using ::JPrintfUTF;

//  xjni_printf.h

	using ::vsjprintf;
	using ::sjprintf;
	using ::vsnjprintf;
	using ::snjprintf;
	using ::vjfprintf;
	using ::jfprintf;
	using ::vjdprintf;
	using ::jdprintf;
	using ::vjprintf;
	using ::jprintf;

//  xjni_string.h

	using ::jmemcpy;
	using ::jmemmove;
	using ::jmemchr;
	using ::jmemset;
	using ::jmemcmp;

	using ::xjni_tochar;
	using ::xjni_tojchar;
	using ::xjni_tostring;
	using ::xjni_tojstring;
	using ::xjni_fromstring;
	using ::xjni_fromjstring;

	using ::jstrlen;
	using ::jstrnlen;
	using ::jstrchr;
	using ::jstrchrnul;
	using ::jstrrchr;
	using ::jstrcpy;
	using ::jstrlcpy;
	using ::jstrcat;
	using ::jstrlcat;
	using ::jstrcmp;
	using ::jstrncmp;
	using ::jstrpbrk;
	using ::jstrstr;
	using ::jstrcspn;
	using ::jstrspn;
	using ::jstrdup;
	using ::jstrndup;
	using ::jstrcoll;
	using ::jstrxfrm;
	using ::jstrtok;
	using ::jstrreverse;
	using ::jstrrev;

	using ::xjni_wcstombs;
	using ::xjni_mbstowcs;
	using ::xjni_jmbstowcs;
	using ::xjni_jwcstombs;

//  xjni_stringarray.h

	using ::NewStringUTFArray;
	using ::GetStringUTFArrayChars;
	using ::ReleaseStringUTFArrayChars;
	using ::GetStringUTFArrayRegion;
	using ::SetStringUTFArrayRegion;
	using ::GetStringUTFArrayLength;

	using ::NewStringArray;
	using ::GetStringArrayLength;
	using ::GetStringArrayChars;
	using ::ReleaseStringArrayChars;
	using ::GetStringArrayRegion;
	using ::SetStringArrayRegion;

//  xjni_struct.h

	using ::xjni_struct_alloc_t;
	using ::xjni_struct_jalloc_t;
	using ::xjni_struct_set_t;
	using ::xjni_struct_get_t;
	using ::xjni_struct_free_t;

	using ::xjni_struct_ctx_init;
	using ::xjni_struct_new;
	using ::xjni_struct_get;
	using ::xjni_struct_release;
	using ::xjni_struct_get_region;
	using ::xjni_struct_set_region;
	using ::xjni_struct_free_null;

//  xjni_stringbuffer.h

	using ::NewStringBuffer;
	using ::NewStringBufferCapacity;
	using ::NewStringBufferString;
	using ::NewStringBufferStringUTF;
	using ::StringBufferToString;
	using ::StringBufferToStringUTF;
	using ::StringBufferSubString;
	using ::StringBufferSubStringIntInt;
	using ::StringBufferSubSequence;
	using ::StringBufferAppendString;
	using ::StringBufferAppendStringUTF;
	using ::StringBufferAppendObject;
	using ::StringBufferAppendStringBuffer;
	using ::StringBufferAppendBoolean;
	using ::StringBufferAppendChar;
	using ::StringBufferAppendCharArray;
	using ::StringBufferAppendInt;
	using ::StringBufferAppendLong;
	using ::StringBufferAppendFloat;
	using ::StringBufferAppendDouble;
	using ::StringBufferAppendCharArrayIntInt;
	using ::StringBufferAppendCodePoint;
	using ::StringBufferLength;
	using ::StringBufferCapacity;
	using ::StringBufferEnsureCapacity;
	using ::StringBufferSetLength;
	using ::StringBufferCharAt;
	using ::StringBufferSetCharAt;
	using ::StringBufferCodePointAt;
	using ::StringBufferCodePointBefore;
	using ::StringBufferCodePointCount;
	using ::StringBufferOffsetByCodePoints;
	using ::StringBufferTrimToSize;
	using ::StringBufferGetChars;
	using ::StringBufferDelete;
	using ::StringBufferDeleteCharAt;
	using ::StringBufferReplace;
	using ::StringBufferReplaceUTF;
	using ::StringBufferInsertCharArrayIntInt;
	using ::StringBufferInsertString;
	using ::StringBufferInsertStringUTF;
	using ::StringBufferInsertObject;
	using ::StringBufferInsertCharArray;
	using ::StringBufferInsertBoolean;
	using ::StringBufferInsertChar;
	using ::StringBufferInsertInt;
	using ::StringBufferInsertLong;
	using ::StringBufferInsertFloat;
	using ::StringBufferInsertDouble;
	using ::StringBufferIndexOf;
	using ::StringBufferIndexOfUTF;
	using ::StringBufferIndexOfI;
	using ::StringBufferIndexOfIUTF;
	using ::StringBufferLastIndexOf;
	using ::StringBufferLastIndexOfUTF;
	using ::StringBufferLastIndexOfI;
	using ::StringBufferLastIndexOfIUTF;
	using ::StringBufferReverse;

//  xjni_stringbuilder.h

	using ::NewStringBuilder;
	using ::NewStringBuilderCapacity;
	using ::NewStringBuilderString;
	using ::NewStringBuilderStringUTF;
	using ::StringBuilderToString;
	using ::StringBuilderToStringUTF;
	using ::StringBuilderAppendString;
	using ::StringBuilderAppendStringUTF;
	using ::StringBuilderAppendObject;
	using ::StringBuilderAppendStringBuffer;
	using ::StringBuilderAppendBoolean;
	using ::StringBuilderAppendChar;
	using ::StringBuilderAppendInt;
	using ::StringBuilderAppendLong;
	using ::StringBuilderAppendFloat;
	using ::StringBuilderAppendDouble;
	using ::StringBuilderAppendCharArrayIntInt;
	using ::StringBuilderAppendCodePoint;
	using ::StringBuilderDelete;
	using ::StringBuilderDeleteCharAt;
	using ::StringBuilderReplace;
	using ::StringBuilderReplaceUTF;
	using ::StringBuilderInsertCharArrayIntInt;
	using ::StringBuilderInsertString;
	using ::StringBuilderInsertStringUTF;
	using ::StringBuilderInsertObject;
	using ::StringBuilderInsertCharArray;
	using ::StringBuilderInsertBoolean;
	using ::StringBuilderInsertChar;
	using ::StringBuilderInsertInt;
	using ::StringBuilderInsertLong;
	using ::StringBuilderInsertFloat;
	using ::StringBuilderInsertDouble;
	using ::StringBuilderIndexOf;
	using ::StringBuilderIndexOfUTF;
	using ::StringBuilderIndexOfI;
	using ::StringBuilderIndexOfIUTF;
	using ::StringBuilderLastIndexOf;
	using ::StringBuilderLastIndexOfUTF;
	using ::StringBuilderLastIndexOfI;
	using ::StringBuilderLastIndexOfIUTF;
	using ::StringBuilderReverse;

//  xjni_stringwriter.h

	using ::NewStringWriter;
	using ::NewStringWriterInitialSize;
	using ::StringWriterToString;
	using ::StringWriterToStringUTF;
	using ::StringWriterWriteCharArrayIntInt;
	using ::StringWriterWriteInt;
	using ::StringWriterWriteString;
	using ::StringWriterWriteStringIntInt;
	using ::StringWriterFlush;
	using ::StringWriterAppendChar;

//  xjni_stringreader.h

	using ::NewStringReader;
	using ::NewStringReaderUTF;
	using ::StringReaderToString;
	using ::StringReaderToStringUTF;
	using ::StringReaderEnsureOpen;
	using ::StringReaderRead;
	using ::StringReaderReadCharArrayIntInt;
	using ::StringReaderReadSkip;
	using ::StringReaderReadReady;
	using ::StringReaderReadMarkSupported;
	using ::StringReaderMark;
	using ::StringReaderReset;
	using ::StringReaderClose;

//  xjni_thread.h

	using ::xjni_mutex_t;
	using ::xjni_once_t;
	using ::xjni_thread_t;
	using ::xjni_thread_result_t;
	using ::xjni_once;
	using ::xjni_oncew;
	using ::xjni_mutex_init;
	using ::xjni_mutex_destroy;
	using ::xjni_mutex_lock;
	using ::xjni_mutex_unlock;
	using ::xjni_mutex_trylock;
	using ::xjni_mutex_timedlock;
	using ::xjni_get_realtime;
	using ::xjni_create;
	using ::xjni_join;
	using ::xjni_self;
	using ::xjni_detach;
	using ::xjni_equal;
	using ::xjni_cancel;
	using ::xjni_setcanceltype;
	using ::xjni_setcancelstate;

//  xjni_class.h

	using ::xjni_class_alloc_t;
	using ::xjni_class_jalloc_t;
	using ::xjni_class_set_t;
	using ::xjni_class_get_t;
	using ::xjni_class_free_t;
	using ::xjni_class_ctx_t;

	using ::xjni_class_ctx_init;
	using ::xjni_class_new;
	using ::xjni_class_get;
	using ::xjni_class_release;
	using ::xjni_class_get_region;
	using ::xjni_class_set_region;
	using ::xjni_class_free_null;
}

#undef __MAKE_XXJNI_CLASS_THROW_FUN
#undef __MAKE_XXJNI_THROW_FUN

#endif /* __XXJNI_HPP__ */
