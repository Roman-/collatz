# Fix 5 — Exact overflow detection for `3n+1` and carry-aware addition

**Ease:** ★★☆☆☆ (a bit more code, but still local)  
**Impact:** Eliminates false “overflow” exits; ensures correctness near the headroom limit.

## Context
Current overflow guard in `multiplyByThreeAndAddOne()`:
```cpp
if (bits.test(N_BITS - 1) || bits.test(N_BITS - 2)) {
    report_and_exit("Overflow when tried to apply 3x+1", 0, EXIT_CODE_OVERFLOW);
}
bits <<= 1;       // 2*n
bits.set(0);      // 2*n + 1
add(original_bits); // + n  => 3*n + 1
```
Checking the top two bits **before** the operation is neither necessary nor sufficient to detect overflow of `3n+1` within `N_BITS`.

## Problem
- **False positives:** Some values with bit `N-2` set still fit in `N_BITS` after computing `3n+1`.  
- **False negatives (in theory):** Even if the top two bits are clear, `2n + 1 + n` can overflow during the final add.

## Minimal Change (recommended)
Implement **carry-aware addition** and detect overflow exactly on the shift and on the final add.

```diff
--- a/src/BigBinary.h
+++ b/src/BigBinary.h
@@
 struct BigBinary {
     std::bitset<N_BITS> bits;
+
+    // Returns true if there was a carry out of the most-significant bit
+    bool add_with_carry(const BigBinary& b) {
+        bool carry = false;
+        for (size_t i = 0; i < N_BITS; ++i) {
+            const bool a = bits[i];
+            const bool bb = b.bits[i];
+            const bool sum = a ^ bb ^ carry;
+            carry = (a & bb) | (carry & (a ^ bb));
+            bits[i] = sum;
+        }
+        return carry;
+    }
@@
 void multiplyByThreeAndAddOne() {
-    // the only place that's checked for overflow
-    if (bits.test(N_BITS - 1) || bits.test(N_BITS - 2)) {
-        report_and_exit("Overflow when tried to apply 3x+1", 0, EXIT_CODE_OVERFLOW);
-    }
     const auto original_bits = *this; // copy
-    bits <<= 1; // 2 * num. Last bit is now zero
+    const bool shift_overflow = bits.test(N_BITS - 1); // shifting left would drop MSB
+    bits <<= 1; // 2 * num. Last bit is now zero
     bits.set(0); // 2 * num + 1. last bit is one
-    add(original_bits); // 3 * num
+    const bool add_overflow = add_with_carry(original_bits); // + num => 3*num + 1
+    if (shift_overflow || add_overflow) {
+        report_and_exit("Overflow when tried to apply 3x+1", 0, EXIT_CODE_OVERFLOW);
+    }
 }
```

*(Optional) Clean-up:* You can re-implement the older `add()` to delegate to `add_with_carry()` and ignore the return value, or remove it if unused.

## Test Plan
1. Add unit tests that:
   - Construct values near the headroom boundary and verify that `multiplyByThreeAndAddOne()` only reports overflow when the **true** result exceeds `N_BITS`.
   - Check a value with bit `N-2` set where `3n+1` still fits, and confirm no false overflow.
2. Run long searches to ensure no spurious exits occur.
