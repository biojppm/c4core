
- Fix [rapidyaml#445](https://github.com/biojppm/biojppm/pull/445): Amalgamate: fix include of `<charconv>`.
- Add `bool from_chars(csubstr s, fmt::overflow_checked_<T> *wrapper)`. There was already is a function receiving `&wrapper`, but `*wrapper` was missing for use with generic code.
- Update fast_float to v6.1.1 ([PR#136](https://github.com/biojppm/c4core/pull/136))
