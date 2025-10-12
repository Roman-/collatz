# Fix 3 — Reliable file saving & `$HOME` expansion in `BigBinary::report_and_exit`

**Ease:** ★★★★☆ (small refactor in one header)  
**Impact:** Truthful logging; results actually saved where the message claims; fewer surprises in production runs.

## Context
In `src/BigBinary.h`:
```cpp
void save_to_file(const std::string& path) const {
    try {
        std::ofstream file(path);
        file << *this;
    } catch (std::exception& e) {
        std::cerr << "failed to save to file: " << e.what() << std::endl;
    }
    std::cout << "saved result to " << path << std::endl;
}

void report_and_exit(...) const {
    ...
    save_to_file("/tmp/" + filename);
    save_to_file("$HOME/" + filename);
    exit(exit_code);
}
```
- `std::ofstream` does **not throw** unless exceptions are enabled; the `try/catch` will not trigger on open errors.  
- `$HOME` is **not expanded** by `std::ofstream`; it will literally try to create a folder named `$HOME`.

## Problem
- The program prints “saved result” even when the file was not opened/written.  
- The `$HOME` path is wrong and silently fails.

## Minimal Change (recommended)
Make the header **self-contained** (add missing includes) and fix saving & home expansion:

```diff
--- a/src/BigBinary.h
+++ b/src/BigBinary.h
@@
 #pragma once
 #include <bitset>
+#include <string>
+#include <fstream>
+#include <iostream>
+#include <cstdlib>
 #include "Config.h"
@@
 void save_to_file(const std::string& path) const {
-    try {
-        std::ofstream file(path);
-        file << *this;
-    } catch (std::exception& e) {
-        std::cerr << "failed to save to file: " << e.what() << std::endl;
-    }
-    std::cout << "saved result to " << path << std::endl;
+    std::ofstream file(path);
+    if (!file) {
+        std::cerr << "failed to open '" << path << "' for write\n";
+        return;
+    }
+    file << *this;
+    std::cout << "saved result to " << path << std::endl;
 }
@@
 void report_and_exit(const char* msg, size_t num_steps, int exit_code) const {
     const auto stars = std::string(120, '*');
     std::cout << stars <<  "\n" << msg << '\n' << stars << "\nNumber:\n" << *this;
     if (num_steps > 0) {
         std::cout << "\nnum_steps: " << num_steps;
     }
     std::cout << '\n' << stars << std::endl;
     const std::string filename = "collatz_result_" + std::to_string(rand() % 10000) + ".txt";
     save_to_file("/tmp/" + filename);
-    save_to_file("$HOME/" + filename);
+    if (const char* home = std::getenv("HOME")) {
+        save_to_file(std::string(home) + "/" + filename);
+    } else {
+        std::cerr << "HOME not set; skipping user directory save\n";
+    }
     exit(exit_code);
 }
```

*(Optional)* Consider adding a timestamp or PID to the filename to reduce collision risk.

## Test Plan
1. Run the program and force an exit path (e.g., temporarily lower `N_BITS` to trigger overflow).
2. Verify that one file appears under `/tmp/` and one under your real home directory.
3. Observe truthful error messages if a path cannot be opened.
