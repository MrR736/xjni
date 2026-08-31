#ifndef __JNI_TEST_H__
#define __JNI_TEST_H__

#include <jni.h>
#include <xxjni/jni.hpp>
#include <xxjni/xxjni.hpp>

namespace {

struct TestState {
	int passed = 0, failed = 0;
	std::string log;
	void check(bool ok, const std::string &n, const std::string &why = {}) {
		if (ok) {
			++passed;
			log += "[PASS] " + n + "\n";
		} else {
			++failed;
			log += "[FAIL] " + n + (why.empty() ? "" : ": " + why) + "\n";
		}
	}
};
}

#endif
