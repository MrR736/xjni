#ifndef __JNI_TEST_H__
#define __JNI_TEST_H__

#include <jni.h>
#include <xxjni/jni.hpp>
#include <xxjni/xxjni.hpp>

#include <stdio.h>

#ifdef _WIN32
# include <windows.h>
# include <io.h>
# include <fcntl.h>
#else
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
#endif

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

static int xmkstemp(char *temp) {
	if (temp == NULL) { return -1; }
#ifdef _WIN32
	if (_mktemp_s(temp, strlen(temp) + 1) != 0) { return -1; }
	return _open(temp,_O_RDWR | _O_CREAT | _O_EXCL | _O_BINARY,_S_IREAD | _S_IWRITE);
#else
	return mkstemp(temp);
#endif
}

static std::string xfmkstemp(const void* data, std::size_t size) {
	if (data == nullptr && size != 0) {
		return std::string();
	}

	char temp[] = "xxjni-XXXXXX";

#ifdef _WIN32
	if (_mktemp_s(temp, sizeof(temp)) != 0) {
		return std::string();
	}

	FILE* file = std::fopen(temp, "wb");
	if (file == nullptr) {
		return std::string();
	}

	const std::size_t written =
	size == 0 ? 0 : std::fwrite(data, 1, size, file);

	const int closeResult = std::fclose(file);

	if (written != size || closeResult != 0) {
		std::remove(temp);
		return std::string();
	}

	return std::string(temp);

#else
	const int fd = mkstemp(temp);
	if (fd < 0) {
		return std::string();
	}

	FILE* file = fdopen(fd, "wb");
	if (file == nullptr) {
		close(fd);
		std::remove(temp);
		return std::string();
	}

	const std::size_t written =
	size == 0 ? 0 : std::fwrite(data, 1, size, file);

	const int closeResult = std::fclose(file);

	if (written != size || closeResult != 0) {
		std::remove(temp);
		return std::string();
	}

	return std::string(temp);
#endif
}

static std::string xfmkstemp(const std::string& data) {
	return xfmkstemp(data.data(), data.size());
}

static std::string xfmkstemp(const char* data) {
	return xfmkstemp(std::string(data));
}

static std::string xomkstemp() {
	return xfmkstemp("");
}

}

#endif
