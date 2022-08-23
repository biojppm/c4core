### Fixes
- `atof()` and `atod()`: ensure hexadecimal floats are accepted. The current fast_float implementation does not accept hexadecimal floats, so we resorted to `fscanf()` or `std::from_chars()` if the standard is c++17 or higher.
