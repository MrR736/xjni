cmake_minimum_required(VERSION 3.20)
project(xjni VERSION 1.0.5 LANGUAGES C CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)

# ------------------------------
# Options
# ------------------------------
option(XJNI_BUILD_STATIC "Build static library" ON)
option(XJNI_BUILD_TESTS "Build JNI/Java tests" OFF)

# ------------------------------
# Find dependencies
# ------------------------------
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

# ------------------------------
# Compiler standards
# ------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# Source files
# ------------------------------
set(XJNI_SOURCES
	${CMAKE_SOURCE_DIR}/src/xjni_arrayfield.c
	${CMAKE_SOURCE_DIR}/src/xjni_new.c
	${CMAKE_SOURCE_DIR}/src/xjni_printf.c
	${CMAKE_SOURCE_DIR}/src/xjni_stringarray.c
	${CMAKE_SOURCE_DIR}/src/xjni_stringbuffer.c
	${CMAKE_SOURCE_DIR}/src/xjni_stringbuilder.c
	${CMAKE_SOURCE_DIR}/src/xjni_stringreader.c
	${CMAKE_SOURCE_DIR}/src/xjni_stringwriter.c
	${CMAKE_SOURCE_DIR}/src/xjni2d.c
	${CMAKE_SOURCE_DIR}/src/xjni.c
)

# ------------------------------
# Shared library
# ------------------------------
add_library(xjni SHARED ${XJNI_SOURCES})
target_link_libraries(xjni PUBLIC JNI::JNI)
target_include_directories(xjni
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${JNI_INCLUDE_DIRS}
)

if(NOT WIN32)
	set_target_properties(xjni PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
	)
endif()

# ------------------------------
# Optional static library
# ------------------------------
if(XJNI_BUILD_STATIC)
	add_library(xjni_static STATIC ${XJNI_SOURCES})
	set_target_properties(xjni_static PROPERTIES OUTPUT_NAME xjni)
	target_link_libraries(xjni_static PUBLIC JNI::JNI)
	target_include_directories(xjni_static
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
			${JNI_INCLUDE_DIRS}
	)
endif()

# ------------------------------
# Installation
# ------------------------------
configure_file(${CMAKE_SOURCE_DIR}/cmake/xjni.pc.in ${CMAKE_BINARY_DIR}/xjni.pc @ONLY)

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

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

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

install(FILES ${CMAKE_BINARY_DIR}/xjni.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

install(EXPORT xjniTargets
	FILE xjniTargets.cmake
	NAMESPACE xjni::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xjni
)

# ------------------------------
# Testing
# ------------------------------
if(XJNI_BUILD_TESTS)
	enable_testing()

	# Java test JAR
	set(JAR_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/jar")
	set(HEADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}//build/headers")
	file(MAKE_DIRECTORY "${JAR_OUTPUT_DIR}" "${HEADER_OUTPUT_DIR}")

	add_jar(
		xjni-test-jar
		${CMAKE_SOURCE_DIR}/test/java/TestStringArray.java
		${CMAKE_SOURCE_DIR}/test/java/ArrayFieldTest.java
		${CMAKE_SOURCE_DIR}/test/java/Array2DTest.java
		${CMAKE_SOURCE_DIR}/test/java/TestStringBuffer.java
		${CMAKE_SOURCE_DIR}/test/java/TestStringBuilder.java
		${CMAKE_SOURCE_DIR}/test/java/TestStringReader.java
		${CMAKE_SOURCE_DIR}/test/java/TestStringWriter.java
		${CMAKE_SOURCE_DIR}/test/java/TestXJNI.java
		VERSION ${PROJECT_VERSION}
		OUTPUT_NAME "xjni-test"
		OUTPUT_DIR "${JAR_OUTPUT_DIR}"
		GENERATE_NATIVE_HEADERS xjni-test-native
			DESTINATION "${HEADER_OUTPUT_DIR}"
	)

	# C test library
	add_library(xjni_test SHARED
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringarray_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_arrayfield_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringbuffer_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringbuilder_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringwriter_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringreader_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni2d_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_test.c
	)
	target_include_directories(xjni_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)
	target_include_directories(xjni_test PUBLIC ${JNI_INCLUDE_DIRS})
	target_link_libraries(xjni_test PRIVATE xjni)

	set(LIBS_TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/lib")
	set_target_properties(xjni_test PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
	)

	# Java test targets
	foreach(TESTCLASS TestStringArray ArrayFieldTest Array2DTest
		TestXJNI TestStringBuilder TestStringWriter TestStringReader TestStringBuffer)
		add_custom_target(run_${TESTCLASS}
			COMMAND ${Java_JAVA_EXECUTABLE} -Djava.library.path=${LIBS_TEST_OUTPUT_DIR} -cp ${JAR_OUTPUT_DIR}/xjni-test.jar ${TESTCLASS}
			WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
			DEPENDS xjni xjni_test xjni-test-jar
			COMMENT "Running Java test ${TESTCLASS}"
		)
		add_custom_target(${TESTCLASS}_test_run ALL DEPENDS run_${TESTCLASS})
	endforeach()
endif()
