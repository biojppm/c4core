### Breaking changes

- [PR#111](https://github.com/biojppm/c4core/pull/111) - Rename formatting overloads accepting `c4::append`:
   - `catrs(append_t, ...) -> catrs_append(...)`
   - `catseprs(append_t, ...) -> catseprs_append(...)`
   - `formatrs(append_t, ...) -> formatrs_append(...)`


### New features

- [PR#105](https://github.com/biojppm/c4core/pull/105): Add macros in `c4/language.hpp` for compile-time flow of exceptions:
  - `C4_EXCEPTIONS`: defined when exceptions are enabled
  - `C4_IF_EXCEPTIONS(code_with_exc, code_without_exc)`: select statements for exceptions enabled/disabled
  - `C4_IF_EXCEPTIONS_(code_with_exc, code_without_exc)`: select code tokens for exceptions enabled/disabled
- [PR#105](https://github.com/biojppm/c4core/pull/105): Add macros in `c4/language.hpp` for compile-time flow of RTTI:
  - `C4_RTTI`: defined when rtti is enabled
  - `C4_IF_RTTI(code_with_rtti, code_without_rtti)`: select statements for rtti enabled/disabled
  - `C4_IF_RTTI_(code_with_rtti, code_without_rtti)`: select code tokens for rtti enabled/disabled


### Fixes

- [PR#110](https://github.com/biojppm/c4core/pull/110)/[PR#107](https://github.com/biojppm/c4core/pull/107) - Update fast_float.
- [PR#106](https://github.com/biojppm/c4core/pull/106) - Fix include guard in the gcc 4.8 compatibility header, causing it to be missing from the amalgamated header.
- [PR#105](https://github.com/biojppm/c4core/pull/105) - Fix existing throw in `c4/ext/sg14/inplace_function.h`. Ensure tests run with exceptions disabled and RTTI disabled. Add examples of exceptional control flow with `setjmp()/std::longjmp()`.
- [PR#104](https://github.com/biojppm/c4core/pull/104) - Fix pedantic warnings in gcc, clang and MSVC
- [PR#104](https://github.com/biojppm/c4core/pull/104) - Fix possible compile error when `__GNUC__` is not defined
