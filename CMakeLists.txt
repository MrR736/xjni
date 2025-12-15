cmake_minimum_required(VERSION 3.20)
project(xjni VERSION 1.0.0 LANGUAGES C CXX)

include(CMakePackageConfigHelpers)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Output directories
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}")
set(LIBS_OUTPUT_DIR "${OUTPUT_DIR}/lib")
set(TEST_OUTPUT_DIR "${OUTPUT_DIR}/test")
set(LIBS_TEST_OUTPUT_DIR "${TEST_OUTPUT_DIR}/lib")
set(JAR_OUTPUT_DIR "${TEST_OUTPUT_DIR}/jar")
set(HEADER_OUTPUT_DIR "${TEST_OUTPUT_DIR}/headers")
file(MAKE_DIRECTORY "${LIBS_OUTPUT_DIR}" "${TEST_OUTPUT_DIR}" "${LIBS_TEST_OUTPUT_DIR}" "${JAR_OUTPUT_DIR}" "${HEADER_OUTPUT_DIR}")

set(XJNI_SOURCES
	${CMAKE_SOURCE_DIR}/src/xjni_arrayfield.c
	${CMAKE_SOURCE_DIR}/src/xjni_stringarray.c
	${CMAKE_SOURCE_DIR}/src/xjni2d.c
	${CMAKE_SOURCE_DIR}/src/xjni.c
)

# Build shared library
add_library(${PROJECT_NAME} SHARED ${XJNI_SOURCES})

# Set library version (for Unix)
if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
	)
endif()

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
	LIBRARY_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
)

# Include directories
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_SOURCE_DIR}/include
	PUBLIC
		${JNI_INCLUDE_DIRS}
)

# Build static library
add_library(${PROJECT_NAME}_static STATIC ${XJNI_SOURCES})

# Output directories for static library
set_target_properties(${PROJECT_NAME}_static PROPERTIES
	OUTPUT_NAME ${PROJECT_NAME}
	ARCHIVE_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
)

# Include directories
target_include_directories(${PROJECT_NAME}_static
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
		${CMAKE_SOURCE_DIR}/include
	PUBLIC
		${JNI_INCLUDE_DIRS}
)

# Link JNI if needed
target_link_libraries(${PROJECT_NAME}_static PUBLIC JNI::JNI)

target_link_libraries(${PROJECT_NAME} PUBLIC JNI::JNI)
if(NOT WIN32)
	# -------------------
	# Install Targets
	# -------------------

	# Install library
	install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_static
		EXPORT ${PROJECT_NAME}Targets
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)

	# Install headers
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
		DESTINATION include
		FILES_MATCHING PATTERN "*.h"
	)

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	configure_package_config_file(
		"${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
	)

	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		DESTINATION lib/cmake/${PROJECT_NAME}
	)

	export(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_static FILE ${PROJECT_NAME}Targets.cmake)

	# Optional: install CMake config for find_package()
	install(EXPORT ${PROJECT_NAME}Targets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION lib/cmake/${PROJECT_NAME}
	)

	if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake")
		add_custom_target(uninstall
			COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake
			COMMENT "Uninstalling ${PROJECT_NAME}"
		)
	endif()
endif()

add_jar(
	xjni-test-jar
		${CMAKE_SOURCE_DIR}/test/java/TestStringArray.java
		${CMAKE_SOURCE_DIR}/test/java/ArrayFieldTest.java
		${CMAKE_SOURCE_DIR}/test/java/Array2DTest.java
	VERSION ${PROJECT_VERSION}
	OUTPUT_NAME "xjni-test"
	OUTPUT_DIR "${JAR_OUTPUT_DIR}"
	GENERATE_NATIVE_HEADERS xjni-test-native
		DESTINATION "${HEADER_OUTPUT_DIR}"
)

add_library(xjni_test SHARED
	${CMAKE_SOURCE_DIR}/test/c/xjni_stringarray_test.c
	${CMAKE_SOURCE_DIR}/test/c/xjni_arrayfield_test.c
	${CMAKE_SOURCE_DIR}/test/c/xjni2d_test.c
)
target_include_directories(xjni_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)
target_include_directories(xjni_test PUBLIC ${JNI_INCLUDE_DIRS})
target_link_libraries(xjni_test PRIVATE xjni)
set_target_properties(xjni_test PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
	LIBRARY_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${LIBS_TEST_OUTPUT_DIR}
)

add_custom_target(run_xjni_stringarray_test
	COMMAND
		${Java_JAVA_EXECUTABLE} -Djava.library.path=${LIBS_TEST_OUTPUT_DIR}
		-cp ${JAR_OUTPUT_DIR}/xjni-test.jar TestStringArray
	WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
	DEPENDS ${PROJECT_NAME}
	COMMENT "Running Java test class xjni-test for TestStringArray"
)
add_custom_target(xjni_stringarray_test_run ALL DEPENDS run_xjni_stringarray_test)



add_custom_target(run_xjni_arrayfield_test
	COMMAND
		${Java_JAVA_EXECUTABLE} -Djava.library.path=${LIBS_TEST_OUTPUT_DIR}
		-cp ${JAR_OUTPUT_DIR}/xjni-test.jar ArrayFieldTest
	WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
	DEPENDS ${PROJECT_NAME}
	COMMENT "Running Java test class xjni-test for ArrayFieldTest"
)
add_custom_target(xjni_arrayfield_test_run ALL DEPENDS run_xjni_arrayfield_test)



add_custom_target(run_xjni2d_test
	COMMAND
		${Java_JAVA_EXECUTABLE} -Djava.library.path=${LIBS_TEST_OUTPUT_DIR}
		-cp ${JAR_OUTPUT_DIR}/xjni-test.jar Array2DTest
	WORKING_DIRECTORY "${JAR_OUTPUT_DIR}"
	DEPENDS ${PROJECT_NAME}
	COMMENT "Running Java test class xjni-test for Array2DTest"
)
add_custom_target(xjni2d_test_run ALL DEPENDS run_xjni2d_test)
