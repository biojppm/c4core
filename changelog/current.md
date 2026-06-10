- [PR#169](https://github.com/biojppm/c4core/pull/169) atof()/atod():
  - disable assertions that prevent returning false
  - c4/std/std.hpp: remove tuple.hpp
- [PR#171](https://github.com/biojppm/c4core/pull/171):
  - fix warnings from names with underscores (clang: -Wreserved-identifier)
  - c4/std/std.hpp: remove tuple.hpp
- [PR#172](https://github.com/biojppm/c4core/pull/172):
  - [**BREAKING**] `C4_LIKELY()` and `C4_UNLIKELY()` now use `[[likely]]` and `[[unlikely]]` in C++20. Every use of this macro needs to refactored:
    ```c++
    if(C4_LIKELY(condition)) ... // error now
    // needs to be rewritten as:
    if C4_LIKELY(condition)  ...
    // same for C4_UNLIKELY
    ```
  - Add `C4_LIKELY20` and `C4_UNLIKELY20`, which resolve directly into `[[likely]]` and `[[unlikely]]` when C++ is 20 or later, or nothing otherwise.
  - Add C++23 detection (`C4_CPP` and `C4_CPP23`)
