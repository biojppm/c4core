### New features

- `charconv.hpp`: added `xtoa()` floating-point overloads accepting precision and format ([PR#88](https://github.com/biojppm/c4core/pull/88)):
  ```c++
  size_t xtoa(substr s,  float v, int precision, RealFormat_e formatting=FTOA_FLEX) noexcept;
  size_t xtoa(substr s, double v, int precision, RealFormat_e formatting=FTOA_FLEX) noexcept;
  ```
- `memory_util.hpp`: added `ipow()` overloads for computing powers with integral exponents ([PR#88](https://github.com/biojppm/c4core/pull/88)).


### Fixes

- `atof()` and `atod()` ([PR#88](https://github.com/biojppm/c4core/pull/88)):
  - Always use the fastest implementation available: `std::from_chars()` if available (C++17 or higher standard, with later compilers), `fast_float::from_chars()` otherwise. On Visual Studio, `fast_float::from_chars()` is preferred over `std::from_chars()`.
  - If `std::from_chars()` is not available and `C4CORE_NO_FAST_FLOAT` is defined, then the fallback is based on `sscanf()`.
  - Ensure hexadecimal floats are accepted. The current fast_float implementation does not accept hexadecimal floats, so an hexfloat scanner was added.
- Fix [#84](https://github.com/biojppm/c4core/issues/84): `csubstr::compare(char)`: refactor to avoid false-positive warning from VS2022.
