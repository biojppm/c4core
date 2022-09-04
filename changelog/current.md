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
- Likewise for `ftoa()` and `dtoa()`. Prefer the fastest implementation available: `std::to_chars()`->`snprintf()`.
  - Change the `FTOA_*` enum values and type to save a function call when converting format. From now on, only the symbols of this enum can be relied on; the values or type will change depending on the selected implementation (`std::to_chars()` or `snprintf()`) ([PR#91](https://github.com/biojppm/c4core/pull/91)).
- Fix [#84](https://github.com/biojppm/c4core/issues/84): `csubstr::compare(char)`: refactor to avoid false-positive warning from VS2022.
- `csubstr` methods: add `noexcept` and annotations `C4_PURE` and `C4_ALWAYS_INLINE`
- `csubstr`: add `C4_RESTRICT` to incoming string on `csubstr::compare()`
- `csubstr::first_real_span()` ([PR#89](https://github.com/biojppm/c4core/pull/89)):
  - Refactor to fix number matching rules. Now fully valid for floating point numbers in decimal (eg `0.123/1.23e+01`), hexadecimal (eg `0x123.abc/0x1.23abcp+01`), binary (eg `0b101.10/0b1.0110p+01`) and octal format (eg `0o701.10/0o7.0110p+01`) , with or without exponent or power, in lower or upper case.
  - Also, make the number parsing stateful to fix cases where repeated characters occur, (like e.g. `0.1.0` or `1.23e+e10`) which are no longer reported as numbers (see [biojppm/rapidyaml#291](https://github.com/biojppm/rapidyaml/issues/291)).
- `csubstr::first_int_span()`, `csubstr::first_uint_span()`: fix edge cases like e.g. `0xzz` which were wrongly reported as numbers.
- Add fully qualified ARM detection macros:
  - `__ARM_ARCH_7EM__` ([PR#90](https://github.com/biojppm/c4core/pull/90)).
  - `__ARM_ARCH_6KZ__` ([PR#93](https://github.com/biojppm/c4core/pull/93)).
- Improve linux and unix platform detection: detect both `__linux` and `__linux__` ([PR#92](https://github.com/biojppm/c4core/pull/92)).


### Thanks

- @mlondono74
- @musicinmybrain
- @pkubaj
