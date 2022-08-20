### Changes

Improved the performance of `c4/charconv.hpp` functions ([PR#77](https://github.com/biojppm/c4core/pull/77)):
  - Added `digits_dec/hex/oct/bin()`.
  - Optimized `write_dec/hex/oct/bin()`:
    - these functions now return immediately without entering the loop if the output buffer is smaller than respectively `digits_dec/hex/oct/bin()`. This enables both:
      - writing every character in its final position without having to revert the string at the end
      - the need to check the buffer size on appending every character.
    - `write_dec()` now writes two digits at once, thus halving the number of integer divisions.
  - Added `write_dec/hex/oct/bin_unchecked()`, which receive precomputed `digits_dec/hex/oct/bin()`, thus speeding up the radix `itoa()/utoa()` overloads.
  - Added `xtoa()` radix+digits overloads:
    - `size_t xtoa(substr s, T v, T radix)`
    - `size_t xtoa(substr s, T v, T radix, size_t num_digits)`
  - `read_dec/hex/oct/bin()`: these functions no longer allow an empty input buffer.
  - Use intrinsic functions `__builtin_clz()` (gcc) / `_BitScanReverse()` (msvc) in `c4::msb()` and `__builtin_ctz()` (gcc) / `_BitScanForward()` (msvc) in `c4::lsb()` when they are available. `msb()` is used by `digits_hex()/digits_bin()`.
  - Refactored the charconv tests to improve consistency and thoroughness.
  - Improved the charconv benchmarks to ensure full consistency across benchmarks.
  - Special thanks and kudos to @fargies for being attentive and pinpointing several issues throughout the PR!
  - Finding the best approach involved [writing a R&D benchmark for the several algorithm components](https://github.com/biojppm/c4core/tree/master/bm/bm_xtoa.cpp). This benchmark is disabled by default, and can be enabled with the flag `C4CORE_BM_XTOA_RND`.
  - With the changes from this PR, the [charconv benchmark](https://github.com/biojppm/c4core/tree/master/bm_charconv.cpp) results show that on Linux/g++11.2, with integral types:
    - `c4::to_chars()` can be expected to be roughly...
      - ~40% to 2x faster than `std::to_chars()`
      - ~10x-30x faster than `sprintf()`
      - ~50x-100x faster than a naive `stringstream::operator<<()` followed by `stringstream::str()`
    - `c4::from_chars()` can be expected to be roughly...
      - ~10%-30% faster than `std::from_chars()`
      - ~10x faster than `scanf()`
      - ~30x-50x faster than a naive `stringstream::str()` followed by `stringstream::operator>>()`
  - Here are the results from the run:
    | Write throughput         |         | Read throughput          |          |
    |:-------------------------|--------:|:-------------------------|---------:|
    |  **write `uint8_t`**     | **MB/s**| **read `uint8_t`**       |  **MB/s**|
    | `c4::to_chars<u8>`       |  526.86 |  `c4::from_chars<u8>`    |   163.06 |
    | `std::to_chars<u8>`      |  379.03 |  `std::from_chars<u8>`   |   154.85 |
    | `std::sprintf<u8>`       |   20.49 |  `std::scanf<u8>`        |    15.75 |
    | `std::stringstream<u8>`  |    3.82 |  `std::stringstream<u8>` |     3.83 |
    |  **write `int8_t`**      | **MB/s**| **read `int8_t`**        |  **MB/s**|
    | `c4::to_chars<i8>`       |  599.98 |  `c4::from_chars<i8>`    |   184.20 |
    | `std::to_chars<i8>`      |  246.32 |  `std::from_chars<i8>`   |   156.40 |
    | `std::sprintf<i8>`       |   19.15 |  `std::scanf<i8>`        |    16.44 |
    | `std::stringstream<i8>`  |    3.83 |  `std::stringstream<i8>` |     3.89 |
    |  **write `uint16_t`**    | **MB/s**| **read `uint16_t`**      |  **MB/s**|
    | `c4::to_chars<u16>`      |  486.40 |  `c4::from_chars<u16>`   |   349.48 |
    | `std::to_chars<u16>`     |  454.24 |  `std::from_chars<u16>`  |   319.13 |
    | `std::sprintf<u16>`      |   38.74 |  `std::scanf<u16>`       |    28.12 |
    | `std::stringstream<u16>` |    7.08 |  `std::stringstream<u16>`|     6.73 |
    |  **write `int16_t`**     | **MB/s**| **read `int16_t`**       |  **MB/s**|
    | `c4::to_chars<i16>`      |  507.44 |  `c4::from_chars<i16>`   |   282.95 |
    | `std::to_chars<i16>`     |  297.49 |  `std::from_chars<i16>`  |   186.18 |
    | `std::sprintf<i16>`      |   39.03 |  `std::scanf<i16>`       |    28.45 |
    | `std::stringstream<i16>` |    6.98 |  `std::stringstream<i16>`|     6.49 |
    |  **write `uint32_t`**    | **MB/s**| **read `uint32_t`**      |  **MB/s**|
    | `c4::to_chars<u32>`      |  730.12 |  `c4::from_chars<u32>`   |   463.95 |
    | `std::to_chars<u32>`     |  514.76 |  `std::from_chars<u32>`  |   329.42 |
    | `std::sprintf<u32>`      |   71.19 |  `std::scanf<u32>`       |    44.97 |
    | `std::stringstream<u32>` |   14.05 |  `std::stringstream<u32>`|    12.57 |
    |  **write `int32_t`**     | **MB/s**| **read `int32_t`**       |  **MB/s**|
    | `c4::to_chars<i32>`      |  618.76 |  `c4::from_chars<i32>`   |   345.53 |
    | `std::to_chars<i32>`     |  394.72 |  `std::from_chars<i32>`  |   224.46 |
    | `std::sprintf<i32>`      |   71.14 |  `std::scanf<i32>`       |    43.49 |
    | `std::stringstream<i32>` |   13.91 |  `std::stringstream<i32>`|    12.03 |
    |  **write `uint64_t`**    | **MB/s**| **read `uint64_t`**      |  **MB/s**|
    | `c4::to_chars<u64>`      | 1118.87 |  `c4::from_chars<u64>`   |   928.49 |
    | `std::to_chars<u64>`     |  886.58 |  `std::from_chars<u64>`  |   759.03 |
    | `std::sprintf<u64>`      |  140.96 |  `std::scanf<u64>`       |    91.60 |
    | `std::stringstream<u64>` |   28.01 |  `std::stringstream<u64>`|    25.00 |
    |  **write `int64_t`**     | **MB/s**| **read `int64_t`**       |  **MB/s**|
    | `c4::to_chars<i64>`      | 1198.78 |  `c4::from_chars<i64>`   |   713.76 |
    | `std::to_chars<i64>`     |  882.17 |  `std::from_chars<i64>`  |   646.18 |
    | `std::sprintf<i64>`      |  138.79 |  `std::scanf<i64>`       |    90.07 |
    | `std::stringstream<i64>` |   27.62 |  `std::stringstream<i64>`|    25.12 |

If you feel suspicious about these bold claims, you can browse through [c4core's CI benchmark results](https://github.com/biojppm/c4core/actions/workflows/benchmarks.yml) which will hopefully give these more substance.


### New features

- Added `bool c4::overflows<T>(csubstr s)` for detecting whether a string overflows a given integral type. See [PR#78](https://github.com/biojppm/c4core/pull/78).
  - Also, added `c4::fmt::overflow_checked()` (and the corresponding `from_chars()` overload) to enable a check for overflow before parsing from string:
  ```c++
  c4::from_chars(str, &val); // no overflow check
  c4::from_chars(str, c4::fmt::overflow_checked(val)); // enable overflow check
  // as an example, the implementation looks like:
  template<class T>
  bool c4::from_chars(c4::csubstr str, c4::fmt::overflow_checked<T> oc)
  {
      if(overflows<T>(str))
          return false;
      return c4::from_chars(str, oc.val);
  }
  ```

### Fixes

- Fix missing endianess macro on windows arm/arm64 compilations [PR #76](https://github.com/biojppm/c4core/pull/76)
- Add missing `#define` for the include guard of the amalgamated header (see [rapidyaml#246](https://github.com/biojppm/rapidyaml/issues/246)).
- Fix CPU detection with ARMEL [PR #86](https://github.com/biojppm/c4core/pull/86).
- Fix GCC version detection [PR #87](https://github.com/biojppm/c4core/pull/87).
- Fix [cmake#8](https://github.com/biojppm/cmake/issues/8): `SOVERSION` missing from shared libraries.
- Update fastfloat to 3.5.1.

### Thanks

- @fargies
- @daichifukui
- @janisozaur
