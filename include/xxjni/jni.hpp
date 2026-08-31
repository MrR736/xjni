/**
 * @file jni.hpp
 * @brief JNI C++ Wrapper
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __JNI_HPP__
#define __JNI_HPP__

#include <jni.h>
#include <xjni_windows.h>
#include <xjnidef.h>

#include <cstring>
#include <cstdint>
#include <string>
#include <streambuf>
#include <istream>
#include <ostream>
#include <vector>
#include <stdexcept>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <list>

#if !defined(JNI_VERSION_21)
# error "xjni requires JNI headers from JDK 21 or newer"
#endif

namespace jni {
	using ::JavaVM;
	using ::JNIEnv;

#if CPLUSPLUS_GE(STDC17PP)
	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
#else
	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
#endif

	template <typename T>
	struct __xjni_call_method {
		static_assert(
			std::is_same<T, void>::value ||
			std::is_same<T, jint>::value ||
			std::is_same<T, jlong>::value ||
			std::is_same<T, jfloat>::value ||
			std::is_same<T, jdouble>::value ||
			std::is_same<T, jbyte>::value ||
			std::is_same<T, jboolean>::value ||
			std::is_same<T, jchar>::value ||
			std::is_same<T, jshort>::value ||
			std::is_same<T, ::jstring>::value ||
			std::is_same<T, jobject>::value,
			"Unsupported JNI Call Method return type"
		);
	};

	template <>
	struct __xjni_call_method<jint> {
		template <typename... Args>
		static jint call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallIntMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jint call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticIntMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jint call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualIntMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jlong> {
		template <typename... Args>
		static jlong call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallLongMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jlong call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticLongMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jlong call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualLongMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jfloat> {
		template <typename... Args>
		static jfloat call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args){
			return env->CallFloatMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jfloat call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args){
			return env->CallStaticFloatMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jfloat call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualFloatMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jdouble> {
		template <typename... Args>
		static jdouble call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallDoubleMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jdouble call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticDoubleMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jdouble call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualDoubleMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jbyte> {
		template <typename... Args>
		static jbyte call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallByteMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jbyte call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticByteMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jbyte call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualByteMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jboolean> {
		template <typename... Args>
		static jboolean call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallBooleanMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jboolean call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticBooleanMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jboolean call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualBooleanMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jchar> {
		template <typename... Args>
		static jchar call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallCharMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jchar call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticCharMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jchar call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualCharMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jshort> {
		template <typename... Args>
		static jshort call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallShortMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jshort call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticShortMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jshort call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualShortMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<jobject> {
		template <typename... Args>
		static jobject call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return env->CallObjectMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jobject call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallStaticObjectMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static jobject call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return env->CallNonvirtualObjectMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<::jstring> {
		template <typename... Args>
		static ::jstring call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			return (::jstring)env->CallObjectMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static ::jstring call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			return (::jstring)env->CallStaticObjectMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static ::jstring call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return (::jstring)env->CallNonvirtualObjectMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <>
	struct __xjni_call_method<void> {
		template <typename... Args>
		static void call(JNIEnv* env,jobject obj,jmethodID methodID,Args&&... args) {
			env->CallVoidMethod(obj,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void call(JNIEnv* env,jclass classz,jmethodID methodID,Args&&... args) {
			env->CallStaticVoidMethod(classz,methodID,std::forward<Args>(args)...);
		}
		template <typename... Args>
		static void call(JNIEnv* env,jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			env->CallNonvirtualVoidMethod(obj,classz,methodID,std::forward<Args>(args)...);
		}
	};

	template <typename T>
	struct __xjni_java_box {
		static_assert(
			std::is_same<T, void>::value ||
			std::is_same<T, jint>::value ||
			std::is_same<T, jlong>::value ||
			std::is_same<T, jfloat>::value ||
			std::is_same<T, jdouble>::value ||
			std::is_same<T, jbyte>::value ||
			std::is_same<T, jboolean>::value ||
			std::is_same<T, jchar>::value ||
			std::is_same<T, jshort>::value,
			"Unsupported JNI Class return type"
		);
	};

	template <>
	struct __xjni_java_box<jint>{
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Integer");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(I)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "intValue", "()I");
		}
	};

	template <>
	struct __xjni_java_box<jlong> {
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Long");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(J)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "longValue", "()J");
		}
	};

	template <>
	struct __xjni_java_box<jfloat> {
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Float");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(F)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "floatValue", "()F");
		}
	};

	template <>
	struct __xjni_java_box<jdouble> {
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Double");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(D)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "doubleValue", "()D");
		}
	};

	template <>
	struct __xjni_java_box<jbyte> {
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Byte");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(B)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "byteValue", "()B");
		}
	};

	template <>
	struct __xjni_java_box<jboolean> {
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Boolean");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(Z)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "booleanValue", "()Z");
		}
	};

	template <>
	struct __xjni_java_box<jchar> {
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Character");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(C)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "charValue", "()C");
		}
	};

	template <>
	struct __xjni_java_box<jshort> {
		static jclass findClass(JNIEnv* env) {
			return env->FindClass("java/lang/Short");
		}
		static jmethodID constructorMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "<init>", "(S)V");
		}
		static jmethodID valueMethod(JNIEnv* env, jclass clazz) {
			return env->GetMethodID(clazz, "shortValue", "()S");
		}
	};

	template <typename T>
	struct __xjni_java_map {
		static_assert(
			!std::is_same<T, T>::value,
			"Unsupported Java Map type"
		);
	};

	template <typename T>
	jobject __CppToJavaMap(JNIEnv* env,const T& value) {
		jclass clazz = __xjni_java_box<T>::findClass(env);
		if (!clazz) return NULL;
		jmethodID constructor = __xjni_java_box<T>::constructorMethod(env,clazz);
		if (!constructor) {
			env->DeleteLocalRef(clazz);
			return NULL;
		}
		jobject result = env->NewObject(clazz,constructor,value);
		env->DeleteLocalRef(clazz);
		return result;
	}

	template <>
	inline jobject __CppToJavaMap<const char*>(JNIEnv* env,const char* const& value) {
		if (!value) return NULL;
		return static_cast<jobject>(env->NewStringUTF(value));
	}

	template <>
	inline jobject __CppToJavaMap<std::string>(JNIEnv* env,const std::string& value) {
		return static_cast<jobject>(env->NewStringUTF(value.c_str()));
	}

	template <typename T>
	T __JavaToCppMap(JNIEnv* env,jobject object) {
		if (!object) return T();
		jclass clazz = env->GetObjectClass(object);
		if (!clazz) return T();
		jmethodID valueMethod = __xjni_java_box<T>::valueMethod(env,clazz);
		if (!valueMethod) {
			env->DeleteLocalRef(clazz);
			return T();
		}
		T value = __xjni_call_method<T>::call(env,object,valueMethod);
		env->DeleteLocalRef(clazz);
		return value;
	}

	template <>
	inline std::string __JavaToCppMap<std::string>(JNIEnv* env,jobject object) {
		if (!object) return std::string();
		::jstring string = static_cast<::jstring>(object);
		const char* chars = env->GetStringUTFChars(string,NULL);
		if (!chars) return std::string();
		std::string result(chars);
		env->ReleaseStringUTFChars(string,chars);
		return result;
	}

	template <>
	inline jobject __JavaToCppMap<jobject>(JNIEnv*,jobject object) {
		return object;
	}

	template <typename K, typename V>
	struct __xjni_std_map_converter {

		typedef std::map<K, V> map_type;

		static map_type fromJava(JNIEnv* env,jobject object) {
			map_type result;

			if (!object) return result;

			jclass mapClass = env->GetObjectClass(object);
			if (!mapClass) return result;
			jmethodID entrySetMethod = env->GetMethodID(mapClass,"entrySet","()Ljava/util/Set;");
			if (!entrySetMethod) {
				env->DeleteLocalRef(mapClass);
				return result;
			}

			jobject entrySet = env->CallObjectMethod(object,entrySetMethod);
			if (env->ExceptionCheck()) {
				env->ExceptionClear();
				env->DeleteLocalRef(mapClass);
				return result;
			}

			if (!entrySet) {
				env->DeleteLocalRef(mapClass);
				return result;
			}

			jclass setClass = env->GetObjectClass(entrySet);
			if (!setClass) {
				env->DeleteLocalRef(entrySet);
				env->DeleteLocalRef(mapClass);
				return result;
			}

			jmethodID iteratorMethod = env->GetMethodID(setClass,"iterator","()Ljava/util/Iterator;");
			if (!iteratorMethod) {
				env->DeleteLocalRef(setClass);
				env->DeleteLocalRef(entrySet);
				env->DeleteLocalRef(mapClass);
				return result;
			}
			jobject iterator = env->CallObjectMethod(entrySet,iteratorMethod);
			if (env->ExceptionCheck()) {
				env->ExceptionClear();
				env->DeleteLocalRef(setClass);
				env->DeleteLocalRef(entrySet);
				env->DeleteLocalRef(mapClass);
				return result;
			}

			if (!iterator) {
				env->DeleteLocalRef(setClass);
				env->DeleteLocalRef(entrySet);
				env->DeleteLocalRef(mapClass);
				return result;
			}

			jclass iteratorClass = env->GetObjectClass(iterator);
			jmethodID hasNextMethod = env->GetMethodID(iteratorClass,"hasNext","()Z");
			jmethodID nextMethod = env->GetMethodID(iteratorClass,"next","()Ljava/lang/Object;");
			if (!hasNextMethod || !nextMethod) {
				env->DeleteLocalRef(iteratorClass);
				env->DeleteLocalRef(iterator);
				env->DeleteLocalRef(setClass);
				env->DeleteLocalRef(entrySet);
				env->DeleteLocalRef(mapClass);
				return result;
			}

			jclass entryClass = NULL;
			jmethodID getKeyMethod = NULL;
			jmethodID getValueMethod = NULL;

			while (env->CallBooleanMethod(iterator,hasNextMethod)) {
				if (env->ExceptionCheck()) {
					env->ExceptionClear();
					break;
				}
				jobject entry = env->CallObjectMethod(iterator,nextMethod);
				if (env->ExceptionCheck()) {
					env->ExceptionClear();
					break;
				}
				if (!entry) continue;
				if (!entryClass) {
					entryClass = env->GetObjectClass(entry);
					getKeyMethod = env->GetMethodID(entryClass,"getKey","()Ljava/lang/Object;");
					getValueMethod = env->GetMethodID(entryClass,"getValue","()Ljava/lang/Object;");
				}
				if (!getKeyMethod || !getValueMethod) {
					env->DeleteLocalRef(entry);
					break;
				}
				jobject keyObject = env->CallObjectMethod(entry,getKeyMethod);
				jobject valueObject = env->CallObjectMethod(entry,getValueMethod);
				if (env->ExceptionCheck()) {
					env->ExceptionClear();
					if (keyObject) env->DeleteLocalRef(keyObject);
					if (valueObject) env->DeleteLocalRef(valueObject);
					env->DeleteLocalRef(entry);
					break;
				}
				K key = __JavaToCppMap<K>(env,keyObject);
				V value = __JavaToCppMap<V>(env,valueObject);
				result.emplace(std::move(key),std::move(value));
				if (keyObject) env->DeleteLocalRef(keyObject);
				if (valueObject) env->DeleteLocalRef(valueObject);
				env->DeleteLocalRef(entry);
			}
			if (entryClass) env->DeleteLocalRef(entryClass);
			env->DeleteLocalRef(iteratorClass);
			env->DeleteLocalRef(iterator);
			env->DeleteLocalRef(setClass);
			env->DeleteLocalRef(entrySet);
			env->DeleteLocalRef(mapClass);
			return result;
		}

		static jobject toJava(JNIEnv* env,const map_type& value) {
			jclass hashMapClass = env->FindClass("java/util/HashMap");
			if (!hashMapClass) return NULL;
			jmethodID constructor = env->GetMethodID(hashMapClass,"<init>","()V");
			if (!constructor) {
				env->DeleteLocalRef(hashMapClass);
				return NULL;
			}
			jobject result = env->NewObject(hashMapClass,constructor);
			if (!result) {
				env->DeleteLocalRef(hashMapClass);
				return NULL;
			}
			jmethodID putMethod = env->GetMethodID(hashMapClass,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
			if (!putMethod) {
				env->DeleteLocalRef(result);
				env->DeleteLocalRef(hashMapClass);
				return NULL;
			}
			typename map_type::const_iterator it;
			for (it = value.begin(); it != value.end(); ++it) {
				jobject key = __CppToJavaMap<K>(env,it->first);
				jobject val = __CppToJavaMap<V>(env,it->second);
				jobject oldValue = env->CallObjectMethod(result,putMethod,key,val);
				if (oldValue) env->DeleteLocalRef(oldValue);
				if (key) env->DeleteLocalRef(key);
				if (val) env->DeleteLocalRef(val);
				if (env->ExceptionCheck()) {
					env->ExceptionClear();
					break;
				}
			}
			env->DeleteLocalRef(hashMapClass);
			return result;
		}
	};

	template <typename K, typename V>
	struct __xjni_std_unordered_map_converter {

		typedef std::unordered_map<K, V> map_type;

		static map_type fromJava(
			JNIEnv* env,
			jobject object
		) {
			map_type result;

			if (!object)
				return result;

			jclass mapClass =
			env->GetObjectClass(object);

			jmethodID entrySetMethod =
			env->GetMethodID(
				mapClass,
				"entrySet",
				"()Ljava/util/Set;"
			);

			if (!entrySetMethod) {
				env->DeleteLocalRef(mapClass);
				return result;
			}

			jobject entrySet =
			env->CallObjectMethod(
				object,
				entrySetMethod
			);

			if (!entrySet) {
				env->DeleteLocalRef(mapClass);
				return result;
			}

			jclass setClass =
			env->GetObjectClass(entrySet);

			jmethodID iteratorMethod =
			env->GetMethodID(
				setClass,
				"iterator",
				"()Ljava/util/Iterator;"
			);

			jobject iterator =
			env->CallObjectMethod(
				entrySet,
				iteratorMethod
			);

			jclass iteratorClass =
			env->GetObjectClass(iterator);

			jmethodID hasNextMethod =
			env->GetMethodID(
				iteratorClass,
				"hasNext",
				"()Z"
			);

			jmethodID nextMethod =
			env->GetMethodID(
				iteratorClass,
				"next",
				"()Ljava/lang/Object;"
			);

			jclass entryClass = NULL;

			jmethodID getKeyMethod = NULL;
			jmethodID getValueMethod = NULL;

			while (
				env->CallBooleanMethod(
					iterator,
					hasNextMethod
				)
			) {
				jobject entry =
				env->CallObjectMethod(
					iterator,
					nextMethod
				);

				if (!entry)
					continue;

				if (!entryClass) {
					entryClass =
					env->GetObjectClass(entry);

					getKeyMethod =
					env->GetMethodID(
						entryClass,
					  "getKey",
					  "()Ljava/lang/Object;"
					);

					getValueMethod =
					env->GetMethodID(
						entryClass,
					  "getValue",
					  "()Ljava/lang/Object;"
					);
				}

				jobject keyObject =
				env->CallObjectMethod(
					entry,
					getKeyMethod
				);

				jobject valueObject =
				env->CallObjectMethod(
					entry,
					getValueMethod
				);

				K key =
				__JavaToCppMap<K>(
					env,
				 keyObject
				);

				V value =
				__JavaToCppMap<V>(
					env,
				 valueObject
				);

				result.emplace(
					std::move(key),
							   std::move(value)
				);

				if (keyObject)
					env->DeleteLocalRef(keyObject);

				if (valueObject)
					env->DeleteLocalRef(valueObject);

				env->DeleteLocalRef(entry);
			}

			if (entryClass)
				env->DeleteLocalRef(entryClass);

			env->DeleteLocalRef(iteratorClass);
			env->DeleteLocalRef(iterator);
			env->DeleteLocalRef(setClass);
			env->DeleteLocalRef(entrySet);
			env->DeleteLocalRef(mapClass);

			return result;
		}


		static jobject toJava(
			JNIEnv* env,
			const map_type& value
		) {
			jclass hashMapClass =
			env->FindClass(
				"java/util/HashMap"
			);

			if (!hashMapClass)
				return NULL;

			jmethodID constructor =
			env->GetMethodID(
				hashMapClass,
				"<init>",
				"()V"
			);

			if (!constructor) {
				env->DeleteLocalRef(hashMapClass);
				return NULL;
			}

			jobject result =
			env->NewObject(
				hashMapClass,
				constructor
			);

			jmethodID putMethod =
			env->GetMethodID(
				hashMapClass,
				"put",
				"(Ljava/lang/Object;"
				"Ljava/lang/Object;)"
				"Ljava/lang/Object;"
			);

			typename map_type::const_iterator it;

			for (
				it = value.begin();
			it != value.end();
			++it
			) {
				jobject key =
				__CppToJavaMap<K>(
					env,
				 it->first
				);

				jobject val =
				__CppToJavaMap<V>(
					env,
				 it->second
				);

				jobject oldValue =
				env->CallObjectMethod(
					result,
					putMethod,
					key,
					val
				);

				if (oldValue)
					env->DeleteLocalRef(oldValue);

				if (key)
					env->DeleteLocalRef(key);

				if (val)
					env->DeleteLocalRef(val);
			}

			env->DeleteLocalRef(hashMapClass);

			return result;
		}
	};

	class jstring : public std::basic_string<jchar> {
	public:
		using std::basic_string<jchar>::basic_string;
		using std::basic_string<jchar>::resize;

		::jsize jsize() const noexcept {
			return static_cast<::jsize>(size());
		}

		void resize(::jsize n) {
			std::basic_string<jchar>::resize(static_cast<std::size_t>(n));
		}
	};

	class XJavaVM {
	private:
		JavaVM* _vm = nullptr;
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr XJavaVM() noexcept = default;
#endif
		explicit constexpr XJavaVM(JavaVM* vm) noexcept : _vm(vm) {}

		JavaVM* get() const noexcept {
			return _vm;
		}

		explicit constexpr operator bool() const noexcept {
			return _vm != nullptr;
		}

		bool SupportsVersion(jint version) const noexcept {
			if (_vm == nullptr) return false;
			JNIEnv* env = nullptr;
			return _vm->GetEnv(reinterpret_cast<void**>(&env),version) == JNI_OK;
		}

		jint DestroyJavaVM() {
			return _vm->DestroyJavaVM();
		}

		jint AttachCurrentThread(void** penv, void* args) {
			return _vm->AttachCurrentThread(penv, args);
		}
		jint AttachCurrentThread(JNIEnv** penv, void* args) {
			return _vm->AttachCurrentThread(reinterpret_cast<void**>(penv),args);
		}

		jint DetachCurrentThread() {
			return _vm->DetachCurrentThread();
		}

		jint GetEnv(void** penv, jint version) {
			return _vm->GetEnv(penv, version);
		}
		jint GetEnv(JNIEnv** penv, jint version) {
			return _vm->GetEnv(reinterpret_cast<void**>(penv),version);
		}

		jint AttachCurrentThreadAsDaemon(void** penv, void* args) {
			return _vm->AttachCurrentThreadAsDaemon(penv, args);
		}
		jint AttachCurrentThreadAsDaemon(JNIEnv** penv, void* args) {
			return _vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void**>(penv),args);
		}
	};

	class JavaInputStreamBuf final : public std::streambuf {
	public:
		JavaInputStreamBuf(JNIEnv* env,jobject inputStream,std::size_t bufferSize = 8192)
		: mEnv(env), mInputStream(env->NewGlobalRef(inputStream)),mBufferArray(nullptr),mRead(nullptr),mBuffer(bufferSize) {
			if (mInputStream == nullptr) {
				throw std::runtime_error("Failed to create InputStream global reference");
			}
			jclass cls = env->GetObjectClass(inputStream);
			if (cls == nullptr) {
				env->DeleteGlobalRef(mInputStream);
				mInputStream = nullptr;
				throw std::runtime_error("Failed to get InputStream class");
			}
			mRead = env->GetMethodID(cls,"read","([BII)I");
			if (mRead == nullptr) {
				env->DeleteLocalRef(cls);
				env->DeleteGlobalRef(mInputStream);
				mInputStream = nullptr;
				throw std::runtime_error("InputStream.read(byte[], int, int) not found");
			}
			jbyteArray array = env->NewByteArray(static_cast<jsize>(mBuffer.size()));
			if (array == nullptr) {
				env->DeleteLocalRef(cls);
				env->DeleteGlobalRef(mInputStream);
				mInputStream = nullptr;
				throw std::runtime_error("Failed to allocate Java InputStream buffer");
			}
			mBufferArray = env->NewGlobalRef(array);
			env->DeleteLocalRef(array);
			env->DeleteLocalRef(cls);
			if (mBufferArray == nullptr) {
				env->DeleteGlobalRef(mInputStream);
				mInputStream = nullptr;
				throw std::runtime_error("Failed to create Java buffer global reference");
			}
			setg(nullptr, nullptr, nullptr);
		}

		~JavaInputStreamBuf() override {
			if (mBufferArray != nullptr) { mEnv->DeleteGlobalRef(mBufferArray); }
			if (mInputStream != nullptr) { mEnv->DeleteGlobalRef(mInputStream); }
		}

	protected:
		int_type underflow() override {
			if (gptr() != nullptr && gptr() < egptr()) { return traits_type::to_int_type(*gptr()); }
			const jint count = mEnv->CallIntMethod(mInputStream,mRead,mBufferArray,0,static_cast<jint>(mBuffer.size()));
			if (mEnv->ExceptionCheck()) { return traits_type::eof(); }
			if (count <= 0) { return traits_type::eof(); }
			mEnv->GetByteArrayRegion(static_cast<jbyteArray>(mBufferArray),0,count,reinterpret_cast<jbyte*>(mBuffer.data()));
			if (mEnv->ExceptionCheck()) { return traits_type::eof(); }
			setg(mBuffer.data(),mBuffer.data(),mBuffer.data() + count);
			return traits_type::to_int_type(*gptr());
		}
	private:
		JNIEnv* mEnv;
		jobject mInputStream;
		jobject mBufferArray;
		jmethodID mRead;

		std::vector<char> mBuffer;
	};

	class JavaInputStream final : public std::istream {
	public:
		JavaInputStream(JNIEnv* env,jobject inputStream) : std::istream(nullptr), mBuf(env, inputStream) {
			rdbuf(&mBuf);
		}
	private:
		JavaInputStreamBuf mBuf;
	};

	class JavaOutputStreamBuf final : public std::streambuf {
	public:
		JavaOutputStreamBuf(JNIEnv* env,jobject outputStream,std::size_t bufferSize = 8192)
		: mEnv(env),mOutputStream(env->NewGlobalRef(outputStream)),mBuffer(bufferSize) {
			if (mOutputStream == nullptr) {
				throw std::runtime_error("Failed to create OutputStream global reference");
			}
			jclass cls = env->GetObjectClass(outputStream);
			if (cls == nullptr) {
				env->DeleteGlobalRef(mOutputStream);
				mOutputStream = nullptr;
				throw std::runtime_error("Failed to get OutputStream class");
			}
			mWrite = env->GetMethodID(cls,"write","([BII)V");
			if (mWrite == nullptr) {
				env->DeleteLocalRef(cls);
				env->DeleteGlobalRef(mOutputStream);
				mOutputStream = nullptr;
				throw std::runtime_error("OutputStream.write(byte[], int, int) not found");
			}

			mFlush = env->GetMethodID(cls,"flush","()V");
			if (mFlush == nullptr) {
				env->DeleteLocalRef(cls);
				env->DeleteGlobalRef(mOutputStream);
				mOutputStream = nullptr;
				throw std::runtime_error("OutputStream.flush() not found");
			}
			jbyteArray array = env->NewByteArray(static_cast<jsize>(mBuffer.size()));
			if (array == nullptr) {
				env->DeleteLocalRef(cls);
				env->DeleteGlobalRef(mOutputStream);
				mOutputStream = nullptr;
				throw std::runtime_error("Failed to allocate Java OutputStream buffer");
			}
			mBufferArray = env->NewGlobalRef(array);
			env->DeleteLocalRef(array);
			env->DeleteLocalRef(cls);
			if (mBufferArray == nullptr) {
				env->DeleteGlobalRef(mOutputStream);
				mOutputStream = nullptr;
				throw std::runtime_error("Failed to create Java buffer global reference");
			}
			setp(mBuffer.data(),mBuffer.data() + mBuffer.size());
		}

		~JavaOutputStreamBuf() override {
			sync();
			if (mBufferArray != nullptr) { mEnv->DeleteGlobalRef(mBufferArray); }
			if (mOutputStream != nullptr) { mEnv->DeleteGlobalRef(mOutputStream); }
		}

	protected:
		int_type overflow(int_type ch = traits_type::eof()) override {
			if (!FlushBuffer()) { return traits_type::eof(); }
			if (!traits_type::eq_int_type(ch, traits_type::eof())) {
				*pptr() = traits_type::to_char_type(ch);
				pbump(1);
			}
			return traits_type::not_eof(ch);
		}

		std::streamsize xsputn(const char* data,std::streamsize size) override {
			std::streamsize written = 0;
			while (written < size) {
				const std::streamsize available =
				epptr() - pptr();
				if (available == 0) {
					if (traits_type::eq_int_type(overflow(),traits_type::eof())) { break; }
				}
				const std::streamsize count = std::min(available, size - written);
				std::memcpy(pptr(),data + written,static_cast<std::size_t>(count));
				pbump(static_cast<int>(count));
				written += count;
			}
			return written;
		}

		int sync() override {
			return FlushBuffer() ? 0 : -1;
		}

	private:
		bool FlushBuffer() {
			const std::ptrdiff_t size = pptr() - pbase();
			if (size <= 0) { return true; }
			mEnv->SetByteArrayRegion(
				static_cast<jbyteArray>(mBufferArray),0,static_cast<jsize>(size),
				reinterpret_cast<const jbyte*>(mBuffer.data()));
			if (mEnv->ExceptionCheck()) { return false; }
			mEnv->CallVoidMethod(mOutputStream,mWrite,mBufferArray,0,static_cast<jint>(size));
			if (mEnv->ExceptionCheck()) {
				return false;
			}
			setp(mBuffer.data(),mBuffer.data() + mBuffer.size());
			return true;
		}

	private:
		JNIEnv* mEnv;
		jobject mOutputStream;
		jobject mBufferArray;

		jmethodID mWrite;
		jmethodID mFlush;

		std::vector<char> mBuffer;
	};

	class JavaOutputStream final : public std::ostream {
	public:
		JavaOutputStream(JNIEnv* env,jobject outputStream) : std::ostream(nullptr),
		mBuf(env, outputStream) { rdbuf(&mBuf); }
		~JavaOutputStream() override { flush(); }

	private:
		JavaOutputStreamBuf mBuf;
	};

	class XJNIEnv {
	public:
#if CPLUSPLUS_GE(STDC23PP)
		constexpr XJNIEnv() noexcept = default;
#endif
		explicit constexpr XJNIEnv(JNIEnv* env) noexcept : _env(env) {}

		JNIEnv* get() const noexcept {
			return _env;
		}

		explicit constexpr operator bool() const noexcept {
			return _env != nullptr;
		}

		jint GetVersion() {
			return _env->GetVersion();
		}
		jclass DefineClass(const char *name,jobject loader,const jbyte *buf,jsize len) {
			return _env->DefineClass(name,loader,buf,len);
		}
		jclass FindClass(const char *name) {
			return _env->FindClass(name);
		}
		jmethodID FromReflectedMethod(jobject method) {
			return _env->FromReflectedMethod(method);
		}
		jfieldID FromReflectedField(jobject field) {
			return _env->FromReflectedField(field);
		}

		jobject ToReflectedMethod(jclass cls,jmethodID methodID,jboolean isStatic) {
			return _env->ToReflectedMethod(cls,methodID,isStatic);
		}

		jclass GetSuperclass(jclass sub) {
			return _env->GetSuperclass(sub);
		}
		jboolean IsAssignableFrom(jclass sub,jclass sup) {
			return _env->IsAssignableFrom(sub,sup);
		}

		jobject ToReflectedField(jclass cls,jfieldID fieldID,jboolean isStatic) {
			return _env->ToReflectedField(cls,fieldID,isStatic);
		}

		jint Throw(jthrowable obj) {
			return _env->Throw(obj);
		}
		jint ThrowNew(jclass clazz,const char *msg) {
			return _env->ThrowNew(clazz,msg);
		}
		jthrowable ExceptionOccurred() {
			return _env->ExceptionOccurred();
		}
		void ExceptionDescribe() {
			_env->ExceptionDescribe();
		}
		void ExceptionClear() {
			_env->ExceptionClear();
		}
		void FatalError(const char *msg) {
			_env->FatalError(msg);
		}
		jint PushLocalFrame(jint capacity) {
			return _env->PushLocalFrame(capacity);
		}
		jobject PopLocalFrame(jobject result) {
			return _env->PopLocalFrame(result);
		}
		jobject NewGlobalRef(jobject lobj) {
			return _env->NewGlobalRef(lobj);
		}
		void DeleteGlobalRef(jobject gref) {
			_env->DeleteGlobalRef(gref);
		}
		void DeleteLocalRef(jobject obj) {
			_env->DeleteLocalRef(obj);
		}
		jboolean IsSameObject(jobject obj1,jobject obj2) {
			return _env->IsSameObject(obj1,obj2);
		}
		jobject NewLocalRef(jobject ref) {
			return _env->NewLocalRef(ref);
		}
		jint EnsureLocalCapacity(jint capacity) {
			return _env->EnsureLocalCapacity(capacity);
		}
		jobject AllocObject(jclass clazz) {
			return _env->AllocObject(clazz);
		}
		jobject NewObject(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jobject result;
			va_start(args,methodID);
			result = _env->NewObjectV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jobject NewObjectV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->NewObjectV(clazz,methodID,args);
		}
		jobject NewObjectA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->NewObjectA(clazz,methodID,args);
		}
		jclass GetObjectClass(jobject obj) {
			return _env->GetObjectClass(obj);
		}
		jboolean IsInstanceOf(jobject obj,jclass clazz) {
			return _env->IsInstanceOf(obj,clazz);
		}
		jmethodID GetMethodID(jclass clazz,const char *name,const char *sig) {
			return _env->GetMethodID(clazz,name,sig);
		}
		jobject CallObjectMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jobject result;
			va_start(args,methodID);
			result = _env->CallObjectMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jobject CallObjectMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallObjectMethodV(obj,methodID,args);
		}
		jobject CallObjectMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallObjectMethodA(obj,methodID,args);
		}
		jboolean CallBooleanMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jboolean result;
			va_start(args,methodID);
			result = _env->CallBooleanMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jboolean CallBooleanMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallBooleanMethodV(obj,methodID,args);
		}
		jboolean CallBooleanMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallBooleanMethodA(obj,methodID,args);
		}
		jbyte CallByteMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jbyte result;
			va_start(args,methodID);
			result = _env->CallByteMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jbyte CallByteMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallByteMethodV(obj,methodID,args);
		}
		jbyte CallByteMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallByteMethodA(obj,methodID,args);
		}
		jchar CallCharMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jchar result;
			va_start(args,methodID);
			result = _env->CallCharMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jchar CallCharMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallCharMethodV(obj,methodID,args);
		}
		jchar CallCharMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallCharMethodA(obj,methodID,args);
		}
		jshort CallShortMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jshort result;
			va_start(args,methodID);
			result = _env->CallShortMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jshort CallShortMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallShortMethodV(obj,methodID,args);
		}
		jshort CallShortMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallShortMethodA(obj,methodID,args);
		}
		jint CallIntMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jint result;
			va_start(args,methodID);
			result = _env->CallIntMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jint CallIntMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallIntMethodV(obj,methodID,args);
		}
		jint CallIntMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallIntMethodA(obj,methodID,args);
		}
		jlong CallLongMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jlong result;
			va_start(args,methodID);
			result = _env->CallLongMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jlong CallLongMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallLongMethodV(obj,methodID,args);
		}
		jlong CallLongMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallLongMethodA(obj,methodID,args);
		}
		jfloat CallFloatMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jfloat result;
			va_start(args,methodID);
			result = _env->CallFloatMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jfloat CallFloatMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallFloatMethodV(obj,methodID,args);
		}
		jfloat CallFloatMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallFloatMethodA(obj,methodID,args);
		}
		jdouble CallDoubleMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			jdouble result;
			va_start(args,methodID);
			result = _env->CallDoubleMethodV(obj,methodID,args);
			va_end(args);
			return result;
		}
		jdouble CallDoubleMethodV(jobject obj,jmethodID methodID,va_list args) {
			return _env->CallDoubleMethodV(obj,methodID,args);
		}
		jdouble CallDoubleMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			return _env->CallDoubleMethodA(obj,methodID,args);
		}
		void CallVoidMethod(jobject obj,jmethodID methodID,...) {
			va_list args;
			va_start(args,methodID);
			_env->CallVoidMethodV(obj,methodID,args);
			va_end(args);
		}
		void CallVoidMethodV(jobject obj,jmethodID methodID,va_list args) {
			_env->CallVoidMethodV(obj,methodID,args);
		}
		void CallVoidMethodA(jobject obj,jmethodID methodID,const jvalue * args) {
			_env->CallVoidMethodA(obj,methodID,args);
		}
		jobject CallNonvirtualObjectMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jobject result;
			va_start(args,methodID);
			result = _env->CallNonvirtualObjectMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jobject CallNonvirtualObjectMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualObjectMethodV(obj,clazz,methodID,args);
		}
		jobject CallNonvirtualObjectMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualObjectMethodA(obj,clazz,methodID,args);
		}
		jboolean CallNonvirtualBooleanMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jboolean result;
			va_start(args,methodID);
			result = _env->CallNonvirtualBooleanMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jboolean CallNonvirtualBooleanMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualBooleanMethodV(obj,clazz,methodID,args);
		}
		jboolean CallNonvirtualBooleanMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualBooleanMethodA(obj,clazz,methodID,args);
		}
		jbyte CallNonvirtualByteMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jbyte result;
			va_start(args,methodID);
			result = _env->CallNonvirtualByteMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jbyte CallNonvirtualByteMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualByteMethodV(obj,clazz,methodID,args);
		}
		jbyte CallNonvirtualByteMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualByteMethodA(obj,clazz,methodID,args);
		}
		jchar CallNonvirtualCharMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jchar result;
			va_start(args,methodID);
			result = _env->CallNonvirtualCharMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jchar CallNonvirtualCharMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualCharMethodV(obj,clazz,methodID,args);
		}
		jchar CallNonvirtualCharMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualCharMethodA(obj,clazz,methodID,args);
		}
		jshort CallNonvirtualShortMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jshort result;
			va_start(args,methodID);
			result = _env->CallNonvirtualShortMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jshort CallNonvirtualShortMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualShortMethodV(obj,clazz,methodID,args);
		}
		jshort CallNonvirtualShortMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualShortMethodA(obj,clazz,methodID,args);
		}
		jint CallNonvirtualIntMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jint result;
			va_start(args,methodID);
			result = _env->CallNonvirtualIntMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jint CallNonvirtualIntMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualIntMethodV(obj,clazz,methodID,args);
		}
		jint CallNonvirtualIntMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualIntMethodA(obj,clazz,methodID,args);
		}
		jlong CallNonvirtualLongMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jlong result;
			va_start(args,methodID);
			result = _env->CallNonvirtualLongMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jlong  CallNonvirtualLongMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualLongMethodV(obj,clazz,methodID,args);
		}
		jlong  CallNonvirtualLongMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualLongMethodA(obj,clazz,methodID,args);
		}
		jfloat CallNonvirtualFloatMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jfloat result;
			va_start(args,methodID);
			result = _env->CallNonvirtualFloatMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jfloat CallNonvirtualFloatMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualFloatMethodV(obj,clazz,methodID,args);
		}
		jfloat CallNonvirtualFloatMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualFloatMethodA(obj,clazz,methodID,args);
		}
		jdouble CallNonvirtualDoubleMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			jdouble result;
			va_start(args,methodID);
			result = _env->CallNonvirtualDoubleMethodV(obj,clazz,methodID,args);
			va_end(args);
			return result;
		}
		jdouble CallNonvirtualDoubleMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallNonvirtualDoubleMethodV(obj,clazz,methodID,args);
		}
		jdouble CallNonvirtualDoubleMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			return _env->CallNonvirtualDoubleMethodA(obj,clazz,methodID,args);
		}
		void CallNonvirtualVoidMethod(jobject obj,jclass clazz,jmethodID methodID,...) {
			va_list args;
			va_start(args,methodID);
			_env->CallNonvirtualVoidMethodV(obj,clazz,methodID,args);
			va_end(args);
		}
		void CallNonvirtualVoidMethodV(jobject obj,jclass clazz,jmethodID methodID,va_list args) {
			_env->CallNonvirtualVoidMethodV(obj,clazz,methodID,args);
		}
		void CallNonvirtualVoidMethodA(jobject obj,jclass clazz,jmethodID methodID,const jvalue * args) {
			_env->CallNonvirtualVoidMethodA(obj,clazz,methodID,args);
		}

		jfieldID GetFieldID(jclass clazz,const char *name,const char *sig) {
			return _env->GetFieldID(clazz,name,sig);
		}

		jobject GetObjectField(jobject obj,jfieldID fieldID) {
			return _env->GetObjectField(obj,fieldID);
		}
		jboolean GetBooleanField(jobject obj,jfieldID fieldID) {
			return _env->GetBooleanField(obj,fieldID);
		}
		jbyte GetByteField(jobject obj,jfieldID fieldID) {
			return _env->GetByteField(obj,fieldID);
		}
		jchar GetCharField(jobject obj,jfieldID fieldID) {
			return _env->GetCharField(obj,fieldID);
		}
		jshort GetShortField(jobject obj,jfieldID fieldID) {
			return _env->GetShortField(obj,fieldID);
		}
		jint GetIntField(jobject obj,jfieldID fieldID) {
			return _env->GetIntField(obj,fieldID);
		}
		jlong GetLongField(jobject obj,jfieldID fieldID) {
			return _env->GetLongField(obj,fieldID);
		}
		jfloat GetFloatField(jobject obj,jfieldID fieldID) {
			return _env->GetFloatField(obj,fieldID);
		}
		jdouble GetDoubleField(jobject obj,jfieldID fieldID) {
			return _env->GetDoubleField(obj,fieldID);
		}

		void SetObjectField(jobject obj,jfieldID fieldID,jobject val) {
			_env->SetObjectField(obj,fieldID,val);
		}
		void SetBooleanField(jobject obj,jfieldID fieldID,jboolean val) {
			_env->SetBooleanField(obj,fieldID,val);
		}
		void SetByteField(jobject obj,jfieldID fieldID,jbyte val) {
			_env->SetByteField(obj,fieldID,val);
		}
		void SetCharField(jobject obj,jfieldID fieldID,jchar val) {
			_env->SetCharField(obj,fieldID,val);
		}
		void SetShortField(jobject obj,jfieldID fieldID,jshort val) {
			_env->SetShortField(obj,fieldID,val);
		}
		void SetIntField(jobject obj,jfieldID fieldID,jint val) {
			_env->SetIntField(obj,fieldID,val);
		}
		void SetLongField(jobject obj,jfieldID fieldID,jlong val) {
			_env->SetLongField(obj,fieldID,val);
		}
		void SetFloatField(jobject obj,jfieldID fieldID,jfloat val) {
			_env->SetFloatField(obj,fieldID,val);
		}
		void SetDoubleField(jobject obj,jfieldID fieldID,jdouble val) {
			_env->SetDoubleField(obj,fieldID,val);
		}
		jmethodID GetStaticMethodID(jclass clazz,const char *name,const char *sig) {
			return _env->GetStaticMethodID(clazz,name,sig);
		}
		jobject CallStaticObjectMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jobject result;
			va_start(args,methodID);
			result = _env->CallStaticObjectMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jobject CallStaticObjectMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticObjectMethodV(clazz,methodID,args);
		}
		jobject CallStaticObjectMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticObjectMethodA(clazz,methodID,args);
		}
		jboolean CallStaticBooleanMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jboolean result;
			va_start(args,methodID);
			result = _env->CallStaticBooleanMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jboolean CallStaticBooleanMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticBooleanMethodV(clazz,methodID,args);
		}
		jboolean CallStaticBooleanMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticBooleanMethodA(clazz,methodID,args);
		}
		jbyte CallStaticByteMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jbyte result;
			va_start(args,methodID);
			result = _env->CallStaticByteMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jbyte CallStaticByteMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticByteMethodV(clazz,methodID,args);
		}
		jbyte CallStaticByteMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticByteMethodA(clazz,methodID,args);
		}
		jchar CallStaticCharMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jchar result;
			va_start(args,methodID);
			result = _env->CallStaticCharMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jchar CallStaticCharMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticCharMethodV(clazz,methodID,args);
		}
		jchar CallStaticCharMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticCharMethodA(clazz,methodID,args);
		}
		jshort CallStaticShortMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jshort result;
			va_start(args,methodID);
			result = _env->CallStaticShortMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jshort CallStaticShortMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticShortMethodV(clazz,methodID,args);
		}
		jshort CallStaticShortMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticShortMethodA(clazz,methodID,args);
		}

		jint CallStaticIntMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jint result;
			va_start(args,methodID);
			result = _env->CallStaticIntMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jint CallStaticIntMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticIntMethodV(clazz,methodID,args);
		}
		jint CallStaticIntMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticIntMethodA(clazz,methodID,args);
		}
		jlong  CallStaticLongMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jlong result;
			va_start(args,methodID);
			result = _env->CallStaticLongMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jlong  CallStaticLongMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticLongMethodV(clazz,methodID,args);
		}
		jlong  CallStaticLongMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticLongMethodA(clazz,methodID,args);
		}
		jfloat CallStaticFloatMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jfloat result;
			va_start(args,methodID);
			result = _env->CallStaticFloatMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jfloat CallStaticFloatMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticFloatMethodV(clazz,methodID,args);
		}
		jfloat CallStaticFloatMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticFloatMethodA(clazz,methodID,args);
		}
		jdouble CallStaticDoubleMethod(jclass clazz,jmethodID methodID,...) {
			va_list args;
			jdouble result;
			va_start(args,methodID);
			result = _env->CallStaticDoubleMethodV(clazz,methodID,args);
			va_end(args);
			return result;
		}
		jdouble CallStaticDoubleMethodV(jclass clazz,jmethodID methodID,va_list args) {
			return _env->CallStaticDoubleMethodV(clazz,methodID,args);
		}
		jdouble CallStaticDoubleMethodA(jclass clazz,jmethodID methodID,const jvalue *args) {
			return _env->CallStaticDoubleMethodA(clazz,methodID,args);
		}
		void CallStaticVoidMethod(jclass cls,jmethodID methodID,...) {
			va_list args;
			va_start(args,methodID);
			_env->CallStaticVoidMethodV(cls,methodID,args);
			va_end(args);
		}
		void CallStaticVoidMethodV(jclass cls,jmethodID methodID,va_list args) {
			_env->CallStaticVoidMethodV(cls,methodID,args);
		}
		void CallStaticVoidMethodA(jclass cls,jmethodID methodID,const jvalue * args) {
			_env->CallStaticVoidMethodA(cls,methodID,args);
		}
		jfieldID GetStaticFieldID(jclass clazz,const char *name,const char *sig) {
			return _env->GetStaticFieldID(clazz,name,sig);
		}
		jobject GetStaticObjectField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticObjectField(clazz,fieldID);
		}
		jboolean GetStaticBooleanField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticBooleanField(clazz,fieldID);
		}
		jbyte GetStaticByteField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticByteField(clazz,fieldID);
		}
		jchar GetStaticCharField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticCharField(clazz,fieldID);
		}
		jshort GetStaticShortField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticShortField(clazz,fieldID);
		}
		jint GetStaticIntField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticIntField(clazz,fieldID);
		}
		jlong  GetStaticLongField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticLongField(clazz,fieldID);
		}
		jfloat GetStaticFloatField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticFloatField(clazz,fieldID);
		}
		jdouble GetStaticDoubleField(jclass clazz,jfieldID fieldID) {
			return _env->GetStaticDoubleField(clazz,fieldID);
		}

		void SetStaticObjectField(jclass clazz,jfieldID fieldID,jobject value) {
			_env->SetStaticObjectField(clazz,fieldID,value);
		}
		void SetStaticBooleanField(jclass clazz,jfieldID fieldID,jboolean value) {
			_env->SetStaticBooleanField(clazz,fieldID,value);
		}
		void SetStaticByteField(jclass clazz,jfieldID fieldID,jbyte value) {
			_env->SetStaticByteField(clazz,fieldID,value);
		}
		void SetStaticCharField(jclass clazz,jfieldID fieldID,jchar value) {
			_env->SetStaticCharField(clazz,fieldID,value);
		}
		void SetStaticShortField(jclass clazz,jfieldID fieldID,jshort value) {
			_env->SetStaticShortField(clazz,fieldID,value);
		}
		void SetStaticIntField(jclass clazz,jfieldID fieldID,jint value) {
			_env->SetStaticIntField(clazz,fieldID,value);
		}
		void SetStaticLongField(jclass clazz,jfieldID fieldID,jlong value) {
			_env->SetStaticLongField(clazz,fieldID,value);
		}
		void SetStaticFloatField(jclass clazz,jfieldID fieldID,jfloat value) {
			_env->SetStaticFloatField(clazz,fieldID,value);
		}
		void SetStaticDoubleField(jclass clazz,jfieldID fieldID,jdouble value) {
			_env->SetStaticDoubleField(clazz,fieldID,value);
		}

		::jstring NewString(const jchar *unicode,jsize len) {
			return _env->NewString(unicode,len);
		}
		jsize GetStringLength(::jstring str) {
			return _env->GetStringLength(str);
		}
		const jchar *GetStringChars(::jstring str,jboolean *isCopy) {
			return _env->GetStringChars(str,isCopy);
		}
		void ReleaseStringChars(::jstring str,const jchar *chars) {
			_env->ReleaseStringChars(str,chars);
		}

		::jstring NewStringUTF(const char *utf) {
			return _env->NewStringUTF(utf);
		}
		jsize GetStringUTFLength(::jstring str) {
			return _env->GetStringUTFLength(str);
		}
		const char* GetStringUTFChars(::jstring str,jboolean *isCopy) {
			return _env->GetStringUTFChars(str,isCopy);
		}
		void ReleaseStringUTFChars(::jstring str,const char* chars) {
			_env->ReleaseStringUTFChars(str,chars);
		}

		jsize GetArrayLength(jarray array) {
			return _env->GetArrayLength(array);
		}

		jobjectArray NewObjectArray(jsize len,jclass clazz,jobject init) {
			return _env->NewObjectArray(len,clazz,init);
		}
		jobject GetObjectArrayElement(jobjectArray array,jsize index) {
			return _env->GetObjectArrayElement(array,index);
		}
		void SetObjectArrayElement(jobjectArray array,jsize index,jobject val) {
			_env->SetObjectArrayElement(array,index,val);
		}

		jbooleanArray NewBooleanArray(jsize len) {
			return _env->NewBooleanArray(len);
		}
		jbyteArray NewByteArray(jsize len) {
			return _env->NewByteArray(len);
		}
		jcharArray NewCharArray(jsize len) {
			return _env->NewCharArray(len);
		}
		jshortArray NewShortArray(jsize len) {
			return _env->NewShortArray(len);
		}
		jintArray NewIntArray(jsize len) {
			return _env->NewIntArray(len);
		}
		jlongArray NewLongArray(jsize len) {
			return _env->NewLongArray(len);
		}
		jfloatArray NewFloatArray(jsize len) {
			return _env->NewFloatArray(len);
		}
		jdoubleArray NewDoubleArray(jsize len) {
			return _env->NewDoubleArray(len);
		}

		jboolean * GetBooleanArrayElements(jbooleanArray array,jboolean *isCopy) {
			return _env->GetBooleanArrayElements(array,isCopy);
		}
		jbyte * GetByteArrayElements(jbyteArray array,jboolean *isCopy) {
			return _env->GetByteArrayElements(array,isCopy);
		}
		jchar * GetCharArrayElements(jcharArray array,jboolean *isCopy) {
			return _env->GetCharArrayElements(array,isCopy);
		}
		jshort * GetShortArrayElements(jshortArray array,jboolean *isCopy) {
			return _env->GetShortArrayElements(array,isCopy);
		}
		jint * GetIntArrayElements(jintArray array,jboolean *isCopy) {
			return _env->GetIntArrayElements(array,isCopy);
		}
		jlong  * GetLongArrayElements(jlongArray array,jboolean *isCopy) {
			return _env->GetLongArrayElements(array,isCopy);
		}
		jfloat * GetFloatArrayElements(jfloatArray array,jboolean *isCopy) {
			return _env->GetFloatArrayElements(array,isCopy);
		}
		jdouble * GetDoubleArrayElements(jdoubleArray array,jboolean *isCopy) {
			return _env->GetDoubleArrayElements(array,isCopy);
		}
		void ReleaseBooleanArrayElements(jbooleanArray array,jboolean *elems,jint mode) {
			_env->ReleaseBooleanArrayElements(array,elems,mode);
		}
		void ReleaseByteArrayElements(jbyteArray array,jbyte *elems,jint mode) {
			_env->ReleaseByteArrayElements(array,elems,mode);
		}
		void ReleaseCharArrayElements(jcharArray array,jchar *elems,jint mode) {
			_env->ReleaseCharArrayElements(array,elems,mode);
		}
		void ReleaseShortArrayElements(jshortArray array,jshort *elems,jint mode) {
			_env->ReleaseShortArrayElements(array,elems,mode);
		}
		void ReleaseIntArrayElements(jintArray array,jint *elems,jint mode) {
			_env->ReleaseIntArrayElements(array,elems,mode);
		}
		void ReleaseLongArrayElements(jlongArray array,jlong *elems,jint mode) {
			_env->ReleaseLongArrayElements(array,elems,mode);
		}
		void ReleaseFloatArrayElements(jfloatArray array,jfloat *elems,jint mode) {
			_env->ReleaseFloatArrayElements(array,elems,mode);
		}
		void ReleaseDoubleArrayElements(jdoubleArray array,jdouble *elems,jint mode) {
			_env->ReleaseDoubleArrayElements(array,elems,mode);
		}
		void GetBooleanArrayRegion(jbooleanArray array,jsize start,jsize len,jboolean *buf) {
			_env->GetBooleanArrayRegion(array,start,len,buf);
		}
		void GetByteArrayRegion(jbyteArray array,jsize start,jsize len,jbyte *buf) {
			_env->GetByteArrayRegion(array,start,len,buf);
		}
		void GetCharArrayRegion(jcharArray array,jsize start,jsize len,jchar *buf) {
			_env->GetCharArrayRegion(array,start,len,buf);
		}
		void GetShortArrayRegion(jshortArray array,jsize start,jsize len,jshort *buf) {
			_env->GetShortArrayRegion(array,start,len,buf);
		}
		void GetIntArrayRegion(jintArray array,jsize start,jsize len,jint *buf) {
			_env->GetIntArrayRegion(array,start,len,buf);
		}
		void GetLongArrayRegion(jlongArray array,jsize start,jsize len,jlong *buf) {
			_env->GetLongArrayRegion(array,start,len,buf);
		}
		void GetFloatArrayRegion(jfloatArray array,jsize start,jsize len,jfloat *buf) {
			_env->GetFloatArrayRegion(array,start,len,buf);
		}
		void GetDoubleArrayRegion(jdoubleArray array,jsize start,jsize len,jdouble *buf) {
			_env->GetDoubleArrayRegion(array,start,len,buf);
		}

		void SetBooleanArrayRegion(jbooleanArray array,jsize start,jsize len,const jboolean *buf) {
			_env->SetBooleanArrayRegion(array,start,len,buf);
		}
		void SetByteArrayRegion(jbyteArray array,jsize start,jsize len,const jbyte *buf) {
			_env->SetByteArrayRegion(array,start,len,buf);
		}
		void SetCharArrayRegion(jcharArray array,jsize start,jsize len,const jchar *buf) {
			_env->SetCharArrayRegion(array,start,len,buf);
		}
		void SetShortArrayRegion(jshortArray array,jsize start,jsize len,const jshort *buf) {
			_env->SetShortArrayRegion(array,start,len,buf);
		}
		void SetIntArrayRegion(jintArray array,jsize start,jsize len,const jint *buf) {
			_env->SetIntArrayRegion(array,start,len,buf);
		}
		void SetLongArrayRegion(jlongArray array,jsize start,jsize len,const jlong *buf) {
			_env->SetLongArrayRegion(array,start,len,buf);
		}
		void SetFloatArrayRegion(jfloatArray array,jsize start,jsize len,const jfloat *buf) {
			_env->SetFloatArrayRegion(array,start,len,buf);
		}
		void SetDoubleArrayRegion(jdoubleArray array,jsize start,jsize len,const jdouble *buf) {
			_env->SetDoubleArrayRegion(array,start,len,buf);
		}

		jint RegisterNatives(jclass clazz,const JNINativeMethod *methods,jint nMethods) {
			return _env->RegisterNatives(clazz,methods,nMethods);
		}
		jint UnregisterNatives(jclass clazz) {
			return _env->UnregisterNatives(clazz);
		}

		jint MonitorEnter(jobject obj) {
			return _env->MonitorEnter(obj);
		}
		jint MonitorExit(jobject obj) {
			return _env->MonitorExit(obj);
		}

		jint GetJavaVM(JavaVM **vm) {
			return _env->GetJavaVM(vm);
		}

		void GetStringRegion(::jstring str,jsize start,jsize len,jchar *buf) {
			_env->GetStringRegion(str,start,len,buf);
		}
		void GetStringUTFRegion(::jstring str,jsize start,jsize len,char *buf) {
			_env->GetStringUTFRegion(str,start,len,buf);
		}

		void * GetPrimitiveArrayCritical(jarray array,jboolean *isCopy) {
			return _env->GetPrimitiveArrayCritical(array,isCopy);
		}
		void ReleasePrimitiveArrayCritical(jarray array,void *carray,jint mode) {
			_env->ReleasePrimitiveArrayCritical(array,carray,mode);
		}

		const jchar * GetStringCritical(::jstring string,jboolean *isCopy) {
			return _env->GetStringCritical(string,isCopy);
		}
		void ReleaseStringCritical(::jstring string,const jchar *cstring) {
			_env->ReleaseStringCritical(string,cstring);
		}

		jweak NewWeakGlobalRef(jobject obj) {
			return _env->NewWeakGlobalRef(obj);
		}
		void DeleteWeakGlobalRef(jweak ref) {
			_env->DeleteWeakGlobalRef(ref);
		}

		jboolean ExceptionCheck() {
			return _env->ExceptionCheck();
		}

		jobject NewDirectByteBuffer(void* address,jlong capacity) {
			return _env->NewDirectByteBuffer(address,capacity);
		}
		void* GetDirectBufferAddress(jobject buf) {
			return _env->GetDirectBufferAddress(buf);
		}
		jlong  GetDirectBufferCapacity(jobject buf) {
			return _env->GetDirectBufferCapacity(buf);
		}
		jobjectRefType GetObjectRefType(jobject obj) {
			return _env->GetObjectRefType(obj);
		}

		/* Module Features */
		jobject GetModule(jclass clazz) {
			return _env->GetModule(clazz);
		}

		/* Virtual threads */
		jboolean IsVirtualThread(jobject obj) {
			return _env->IsVirtualThread(obj);
		}

		void GetStringRegion(::jstring str,jsize start,jsize len,jstring& buf) {
			if (str == nullptr || len <= 0) {
				buf.clear();
				return;
			}
			buf.resize(len);
			_env->GetStringRegion(str,start,len,&buf[0]);
		}

		void GetStringUTFRegion(::jstring str, jsize start, jsize len, std::string& buf) {
			if (str == nullptr || len <= 0) {
				buf.clear();
				return;
			}
			buf.resize(static_cast<std::size_t>(len) * 3);
			_env->GetStringUTFRegion(str,start,len,&buf[0]);
			buf.resize(std::strlen(buf.c_str()));
		}

		::jstring NewString(const jstring unicode) {
			return _env->NewString(unicode.c_str(),unicode.jsize());
		}
		jsize GetStringLength(const jstring& str) const noexcept {
			return str.jsize();
		}
		jclass DefineClass(const std::string& name,jobject loader,const jbyte *buf,jsize len) {
			return _env->DefineClass(name.c_str(),loader,buf,len);
		}
		jclass FindClass(const std::string& name) {
			return _env->FindClass(name.c_str());
		}
		void FatalError(const std::string& msg) {
			_env->FatalError(msg.c_str());
		}
		jint ThrowNew(jclass clazz,const std::string& msg) {
			return _env->ThrowNew(clazz,msg.c_str());
		}
		jmethodID GetMethodID(jclass clazz,const std::string& name,const std::string& sig) {
			return _env->GetMethodID(clazz,name.c_str(),sig.c_str());
		}
		jfieldID GetFieldID(jclass clazz,const std::string& name,const std::string& sig) {
			return _env->GetFieldID(clazz,name.c_str(),sig.c_str());
		}
		jmethodID GetStaticMethodID(jclass clazz,const std::string& name,const std::string& sig) {
			return _env->GetStaticMethodID(clazz,name.c_str(),sig.c_str());
		}
		jfieldID GetStaticFieldID(jclass clazz,const std::string& name,const std::string& sig) {
			return _env->GetStaticFieldID(clazz,name.c_str(),sig.c_str());
		}

		::jstring NewStringUTF(const std::string& utf) {
			return _env->NewStringUTF(utf.c_str());
		}

		std::string GetStringUTF(::jstring js) {
			if (js == nullptr) return {};
			const char* cs = _env->GetStringUTFChars(js, nullptr);
			if (cs == nullptr) return {};
			std::string result(cs);
			_env->ReleaseStringUTFChars(js, cs);
			return result;
		}

		jstring GetString(::jstring js) {
			if (js == nullptr) return {};
			const jchar* cs = _env->GetStringChars(js, nullptr);
			if (cs == nullptr) return {};
			jstring result(cs);
			_env->ReleaseStringChars(js, cs);
			return result;
		}

		jobject NewIStream(std::istream& stream) {
			jclass cls = _env->FindClass("xjava/io/IStream");
			if (cls == nullptr) { return nullptr; }
			jmethodID ctor = _env->GetMethodID(cls,"<init>","(J)V");
			if (ctor == nullptr) {
				_env->DeleteLocalRef(cls);
				return nullptr;
			}
			jobject result = _env->NewObject(cls,ctor,static_cast<jlong>(reinterpret_cast<std::uintptr_t>(&stream)));
			_env->DeleteLocalRef(cls);
			return result;
		}

		std::unique_ptr<JavaInputStream> GetIStream(jobject stream) {
			return make_unique<JavaInputStream>(_env, stream);
		}

		jobject NewOStream(std::ostream& stream) {
			jclass cls = _env->FindClass("xjava/io/OStream");
			if (cls == nullptr) { return nullptr; }
			jmethodID ctor = _env->GetMethodID(cls,"<init>","(J)V");
			if (ctor == nullptr) {
				_env->DeleteLocalRef(cls);
				return nullptr;
			}
			jobject result = _env->NewObject(cls,ctor,static_cast<jlong>(reinterpret_cast<std::uintptr_t>(&stream)));
			_env->DeleteLocalRef(cls);
			return result;
		}

		std::unique_ptr<JavaOutputStream> GetOStream(jobject stream) {
			return make_unique<JavaOutputStream>(_env, stream);
		}

		template <typename T, typename... Args>
		T CallMethod(jobject obj,jmethodID methodID,Args&&... args) {
			return __xjni_call_method<T>::call(_env,obj,methodID,std::forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		T CallStaticMethod(jclass classz,jmethodID methodID,Args&&... args) {
			return __xjni_call_method<T>::call(_env,classz,methodID,std::forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		T CallNonvirtualMethod(jobject obj,jclass classz,jmethodID methodID,Args&&... args) {
			return __xjni_call_method<T>::call(_env,obj,classz,methodID,std::forward<Args>(args)...);
		}

		template <typename T>
		std::vector<T> JavaVectorToCppVector(jobject javaVector) {
			std::vector<T> result;
			jclass vectorClass = _env->GetObjectClass(javaVector);
			jmethodID sizeMethod = _env->GetMethodID(vectorClass, "size", "()I");
			jmethodID getMethod = _env->GetMethodID(vectorClass,"get","(I)Ljava/lang/Object;");
			jclass typeClass = __xjni_java_box<T>::findClass(_env);
			jmethodID valueMethod = __xjni_java_box<T>::valueMethod(_env,typeClass);
			jint size = _env->CallIntMethod(javaVector,sizeMethod);
			result.reserve(static_cast<size_t>(size));
			for (jint i = 0; i < size; ++i) {
				jobject object = _env->CallObjectMethod(javaVector,getMethod,i);
				if (object == nullptr) {
					_env->DeleteLocalRef(typeClass);
					_env->DeleteLocalRef(vectorClass);
					throw std::runtime_error("Null element in Java Vector");
				}
				T value = CallMethod<T>(object,valueMethod);
				result.push_back(value);
				_env->DeleteLocalRef(object);
			}
			_env->DeleteLocalRef(typeClass);
			_env->DeleteLocalRef(vectorClass);
			return result;
		}

		template <typename T>
		jobject CppVectorToJavaVector(const std::vector<T>& values) {
			jclass vectorClass = _env->FindClass("java/util/Vector");
			jmethodID constructor = _env->GetMethodID(vectorClass,"<init>","()V");
			jmethodID addMethod = _env->GetMethodID(vectorClass,"add","(Ljava/lang/Object;)Z");
			jobject vector = _env->NewObject(vectorClass,constructor);
			jclass typeClass = __xjni_java_box<T>::findClass(_env);
			jmethodID valueConstructor = __xjni_java_box<T>::constructorMethod(_env,typeClass);
			for (const T& value : values) {
				jobject object = _env->NewObject(typeClass,valueConstructor,value);
				_env->CallBooleanMethod(vector,addMethod,object);
				_env->DeleteLocalRef(object);
			}
			_env->DeleteLocalRef(typeClass);
			_env->DeleteLocalRef(vectorClass);
			return vector;
		}

		template <typename T,typename J>
		std::vector<T> ToCppVector(const std::vector<J>& jVector) {
			std::vector<T> result;
			result.reserve(jVector.size());
			for (const J& value : jVector) {
				result.push_back(static_cast<T>(value));
			}
			return result;
		}

		template <typename J,typename T>
		std::vector<J> ToJVector(const std::vector<T>& CppVector) {
			std::vector<J> result;
			result.reserve(CppVector.size());
			for (const T& value : CppVector) {
				result.push_back(static_cast<T>(value));
			}
			return result;
		}

		FILETIME ToCppFILETIME(jobject obj) {
			jclass clazz = _env->GetObjectClass(obj);
			jfieldID low = _env->GetFieldID(clazz, "dwLowDateTime", "I");
			jfieldID high = _env->GetFieldID(clazz, "dwHighDateTime", "I");
			FILETIME ft;
			ft.dwLowDateTime = static_cast<DWORD>(_env->GetIntField(obj, low));
			ft.dwHighDateTime = static_cast<DWORD>(_env->GetIntField(obj, high));
			_env->DeleteLocalRef(clazz);
			return ft;
		}

		jobject ToJavaFILETIME(const FILETIME& ft) {
			jclass clazz = _env->FindClass("xjava/nio/file/attribute/FILETIME");
			if (clazz == nullptr) return nullptr;
			jmethodID constructor = _env->GetMethodID(clazz,"<init>","(II)V");
			if (constructor == nullptr) {
				_env->DeleteLocalRef(clazz);
				return nullptr;
			}
			jobject object = _env->NewObject(clazz,constructor,static_cast<jint>(ft.dwLowDateTime),static_cast<jint>(ft.dwHighDateTime));
			_env->DeleteLocalRef(clazz);
			if (object == nullptr) {
				return nullptr;
			}
			return object;
		}

		template <typename K, typename V>
		std::unordered_map<K, V> UnorderedMapFromJava(jobject object) {
			return __xjni_std_unordered_map_converter<K, V>::fromJava(_env,object);
		}

		template <typename K, typename V>
		jobject UnorderedMapToJava(const std::unordered_map<K, V>& value) {
			return __xjni_std_unordered_map_converter<K, V>::toJava(_env,value);
		}

		template <typename K, typename V>
		std::map<K, V> JavaMapToCpp(jobject object,std::map<K, V>*) {
			return __xjni_std_map_converter<K, V>::fromJava(_env,object);
		}

		template <typename K, typename V>
		jobject CppMapToJava(const std::map<K, V>& value) {
			return __xjni_std_map_converter<K, V>::toJava(_env,value);
		}

		template <typename T>
		std::list<T> JavaListToCppList(jobject object) {
			std::list<T> result;
			if (!object) return result;
			jclass listClass = _env->GetObjectClass(object);
			jmethodID sizeMethod =
			_env->GetMethodID(listClass,"size","()I");
			jmethodID getMethod =
			_env->GetMethodID(listClass,"get","(I)Ljava/lang/Object;");
			if (!sizeMethod || !getMethod) {
				_env->DeleteLocalRef(listClass);
				return result;
			}
			jint size = _env->CallIntMethod(object,sizeMethod);
			jint i;
			for (i = 0; i < size; ++i) {
				jobject item =
				_env->CallObjectMethod(object,getMethod,i);
				T value = __JavaToCppMap<T>(_env,item);
				result.push_back(std::move(value));
				if (item) _env->DeleteLocalRef(item);
			}
			_env->DeleteLocalRef(listClass);
			return result;
		}

		template <typename T>
		jobject CppToJava(JNIEnv* env,const std::list<T>& value) {
			jclass listClass = _env->FindClass("java/util/ArrayList");
			if (!listClass) return NULL;
			jmethodID constructor = env->GetMethodID(listClass,"<init>","()V");
			jobject result = env->NewObject(listClass,constructor);
			jmethodID addMethod = env->GetMethodID(listClass,"add","(Ljava/lang/Object;)Z");
			typename std::list<T>::const_iterator it;
			for (it = value.begin(); it != value.end(); ++it) {
				jobject item = __CppToJavaMap<T>(env,*it);
				env->CallBooleanMethod(result,addMethod,item);
				if (item) env->DeleteLocalRef(item);
			}
			env->DeleteLocalRef(listClass);
			return result;
		}
	private:
		JNIEnv *_env = nullptr;
	};
}

#endif /* __JNI_HPP__ */
