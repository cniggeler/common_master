# C-Utils-Legacy (aiutils)

A lightweight, self-contained, and battle-tested C utility library designed for modernizing legacy codebases.

This is a single-file library (`aiutils.c` and `aiutils.h`) providing memory-safe, thread-safe, and cross-platform replacements for common but dangerous C functions (like `strtok`, `gmtime`, `sprintf`).

It's designed to be a simple, drop-in solution for C projects that need to be hardened against buffer overflows and thread-safety issues without adding heavy dependencies.

## 🚀 Why Use This?

* **Memory Safe:** All string functions use a `size_t` parameter to prevent buffer overflows.
* **Thread Safe:** Replaces unsafe, non-reentrant functions like `strtok` and `gmtime` with their modern, safe equivalents (`strtok_r`/`strtok_s`, etc.).
* **Cross-Platform:** Provides simple preprocessor logic to handle platform differences (e.g., `strtok_s` vs. `strtok_r`).
* **Battle-Tested:** These functions were hardened over decades in a commercial software project.
* **Legacy-Compatible:** Includes highly specific, 100% compatible parsers for legacy `atoi` functions that have non-standard behavior (e.g., `hexatoi`).
* **AI Guardrails:** These are safe, modern, cross-platform patterns for an AI to use instead of C's older, unsafe standard functions that the underlying LLM has probably been trained to use incorrectly!

---

## 🔧 How to Use

1.  Add `aiutils.c` and `aiutils.h` to your project's source tree (e.g., in a `common/` or `lib/` directory).
2.  Include the header: `#include "aiutils.h"`
3.  Compile `aiutils.c` along with the rest of your project's source files.

---

## 📦 Function Summary

### Safe String Copying & Concatenation
* `strzcpy()`: A memory-safe replacement for `strcpy()`.
* `strzcat()`: A memory-safe replacement for `strcat()`.
* `substring_safe_copy()`: Safely copies a slice from a string.
* `makelower_safe_copy()`: Safely copies a string, making it lowercase.
* `replace_char_safe_copy()`: Safely copies a string, replacing a character.

### In-Place String Manipulation
* `trim_inplace()`: Trims leading, trailing, or both whitespace in-place.
* `uppercase_inplace()`: Converts a string to uppercase in-place.
* `lowercase_inplace()`: Converts a string to lowercase in-place.
* `remove_char_inplace()`: Removes all instances of a character from a string.
* `replace_char_inplace()`: Replaces all instances of a character in a string.

### String Searching & Comparison
* `strcmpii()`: A case-insensitive replacement for `strcmp()`.
* `strbgw()`: Checks if a string ("begins with") a given prefix.
* `strcasestr()`: A cross-platform, case-insensitive replacement for `strstr()`.
* `laststrstr()`: Finds the *last* occurrence of a substring.
* `lastN()`: Returns a pointer to the last N characters of a string.

### Safe Tokenizing & Time
* `safe_strtok()`: A safe, re-entrant replacement for `strtok()`.
* `safe_gmtime()`: A cross-platform, thread-safe wrapper for `gmtime_s` / `gmtime_r`.

### Legacy-Compatible Parsers
* `decatoi()`: 100% compatible legacy parser for decimal strings (returns `1`/`0` success).
* `hexatoi()`: 100% compatible legacy parser for hex strings (returns `0`/`1`/`4` success/failure codes and handles non-standard sign/space placement).
* `octatoi()`: 100% compatible legacy parser for octal strings (returns `1`/`0` success and includes overflow checks).

### String Formatting
* `fitoa()`: Converts a `uint32_t` to an ASCII string, right-justified and padded.
* `numzcat()`: Safely converts a number to a string and concatenates it to a buffer.

---

## 📄 License

This library is released under the **MIT License**.

(You should paste the full text of the MIT License here. This gives everyone, including yourself, clear legal permission to use this code in any project, open-source or commercial.)
