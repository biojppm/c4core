### New features

- Added `xtoa()` floating-point overloads accepting precision and format:
  ```c++
  size_t xtoa(substr s,  float v, int precision, RealFormat_e formatting=FTOA_FLEX) noexcept;
  size_t xtoa(substr s, double v, int precision, RealFormat_e formatting=FTOA_FLEX) noexcept;
  ```

### Fixes
- `atof()` and `atod()`: ensure hexadecimal floats are accepted. The current fast_float implementation does not accept hexadecimal floats, so we resorted to `fscanf()` or `std::from_chars()` if the standard is c++17 or higher.
