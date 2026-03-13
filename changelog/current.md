[PR#156](https://github.com/biojppm/c4core/pull/156):
  - Add traits class `c4::is_writeable_string<>`
  - `c4/std/*.hpp` adapters: add missing forward declarations of `c4::is_string<>`
  - Add default-less `csubstr::copy_from()` to simplify implementations of `to_chars()`/`from_chars()`
  - Minor internal changes in c4::substr
