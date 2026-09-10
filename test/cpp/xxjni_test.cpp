/*
 * xxjni_test.cpp
 *
 * Comprehensive integration test for xxjni.hh.
 *
 * Build against the XJNI implementation and run from Java:
 *	XXJNITest.runAll()
 *
 * The test intentionally keeps JNI objects local to the JNI call and
 * releases/clears everything it allocates.
 */

#include "jni_test.h"

#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>

namespace {
static bool clearJavaException(JNIEnv* env, std::string* text = nullptr) {
	if (!env->ExceptionCheck())
		return false;

	jthrowable ex = env->ExceptionOccurred();
	env->ExceptionClear();

	if (text) {
		jclass throwable = env->FindClass("java/lang/Throwable");
		if (throwable && ex) {
			jmethodID mid = env->GetMethodID(
				throwable, "toString", "()Ljava/lang/String;");
			if (mid) {
				jstring s = static_cast<jstring>(env->CallObjectMethod(ex, mid));
				if (!env->ExceptionCheck() && s) {
					const char* p = env->GetStringUTFChars(s, nullptr);
					if (p) {
						*text = p;
						env->ReleaseStringUTFChars(s, p);
					}
					env->DeleteLocalRef(s);
				} else { env->ExceptionClear(); }
			}
			env->DeleteLocalRef(throwable);
		}
	}
	if (ex) env->DeleteLocalRef(ex);
	return true;
}


static bool expectNoJavaException(JNIEnv* env, TestState& t, const char* name) {
	std::string e;
	if (!clearJavaException(env, &e))
		return true;
	t.check(false,name, e);
	return false;
}

/* ---------- basic/version/string tests ---------- */

static void testVersionAndStrings(JNIEnv* env, TestState& t) {
	xxjni::xxjni j(env);

	jstring js = env->NewStringUTF("Hello XJNI");
	t.check(js != nullptr, "xjni_tojstring");
	if (!js) return;

	t.check(j.GetStringUTF(js) == "Hello XJNI", "GetString");

	const char* utf = env->GetStringUTFChars(js,nullptr);
	t.check(utf != nullptr && std::string(utf) == "Hello XJNI", "xjni_tochar");
	env->ReleaseStringUTFChars(js,utf);

	jchar unicode[] = { 'A', 0x03A9, 0x4E2D, 0x6587 };
	jstring ju = env->NewStringUTF("AΩ中文");
	t.check(ju != nullptr, "xjni_tojstring(jchar)");
	if (ju) {
		const jchar* back = env->GetStringChars(ju,nullptr);
		t.check(back != nullptr, "xjni_tojchar");
		if (back) {
			t.check(back[0] == 'A' && back[1] == 0x03A9 &&
					back[2] == 0x4E2D && back[3] == 0x6587,
					"jchar round-trip");
			env->ReleaseStringChars(ju,back);
		}
		env->DeleteLocalRef(ju);
	}

	env->DeleteLocalRef(js);

	const jchar src[] = {'a', 'b', 'c', 'd', 'e', 'f', 0};
	jchar dst[16] = {};
	const jchar needle[] = {
		static_cast<jchar>('c'),
		static_cast<jchar>('d'),
		static_cast<jchar>('e'),
		0
	};
	const jchar spn_s[] = {
		static_cast<jchar>('1'),
		static_cast<jchar>('2'),
		static_cast<jchar>('3'),
		static_cast<jchar>('4'),
		static_cast<jchar>('5'),
		static_cast<jchar>('a'),
		static_cast<jchar>('b'),
		static_cast<jchar>('c'),
		0
	};
	const jchar spn_accept[] = {
		static_cast<jchar>('1'),
		static_cast<jchar>('2'),
		static_cast<jchar>('3'),
		static_cast<jchar>('4'),
		static_cast<jchar>('5'),
		static_cast<jchar>('6'),
		static_cast<jchar>('7'),
		static_cast<jchar>('8'),
		static_cast<jchar>('9'),
		0
	};
	const jchar cspn_s[] = {
		static_cast<jchar>('a'),
		static_cast<jchar>('b'),
		static_cast<jchar>('c'),
		static_cast<jchar>('1'),
		static_cast<jchar>('2'),
		static_cast<jchar>('3'),
		0
	};
	const jchar cspn_reject[] = {
		static_cast<jchar>('0'),
		static_cast<jchar>('1'),
		static_cast<jchar>('2'),
		static_cast<jchar>('3'),
		static_cast<jchar>('4'),
		static_cast<jchar>('5'),
		static_cast<jchar>('6'),
		static_cast<jchar>('7'),
		static_cast<jchar>('8'),
		static_cast<jchar>('9'),
		0
	};
	
	xxjni::jstrcpy(dst, src);
	t.check(xxjni::jstrlen(src) == 6, "jstrlen");
	t.check(xxjni::jstrcmp(dst, src) == 0, "jstrcmp");
	t.check(xxjni::jstrstr(dst, needle) != nullptr, "jstrstr");
	t.check(xxjni::jstrchr(dst, 'd') != nullptr, "jstrchr");
	t.check(xxjni::jstrrchr(dst, 'f') != nullptr, "jstrrchr");
	t.check(xxjni::jstrspn(spn_s, spn_accept) == 5,"jstrspn");
	t.check(xxjni::jstrcspn(cspn_s, cspn_reject) == 3, "jstrcspn");
}

/* ---------- StringBuffer ---------- */

static void testStringBuffer(JNIEnv* env, TestState& t) {
	jni::XJNIEnv j(env);
	xxjni::xxjni_stringbuffer b(env, "hello");
	t.check(b.length() == 5, "StringBuffer.length");
	t.check(b.capacity() >= 5, "StringBuffer.capacity");

	b.append(" world");
	b.append((jint)42);
	b.append((jlong)1000000000LL);
	b.append((jboolean)JNI_TRUE);
	b.append((jchar)'!');
	b.appendCodePoint(0x03A9);

	char* s = b.toStringUTF();
	t.check(s != nullptr, "StringBuffer.toStringUTF");
	if (s) {
		std::string x(s);
		t.check(x.find("hello world") == 0, "StringBuffer.append string");
		t.check(x.find("42") != std::string::npos, "StringBuffer.append int");
		std::free(s);
	}

	b.insert(0, "X");
	t.check(b.charAt(0) == 'X', "StringBuffer.insert/charAt");
	b.setCharAt(0, 'H');
	t.check(b.charAt(0) == 'H', "StringBuffer.setCharAt");

	t.check(b.indexOf("world") >= 0, "StringBuffer.indexOf");
	t.check(b.lastIndexOf("world") >= 0, "StringBuffer.lastIndexOf");

	b.replace(0, 1, "h");
	t.check(b.charAt(0) == 'h', "StringBuffer.replace");

	jstring sub = b.substring(1, 6);
	{
		std::string got = j.GetStringUTF(sub);
		t.check(got == "hello", "StringBuffer.substring", got);
	}
	if (sub) env->DeleteLocalRef(sub);

	b.reverse();
	t.check(b.length() > 0, "StringBuffer.reverse");
}

/* ---------- StringBuilder ---------- */

static void testStringBuilder(JNIEnv* env, TestState& t) {
	xxjni::xxjni_stringbuilder b(env, "abc");
	b.append("def");
	b.append((jint)123);
	b.insert(0, "X");

	char* s = b.toStringUTF();
	t.check(s != nullptr, "StringBuilder.toStringUTF");
	if (s) {
		std::string x(s);
		t.check(x.find("Xabcdef") == 0, "StringBuilder.append/insert");
		t.check(x.find("123") != std::string::npos, "StringBuilder.append int");
		std::free(s);
	}

	t.check(b.indexOf("def") >= 0, "StringBuilder.indexOf");
	b.replace(0, 1, "Y");
	b.deleteCharAt(0);
	b.reverse();
	t.check(b.toStringUTF() != nullptr, "StringBuilder.mutation");
}

/* ---------- StringWriter / StringReader ---------- */

static void testWriterReader(JNIEnv* env, TestState& t) {
	xxjni::xxjni_stringwriter w(env);
	w.write((jint)'1');
	w.append(' ');
	jstring abc = env->NewStringUTF("abc");
	w.write(abc);
	if (abc) env->DeleteLocalRef(abc);

	char* ws = w.toStringUTF();
	t.check(ws != nullptr, "StringWriter.toStringUTF");
	if (ws) {
		t.check(std::string(ws) == "1 abc", "StringWriter.write/append", ws);
		std::free(ws);
	}

	xxjni::xxjni_stringreader r(env, "abcdef");
	t.check(r.ready() == JNI_TRUE, "StringReader.ready");
	t.check(r.markSupported() == JNI_TRUE, "StringReader.markSupported");
	t.check(r.read() == 'a', "StringReader.read");

	r.mark(32);
	t.check(r.read() == 'b', "StringReader.read second");
	t.check(r.skip(2) == 2, "StringReader.skip");
	t.check(r.read() == 'e', "StringReader.read after skip");
	r.reset();
	t.check(r.read() == 'b', "StringReader.reset");

	r.close();
	(void)r.read();
	t.check(env->ExceptionCheck(), "StringReader.closed behavior");
	clearJavaException(env);
}

/* ---------- String arrays ---------- */

static void testStringArrays(JNIEnv* env, TestState& t) {
	const char* values[] = {"one", "two", "three"};
	xxjni::xxjni_stringarray a(env);

	jobjectArray arr = a.newStringUTFArray(values, 3);
	t.check(arr != nullptr, "NewStringUTFArray");
	if (!arr) return;

	t.check(a.getStringUTFArrayLength(arr) == 3, "GetStringUTFArrayLength");

	char* region[2] = {};
	a.getRegion(arr, 1, 2, region);
	t.check(region[0] && region[1] &&
			std::strcmp(region[0], "two") == 0 &&
			std::strcmp(region[1], "three") == 0,
			"GetStringUTFArrayRegion");
	for (auto* p : region) std::free(p);

	const char* replacement[] = {"TWO"};
	a.setRegion(arr, 1, 1, replacement);

	char** all = const_cast<char**>(a.getStringUTFArrayChars(arr, nullptr));
	t.check(all != nullptr, "GetStringUTFArrayChars");
	if (all) {
		t.check(std::strcmp(all[1], "TWO") == 0, "SetStringUTFArrayRegion");
		a.release(arr, const_cast<const char**>(all), JNI_ABORT);
	}

	env->DeleteLocalRef(arr);
}

/* ---------- primitive 2D array ---------- */

static void test2D(JNIEnv* env, TestState& t) {
	xxjni::xxjni2d d(env);

	jobjectArray arr = d.newIntArray(2, 3);
	t.check(arr != nullptr, "NewInt2DArray");
	if (!arr) return;

	jint r0[] = {1, 2, 3};
	jint r1[] = {4, 5, 6};
	const jint* rows[] = {r0, r1};
	d.setRegion(arr, 0, 2, rows);

	jint* out0 = static_cast<jint*>(std::calloc(3, sizeof(jint)));
	jint* out1 = static_cast<jint*>(std::calloc(3, sizeof(jint)));
	jint* outRows[] = {out0, out1};
	d.getRegion(arr, 0, 2, outRows);

	bool good = outRows[0] && outRows[1] &&
				outRows[0][0] == 1 && outRows[0][2] == 3 &&
				outRows[1][0] == 4 && outRows[1][2] == 6;
	t.check(good, "GetInt2DArrayRegion");

	if (outRows[0]) std::free(outRows[0]);
	if (outRows[1]) std::free(outRows[1]);
	env->DeleteLocalRef(arr);

	jobjectArray ba = d.newByteArray(2, 2);
	t.check(ba != nullptr, "NewByte2DArray");
	if (ba) {
		jbyte x0[] = {7, 8};
		jbyte x1[] = {9, 10};
		const jbyte* rowsb[] = {x0, x1};
		d.setRegion(ba, 0, 2, rowsb);
		jbyte* got0 = static_cast<jbyte*>(std::calloc(2, sizeof(jbyte)));
		jbyte* got1 = static_cast<jbyte*>(std::calloc(2, sizeof(jbyte)));
		jbyte* got[] = {got0, got1};
		d.getRegion(ba, 0, 2, got);
		t.check(got[0] && got[1] && got[0][1] == 8 && got[1][0] == 9,
				"Byte2D region");
		if (got[0]) std::free(got[0]);
		if (got[1]) std::free(got[1]);
		env->DeleteLocalRef(ba);
	}

	jobjectArray sa = d.newStringUTFArray(nullptr, 2, 2);
	t.check(sa != nullptr, "NewStringUTF2DArray");
	if (sa) env->DeleteLocalRef(sa);
}

/* ---------- object/primitive fields ---------- */

static void testArrayFields(JNIEnv* env, TestState& t) {
	jclass cls = env->FindClass("XXJNITestObject");
	if (!cls) {
		clearJavaException(env);
		t.check(false,"FindClass(XXJNITestObject)");
		return;
	}

	jmethodID ctor = env->GetMethodID(cls, "<init>", "()V");
	jobject obj = ctor ? env->NewObject(cls, ctor) : nullptr;
	if (!obj) {
		clearJavaException(env);
		t.check(false,"Create XXJNITestObject");
		env->DeleteLocalRef(cls);
		return;
	}

	xxjni::xxjni_arrayfield af(env);

	jfieldID ints = env->GetFieldID(cls, "ints", "[I");
	jfieldID longs = env->GetFieldID(cls, "longs", "[J");
	jfieldID doubles = env->GetFieldID(cls, "doubles", "[D");
	jfieldID bools = env->GetFieldID(cls, "bools", "[Z");
	jfieldID strings = env->GetFieldID(cls, "strings", "[Ljava/lang/String;");

	t.check(ints && longs && doubles && bools && strings, "array field IDs");

	if (ints) {
		jintArray a = env->NewIntArray(3);
		jint x[] = {10, 20, 30};
		env->SetIntArrayRegion(a, 0, 3, x);
		af.SetIntArrayField(obj, ints, a);
		jintArray got = af.GetIntArrayField(obj, ints);
		jint y[3] = {};
		env->GetIntArrayRegion(got, 0, 3, y);
		t.check(y[0] == 10 && y[1] == 20 && y[2] == 30,
				"Get/SetIntArrayField");
		env->DeleteLocalRef(a);
		env->DeleteLocalRef(got);
	}

	if (longs) {
		jlongArray a = env->NewLongArray(2);
		jlong x[] = {111, 222};
		env->SetLongArrayRegion(a, 0, 2, x);
		af.SetLongArrayField(obj, longs, a);
		jlongArray got = af.GetLongArrayField(obj, longs);
		jlong y[2] = {};
		env->GetLongArrayRegion(got, 0, 2, y);
		t.check(y[0] == 111 && y[1] == 222, "Get/SetLongArrayField");
		env->DeleteLocalRef(a);
		env->DeleteLocalRef(got);
	}

	env->DeleteLocalRef(obj);
	env->DeleteLocalRef(cls);
}

/* ---------- class cache ---------- */

static void testClassCache(JNIEnv* env, TestState& t) {
	xxjni::xxjni_classcache cache(env);

	t.check(cache.init() != JNI_FALSE, "classcache.init");

	jclass local = env->FindClass("java/lang/String");
	t.check(local != nullptr, "classcache.source class");
	jclass inserted = local ? cache.insert("java/lang/String", local) : nullptr;
	t.check(inserted != nullptr, "classcache.insert");
	if (local) env->DeleteLocalRef(local);

	jclass a = cache.get("java/lang/String");
	t.check(a != nullptr, "classcache.get");
	/* cache.get()/insert() return cached global references; the cache owns them. */

	cache.clear();
}

/* ---------- exception helpers ---------- */

static void testExceptions(JNIEnv* env, TestState& t) {
	xxjni::xxjni x(env);

	jclass ex = env->FindClass("java/lang/IllegalArgumentException");
	t.check(ex != nullptr, "FindClass exception");
	if (!ex) return;

	jint rc = x.ThrowNewF(ex, "xxjni test exception %d", 123);
	t.check(rc == 0 && env->ExceptionCheck(), "ThrowNewF");

	std::string text;
	bool had = clearJavaException(env, &text);
	t.check(had && text.find("xxjni test exception") != std::string::npos,
			"exception capture");

	env->DeleteLocalRef(ex);
}

/* ---------- thread / mutex / once / clock ---------- */

static std::atomic<int> g_threadValue{0};
static std::atomic<int> g_onceValue{0};

static void* threadRoutine(void* p) {
	auto* v = static_cast<int*>(p);
	g_threadValue.fetch_add(*v, std::memory_order_relaxed);
	return p;
}

static void onceRoutine() {
	g_onceValue.fetch_add(1, std::memory_order_relaxed);
}

static void testThreading(TestState& t) {
	g_threadValue = 0;

	int value = 7;
	xxjni::xxjni_thread th;
	t.check(!th.running() && !th.joinable(), "thread initial state");

	auto rc = th.start(threadRoutine, &value);
	t.check(rc == 0, "thread.start");
	t.check(th.running(), "thread.running");

	void* result = nullptr;
	rc = th.join(&result);
	t.check(rc == 0 && result == &value, "thread.join");
	t.check(g_threadValue.load() == 7, "thread routine");

	xxjni::xxjni_mutex mutex;
	t.check(mutex.initialized(), "mutex initialized");
	t.check(mutex.lock() == 0, "mutex.lock");
	t.check(mutex.unlock() == 0, "mutex.unlock");

	{
		xxjni::xxjni_lock_guard guard(mutex);
		t.check(guard.locked(), "lock_guard");
	}

	g_onceValue = 0;
	xxjni::xxjni_once once;
	t.check(once.call(onceRoutine) == 0, "once.call first");
	t.check(once.call(onceRoutine) == 0, "once.call second");
	t.check(g_onceValue.load() == 1, "once executes once");

	timespec ts{};
	xxjni::xxjni_clock::realtime(&ts);
	t.check(ts.tv_sec > 0, "clock.realtime");
}

/* ---------- native new helpers ---------- */

static void testNewHelpers(JNIEnv* env, TestState& t) {
	xxjni::xxjni_new n(env);

	jbyte bytes[] = {1, 2, 3};
	jint ints[] = {4, 5, 6};
	jlong longs[] = {7, 8, 9};
	jfloat floats[] = {1.0f, 2.0f};
	jdouble doubles[] = {3.0, 4.0};

	jbyteArray ba = n.NewByteArray(bytes, 3);
	jintArray ia = n.NewIntArray(ints, 3);
	jlongArray la = n.NewLongArray(longs, 3);
	jfloatArray fa = n.NewFloatArray(floats, 2);
	jdoubleArray da = n.NewDoubleArray(doubles, 2);

	t.check(ba && ia && la && fa && da, "xxjni_new primitive arrays");

	if (ba) env->DeleteLocalRef(ba);
	if (ia) env->DeleteLocalRef(ia);
	if (la) env->DeleteLocalRef(la);
	if (fa) env->DeleteLocalRef(fa);
	if (da) env->DeleteLocalRef(da);
}

/* ---------- run ---------- */

static std::string runAll(JNIEnv* env) {
	TestState t;

	testVersionAndStrings(env, t);
	expectNoJavaException(env, t, "after strings");

	testNewHelpers(env, t);
	expectNoJavaException(env, t, "after new helpers");

	testStringBuffer(env, t);
	expectNoJavaException(env, t, "after StringBuffer");

	testStringBuilder(env, t);
	expectNoJavaException(env, t, "after StringBuilder");

	testWriterReader(env, t);
	expectNoJavaException(env, t, "after Writer/Reader");

	testStringArrays(env, t);
	expectNoJavaException(env, t, "after String arrays");

	test2D(env, t);
	expectNoJavaException(env, t, "after 2D arrays");

	testArrayFields(env, t);
	expectNoJavaException(env, t, "after array fields");

	testClassCache(env, t);
	expectNoJavaException(env, t, "after class cache");

	testExceptions(env, t);
	expectNoJavaException(env, t, "after exceptions");

	testThreading(t);

	std::ostringstream out;
	out << "XXJNI TEST RESULT\n"
		<< "version: " << xxjni::xxjni_version() << "\n"
		<< "passed: " << t.passed << "\n"
		<< "failed: " << t.failed << "\n\n"
		<< t.log;

	if (t.failed == 0)
		out << "\nALL TESTS PASSED\n";
	else
		out << "\nTESTS FAILED\n";

	return out.str();
}

} // namespace

extern "C"
JNIEXPORT jstring JNICALL
Java_XXJNITest_runAll(JNIEnv* env, jclass) {
	std::string result;
	try {
		result = runAll(env);
	} catch (const std::exception& e) {
		result = "XXJNI TEST CRASHED: ";
		result += e.what();
		result += '\n';
	} catch (...) {
		result = "XXJNI TEST CRASHED: unknown exception\n";
	}

	return env->NewStringUTF(result.c_str());
}
