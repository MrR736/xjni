#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "jni_test.h"

namespace {
static bool clearException(JNIEnv *e, std::string *out = nullptr) {
  if (!e->ExceptionCheck())
    return false;
  jthrowable x = e->ExceptionOccurred();
  e->ExceptionClear();
  if (out && x) {
    jclass c = e->GetObjectClass(x);
    jmethodID m =
        c ? e->GetMethodID(c, "toString", "()Ljava/lang/String;") : nullptr;
    if (m) {
      jstring s = static_cast<jstring>(e->CallObjectMethod(x, m));
      if (!e->ExceptionCheck() && s) {
        const char *p = e->GetStringUTFChars(s, 0);
        if (p) {
          *out = p;
          e->ReleaseStringUTFChars(s, p);
        }
        e->DeleteLocalRef(s);
      } else
        e->ExceptionClear();
    }
    if (c)
      e->DeleteLocalRef(c);
    e->DeleteLocalRef(x);
  }
  return true;
}

static void noex(JNIEnv *e, TestState &t, const char *n) {
  std::string x;
  if (clearException(e, &x))
    t.check(false, n, x);
}

static void testJavaVM(JNIEnv *env, TestState &t) {
  JavaVM *raw = nullptr;
  t.check(env->GetJavaVM(&raw) == JNI_OK && raw != nullptr,
          "JNIEnv::GetJavaVM");
  jni::XJavaVM vm(raw);
  t.check(static_cast<bool>(vm) && vm.get() == raw, "XJavaVM basics");
  JNIEnv *current = nullptr;
  t.check(vm.GetEnv(&current, JNI_VERSION_1_8) == JNI_OK && current == env,
          "XJavaVM::GetEnv");
  t.check(vm.SupportsVersion(JNI_VERSION_1_8), "XJavaVM::SupportsVersion");
}

static void testBasics(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  t.check(static_cast<bool>(x) && x.get() == env, "XJNIEnv basics");
  t.check(x.GetVersion() == env->GetVersion(), "GetVersion");
  jclass obj = x.FindClass("java/lang/Object");
  t.check(obj != nullptr, "FindClass");
  t.check(x.GetSuperclass(obj) == nullptr, "GetSuperclass(Object)");
  t.check(x.IsAssignableFrom(obj, obj) == JNI_TRUE, "IsAssignableFrom");
  t.check(x.EnsureLocalCapacity(32) == JNI_OK, "EnsureLocalCapacity");
  t.check(x.PushLocalFrame(8) == JNI_OK, "PushLocalFrame");
  jobject local = x.NewLocalRef(obj);
  t.check(local != nullptr, "NewLocalRef");
  jobject global = x.NewGlobalRef(obj);
  t.check(global != nullptr && x.IsSameObject(global, obj),
          "GlobalRef/IsSameObject");
  t.check(x.GetObjectRefType(global) == JNIGlobalRefType,
          "GetObjectRefType(global)");
  x.DeleteGlobalRef(global);
  if (local)
    x.DeleteLocalRef(local);
  jobject result = x.PopLocalFrame(nullptr);
  t.check(result == nullptr, "PopLocalFrame");
  if (obj)
    x.DeleteLocalRef(obj);
}

static void testStrings(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jstring s = x.NewStringUTF("Hello UTF");
  t.check(s && x.GetStringUTFLength(s) == 9, "NewStringUTF/GetStringUTFLength");
  t.check(x.GetStringUTF(s) == "Hello UTF", "GetStringUTF(std::string)");
  jni::jstring u;
  const jchar a[] = {'A', 0x03a9, 0x4e2d, 0x6587};
  jstring js = x.NewString(a, 4);
  x.GetStringRegion(js, 0, 4, u);
  t.check(u.size() == 4 && u[1] == 0x03a9 && u[3] == 0x6587,
          "jstring/GetStringRegion");
  std::string ur;
  x.GetStringUTFRegion(s, 0, 4, ur);
  t.check(ur == "Hell", "GetStringUTFRegion(std::string)");
  jni::jstring src;
  src.push_back('x');
  src.push_back(0x03a9);
  src.push_back('y');
  jstring made = x.NewString(src);
  t.check(made && x.GetStringLength(src) == 3, "jstring/NewString");
  if (js)
    x.DeleteLocalRef(js);
  if (made)
    x.DeleteLocalRef(made);
  if (s)
    x.DeleteLocalRef(s);
}

static jclass testClass(JNIEnv *env) { return env->FindClass("JNITestObject"); }
static void testConstructionAndMethods(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass c = testClass(env);
  jmethodID ctor = x.GetMethodID(c, "<init>", "(I)V");
  jobject o = x.NewObject(c, ctor, 77);
  t.check(o != nullptr, "NewObject variadic");
  jfieldID intF = x.GetFieldID(c, "intField", "I");
  t.check(x.GetIntField(o, intF) == 77, "GetIntField");
  x.SetIntField(o, intF, 88);
  t.check(x.GetIntField(o, intF) == 88, "SetIntField");
  jmethodID add = x.GetMethodID(c, "add", "(II)I");
  t.check(x.CallIntMethod(o, add, 3, 4) == 7, "CallIntMethod");
  jvalue av[2];
  av[0].i = 5;
  av[1].i = 6;
  t.check(x.CallIntMethodA(o, add, av) == 11, "CallIntMethodA");
  jmethodID concat = x.GetMethodID(
      c, "concat", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
  jstring a = x.NewStringUTF("A"), b = x.NewStringUTF("B");
  jstring ab = static_cast<jstring>(x.CallObjectMethod(o, concat, a, b));
  t.check(x.GetStringUTF(ab) == "AB", "CallObjectMethod");
  jmethodID self = x.GetMethodID(c, "self", "()Ljava/lang/Object;");
  jobject so = x.CallObjectMethod(o, self);
  t.check(x.IsSameObject(o, so), "CallObjectMethod object");
  jmethodID set = x.GetMethodID(c, "setInt", "(I)V");
  x.CallVoidMethod(o, set, 123);
  t.check(x.GetIntField(o, intF) == 123, "CallVoidMethod");
  // generic dispatcher
  t.check(x.CallMethod<jint>(o, add, 9, 1) == 10, "CallMethod<T>");
  if (ab)
    x.DeleteLocalRef(ab);
  if (a)
    x.DeleteLocalRef(a);
  if (b)
    x.DeleteLocalRef(b);
  if (so)
    x.DeleteLocalRef(so);
  if (o)
    x.DeleteLocalRef(o);
  if (c)
    x.DeleteLocalRef(c);
}

static void testAllPrimitiveMethods(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass c = testClass(env);
  jmethodID ctor = x.GetMethodID(c, "<init>", "()V");
  jobject o = x.NewObject(c, ctor);
  t.check(x.CallLongMethod(o, x.GetMethodID(c, "addLong", "(JJ)J"), 2LL, 3LL) ==
              5,
          "CallLongMethod");
  t.check(std::fabs(x.CallFloatMethod(o, x.GetMethodID(c, "addFloat", "(FF)F"),
                                      1.25f, 2.5f) -
                    3.75f) < 1e-6,
          "CallFloatMethod");
  t.check(std::fabs(x.CallDoubleMethod(
                        o, x.GetMethodID(c, "addDouble", "(DD)D"), 1.25, 2.5) -
                    3.75) < 1e-12,
          "CallDoubleMethod");
  t.check(x.CallByteMethod(o, x.GetMethodID(c, "addByte", "(BB)B"), (jbyte)2,
                           (jbyte)3) == 5,
          "CallByteMethod");
  t.check(x.CallShortMethod(o, x.GetMethodID(c, "addShort", "(SS)S"), (jshort)2,
                            (jshort)3) == 5,
          "CallShortMethod");
  t.check(x.CallCharMethod(o, x.GetMethodID(c, "nextChar", "(C)C"),
                           (jchar)'A') == 'B',
          "CallCharMethod");
  t.check(x.CallBooleanMethod(o, x.GetMethodID(c, "and", "(ZZ)Z"), JNI_TRUE,
                              JNI_FALSE) == JNI_FALSE,
          "CallBooleanMethod");
  if (o)
    x.DeleteLocalRef(o);
  if (c)
    x.DeleteLocalRef(c);
}

static void testStatic(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass c = testClass(env);
  t.check(x.CallStaticIntMethod(c, x.GetStaticMethodID(c, "staticAdd", "(II)I"),
                                2, 3) == 5,
          "CallStaticIntMethod");
  t.check(x.CallStaticLongMethod(
              c, x.GetStaticMethodID(c, "staticLong", "(JJ)J"), 2LL, 3LL) == 5,
          "CallStaticLongMethod");
  t.check(std::fabs(
              x.CallStaticFloatMethod(
                  c, x.GetStaticMethodID(c, "staticFloat", "(FF)F"), 1.f, 2.f) -
              3.f) < 1e-6,
          "CallStaticFloatMethod");
  t.check(std::fabs(
              x.CallStaticDoubleMethod(
                  c, x.GetStaticMethodID(c, "staticDouble", "(DD)D"), 1., 2.) -
              3.) < 1e-12,
          "CallStaticDoubleMethod");
  t.check(x.CallStaticByteMethod(c,
                                 x.GetStaticMethodID(c, "staticByte", "(BB)B"),
                                 (jbyte)2, (jbyte)3) == 5,
          "CallStaticByteMethod");
  t.check(
      x.CallStaticShortMethod(c, x.GetStaticMethodID(c, "staticShort", "(SS)S"),
                              (jshort)2, (jshort)3) == 5,
      "CallStaticShortMethod");
  t.check(x.CallStaticCharMethod(c,
                                 x.GetStaticMethodID(c, "staticChar", "(C)C"),
                                 (jchar)'A') == 'C',
          "CallStaticCharMethod");
  t.check(
      x.CallStaticBooleanMethod(c, x.GetStaticMethodID(c, "staticAnd", "(ZZ)Z"),
                                JNI_TRUE, JNI_TRUE) == JNI_TRUE,
      "CallStaticBooleanMethod");
  jstring a = x.NewStringUTF("X"), b = x.NewStringUTF("Y");
  jstring r = static_cast<jstring>(x.CallStaticObjectMethod(
      c,
      x.GetStaticMethodID(
          c, "staticConcat",
          "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"),
      a, b));
  t.check(x.GetStringUTF(r) == "XY", "CallStaticObjectMethod");
  jobject o = x.NewObject(c, x.GetMethodID(c, "<init>", "()V"));
  x.CallStaticVoidMethod(
      c, x.GetStaticMethodID(c, "staticSetInt", "(LJNITestObject;I)V"), o, 321);
  t.check(x.GetIntField(o, x.GetFieldID(c, "intField", "I")) == 321,
          "CallStaticVoidMethod");
  t.check(x.CallStaticMethod<jint>(
              c, x.GetStaticMethodID(c, "staticAdd", "(II)I"), 7, 8) == 15,
          "CallStaticMethod<T>");
  if (r)
    x.DeleteLocalRef(r);
  if (a)
    x.DeleteLocalRef(a);
  if (b)
    x.DeleteLocalRef(b);
  if (o)
    x.DeleteLocalRef(o);
  if (c)
    x.DeleteLocalRef(c);
}

static void testNonvirtual(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass base = env->FindClass("JNITestBase");
  jclass der = env->FindClass("JNITestDerived");
  jobject o = x.NewObject(der, x.GetMethodID(der, "<init>", "()V"));
  jmethodID vm = x.GetMethodID(base, "virtualValue", "()I");
  t.check(x.CallIntMethod(o, vm) == 20, "virtual dispatch");
  t.check(x.CallNonvirtualIntMethod(o, base, vm) == 10,
          "CallNonvirtualIntMethod");
  jmethodID vs = x.GetMethodID(base, "virtualString", "()Ljava/lang/String;");
  jstring s = static_cast<jstring>(x.CallNonvirtualObjectMethod(o, base, vs));
  t.check(x.GetStringUTF(s) == "base", "CallNonvirtualObjectMethod");
  t.check(x.CallNonvirtualMethod<jint>(o, base, vm) == 10,
          "CallNonvirtualMethod<T>");
  if (s)
    x.DeleteLocalRef(s);
  if (o)
    x.DeleteLocalRef(o);
  if (base)
    x.DeleteLocalRef(base);
  if (der)
    x.DeleteLocalRef(der);
}

static void testFields(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass c = testClass(env);
  jobject o = x.NewObject(c, x.GetMethodID(c, "<init>", "()V"));
#define IFIELD(name, sig, GET, SET, val)                                       \
  do {                                                                         \
    jfieldID f = x.GetFieldID(c, name, sig);                                   \
    x.SET(o, f, val);                                                          \
    t.check(x.GET(o, f) == val, std::string("instance ") + name);              \
  } while (0)
  IFIELD("booleanField", "Z", GetBooleanField, SetBooleanField, JNI_FALSE);
  IFIELD("byteField", "B", GetByteField, SetByteField, (jbyte)9);
  IFIELD("charField", "C", GetCharField, SetCharField, (jchar)'Z');
  IFIELD("shortField", "S", GetShortField, SetShortField, (jshort)19);
  IFIELD("intField", "I", GetIntField, SetIntField, (jint)29);
  IFIELD("longField", "J", GetLongField, SetLongField, (jlong)39);
  IFIELD("floatField", "F", GetFloatField, SetFloatField, (jfloat)49.5f);
  IFIELD("doubleField", "D", GetDoubleField, SetDoubleField, (jdouble)59.5);
#undef IFIELD
  jfieldID of = x.GetFieldID(c, "objectField", "Ljava/lang/String;");
  jstring s = x.NewStringUTF("object");
  x.SetObjectField(o, of, s);
  jobject got = x.GetObjectField(o, of);
  t.check(x.GetStringUTF((jstring)got) == "object", "instance object field");
  if (got)
    x.DeleteLocalRef(got);
  if (s)
    x.DeleteLocalRef(s);
#define SFIELD(name, sig, GET, SET, val)                                       \
  do {                                                                         \
    jfieldID f = x.GetStaticFieldID(c, name, sig);                             \
    x.SET(c, f, val);                                                          \
    t.check(x.GET(c, f) == val, std::string("static ") + name);                \
  } while (0)
  SFIELD("staticBooleanField", "Z", GetStaticBooleanField,
         SetStaticBooleanField, JNI_FALSE);
  SFIELD("staticByteField", "B", GetStaticByteField, SetStaticByteField,
         (jbyte)8);
  SFIELD("staticCharField", "C", GetStaticCharField, SetStaticCharField,
         (jchar)'Q');
  SFIELD("staticShortField", "S", GetStaticShortField, SetStaticShortField,
         (jshort)18);
  SFIELD("staticIntField", "I", GetStaticIntField, SetStaticIntField, (jint)28);
  SFIELD("staticLongField", "J", GetStaticLongField, SetStaticLongField,
         (jlong)38);
  SFIELD("staticFloatField", "F", GetStaticFloatField, SetStaticFloatField,
         (jfloat)48.5f);
  SFIELD("staticDoubleField", "D", GetStaticDoubleField, SetStaticDoubleField,
         (jdouble)58.5);
#undef SFIELD
  jfieldID sof =
      x.GetStaticFieldID(c, "staticObjectField", "Ljava/lang/String;");
  jstring ss = x.NewStringUTF("so");
  x.SetStaticObjectField(c, sof, ss);
  jobject sg = x.GetStaticObjectField(c, sof);
  t.check(x.GetStringUTF((jstring)sg) == "so", "static object field");
  if (sg)
    x.DeleteLocalRef(sg);
  if (ss)
    x.DeleteLocalRef(ss);
  if (o)
    x.DeleteLocalRef(o);
  if (c)
    x.DeleteLocalRef(c);
}

static void testArrays(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jboolean bv[2] = {JNI_FALSE, JNI_TRUE};
  jbyte by[2] = {1, 2};
  jchar ch[2] = {'a', 'b'};
  jshort sh[2] = {3, 4};
  jint in[2] = {5, 6};
  jlong lo[2] = {7, 8};
  jfloat fl[2] = {1.5f, 2.5f};
  jdouble db[2] = {3.5, 4.5};
  jbooleanArray ba = x.NewBooleanArray(2);
  jbyteArray bya = x.NewByteArray(2);
  jcharArray ca = x.NewCharArray(2);
  jshortArray sa = x.NewShortArray(2);
  jintArray ia = x.NewIntArray(2);
  jlongArray la = x.NewLongArray(2);
  jfloatArray fa = x.NewFloatArray(2);
  jdoubleArray da = x.NewDoubleArray(2);
  x.SetBooleanArrayRegion(ba, 0, 2, bv);
  x.SetByteArrayRegion(bya, 0, 2, by);
  x.SetCharArrayRegion(ca, 0, 2, ch);
  x.SetShortArrayRegion(sa, 0, 2, sh);
  x.SetIntArrayRegion(ia, 0, 2, in);
  x.SetLongArrayRegion(la, 0, 2, lo);
  x.SetFloatArrayRegion(fa, 0, 2, fl);
  x.SetDoubleArrayRegion(da, 0, 2, db);
  jboolean br[2];
  jbyte byr[2];
  jchar chr[2];
  jshort shr[2];
  jint ir[2];
  jlong lr[2];
  jfloat fr[2];
  jdouble dr[2];
  x.GetBooleanArrayRegion(ba, 0, 2, br);
  x.GetByteArrayRegion(bya, 0, 2, byr);
  x.GetCharArrayRegion(ca, 0, 2, chr);
  x.GetShortArrayRegion(sa, 0, 2, shr);
  x.GetIntArrayRegion(ia, 0, 2, ir);
  x.GetLongArrayRegion(la, 0, 2, lr);
  x.GetFloatArrayRegion(fa, 0, 2, fr);
  x.GetDoubleArrayRegion(da, 0, 2, dr);
  t.check(br[1] && byr[1] == 2 && chr[1] == 'b' && shr[1] == 4 && ir[1] == 6 &&
              lr[1] == 8 && fr[1] == 2.5f && dr[1] == 4.5,
          "array regions");
  jboolean copy;
  jboolean *be = x.GetBooleanArrayElements(ba, &copy);
  jbyte *bye = x.GetByteArrayElements(bya, &copy);
  jchar *ce = x.GetCharArrayElements(ca, &copy);
  jshort *se = x.GetShortArrayElements(sa, &copy);
  jint *ie = x.GetIntArrayElements(ia, &copy);
  jlong *le = x.GetLongArrayElements(la, &copy);
  jfloat *fe = x.GetFloatArrayElements(fa, &copy);
  jdouble *de = x.GetDoubleArrayElements(da, &copy);
  t.check(be && bye && ce && se && ie && le && fe && de, "array elements");
  if (be) {
    be[0] = JNI_TRUE;
    x.ReleaseBooleanArrayElements(ba, be, 0);
  }
  if (bye)
    x.ReleaseByteArrayElements(bya, bye, JNI_ABORT);
  if (ce)
    x.ReleaseCharArrayElements(ca, ce, JNI_ABORT);
  if (se)
    x.ReleaseShortArrayElements(sa, se, JNI_ABORT);
  if (ie)
    x.ReleaseIntArrayElements(ia, ie, JNI_ABORT);
  if (le)
    x.ReleaseLongArrayElements(la, le, JNI_ABORT);
  if (fe)
    x.ReleaseFloatArrayElements(fa, fe, JNI_ABORT);
  if (de)
    x.ReleaseDoubleArrayElements(da, de, JNI_ABORT);
  t.check(x.GetArrayLength(ia) == 2, "GetArrayLength");
  jclass strc = env->FindClass("java/lang/String");
  jstring s1 = x.NewStringUTF("one"), s2 = x.NewStringUTF("two");
  jobjectArray oa = x.NewObjectArray(2, strc, s1);
  x.SetObjectArrayElement(oa, 1, s2);
  jobject e0 = x.GetObjectArrayElement(oa, 0),
          e1 = x.GetObjectArrayElement(oa, 1);
  t.check(x.GetStringUTF((jstring)e0) == "one" && x.GetStringUTF((jstring)e1) == "two",
          "object array");
  if (e0)
    x.DeleteLocalRef(e0);
  if (e1)
    x.DeleteLocalRef(e1);
  if (s1)
    x.DeleteLocalRef(s1);
  if (s2)
    x.DeleteLocalRef(s2);
  if (strc)
    x.DeleteLocalRef(strc);
  if (ba)
    x.DeleteLocalRef(ba);
  if (bya)
    x.DeleteLocalRef(bya);
  if (ca)
    x.DeleteLocalRef(ca);
  if (sa)
    x.DeleteLocalRef(sa);
  if (ia)
    x.DeleteLocalRef(ia);
  if (la)
    x.DeleteLocalRef(la);
  if (fa)
    x.DeleteLocalRef(fa);
  if (da)
    x.DeleteLocalRef(da);
  if (oa)
    x.DeleteLocalRef(oa);
}

static void testCriticalDirectRefs(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jint vals[2] = {1, 2};
  jintArray a = x.NewIntArray(2);
  x.SetIntArrayRegion(a, 0, 2, vals);
  void *p = x.GetPrimitiveArrayCritical(a, nullptr);
  t.check(p != nullptr, "GetPrimitiveArrayCritical");
  if (p) {
    static_cast<jint *>(p)[0] = 9;
    x.ReleasePrimitiveArrayCritical(a, p, 0);
  }
  jstring s = x.NewStringUTF("critical");
  const jchar *cp = x.GetStringCritical(s, nullptr);
  t.check(cp && cp[0] == 'c', "GetStringCritical");
  if (cp)
    x.ReleaseStringCritical(s, cp);
  char bytes[8] = {};
  jobject bb = x.NewDirectByteBuffer(bytes, 8);
  t.check(bb && x.GetDirectBufferAddress(bb) == bytes &&
              x.GetDirectBufferCapacity(bb) == 8,
          "DirectByteBuffer");
  if (bb)
    x.DeleteLocalRef(bb);
  if (s)
    x.DeleteLocalRef(s);
  if (a)
    x.DeleteLocalRef(a);
}

static void testReflectionModuleVirtual(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass c = testClass(env);
  jmethodID m = x.GetMethodID(c, "add", "(II)I");
  jobject reflected = x.ToReflectedMethod(c, m, JNI_FALSE);
  jmethodID back = x.FromReflectedMethod(reflected);
  t.check(reflected && back == m, "method reflection");
  if (reflected)
    x.DeleteLocalRef(reflected);
  jfieldID f = x.GetFieldID(c, "intField", "I");
  jobject rf = x.ToReflectedField(c, f, JNI_FALSE);
  jfieldID fb = x.FromReflectedField(rf);
  t.check(rf && fb == f, "field reflection");
  if (rf)
    x.DeleteLocalRef(rf);
  jobject module = x.GetModule(c);
  t.check(module != nullptr, "GetModule");
  if (module)
    x.DeleteLocalRef(module);
  jclass thread = env->FindClass("java/lang/Thread");
  jmethodID cur =
      x.GetStaticMethodID(thread, "currentThread", "()Ljava/lang/Thread;");
  jobject th = x.CallStaticObjectMethod(thread, cur);
  t.check(th && x.IsVirtualThread(th) == JNI_FALSE,
          "IsVirtualThread(platform)");
  if (th)
    x.DeleteLocalRef(th);
  if (thread)
    x.DeleteLocalRef(thread);
  if (c)
    x.DeleteLocalRef(c);
}

static void testExceptionsFramesMonitor(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass ex = env->FindClass("java/lang/IllegalArgumentException");
  t.check(x.ThrowNew(ex, "xxjni test") == JNI_OK, "ThrowNew");
  t.check(x.ExceptionCheck() == JNI_TRUE && x.ExceptionOccurred() != nullptr,
          "ExceptionCheck/Occurred");
  x.ExceptionClear();
  t.check(!x.ExceptionCheck(), "ExceptionClear");
  jclass c = testClass(env);
  jobject o = x.NewObject(c, x.GetMethodID(c, "<init>", "()V"));
  t.check(x.MonitorEnter(o) == JNI_OK, "MonitorEnter");
  t.check(x.MonitorExit(o) == JNI_OK, "MonitorExit");
  jobject g = x.NewGlobalRef(o);
  jweak w = x.NewWeakGlobalRef(o);
  t.check(g && w, "weak/global refs");
  t.check(x.GetObjectRefType(w) == JNIWeakGlobalRefType, "weak ref type");
  x.DeleteWeakGlobalRef(w);
  x.DeleteGlobalRef(g);
  if (o)
    x.DeleteLocalRef(o);
  if (c)
    x.DeleteLocalRef(c);
  if (ex)
    x.DeleteLocalRef(ex);
}

static void testContainers(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  std::vector<jint> vv;
  vv.push_back(1);
  vv.push_back(2);
  vv.push_back(3);
  jobject jv = x.CppVectorToJavaVector(vv);
  std::vector<jint> back = x.JavaVectorToCppVector<jint>(jv);
  t.check(back == vv, "vector C++<->Java");
  if (jv)
    x.DeleteLocalRef(jv);
  std::map<std::string, jint> m;
  m["a"] = 1;
  m["b"] = 2;
  jobject jm = x.CppMapToJava(m);
  std::map<std::string, jint> mb =
      x.JavaMapToCpp(jm, (std::map<std::string, jint> *)nullptr);
  t.check(mb == m, "map C++<->Java");
  if (jm)
    x.DeleteLocalRef(jm);
  std::unordered_map<std::string, jint> um;
  um["x"] = 7;
  um["y"] = 8;
  jobject jum = x.UnorderedMapToJava(um);
  std::unordered_map<std::string, jint> umb =
      x.UnorderedMapFromJava<std::string, jint>(jum);
  t.check(umb == um, "unordered_map C++<->Java");
  if (jum)
    x.DeleteLocalRef(jum);
  std::list<std::string> li;
  li.push_back("a");
  li.push_back("b");
  jobject jl = x.CppToJava(env, li);
  std::list<std::string> lib = x.JavaListToCppList<std::string>(jl);
  t.check(lib == li, "list C++<->Java");
  if (jl)
    x.DeleteLocalRef(jl);
  std::vector<long> n;
  n.push_back(4);
  n.push_back(5);
  std::vector<int> ni = x.ToCppVector<int>(n);
  std::vector<long> nl = x.ToJVector<long>(ni);
  t.check(ni.size() == 2 && ni[1] == 5 && nl[0] == 4, "ToCppVector/ToJVector");
}

static void testStreams(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  jclass bic = env->FindClass("java/io/ByteArrayInputStream");
  jmethodID bicc = x.GetMethodID(bic, "<init>", "([B)V");
  jbyteArray data = x.NewByteArray(11);
  const jbyte text[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  x.SetByteArrayRegion(data, 0, 11, text);
  jobject bis = x.NewObject(bic, bicc, data);
  std::unique_ptr<jni::JavaInputStream> cin = x.GetIStream(bis);
  char b[6] = {};
  cin->read(b, 5);
  t.check(std::string(b, 5) == "hello", "JavaInputStream", std::string(b, 5));
  if (bis)
    x.DeleteLocalRef(bis);
  if (data)
    x.DeleteLocalRef(data);
  if (bic)
    x.DeleteLocalRef(bic);
  jclass boc = env->FindClass("java/io/ByteArrayOutputStream");
  jmethodID bocc = x.GetMethodID(boc, "<init>", "()V");
  jobject bos = x.NewObject(boc, bocc);
  std::unique_ptr<jni::JavaOutputStream> cout = x.GetOStream(bos);
  (*cout) << "hello" << " world";
  cout->flush();
  jmethodID to = x.GetMethodID(boc, "toString", "()Ljava/lang/String;");
  jstring got = static_cast<jstring>(x.CallObjectMethod(bos, to));
  t.check(x.GetStringUTF(got) == "hello world", "JavaOutputStream", x.GetStringUTF(got));
  if (got)
    x.DeleteLocalRef(got);
  if (bos)
    x.DeleteLocalRef(bos);
  if (boc)
    x.DeleteLocalRef(boc);
  std::istringstream in("unused");
  std::ostringstream out;
  jobject ji = x.NewIStream(in);
  jobject jo = x.NewOStream(out);
  t.check(ji && jo, "NewIStream/NewOStream");
  if (ji)
    x.DeleteLocalRef(ji);
  if (jo)
    x.DeleteLocalRef(jo);
}

static void testFileTime(JNIEnv *env, TestState &t) {
  jni::XJNIEnv x(env);
  FILETIME ft;
  ft.dwLowDateTime = 0x12345678u;
  ft.dwHighDateTime = 0x9abcdef0u;
  jobject j = x.ToJavaFILETIME(ft);
  FILETIME back = x.ToCppFILETIME(j);
  t.check(back.dwLowDateTime == ft.dwLowDateTime &&
              back.dwHighDateTime == ft.dwHighDateTime,
          "FILETIME conversion");
  if (j)
    x.DeleteLocalRef(j);
}

static std::string run(JNIEnv *env) {
  TestState t;
  testJavaVM(env, t);
  noex(env, t, "exceptions after javavm");
  testBasics(env, t);
  noex(env, t, "exceptions after basics");
  testStrings(env, t);
  noex(env, t, "exceptions after strings");
  testConstructionAndMethods(env, t);
  noex(env, t, "exceptions after methods");
  testAllPrimitiveMethods(env, t);
  noex(env, t, "exceptions after primitive methods");
  testStatic(env, t);
  noex(env, t, "exceptions after static");
  testNonvirtual(env, t);
  noex(env, t, "exceptions after nonvirtual");
  testFields(env, t);
  noex(env, t, "exceptions after fields");
  testArrays(env, t);
  noex(env, t, "exceptions after arrays");
  testCriticalDirectRefs(env, t);
  noex(env, t, "exceptions after critical");
  testReflectionModuleVirtual(env, t);
  noex(env, t, "exceptions after reflection");
  testExceptionsFramesMonitor(env, t);
  noex(env, t, "exceptions after exception/ref");
  testContainers(env, t);
  noex(env, t, "exceptions after containers");
  testStreams(env, t);
  noex(env, t, "exceptions after streams");
  testFileTime(env, t);
  noex(env, t, "exceptions after FILETIME");
  std::ostringstream o;
  o << "XXJNI TEST RESULT\npassed: " << t.passed << "\nfailed: " << t.failed
    << "\n\n"
    << t.log;
  o << (t.failed ? "\nTESTS FAILED\n" : "\nALL TESTS PASSED\n");
  return o.str();
}
} // namespace
extern "C" JNIEXPORT jstring JNICALL Java_JNITest_runAll(JNIEnv *env, jclass) {
  try {
    return env->NewStringUTF(run(env).c_str());
  } catch (const std::exception &e) {
    return env->NewStringUTF((std::string("XXJNI TEST CRASHED: ") + e.what()).c_str());
  } catch (...) {
    return env->NewStringUTF("XXJNI TEST CRASHED: unknown exception");
  }
}
