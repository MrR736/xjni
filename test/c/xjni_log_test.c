#include <jni.h>
#include "xjni_log.h"

JNIEXPORT void JNICALL
Java_TestXJNILOG_nativeLogTest(JNIEnv *env, jclass clazz) {
    int errCode = -42;
    XJNI_LOGV("XJNI-Test", "Verbose log test");
    XJNI_LOGD("XJNI-Test", "Debug log test");
    XJNI_LOGI("XJNI-Test", "Info log test");
    XJNI_LOGW("XJNI-Test", "Warning log test");
    XJNI_LOGE("XJNI-Test", "Error log test");
    XJNI_LOGF("XJNI-Test", "Fatal log test, errCode=%d\n", errCode);

    XJNI_LOGVC("XJNI-Test", "Verbose log test");
    XJNI_LOGDC("XJNI-Test", "Debug log test");
    XJNI_LOGIC("XJNI-Test", "Info log test");
    XJNI_LOGWC("XJNI-Test", "Warning log test");
    XJNI_LOGEC("XJNI-Test", "Error log test");
    XJNI_LOGFC("XJNI-Test", "Fatal log test, errCode=%d", errCode);
}
