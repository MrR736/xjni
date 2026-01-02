# WLD — Windows Libraries Downloader
# Distributed under the OSI-approved BSD 3-Clause License.

cmake_minimum_required(VERSION 3.19)
project(WLDL NONE)

include(Platform/Windows)
include(Platform/Windows-windres)

if(CMAKE_C_COMPILER_LINKER_ID STREQUAL "GNU"
   OR CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	include(Platform/Windows-GNU-C)
	include(Platform/Linker/Windows-GNU-C)
else()
	include(Platform/Windows-MSVC-C)
	include(Platform/Linker/Windows-MSVC-C)
endif()

if(CMAKE_CXX_COMPILER_LINKER_ID STREQUAL "GNU"
   OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	include(Platform/Windows-GNU-CXX)
	include(Platform/Linker/Windows-GNU-CXX)
else()
	include(Platform/Windows-MSVC-CXX)
	include(Platform/Linker/Windows-MSVC-CXX)
endif()

# --- Setup cross compilation ---
set(CMAKE_SYSTEM_NAME Windows)

if(NOT DEFINED CMAKE_SIZEOF_VOID_P)
    message(WARNING "CMAKE_SIZEOF_VOID_P not defined; assuming 64-bit build.")
    set(CMAKE_SIZEOF_VOID_P 8)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CROSS_PREFIX i686-w64-mingw32)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CROSS_PREFIX x86_64-w64-mingw32)
endif()

set(CMAKE_RC_COMPILER  ${CROSS_PREFIX}-windres)
set(CMAKE_C_COMPILER   ${CROSS_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_PREFIX}-g++)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER "${CROSS_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_PREFIX}-g++")
set(CMAKE_CXX_COMPILER_AR "${CROSS_PREFIX}-gcc-ar")
set(CMAKE_CXX_COMPILER_RANLIB "${CROSS_PREFIX}-gcc-ranlib")
set(CMAKE_CXX_COMPILER_LINKER "${CROSS_PREFIX}-ld")
set(CMAKE_AR "${CROSS_PREFIX}-ar")
set(CMAKE_RANLIB "${CROSS_PREFIX}-ranlib")
set(CMAKE_OBJDUMP "${CROSS_PREFIX}-objdump")
set(CMAKE_OBJCOPY "${CROSS_PREFIX}-objcopy")
set(CMAKE_READELF "${CROSS_PREFIX}-readelf")
set(CMAKE_NM "${CROSS_PREFIX}-nm")
set(CMAKE_LINKER "${CROSS_PREFIX}-ld")
set(CMAKE_STRIP "${CROSS_PREFIX}-strip")
set(CMAKE_ADDR2LINE "${CROSS_PREFIX}-addr2line")
set(CMAKE_DLLTOOL "${CROSS_PREFIX}-dlltool")
set(CMAKE_DLLWRAP "${CROSS_PREFIX}-dllwrap")
set(CMAKE_RC_COMPILER "${CROSS_PREFIX}-windres")

# --- JSON parsing ---
set(WLDL_JSON_PATH "${CMAKE_SOURCE_DIR}/cmake/wldl.json")
if(NOT EXISTS "${WLDL_JSON_PATH}")
    message(FATAL_ERROR "JSON file not found: ${WLDL_JSON_PATH}")
endif()

file(READ "${WLDL_JSON_PATH}" JSON_CONTENT)
string(JSON downloads_length LENGTH "${JSON_CONTENT}" downloads)
if(NOT downloads_length)
    message(FATAL_ERROR "Malformed JSON: 'downloads' array missing.")
endif()

message(STATUS "Found ${downloads_length} download entries")

# --- Output directory ---
set(WLD_DIR "${CMAKE_BINARY_DIR}/WLD")
file(MAKE_DIRECTORY "${WLD_DIR}")

