
[PR#154](https://github.com/biojppm/c4core/pull/154):
  - Add `c4::is_string<T>` trait in `c4/substr.hpp`, and appropriate definitions for `std::string`, `std::string_view` and `std::vector<char>`. Default `c4::dump_directly<T>` to `c4::is_string<T>`.
  - Minor cleanup to `c4::srcloc` in `c4/error.hpp`
  - Minor implementation changes in `c4::mem_repeat()`
  - Suppress false-positive null-dereference warnings for GCC >6
  - Update fast_float to 8.2.4
