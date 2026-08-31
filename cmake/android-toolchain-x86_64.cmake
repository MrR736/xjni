# Android toolchain for x86_64

set(CMAKE_SYSTEM_NAME Android)

# Path to your NDK
set(ANDROID_NDK /lib/android-ndk)

# Target ABI
set(ANDROID_ABI x86_64)

# Android API level (adjust if needed)
set(ANDROID_PLATFORM android-21)

# Use clang (required for modern NDKs)
set(ANDROID_TOOLCHAIN clang)

# STL choice (c++_shared is most common)
set(ANDROID_STL c++_shared)

# Tell CMake where the official Android toolchain file is
set(CMAKE_TOOLCHAIN_FILE
    ${ANDROID_NDK}/build/cmake/android.toolchain.cmake
    CACHE FILEPATH "Android toolchain file"
)
