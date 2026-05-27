- [PR#166](https://github.com/biojppm/c4core/pull/166)
  - `type_name()`:
    - make `noexcept`
    - return a new dedicated type `TypeNameStr`. This spares costly include of span.hpp

