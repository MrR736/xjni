cmake_minimum_required(VERSION 3.20)
project(xjni VERSION 1.0.8 LANGUAGES C CXX)

# ------------------------------------
# Verbose & helper options
# ------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)

# ------------------------------------
# Project options
# ------------------------------------
option(XJNI_BUILD_STATIC "Build static library" ON)
option(XJNI_BUILD_DOCS "Build Doxygen documentation" ON)
option(XJNI_BUILD_TESTS "Build JNI/Java tests" OFF)

# ------------------------------------
# Detect Java / JNI
# ------------------------------------
if(NOT WIN32)
	find_package(Java REQUIRED)
	find_package(JNI REQUIRED)
	set(GENERATE_HEADERS TRUE)
else()
	set(CMAKE_STATIC_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
	set(CMAKE_IMPORT_LIBRARY_PREFIX "")

	find_program(WINE_EXECUTABLE NAMES wine wine64)
	if(WINE_EXECUTABLE)
		set(GENERATE_HEADERS FALSE)
		include(FetchContent)
		FetchContent_Declare(
			win_openjdk
			URL https://github.com/adoptium/temurin17-binaries/releases/download/jdk-17.0.10%2B7/OpenJDK17U-jdk_x64_windows_hotspot_17.0.10_7.zip
		)
		FetchContent_MakeAvailable(win_openjdk)
		set(WIN_JAVA_HOME ${win_openjdk_SOURCE_DIR})
		set(WIN_JAR_EXECUTABLE "${WIN_JAVA_HOME}/bin/jar.exe")
		set(WIN_JAVA_EXECUTABLE "${WIN_JAVA_HOME}/bin/java.exe")
		set(WIN_JAVAC_EXECUTABLE "${WIN_JAVA_HOME}/bin/javac.exe")
	else()
		set(GENERATE_HEADERS TRUE)
	endif()
endif()

include(UseJava)

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
set(XJNI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(XJNI_SOURCES
	${XJNI_SOURCE_DIR}/src/xjni_args.c
	${XJNI_SOURCE_DIR}/src/xjni_arrayfield.c
	${XJNI_SOURCE_DIR}/src/xjni_log.c
	${XJNI_SOURCE_DIR}/src/xjni_new.c
	${XJNI_SOURCE_DIR}/src/xjni_printf.c
	${XJNI_SOURCE_DIR}/src/xjni_stringarray.c
	${XJNI_SOURCE_DIR}/src/xjni_stringbuffer.c
	${XJNI_SOURCE_DIR}/src/xjni_stringbuilder.c
	${XJNI_SOURCE_DIR}/src/xjni_stringreader.c
	${XJNI_SOURCE_DIR}/src/xjni_stringwriter.c
	${XJNI_SOURCE_DIR}/src/xjni_va_list.c
	${XJNI_SOURCE_DIR}/src/xjni2d.c
	${XJNI_SOURCE_DIR}/src/xjni.c
)

# ------------------------------------
# Shared library
# ------------------------------------
add_library(xjni SHARED ${XJNI_SOURCES})

if (WIN32)
	target_include_directories(xjni PRIVATE
		${WIN_JAVA_HOME}/include
		${WIN_JAVA_HOME}/include/win32
	)
else()
	target_link_libraries(xjni PUBLIC JNI::JNI)
endif()

target_include_directories(xjni
	PUBLIC
		$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include/xjni>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xjni>
		${JNI_INCLUDE_DIRS}
)

# Version info
if(NOT WIN32)
	set_target_properties(xjni PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
	)
endif()

# ------------------------------------
# Optional static library
# ------------------------------------
if(XJNI_BUILD_STATIC)
	add_library(xjni_static STATIC ${XJNI_SOURCES})
	set_target_properties(xjni_static PROPERTIES OUTPUT_NAME xjni)
	target_include_directories(xjni_static
		PUBLIC
			$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include>
			$<BUILD_INTERFACE:${XJNI_SOURCE_DIR}/include/xjni>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xjni>
			${JNI_INCLUDE_DIRS}
	)
	if(NOT WIN32)
		target_link_libraries(xjni_static PUBLIC JNI::JNI)
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


# ------------------------------
# Testing
# ------------------------------
if(XJNI_BUILD_TESTS)
	enable_testing()

	if (WIN_JAVA_EXECUTABLE)
		if (WINE_EXECUTABLE)
			set(_JAVAC_CMD ${WINE_EXECUTABLE} ${WIN_JAVAC_EXECUTABLE})
			set(_JAVA_CMD  ${WINE_EXECUTABLE} ${WIN_JAVA_EXECUTABLE})
		else()
			set(_JAVAC_CMD ${WIN_JAVAC_EXECUTABLE})
			set(_JAVA_CMD  ${WIN_JAVA_EXECUTABLE})
		endif()
	else()
		set(_JAVAC_CMD ${Java_JAVAC_EXECUTABLE})
		set(_JAVA_CMD ${Java_JAVA_EXECUTABLE})
	endif()

	# Java test JAR
	set(JAR_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/jar")
	file(MAKE_DIRECTORY "${JAR_OUTPUT_DIR}")

	set(XJNI_JAVA_SOURCES
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
	)

	if(GENERATE_HEADERS)
		set(HEADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/headers")
		file(MAKE_DIRECTORY "${JAR_OUTPUT_DIR}" "${HEADER_OUTPUT_DIR}")
		add_jar(
			xjni-test-jar
			${XJNI_JAVA_SOURCES}
			VERSION ${PROJECT_VERSION}
			OUTPUT_NAME "xjni-test"
			OUTPUT_DIR "${JAR_OUTPUT_DIR}"
			GENERATE_NATIVE_HEADERS xjni-test-native
				DESTINATION "${HEADER_OUTPUT_DIR}"
		)
	else()
		# Set the output JAR path
		set(XJNI_TEST_JAR "${JAR_OUTPUT_DIR}/xjni-test.jar")
		set(JAR_CLASSES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/classes")
		file(MAKE_DIRECTORY "${JAR_CLASSES_OUTPUT_DIR}")

		# Compile Java sources
		add_custom_command(
			OUTPUT ${XJNI_TEST_JAR}
			COMMAND ${_JAVAC_CMD} -encoding UTF-8 -d "${JAR_CLASSES_OUTPUT_DIR}" ${XJNI_JAVA_SOURCES}
			COMMAND ${WINE_EXECUTABLE} ${WIN_JAR_EXECUTABLE} cf ${XJNI_TEST_JAR} -C "${JAR_CLASSES_OUTPUT_DIR}" .
			WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
			DEPENDS ${XJNI_JAVA_SOURCES}
			COMMENT "Compiling Java sources and creating JAR ${XJNI_TEST_JAR}"
			VERBATIM
		)
		add_custom_target(xjni-test-jar ALL
			DEPENDS ${XJNI_TEST_JAR}
		)
	endif()

	# C test library
	add_library(xjni_test SHARED
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringarray_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_arrayfield_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringbuffer_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringbuilder_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringwriter_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_stringreader_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni2d_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_va_list_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_va_list_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_log_test.c
		${CMAKE_SOURCE_DIR}/test/c/xjni_test.c
	)
	target_include_directories(xjni_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)
	target_include_directories(xjni_test PUBLIC ${JNI_INCLUDE_DIRS})
	if (WIN32)
		target_include_directories(xjni_test PRIVATE
			${WIN_JAVA_HOME}/include
			${WIN_JAVA_HOME}/include/win32
		)
	endif()
	target_link_libraries(xjni_test PRIVATE xjni)

	set(LIBS_TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/lib")
	set_target_properties(xjni_test PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
	)

	# Java test targets
	foreach(TESTCLASS TestStringArray ArrayFieldTest Array2DTest
		TestXJNI TestStringBuilder TestStringWriter TestStringReader TestStringBuffer TestXJNIPrintf
		TestXJNILOG)
		add_custom_target(run_${TESTCLASS}
			COMMAND ${_JAVA_CMD} -Djava.library.path=${LIBS_TEST_OUTPUT_DIR} -cp ${JAR_OUTPUT_DIR}/xjni-test.jar ${TESTCLASS}
			WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
			DEPENDS xjni xjni_test xjni-test-jar
			COMMENT "Running Java test ${TESTCLASS}"
		)
		add_custom_target(${TESTCLASS}_test_run ALL DEPENDS run_${TESTCLASS})
	endforeach()
endif()
