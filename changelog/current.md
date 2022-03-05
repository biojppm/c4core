### Changes

- Improve performance of charconv `xtoa` functions by 10%-20% for integral types by returning early when the buffer may not fit the type ([PR#77](https://github.com/biojppm/c4core/pull/77)).
  - `write_dec()`, `write_hex()`, `write_bin()`, `write_oct()`: these functions now return early if the output buffer is smaller than each type's largest possible number, and return the size required to accomodate that largest number. For example, with `int8_t` the largest stringified decimal number is `-128` (note that largest is understood not in magnitude but in the size of its string representation), so `write_dec()` now returns 4 if the output buffer does not have at least that size - even if the input number is 6 (which only takes a single character in decimal format). If the buffer size is larger or equal to 4, then the returned size is the actually used size (in this example, 1). So repeated calls to `write_dec()`and friends with the same input value will return a size that will diminish from the max possible size if the buffer size is not enough to the actually used size once the buffer is large enough to accomodate any possible value for that type.
  - This improves the performance because it eliminates the need to check the buffer size on appending every character. It enables a single buffer size check at the beginning of the function; thereupon, every character can be added at will with full confidence that the output buffer can accomodate the character.
  - This change has subtle implications in functions such as `catrs()` or `formatrs()`: now `write_dec()` et al (and all the functions which use any of these as a building block) now may return different sizes depending in the input buffer size, so `catrs()` and friends must resize their output buffers with the latest size obtained.
  - Add `C4_ALWAYS_INLINE` to these functions and `atoi()`, `atou()`, `atox()`, `itoa()`, `utoa()`, `xtoa()` to improve performance. Previously there was a large performance gap in benchmarks where `read_dec()` and friends were much faster (anywhere between 3x and 20x faster) than `utoa()` and friends. Adding `C4_ALWAYS_INLINE` to `utoa()` al brought their performances to similar levels as `read_dec()` et al. See [eg this thread](https://github.com/biojppm/c4core/pull/77#issuecomment-1059019667) for some example figures before and after the improvement.
  - Improved the charconv benchmarks to ensure full consistency across benchmarks, to ensure comparison of apples-to-apples.

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
  - Thanks to @fargies for the significant work.

### Thanks

- @fargies
