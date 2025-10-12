# Fix 4 — Bound trailing-zero scan to avoid out-of-bounds when `(3n+1) == 2^N`

**Ease:** ★★★☆☆ (small but subtle loop guard)  
**Impact:** Prevents undefined behavior / crashes on rare edge cases.

## Context
In `apply_reduction()`:
```cpp
void apply_reduction() {
    multiplyByThreeAndAddOne();
    // divide by 2 until it's odd by removing all trailing zeros
    size_t num_zeros = 0;
    while (!bits.test(num_zeros)) {
        ++num_zeros;
    }
    bits >>= num_zeros;
}
```
This scans for trailing zeros without a **bound check**.

## Problem
After `(3n+1)` modulo `2^N` arithmetic, it is possible that the lower **N** bits are **all zero** (e.g., when `(3n+1) == 2^N`). In that case `bits.test(num_zeros)` is `false` for every `num_zeros` in `[0, N-1]`, and the loop walks **past the bitset size**, invoking undefined behavior.

## Minimal Change (recommended)
Add a bound and handle the “all zeros” case by setting the odd result to `1`:

```diff
--- a/src/BigBinary.h
+++ b/src/BigBinary.h
@@
 void apply_reduction() {
     multiplyByThreeAndAddOne();
     // divide by 2 until it's odd by removing all trailing zeros
     size_t num_zeros = 0;
-    while (!bits.test(num_zeros)) {
-        ++num_zeros;
-    }
-    bits >>= num_zeros;
+    while (num_zeros < N_BITS && !bits.test(num_zeros)) {
+        ++num_zeros;
+    }
+    if (num_zeros == N_BITS) {
+        // (3n+1) == 2^N  ⇒ resulting odd value is 1
+        bits.reset();
+        bits.set(0);
+    } else {
+        bits >>= num_zeros;
+    }
 }
```

## Test Plan
1. Temporarily set small `N_BITS` and brute-force all odd inputs to try to hit `(3n+1) == 2^N`.
2. Run under AddressSanitizer (`-fsanitize=address`) to confirm no out-of-bounds access occurs.