macro(_extract_file package extract_path)
    if(EXISTS "${extract_path}")
        set(${package}_DIR "${extract_path}" CACHE PATH "Main directory for ${package}")
    endif()

    if(EXISTS "${extract_path}/include")
        set(${package}_INCLUDE_DIRS "${extract_path}/include" CACHE PATH "Include directory for ${package}")
    endif()
    message(STATUS "Checking include path: ${${package}_INCLUDE_DIRS}")

    if(EXISTS "${extract_path}/lib/x64" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(${package}_LIB_DIRS "${extract_path}/lib/x64" CACHE PATH "Library directory for ${package}")
    elseif(EXISTS "${extract_path}/lib/x86" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(${package}_LIB_DIRS "${extract_path}/lib/x86" CACHE PATH "Library directory for ${package}")
    elseif(EXISTS "${extract_path}/lib")
        set(${package}_LIB_DIRS "${extract_path}/lib" CACHE PATH "Library directory for ${package}")
    endif()
    message(STATUS "Checking library path: ${${package}_LIB_DIRS}")

    if(EXISTS "${extract_path}/bin/x64" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(${package}_BIN_DIRS "${extract_path}/bin/x64" CACHE PATH "Binary directory for ${package}")
    elseif(EXISTS "${extract_path}/bin/x86" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(${package}_BIN_DIRS "${extract_path}/bin/x86" CACHE PATH "Binary directory for ${package}")
    elseif(EXISTS "${extract_path}/bin")
        set(${package}_BIN_DIRS "${extract_path}/bin" CACHE PATH "Binary directory for ${package}")
    endif()
    message(STATUS "Checking binary path: ${${package}_BIN_DIRS}")
endmacro()

math(EXPR last_index "${downloads_length} - 1")
foreach(i RANGE ${last_index})
    string(JSON package GET "${JSON_CONTENT}" downloads ${i} package)
    string(JSON url_dev GET "${JSON_CONTENT}" downloads ${i} url-dev)

    if(NOT package OR NOT url_dev)
        message(WARNING "Skipping invalid entry #${i} (missing package/url-dev).")
        continue()
    endif()

    message(STATUS "→ Package: ${package}")
    message(STATUS "   URL: ${url_dev}")

    # --- Detect file extension ---
    get_filename_component(filename "${url_dev}" NAME)
    set(zip_path "${WLD_DIR}/${filename}")
    set(extract_path "${WLD_DIR}/${package}")

    if(EXISTS "${extract_path}")
        message(STATUS "   Skipping extraction: ${extract_path} already exists.")
        _extract_file(${package} ${extract_path})
        list(APPEND downloaded_packages "${package}")
        continue()
    endif()

    # --- Download file if missing ---
    if(EXISTS "${zip_path}")
        message(STATUS "   Using cached file: ${zip_path}")
    else()
        message(STATUS "   Downloading ${package}...")
        file(DOWNLOAD "${url_dev}" "${zip_path}" SHOW_PROGRESS STATUS dl_status LOG dl_log)
        list(GET dl_status 0 status_code)
        if(NOT status_code EQUAL 0)
            message(WARNING "   Download failed for ${package}: ${dl_log}")
            continue()
        endif()
    endif()

    # --- Extract archive ---
    file(MAKE_DIRECTORY "${extract_path}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xvf "${zip_path}"
        WORKING_DIRECTORY "${extract_path}"
        RESULT_VARIABLE extract_result
    )

    if(NOT extract_result EQUAL 0)
        message(WARNING "   Extraction failed for ${package}")
        continue()
    endif()

    # --- Flatten top-level directory if needed ---
    file(GLOB top_level "${extract_path}/*")
    list(LENGTH top_level num_items)
    if(num_items EQUAL 1)
        list(GET top_level 0 first_item)
        if(IS_DIRECTORY "${first_item}")
            file(GLOB children "${first_item}/*")
            foreach(child IN LISTS children)
                get_filename_component(child_name "${child}" NAME)
                if(IS_DIRECTORY "${child}")
                    execute_process(
                        COMMAND ${CMAKE_COMMAND} -E copy_directory "${child}" "${extract_path}/${child_name}"
                    )
                else()
                    execute_process(
                        COMMAND ${CMAKE_COMMAND} -E copy "${child}" "${extract_path}/${child_name}"
                    )
                endif()
            endforeach()
            execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${first_item}")
        endif()
    endif()

    message(STATUS "   Extracted to ${extract_path}")

    # --- Source and binary directories ---
    _extract_file(${package} ${extract_path})

    list(APPEND downloaded_packages "${package}")
endforeach()

message(STATUS "✅ Completed downloads: ${downloaded_packages}")
