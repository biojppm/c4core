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
- [PR#109](https://github.com/biojppm/c4core/pull/109): Add partial support for XTENSA processors (missing implementation of `c4::aalloc()`). See [rapidyaml#358](https://github.com/biojppm/rapidyaml/issues/358).


### Fixes

- [PR#115](https://github.com/biojppm/c4core/pull/115) - Refactor of `c4::blob`/`c4::cblob`. Use SFINAE to invalidate some of the constructors.
- [PR#110](https://github.com/biojppm/c4core/pull/110)/[PR#107](https://github.com/biojppm/c4core/pull/107) - Update fast_float.
- [PR#108](https://github.com/biojppm/c4core/pull/108) - Fix preprocessor pasting of strings in `C4_NOT_IMPLEMENTED_MSG()` and `C4_NOT_IMPLEMENTED_IF_MSG()`.
- [PR#106](https://github.com/biojppm/c4core/pull/106) - Fix include guard in the gcc 4.8 compatibility header, causing it to be missing from the amalgamated header.
- [PR#105](https://github.com/biojppm/c4core/pull/105) - Fix existing throw in `c4/ext/sg14/inplace_function.h`. Ensure tests run with exceptions disabled and RTTI disabled. Add examples of exceptional control flow with `setjmp()/std::longjmp()`.
- [PR#104](https://github.com/biojppm/c4core/pull/104)/[PR#112](https://github.com/biojppm/c4core/pull/112) - Fix pedantic warnings in gcc, clang and MSVC
- [PR#104](https://github.com/biojppm/c4core/pull/104) - Fix possible compile error when `__GNUC__` is not defined
- Inject explicit `#include <charconv>` on the amalgamated header. The amalgamation tool was filtering all prior includes, thus causing a compilation error. Addresses [rapidyaml#364](https://github.com/biojppm/rapidyaml/issues/364).
