# Fix 1 — Remove C++20-only designated initializer (keep project at C++17)

**Ease:** ★★★★★ (one-line change)  
**Impact:** Prevents build failures with `-std=c++17` (and on compilers that don't accept designated initializers)

## Context
The project’s `CMakeLists.txt` sets:
```cmake
set(CMAKE_CXX_STANDARD 17)
```
but `src/main.cpp` uses a **designated initializer**:
```cpp
BigBinary num_with_max_num_steps = {.bits = 0};
```
Designated initializers for aggregates are **C++20**, not C++17. Many toolchains will refuse to build this.

## Problem
With `-std=c++17`, `main.cpp` may fail to compile due to the designated initializer, even though everything else is C++17.

## Minimal Change (recommended)
Rely on default initialization (all-zero bitset):
```diff
--- a/src/main.cpp
+++ b/src/main.cpp
@@
-    BigBinary num_with_max_num_steps = {.bits = 0};
+    BigBinary num_with_max_num_steps{};
```

## Alternative (if you really want C++20)
Update CMake (but this is heavier than the one-line code change):
```diff
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@
-set(CMAKE_CXX_STANDARD 17)
+set(CMAKE_CXX_STANDARD 20)
```

## Test Plan
1. Clean build directory.
2. Configure and build with `-DCMAKE_BUILD_TYPE=Release`.
3. Confirm the project compiles under C++17.
