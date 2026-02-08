# xjni - Extern JNI Utility

**xjni** is a lightweight C/C++ JNI utility library that simplifies working with Java arrays, 2D arrays, and string arrays from native code. It provides helper functions for:

- Accessing and modifying Java primitive arrays (`int[][]`, `byte[][]`, etc.)
- Working with Java `String[][]` arrays in native code
- Throwing Java exceptions from C/C++ code
- Versioning and utility functions

---

## Features

* Access and release functions for Java primitive 2D arrays:
  `int[][]`, `byte[][]`, `long[][]`, `float[][]`, `double[][]`, `short[][]`, `char[][]`, `boolean[][]`
* Access and release functions for Java `String[][]` arrays
* **Argument array utilities (`xjni_args.h`)**:

  * Create, append, insert, replace, delete, and retrieve Java arguments (`jargs_t`)
* **Formatted printing utilities (`xjni_va_list.h`)**:

  * Print Java strings and `jargs_t` arrays to buffers, FILE streams, file descriptors, or stdout
* **UTF-16 `jchar` printf utilities (`xjni_printf.h`)**:

  * Print formatted `jchar` strings to buffers, FILE streams, file descriptors, or stdout
* **Logging utilities (`xjni_log.h`)**:

  * Log messages with priority, automatic file/line tagging, and optional colors
* Exception helpers for Java exceptions from native code
* Supports both **shared (`.so`)** and **static (`.a`)** libraries
* Tested with multiple native/Java test cases

---

## Installation

1. Clone the repository:

```bash
git clone https://github.com/MrR736/xjni.git
cd xjni
````

2. Build with CMake:

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

---

## Usage

Include the main header in your native code:

```c
#include <xjni.h>
```

### Example: Modify 2D int array

```c
JNIEXPORT void JNICALL Java_Array2DTest_nativeTest(JNIEnv *env, jobject obj,jobjectArray intArr) {
    jint **ints = GetInt2DArrayElements(env, intArr, NULL);

    ints[0][0] = 42;
    ints[1][2] = 99;

    ReleaseInt2DArrayElements(env, intArr, ints, 0);
}
```

### Example: Throw Java exception

```c
throwIOException(env, "MyTag", "Something went wrong");
throwJava(env, "MyTag", "Custom message", "java/lang/RuntimeException");
```

---

## Java Test Classes

* `TestStringArray.java` – tests string array utilities
* `ArrayFieldTest.java` – tests array field access
* `Array2DTest.java` – tests 2D array access and modification

Run tests via CMake targets:

```bash
cmake --build . --target xjni_stringarray_test_run
cmake --build . --target xjni_arrayfield_test_run
cmake --build . --target xjni2d_test_run
```

---

## Versioning

Current version: **1.0.8**

Compile-time version macro:

```c
_XJNI_VERSION        // Encoded as 1008
_XJNI_VERSION_MAJOR  // 1
_XJNI_VERSION_MINOR  // 0
_XJNI_VERSION_PATCH  // 8
```

Runtime version function:

```c
const char* ver = xjni_version();
printf("xjni version: %s\n", ver);
```

---

## License

This project is licensed under the **GNU General Public License v3.0**. See [LICENSE](LICENSE) for details.

---

## Author

**MrR736** – [MrR736@users.github.com](mailto:MrR736@users.github.com)
