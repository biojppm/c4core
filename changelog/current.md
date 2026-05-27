- [PR#166](https://github.com/biojppm/c4core/pull/166)
  - `substr`:
    - improve SFINAE rules and overloads
    - add member type `value_type`
  - Improve std interop headers
  - `type_name()`:
    - make `noexcept`
    - return a new dedicated type `TypeNameStr`. This spares costly include of span.hpp

