#ifndef __XJAVA_H
#define __XJAVA_H

#include "xjni/xjnidef.h"

#include <istream>
#include <ostream>
#include <sstream>

#if CPLUSPLUS_GE(STDC17PP)
# include <fstream>
# include <filesystem>
#else
# include <ghc/filesystem.hpp>
#endif

namespace jni {
	namespace filesystem {
#if CPLUSPLUS_GE(STDC17PP)
		namespace fs = std::filesystem;
		using ifstream = std::ifstream;
		using ofstream = std::ofstream;
		using wfstream = std::wfstream;
		using wofstream = std::wofstream;
		using fstream = std::fstream;
		using filebuf = std::filebuf;
		using wfilebuf = std::wfilebuf;
#else
		namespace fs = ghc::filesystem;
		using ifstream = fs::ifstream;
		using ofstream = fs::ofstream;
		using wfstream = fs::wfstream;
		using wofstream = fs::wofstream;
		using fstream = fs::fstream;
		using filebuf = fs::filebuf;
		using wfilebuf = fs::wfilebuf;
#endif
		using path = fs::path;
		using filesystem_error = fs::filesystem_error;
		using file_status = fs::file_status;
		using file_time_type = fs::file_time_type;
		using file_type = fs::file_type;
		using copy_options = fs::copy_options;
		using directory_options = fs::directory_options;
		using directory_entry = fs::directory_entry;
		using directory_iterator = fs::directory_iterator;
		using recursive_directory_iterator = fs::recursive_directory_iterator;
		using perm_options = fs::perm_options;
	}
}

typedef std::basic_string<uint8_t> u8_string;
typedef std::basic_ostringstream<uint8_t> u8ostringstream;
typedef std::basic_istringstream<uint8_t> u8istringstream;

#endif
