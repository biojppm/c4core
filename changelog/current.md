
### New features

- Add amalgamation into a single header file ([PR #48](https://github.com/biojppm/c4core/pull/48)):
  - The amalgamated header will be available together with the deliverables from each release.
  - To generate the amalgamated header:
    ```
    $ python tools/amalgamate.py c4core_all.hpp
    ```
  - To use the amalgamated header:
    - Include at will in any header of your project.
    - In one - and only one - of your project source files, `#define C4CORE_SINGLE_HDR_DEFINE_NOW` and then `#include <c4core_all.hpp>`. This will enable the function and class definitions in the header file. For example, here's a sample program:
      ```c++
      #include <iostream>
      #define C4CORE_SINGLE_HDR_DEFINE_NOW // do this before the include
      #include <c4core_all.hpp>
      int main()
      {
          for(c4::csubstr s : c4::csubstr("a/b/c/d").split('/'))
              std::cout << s << "\n";
      }
      ```


### Fixes

- Fix edge cases with empty strings in `span::first()`, `span::last()` and `span::range()`  ([PR #49](https://github.com/biojppm/c4core/pull/49).
- Add `substr::is_unsigned_integer()` and `substr::is_real()` ([PR #49](https://github.com/biojppm/c4core/pull/49).
- Accept octal numbers in `substr::first_real_span()` and `substr::is_real()` ([PR #49](https://github.com/biojppm/c4core/pull/49).
- `substr`: fix coverage misses in number query methods ([PR #49](https://github.com/biojppm/c4core/pull/49).
