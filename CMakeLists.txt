cmake_minimum_required(VERSION 3.20)
project(xjni VERSION 1.1.4 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(JLD)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)

# ------------------------------------
# Project options
# ------------------------------------
option(XJNI_BUILD_DEBUG "Build Debug library" OFF)
option(XJNI_BUILD_TESTS "Build JNI/Java tests" OFF)

option(XJNI_BUILD_STATIC "Build static library" ON)
option(XJNI_BUILD_DOCS "Build Doxygen documentation" ON)

option(DISABLE_XLOG_COLOR "disable Logs with Colored" ON)
option(XJNI_BUILD_XJAVA "Build xjava library" ON)

# ------------------------------------
# Detect Java / JNI
# ------------------------------------
if(NOT WIN32)
	find_package(Java REQUIRED)
	find_package(JNI REQUIRED)
	set(GENERATE_HEADERS TRUE)
	set(_JAVA_PATH_SER ":")
else()
	set(_JAVA_PATH_SER ";")
	set(CMAKE_STATIC_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
	set(CMAKE_IMPORT_LIBRARY_PREFIX "")

	find_program(WINE_EXECUTABLE NAMES wine wine64)
	find_program(WINEPATH_EXECUTABLE NAMES winepath)
	if(WINE_EXECUTABLE)
		set(GENERATE_HEADERS FALSE)
		include(FetchContent)
		FetchContent_Declare(
			win_openjdk
			URL https://github.com/adoptium/temurin21-binaries/releases/download/jdk-21.0.9%2B10/OpenJDK21U-jdk_x64_windows_hotspot_21.0.9_10.zip
		)
		FetchContent_MakeAvailable(win_openjdk)
		set(WIN_JAVA_HOME ${win_openjdk_SOURCE_DIR})
		set(WIN_JAR_EXECUTABLE "${WIN_JAVA_HOME}/bin/jar.exe")
		set(WIN_JAVA_EXECUTABLE "${WIN_JAVA_HOME}/bin/java.exe")
		set(WIN_JAVAC_EXECUTABLE "${WIN_JAVA_HOME}/bin/javac.exe")

		FetchContent_Declare(
			ghc_filesystem
			URL https://github.com/gulrak/filesystem/archive/refs/tags/v1.5.16.tar.gz
		)
		FetchContent_MakeAvailable(ghc_filesystem)
	else()
		set(GENERATE_HEADERS TRUE)
	endif()
endif()

include(UseJava)

# JUnit 4
add_java_library(junit
	GROUP junit
	ARTIFACT junit
	VERSION 4.13.2
)

# JUnit 4 requires Hamcrest Core
add_java_library(hamcrest-core
	GROUP org.hamcrest
	ARTIFACT hamcrest-core
	VERSION 1.3
)

# ------------------------------------
# Compiler standards
# ------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------
# Source files
# ------------------------------------
if (WIN32)
	include_directories(
		"${ghc_filesystem_SOURCE_DIR}/include"
		"${WIN_JAVA_HOME}/include"
		"${WIN_JAVA_HOME}/include/win32"
	)
	set(JNI_CFLAGS "-I${WIN_JAVA_HOME}/include -I${WIN_JAVA_HOME}/include/win32")
else()
	set(JNI_CFLAGS "")
	foreach(dir IN LISTS JNI_INCLUDE_DIRS)
		string(APPEND JNI_CFLAGS " -I${dir}")
	endforeach()
endif()

set(XJNI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(XJNI_JNI_SOURCES
	${XJNI_SOURCE_DIR}/src/jni/file.c
	${XJNI_SOURCE_DIR}/src/jni/ifstream.cpp
	${XJNI_SOURCE_DIR}/src/jni/ofstream.cpp
	${XJNI_SOURCE_DIR}/src/jni/istream.cpp
	${XJNI_SOURCE_DIR}/src/jni/ostream.cpp
)
set(XJNI_SOURCES
	${XJNI_SOURCE_DIR}/src/xjni/xjni_args.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_hashmap.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_classcache.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_arrayfield.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_log.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_pool.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_new.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_printf.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_string.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_stringarray.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_stringbuffer.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_stringbuilder.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_stringreader.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_stringwriter.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_struct.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_thread.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_utils.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni_va_list.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni2d.c
	${XJNI_SOURCE_DIR}/src/xjni/xjni.c
)

# --------------------------------------------------------
# Java compiler / runtime
# --------------------------------------------------------
set(XJAVA_SOURCES
	${XJNI_SOURCE_DIR}/src/java/xjava/XJNILibrary.java

	${XJNI_SOURCE_DIR}/src/java/xjava/io/DataIStream.java
	${XJNI_SOURCE_DIR}/src/java/xjava/io/DataOStream.java
	${XJNI_SOURCE_DIR}/src/java/xjava/io/FILE.java
	${XJNI_SOURCE_DIR}/src/java/xjava/io/IFStream.java
	${XJNI_SOURCE_DIR}/src/java/xjava/io/OFStream.java
	${XJNI_SOURCE_DIR}/src/java/xjava/io/IStream.java
	${XJNI_SOURCE_DIR}/src/java/xjava/io/OStream.java

	${XJNI_SOURCE_DIR}/src/java/xjava/lang/XSystem.java

	${XJNI_SOURCE_DIR}/src/java/xjava/nio/file/attribute/FILETIME.java
)

set(JAR_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/jar")
set(XJAR_CLASSES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/xjava/classes")
set(JAR_CLASSES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/classes")
set(HEADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/headers")
set(LIBS_TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/lib")

file(MAKE_DIRECTORY
	"${JAR_OUTPUT_DIR}"
	"${XJAR_CLASSES_OUTPUT_DIR}"
	"${JAR_CLASSES_OUTPUT_DIR}"
	"${HEADER_OUTPUT_DIR}"
	"${LIBS_TEST_OUTPUT_DIR}"
)

# --------------------------------------------------------
# Java compiler / runtime
# --------------------------------------------------------
if(WIN_JAVA_EXECUTABLE)
	if(WINE_EXECUTABLE)
		set(_JAR_CMD ${WINE_EXECUTABLE} ${WIN_JAR_EXECUTABLE})
		set(_JAVAC_CMD ${WINE_EXECUTABLE} ${WIN_JAVAC_EXECUTABLE})
		set(_JAVA_CMD ${WINE_EXECUTABLE} ${WIN_JAVA_EXECUTABLE})
	else()
		set(_JAR_CMD ${WIN_JAR_EXECUTABLE})
		set(_JAVAC_CMD ${WIN_JAVAC_EXECUTABLE})
		set(_JAVA_CMD ${WIN_JAVA_EXECUTABLE})
	endif()
else()
	set(_JAR_CMD ${Java_JAR_EXECUTABLE})
	set(_JAVAC_CMD ${Java_JAVAC_EXECUTABLE})
	set(_JAVA_CMD ${Java_JAVA_EXECUTABLE})
endif()

if(XJNI_BUILD_XJAVA)
	set(XJAVA_JAR "${JAR_OUTPUT_DIR}/xjava-${PROJECT_VERSION}.jar")
	if(GENERATE_HEADERS)
		add_jar(xjava-jar
			${XJAVA_SOURCES}
			VERSION ${PROJECT_VERSION}
			OUTPUT_NAME "xjava"
			OUTPUT_DIR "${JAR_OUTPUT_DIR}"
			GENERATE_NATIVE_HEADERS xjava-native
			DESTINATION "${HEADER_OUTPUT_DIR}"
		)
	else()
		add_custom_command(
			OUTPUT "${XJAVA_JAR}"
			COMMAND "${CMAKE_COMMAND}" -E make_directory
					"${XJAR_CLASSES_OUTPUT_DIR}"
			COMMAND ${_JAVAC_CMD}
					-encoding UTF-8
					-d "${XJAR_CLASSES_OUTPUT_DIR}"
					${XJAVA_SOURCES}
			COMMAND ${_JAR_CMD} cf "${XJAVA_JAR}"
					-C "${XJAR_CLASSES_OUTPUT_DIR}" .
			WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
			DEPENDS ${XJAVA_SOURCES}
			COMMENT "Compiling Java library sources"
			VERBATIM
		)

		add_custom_target(xjava-jar DEPENDS "${XJAVA_JAR}")
	endif()
endif()

# ------------------------------------
# Shared library
# ------------------------------------
if(XJNI_BUILD_XJAVA)
	add_library(xjni SHARED ${XJNI_JNI_SOURCES} ${XJNI_SOURCES})
else()
	add_library(xjni SHARED ${XJNI_SOURCES})
endif()

target_compile_definitions(xjni PUBLIC XJNI_BUILD)

if (DISABLE_XLOG_COLOR)
	target_compile_definitions(xjni PUBLIC DISABLE_XLOG_COLOR)
endif()

if (WIN32)
	target_include_directories(xjni PRIVATE
		${WIN_JAVA_HOME}/include
		${WIN_JAVA_HOME}/include/win32
	)
	target_link_options(xjni PUBLIC -static -static-libgcc -static-libstdc++)
	target_link_options(xjni PRIVATE "LINKER:--output-def=${CMAKE_CURRENT_BINARY_DIR}/xjni.def")
else()
	target_link_libraries(xjni PUBLIC JNI::JNI pthread)
endif()

target_include_directories(xjni
	PUBLIC
		$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/src/internal>
		$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include/xjni>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xjni>
		${JNI_INCLUDE_DIRS}
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	target_link_libraries(xjni PRIVATE atomic)
endif()

# Version info
if(NOT WIN32)
	set_target_properties(xjni PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
	)
endif()

if(XJNI_BUILD_DEBUG)
	target_compile_definitions(xjni PUBLIC DEBUG)
endif()

# ------------------------------------
# Optional static library
# ------------------------------------
if(XJNI_BUILD_STATIC)
	add_library(xjni_static STATIC ${XJNI_SOURCES})
	set_target_properties(xjni_static PROPERTIES OUTPUT_NAME xjni)
	target_compile_definitions(xjni_static PRIVATE XJNI_STATIC)
	if (DISABLE_XLOG_COLOR)
		target_compile_definitions(xjni_static PUBLIC DISABLE_XLOG_COLOR)
	endif()
	target_include_directories(xjni_static
		PUBLIC
			$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/src/internal>
			$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include>
			$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include/xjni>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xjni>
			${JNI_INCLUDE_DIRS}
	)

	if(WIN32)
		target_link_options(xjni_static PUBLIC -static -static-libgcc -static-libstdc++)
	else()
		target_link_libraries(xjni_static PUBLIC JNI::JNI)
	endif()

	if(XJNI_BUILD_DEBUG)
		target_compile_definitions(xjni_static PUBLIC DEBUG)
	endif()

	if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
		target_link_libraries(xjni PRIVATE atomic)
	endif()
endif()

# ------------------------------------
# Doxygen documentation
# ------------------------------------
if(XJNI_BUILD_DOCS)
	find_package(Doxygen REQUIRED)
	set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc)
	configure_file(${XJNI_SOURCE_DIR}/doc/Doxyfile.in ${DOXYGEN_OUTPUT_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUTPUT_DIR}/Doxyfile
		WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}
		COMMENT "Generating XJNI documentation with Doxygen"
		VERBATIM
	)
	add_custom_target(doc_build ALL DEPENDS doc)

	install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html/
		DESTINATION ${CMAKE_INSTALL_DOCDIR}
	)
endif()

# ------------------------------------
# Installation
# ------------------------------------
install(TARGETS xjni
	EXPORT xjniTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(XJNI_BUILD_STATIC)
	install(TARGETS xjni_static
		EXPORT xjniTargets
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
endif()

install(DIRECTORY ${XJNI_SOURCE_DIR}/include/xjni/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xjni
	FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${XJNI_SOURCE_DIR}/include/xxjni/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xxjni
	FILES_MATCHING PATTERN "*.hpp"
)

configure_file(${CMAKE_SOURCE_DIR}/cmake/xjni.pc.in ${CMAKE_BINARY_DIR}/xjni.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/xjni.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/xjniConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/xjniConfig.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xjni
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/xjniConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/xjniConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xjni
)

install(EXPORT xjniTargets
	FILE xjniTargets.cmake
	NAMESPACE xjni::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xjni
)

install(
	FILES
	"${XJAVA_JAR}"
	DESTINATION ${CMAKE_INSTALL_DATADIR}/java
)

install(CODE [[
	execute_process(
		COMMAND "${CMAKE_COMMAND}" -E create_symlink
			"xjava-${PROJECT_VERSION}.jar"
			"${CMAKE_INSTALL_PREFIX}/share/java/xjava.jar"
	)
]])

# ============================================================
# Testing
# ============================================================

if(XJNI_BUILD_TESTS)

	enable_testing()
	# ========================================================
	# Java test sources
	# ========================================================

	set(XJNI_JAVA_SOURCES

		# Existing XJNI tests
		${CMAKE_SOURCE_DIR}/test/java/StreamTest.java
		${CMAKE_SOURCE_DIR}/test/java/FStreamTest.java

		${CMAKE_SOURCE_DIR}/test/java/TestStringArray.java
		${CMAKE_SOURCE_DIR}/test/java/ArrayFieldTest.java
		${CMAKE_SOURCE_DIR}/test/java/Array2DTest.java

		${CMAKE_SOURCE_DIR}/test/java/TestStringBuffer.java
		${CMAKE_SOURCE_DIR}/test/java/TestStringBuilder.java
		${CMAKE_SOURCE_DIR}/test/java/TestStringReader.java
		${CMAKE_SOURCE_DIR}/test/java/TestStringWriter.java

		${CMAKE_SOURCE_DIR}/test/java/TestXJNI.java
		${CMAKE_SOURCE_DIR}/test/java/TestXJNILOG.java
		${CMAKE_SOURCE_DIR}/test/java/TestXJNIPrintf.java

		${CMAKE_SOURCE_DIR}/test/java/MyStructObject.java

		# xxjni C++ wrapper tests
		${CMAKE_SOURCE_DIR}/test/java/XXJNITest.java
		${CMAKE_SOURCE_DIR}/test/java/XXJNITestObject.java

		${CMAKE_SOURCE_DIR}/test/java/JNITest.java
		${CMAKE_SOURCE_DIR}/test/java/JNITestObject.java
		${CMAKE_SOURCE_DIR}/test/java/JNITestBase.java
		${CMAKE_SOURCE_DIR}/test/java/JNITestDerived.java
	)

	if(XJNI_BUILD_XJAVA)
		list(APPEND XJNI_JAVA_SOURCES ${CMAKE_SOURCE_DIR}/test/java/xjava/io/FILETest.java)
	endif()


	# ========================================================
	# Java test JAR
	# ========================================================

	if(GENERATE_HEADERS)
		add_jar(
			xjni-test-jar ${XJNI_JAVA_SOURCES}
			VERSION ${PROJECT_VERSION}
			OUTPUT_NAME "xjni-test"
			OUTPUT_DIR "${JAR_OUTPUT_DIR}"
			INCLUDE_JARS xjava-jar jld::junit jld::hamcrest-core
			GENERATE_NATIVE_HEADERS xjni-test-native
				DESTINATION "${HEADER_OUTPUT_DIR}"
		)
		set(XJNI_TEST_JAR "${JAR_OUTPUT_DIR}/xjni-test.jar")
	else()
		set(XJNI_TEST_JAR "${JAR_OUTPUT_DIR}/xjni-test.jar")

		add_custom_command(
			OUTPUT "${XJNI_TEST_JAR}"

			COMMAND "${CMAKE_COMMAND}" -E make_directory
					"${JAR_CLASSES_OUTPUT_DIR}"

			COMMAND ${_JAVAC_CMD}
					-encoding UTF-8
					-d "${JAR_CLASSES_OUTPUT_DIR}"
					-classpath
					"${XJAVA_JAR}${_JAVA_PATH_SER}$<TARGET_PROPERTY:jld::junit,JLD_JAR>${_JAVA_PATH_SER}$<TARGET_PROPERTY:jld::hamcrest-core,JLD_JAR>"
					${XJNI_JAVA_SOURCES}

			COMMAND ${_JAR_CMD} cf "${XJNI_TEST_JAR}"
					-C "${JAR_CLASSES_OUTPUT_DIR}" .

			WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
			DEPENDS
				${XJNI_JAVA_SOURCES}
				xjava-jar
			COMMENT "Compiling Java test sources"
			VERBATIM
		)

		add_custom_target(xjni-test-jar DEPENDS "${XJNI_TEST_JAR}")
	endif()

	# ========================================================
	# C JNI test library
	# ========================================================

	add_library(xjni_test SHARED
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringarray_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_arrayfield_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringbuffer_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringbuilder_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringwriter_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringreader_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni2d_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_va_list_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_struct_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_log_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_test.c
	)

	target_include_directories(xjni_test
		PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include
		PUBLIC ${JNI_INCLUDE_DIRS}
	)

	if(WIN32)
		target_include_directories(xjni_test
			PRIVATE
				${WIN_JAVA_HOME}/include
				${WIN_JAVA_HOME}/include/win32
		)
	endif()

	target_link_libraries(xjni_test PRIVATE xjni)

	set_target_properties(xjni_test PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${LIBS_TEST_OUTPUT_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${LIBS_TEST_OUTPUT_DIR}"
		RUNTIME_OUTPUT_DIRECTORY "${LIBS_TEST_OUTPUT_DIR}"
	)


	# ========================================================
	# xxjni.hh C++ wrapper test library
	# ========================================================

	add_library(xxjni_cpp_test SHARED
		${CMAKE_SOURCE_DIR}/test/cpp/jni_test.cpp
		${CMAKE_SOURCE_DIR}/test/cpp/xxjni_test.cpp
	)

	target_include_directories(xxjni_cpp_test
		PRIVATE
			${CMAKE_SOURCE_DIR}/include
			${CMAKE_SOURCE_DIR}/include/xjni
			${CMAKE_SOURCE_DIR}/include/xxjni
			${CMAKE_SOURCE_DIR}/src
			${JNI_INCLUDE_DIRS}
	)

	if(WIN32)
		target_include_directories(xxjni_cpp_test
			PRIVATE
				${WIN_JAVA_HOME}/include
				${WIN_JAVA_HOME}/include/win32
		)
		target_link_options(xxjni_cpp_test PUBLIC -static -static-libgcc -static-libstdc++)
	endif()

	target_link_libraries(xxjni_cpp_test PUBLIC xjni)

	set_target_properties(xxjni_cpp_test PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED ON
		ARCHIVE_OUTPUT_DIRECTORY "${LIBS_TEST_OUTPUT_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${LIBS_TEST_OUTPUT_DIR}"
		RUNTIME_OUTPUT_DIRECTORY "${LIBS_TEST_OUTPUT_DIR}"
	)


	# ========================================================
	# Standalone hashmap test
	# ========================================================

	add_executable(xjni_hashmap_test
		${CMAKE_SOURCE_DIR}/test/xjni_hashmap_test.c
	)

	target_include_directories(xjni_hashmap_test
		PRIVATE
			${CMAKE_SOURCE_DIR}/src
			${CMAKE_SOURCE_DIR}/include
	)
	target_link_libraries(xjni_hashmap_test PRIVATE xjni)


	# ========================================================
	# Standalone pool test
	# ========================================================

	add_executable(xjni_pool_test
		${CMAKE_SOURCE_DIR}/test/xjni_pool_test.c
	)

	target_include_directories(xjni_pool_test
		PRIVATE
			${CMAKE_SOURCE_DIR}/src
			${CMAKE_SOURCE_DIR}/include
	)

	target_link_libraries(xjni_pool_test
		PRIVATE
			xjni
	)


	# ========================================================
	# CTest: standalone C tests
	# ========================================================

	add_test(
		NAME xjni_hashmap_test
		COMMAND xjni_hashmap_test
	)

	add_test(
		NAME xjni_pool_test
		COMMAND xjni_pool_test
	)


	# ========================================================
	# Java test custom targets
	# ========================================================

	set(XJNI_JAVA_TEST_CLASSES
		StreamTest
		FStreamTest

		TestStringArray
		ArrayFieldTest
		Array2DTest

		TestXJNI

		TestStringBuilder
		TestStringWriter
		TestStringReader
		TestStringBuffer

		TestXJNIPrintf
		TestXJNILOG

		MyStructObject

		XXJNITest
		JNITest
	)

	# Actual CMake targets
	set(XJNI_JAVA_TEST_TARGETS)


	# ========================================================
	# Individual Java test build targets
	# ========================================================

	if (WINEPATH_EXECUTABLE)
		execute_process(
			COMMAND winepath -w "${LIBS_TEST_OUTPUT_DIR}"
			OUTPUT_VARIABLE WINEPATH_WINDOWS
			OUTPUT_STRIP_TRAILING_WHITESPACE
			COMMAND_ERROR_IS_FATAL ANY
		)
	else()
		set(WINEPATH_WINDOWS ${LIBS_TEST_OUTPUT_DIR})
	endif()

	foreach(TESTCLASS IN LISTS XJNI_JAVA_TEST_CLASSES)
		set(TEST_TARGET "run_${TESTCLASS}")

		if(WINE_EXECUTABLE)
			add_custom_target(${TEST_TARGET}
				COMMAND "${CMAKE_COMMAND}" -E env
					"WINEPATH=${WINEPATH_WINDOWS}"
					${_JAVA_CMD}
					"-Djava.library.path=${LIBS_TEST_OUTPUT_DIR}${_JAVA_PATH_SER}${CMAKE_CURRENT_BINARY_DIR}"
					"-cp" "${XJAVA_JAR}${_JAVA_PATH_SER}${XJNI_TEST_JAR}"
					"${TESTCLASS}"
				WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
				DEPENDS xjni xjni_test xjni-test-jar
				COMMENT "Running Java test ${TESTCLASS}"
				VERBATIM
			)
		else()
			add_custom_target(${TEST_TARGET}
				COMMAND
					${_JAVA_CMD}
					"-Djava.library.path=${LIBS_TEST_OUTPUT_DIR}${_JAVA_PATH_SER}${CMAKE_CURRENT_BINARY_DIR}"
					"-cp" "${XJAVA_JAR}${_JAVA_PATH_SER}${XJNI_TEST_JAR}"
					"${TESTCLASS}"
				WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
				DEPENDS xjni xjni_test xjni-test-jar
				COMMENT "Running Java test ${TESTCLASS}"
				VERBATIM
			)
		endif()

		list(APPEND XJNI_JAVA_TEST_TARGETS "${TEST_TARGET}")
	endforeach()


	# ========================================================
	# CTest Java tests
	# ========================================================

	foreach(TESTCLASS IN LISTS XJNI_JAVA_TEST_CLASSES)
		add_test(
			NAME "${TESTCLASS}"
			COMMAND
				${_JAVA_CMD}
				"-Djava.library.path=${LIBS_TEST_OUTPUT_DIR}${_JAVA_PATH_SER}${CMAKE_CURRENT_BINARY_DIR}"
				"-cp"
				"${XJAVA_JAR}${_JAVA_PATH_SER}${XJNI_TEST_JAR}"
				"${TESTCLASS}"
		)

		set_tests_properties(
			"${TESTCLASS}"
			PROPERTIES
				WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
				ENVIRONMENT
					"LD_LIBRARY_PATH=${LIBS_TEST_OUTPUT_DIR}:$ENV{LD_LIBRARY_PATH}"
		)

	endforeach()


	if(XJNI_BUILD_XJAVA)
		# ========================================================
		# JUnit FILE test
		# ========================================================

		java_get_classpath(
			XJNI_JUNIT_CLASSPATH
			jld::junit
			jld::hamcrest-core
		)

		set(XJNI_FILE_TEST_CLASSPATH
			"${XJAVA_JAR}${_JAVA_PATH_SER}${XJNI_TEST_JAR}${_JAVA_PATH_SER}${XJNI_JUNIT_CLASSPATH}"
		)

		add_custom_target(
			run_FILETest

			COMMAND
				${_JAVA_CMD}
				"-Djava.library.path=${LIBS_TEST_OUTPUT_DIR}${_JAVA_PATH_SER}${CMAKE_CURRENT_BINARY_DIR}"
				"-cp" "${XJNI_FILE_TEST_CLASSPATH}"
				org.junit.runner.JUnitCore
				xjava.io.FILETest

				WORKING_DIRECTORY
					"${JAR_OUTPUT_DIR}"

			DEPENDS
				xjni
				xjni_test
				xjni-test-jar

			COMMENT
				"Running JUnit xjava.io.FILETest"

			VERBATIM
		)

		add_test(
			NAME xjava.io.FILETest
			COMMAND
				${_JAVA_CMD}
				"-Djava.library.path=${LIBS_TEST_OUTPUT_DIR}${_JAVA_PATH_SER}${CMAKE_CURRENT_BINARY_DIR}"
				"-cp"
				"${XJNI_FILE_TEST_CLASSPATH}"
				org.junit.runner.JUnitCore
				xjava.io.FILETest
		)

		set_tests_properties(
			xjava.io.FILETest
			PROPERTIES
				WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
				ENVIRONMENT "LD_LIBRARY_PATH=${LIBS_TEST_OUTPUT_DIR}:$ENV{LD_LIBRARY_PATH}"
		)
	endif()

	# ========================================================
	# Run all Java tests
	# ========================================================
	add_custom_target(RUN_ALLTEST)
	foreach(TEST_TARGET IN LISTS XJNI_JAVA_TEST_TARGETS)
		add_dependencies(RUN_ALLTEST "${TEST_TARGET}")
	endforeach()

	if(XJNI_BUILD_XJAVA)
		add_dependencies(RUN_ALLTEST run_FILETest)
	endif()
endif()
