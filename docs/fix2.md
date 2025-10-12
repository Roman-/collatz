# Fix 2 — Make `Helpers.h` ODR-safe and C++17-friendly

**Ease:** ★★★★☆ (few-line header edit)  
**Impact:** Avoids multiple-definition linker errors; improves portability to strict C++17 builds.

## Context
`src/Helpers.h` defines a **non-`inline`** function in a header:
```cpp
std::string format_round_number(uint64_t number) {
    std::array rounds{1'000'000'000'000'000ULL, 1'000'000'000'000ULL, 1'000'000'000ULL, 1'000'000ULL, 1'000ULL};
    std::array suffixes{"Q", "T", "B", "M", "k"};
    ...
}
```
When a non-`inline` function with external linkage is defined in a header and that header is included by multiple translation units, the linker may see **multiple identical definitions** (ODR violation).

Additionally, the use of class template argument deduction (CTAD) for `std::array` can be brittle across compilers/standards.

## Problem
- Potential **ODR** (One Definition Rule) violation at link time as the project grows.
- Subtle C++17 portability issues due to CTAD on `std::array`.

## Minimal Change (recommended)
- Mark the function `inline`.  
- Use explicit `std::array` types to be C++17-friendly.  
- Include `<cstdint>` for `std::uint64_t`.

```diff
--- a/src/Helpers.h
+++ b/src/Helpers.h
@@
 #pragma once
 #include <string>
 #include <array>
+#include <cstdint>
-std::string format_round_number(uint64_t number) {
-    std::array rounds{1'000'000'000'000'000ULL, 1'000'000'000'000ULL, 1'000'000'000ULL, 1'000'000ULL, 1'000ULL};
-    std::array suffixes{"Q", "T", "B", "M", "k"};
+inline std::string format_round_number(std::uint64_t number) {
+    const std::array<std::uint64_t, 5> rounds{
+        1'000'000'000'000'000ULL, 1'000'000'000'000ULL, 1'000'000'000ULL, 1'000'000ULL, 1'000ULL
+    };
+    const std::array<const char*, 5> suffixes{"Q", "T", "B", "M", "k"};
     for (size_t i = 0; i < rounds.size(); ++i) {
         if (number % rounds[i] == 0) {
             return std::to_string(number / rounds[i]) + suffixes[i];
         }
     }
     return std::to_string(number);
 }
```

## Test Plan
1. Include `Helpers.h` from at least two `.cpp` files (e.g., `main.cpp` and a new small test TU).
2. Build in Release and Debug; verify no multiple-definition errors.
