# xjni – External JNI Utility

**xjni** is a lightweight, high-performance C/C++ utility library designed to simplify interaction with Java through the Java Native Interface (JNI). It provides structured helpers for working with Java arrays (including multidimensional arrays), strings, argument construction, formatted output, logging, threading, caching, and low-level portability utilities.

The library is suitable for both embedded JNI runtimes and larger native integrations where safety, performance, and maintainability are priorities.

---

## Key Features

### 1. Java 2D Array Utilities (`xjni2d.h`)

Full access and release helpers for:

* `int[][]`
* `byte[][]`
* `long[][]`
* `float[][]`
* `double[][]`
* `short[][]`
* `char[][]`
* `boolean[][]`
* `String[][]`

Includes safe acquisition and release patterns for native-side modification.

---

### 2. Argument Array Utilities (`xjni_args.h`)

Structured `jargs_t` abstraction for building `jobjectArray` arguments:

* Append, insert, replace, remove operations
* Primitive support: `int`, `long`, `byte`, `short`, `float`, `double`, `boolean`
* UTF-8 string helpers
* Automatic boxing (e.g., `java.lang.Integer`, `java.lang.Byte`, etc.)
* Correct C default promotion handling in variadic APIs

---

### 3. Formatted Output & Variadic Utilities

#### `xjni_va_list.h`

* Safe formatted printing into:

  * Buffers
  * `FILE*`
  * File descriptors
  * `stdout`
* Supports both `jstring` and UTF-8 formats
* Safe `jobjectArray`-based argument formatting

#### `xjni_printf.h`

* UTF-16 (`jchar`) printf-style utilities
* Buffer, file, descriptor, and console output support

---

### 4. String & Encoding Utilities (`xjni_string.h`)

Robust UTF handling:

* UTF-8 ↔ UTF-16 (`jchar`)
* UTF-8 ↔ `wchar_t` (UTF-16 on Windows, UTF-32 on POSIX)
* Surrogate pair support
* Resilient against invalid sequences

Includes:

* `jstrlen`, `jstrcpy`, `jstrcat`, `jstrdup`, `jstrncmp`
* `jstrreverse`, `jstrrev`
* JNI-compatible memory helpers (`jmemcpy`, `jmemmove`, etc.)

All returned dynamic buffers must be released using `free()`.

---

### 5. Logging Utilities (`xjni_log.h`)

Portable logging layer with:

* Android-style priority levels
* Automatic file and line tagging
* Colored output (non-Android platforms)
* `printf`-style formatting

---

### 6. JNI Class Cache (`xjni_classcache.h`)

Thread-safe LRU class cache to reduce repeated `FindClass()` calls:

* Internal synchronization
* Automatic eviction policy
* Designed for high-frequency JNI environments

---

### 7. Thread Utilities (`xjni_thread.h`)

Cross-platform threading support:

* Mutex abstraction
* One-time initialization (`xjni_once`, `xjni_oncew`)
* Thread create/join/detach/self
* Cancellation type/state management
* Realtime clock retrieval

---

### 8. Hash Map Utility (`xjni_hashmap.h`)

Thread-safe open-addressing hash map:

* Linear probing
* Tombstone-based deletion
* Automatic resizing and cleanup
* Optional internal mutex
* Atomic-style replace and compare-and-swap support

Designed for lightweight runtime infrastructure usage.

---

### 9. Portability & Compile-Time Utilities (`xjnidef.h`)

* CPU detection (x86/x64/ARM/MIPS)
* SIMD detection (SSE4.2, AVX, AVX2)
* Compiler builtin wrappers
* Branch prediction hints
* Popcount / CLZ helpers
* Compile-time C/C++ version checks
* Type compatibility checks (C & C++)
* `arrayof()` macro and other utility helpers

---

### 10. Memory Pool (`xjni_pool`)

* Growable slab allocator
* Optional lock-free freelist
* High-concurrency optimized
* Debug-hardened mode (poisoning + double-free detection)

---

## Installation

```bash
git clone https://github.com/MrR736/xjni.git
cd xjni
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

---

## Basic Usage

Include the main header:

```c
#include <xjni.h>
```

### Example: Modify 2D int array

```c
JNIEXPORT void JNICALL
Java_Array2DTest_nativeTest(JNIEnv *env, jobject obj, jobjectArray intArr)
{
    jint **ints = GetInt2DArrayElements(env, intArr, NULL);

    ints[0][0] = 42;
    ints[1][2] = 99;

    ReleaseInt2DArrayElements(env, intArr, ints, 0);
}
```

---

### Example: Throw Java Exception

```c
throwIOException(env, "MyTag", "Something went wrong");
throwJava(env, "MyTag", "Custom message", "java/lang/RuntimeException");
```

---

## Testing

CMake test targets:

```bash
cmake --build . --target xjni_stringarray_test_run
cmake --build . --target xjni_arrayfield_test_run
cmake --build . --target xjni2d_test_run
```

---

## Versioning

Current version: **1.1.2**

### Compile-Time Macros

```c
_XJNI_VERSION        // 1101
_XJNI_VERSION_MAJOR  // 1
_XJNI_VERSION_MINOR  // 1
_XJNI_VERSION_PATCH  // 2
```

### Runtime Query

```c
const char* ver = xjni_version();
printf("xjni version: %s\n", ver);
```

---

## License

Licensed under the **GNU General Public License v3.0**.

---

## Author

**MrR736**
[MrR736@users.github.com](mailto:MrR736@users.github.com)
