
- Amalgamate: fix include of `<charconv>` (see [rapidyaml#445](https://github.com/biojppm/biojppm/pull/445)).
- Add `C4_MINGW` ([PR#139](https://github.com/biojppm/c4core/pull/139))
- Annotate `c4::handle_error()` with `[[noreturn]]` ([PR#137](https://github.com/biojppm/c4core/pull/137)).
- Add `bool from_chars(csubstr s, fmt::overflow_checked_<T> *wrapper)`. There was already a function receiving `&wrapper`, but `*wrapper` was missing for use with generic code.
- Ensure `posix_memalign()` is never called with bad alignment values ([PR#138](https://github.com/biojppm/c4core/pull/138))
- Update fast_float to v6.1.1 ([PR#136](https://github.com/biojppm/c4core/pull/136))


### Thanks

- @toge
