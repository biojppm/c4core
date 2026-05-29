#include "c4/test.hpp"
#ifndef C4CORE_SINGLE_HEADER
#include "c4/std/string.hpp"
#include "c4/std/vector.hpp"
#include "c4/format.hpp"
#include "c4/base64.hpp"
#include "c4/format_base64.hpp"
#include "c4/span.hpp"
#endif

#include "c4/libtest/supprwarn_push.hpp"

#include <cstring>

C4_SUPPRESS_WARNING_MSVC_WITH_PUSH(4310)  // cast truncates constant value

namespace c4 {

namespace detail {
void base64_test_tables();
TEST_CASE("base64.infrastructure")
{
    #ifndef NDEBUG
    detail::base64_test_tables();
    #endif
}

// Since some the macros in c4/cpu.cpp cannot identify endianness at
// compile time, we use a simple runtime endianness-detection routine.
bool is_little_endian()
{
    unsigned long const v = 1UL;
    unsigned char b[sizeof(v)];
    std::memcpy(&b[0], &v, sizeof(v));
    return !!b[0];
}
} // namespace detail

csubstr native(csubstr little_endian, csubstr big_endian)
{
    return detail::is_little_endian() ? little_endian : big_endian;
}

template<class T>
void test_base64_tag_functions()
{
    size_t len = 10;
    T c = 'a';
    T      &  ref = c;
    T const& cref = c;
    T *ptr = &c;
    const T * cptr = &c;
    const T * const cptrc = &c;
    static_assert(!fmt::detail::has_resize<decltype(c)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(c)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(c)&&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(ref)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(ref)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(ref)&&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(cref)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(cref)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(cref)&&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*ptr)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*ptr)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*ptr)&&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*cptr)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*cptr)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*cptr)&&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*cptrc)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*cptrc)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(*cptrc)&&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(ptr)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(ptr)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(ptr)&&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(cptr)>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(cptr)&>::value, "??");
    static_assert(!fmt::detail::has_resize<decltype(cptr)&&>::value, "??");
    size_t reqsize = 0;
    static_assert(std::is_same<decltype(fmt:: base64(c)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(c)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(c).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(c).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(c).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(c).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(c).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(c).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(c, &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(c, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(c, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(c, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(c, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(c, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(c, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(c, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(ref)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(ref)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(ref).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(ref).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(ref).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(ref).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(ref).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(ref).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(ref, &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(ref, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(ref, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(ref, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(ref, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(ref, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(c, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(c, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(cref)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cref)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cref).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cref).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cref).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(cref).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(cref).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(cref).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(cref, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cref, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cref, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cref, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cref, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(cref, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(cref, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(cref, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(*ptr)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*ptr)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*ptr).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*ptr).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*ptr).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*ptr).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*ptr).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(*ptr).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(*ptr, &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*ptr, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*ptr, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*ptr, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*ptr, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*ptr, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*ptr, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(*ptr, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(*cptr)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*cptr)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*cptr).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*cptr).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*cptr).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*cptr).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*cptr).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(*cptr).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(*cptr, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*cptr, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*cptr, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*cptr, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*cptr, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*cptr, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*cptr, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(*cptr, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(*cptrc)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*cptrc)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*cptrc).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*cptrc).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*cptrc).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*cptrc).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*cptrc).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(*cptrc).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(*cptrc, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*cptrc, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*cptrc, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*cptrc, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*cptrc, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*cptrc, &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*cptrc, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(*cptrc, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(ptr, size_t(1))), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(ptr, size_t(1))), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(ptr, size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(ptr, size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(ptr, size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(ptr, size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(ptr, size_t(1)).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(ptr, size_t(1)).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(ptr, size_t(1), &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(ptr, size_t(1), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(ptr, size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(ptr, size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(ptr, size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(ptr, size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(ptr, size_t(1), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(ptr, size_t(1), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(cptr, size_t(1))), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptr, size_t(1))), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptr, size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptr, size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptr, size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(cptr, size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(cptr, size_t(1)).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(cptr, size_t(1)).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(cptr, size_t(1), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptr, size_t(1), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptr, size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptr, size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptr, size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(cptr, size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(cptr, size_t(1), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(cptr, size_t(1), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(cptrc, size_t(1))), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptrc, size_t(1))), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptrc, size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptrc, size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptrc, size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(cptrc, size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(cptrc, size_t(1)).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(cptrc, size_t(1)).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(cptrc, size_t(1), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptrc, size_t(1), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptrc, size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptrc, size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptrc, size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(cptrc, size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(cptrc, size_t(1), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(cptrc, size_t(1), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(ptr, len)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(ptr, len)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(ptr, len).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(ptr, len).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(ptr, len).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(ptr, len).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(ptr, len).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(ptr, len).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(ptr, len, &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(ptr, len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(ptr, len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(ptr, len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(ptr, len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(ptr, len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(ptr, len, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(ptr, len, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(cptr, len)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptr, len)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptr, len).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptr, len).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptr, len).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(cptr, len).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(cptr, len).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(cptr, len).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(cptr, len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptr, len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptr, len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptr, len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptr, len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(cptr, len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(cptr, len, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(cptr, len, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(cptrc, len)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptrc, len)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptrc, len).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptrc, len).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptrc, len).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(cptrc, len).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(cptrc, len).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(cptrc, len).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(cptrc, len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cptrc, len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(cptrc, len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(cptrc, len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(cptrc, len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(cptrc, len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(cptrc, len, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(cptrc, len, &reqsize).required_size, &reqsize);
    //
    // test with tmp values (rvalues)
    auto tmplen = [&]{return len;};
    auto tmpval = [&]() -> T& {return c;};
    auto tmpptr = [&]() -> T* {return &c;};
    auto tmpcval = [&]() -> T const& {return c;};
    auto tmpcptr = [&]() -> T const* {return &c;};
    static_assert(std::is_same<decltype(fmt:: base64(tmpval())), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpval())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpval()).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpval()).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpval()).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpval()).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpval()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpval()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpval(), &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpval(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpval(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpval(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpval(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpval(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpval(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpval(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(tmpcval())), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcval())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcval()).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcval()).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcval()).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcval()).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcval()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpcval()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpcval(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcval(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcval(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcval(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcval(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcval(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcval(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpcval(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(*tmpptr())), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*tmpptr())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*tmpptr()).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*tmpptr()).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*tmpptr()).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*tmpptr()).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*tmpptr()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(*tmpptr()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(*tmpptr(), &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*tmpptr(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*tmpptr(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*tmpptr(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*tmpptr(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*tmpptr(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*tmpptr(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(*tmpptr(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(*tmpcptr())), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*tmpcptr())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*tmpcptr()).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*tmpcptr()).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*tmpcptr()).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*tmpcptr()).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*tmpcptr()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(*tmpcptr()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(*tmpcptr(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(*tmpcptr(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(*tmpcptr(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(*tmpcptr(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(*tmpcptr(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(*tmpcptr(), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(*tmpcptr(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(*tmpcptr(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(tmpptr(), 1)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpptr(), 1)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpptr(), size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpptr(), size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpptr(), size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpptr(), size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpptr(), size_t(1)).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpptr(), size_t(1)).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpptr(), 1, &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpptr(), 1, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpptr(), size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpptr(), size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpptr(), size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpptr(), size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpptr(), size_t(1), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpptr(), size_t(1), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(tmpcptr(), 1)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcptr(), 1)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcptr(), size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcptr(), size_t(1)).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcptr(), size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcptr(), size_t(1)).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcptr(), size_t(1)).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpcptr(), size_t(1)).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpcptr(), 1, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcptr(), 1, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcptr(), size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcptr(), size_t(1), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcptr(), size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcptr(), size_t(1), &reqsize).data.len, sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcptr(), size_t(1), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpcptr(), size_t(1), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(tmpptr(), len)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpptr(), len)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpptr(), len).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpptr(), len).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpptr(), len).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpptr(), len).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpptr(), len).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpptr(), len).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpptr(), len, &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpptr(), len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpptr(), len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpptr(), len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpptr(), len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpptr(), len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpptr(), len, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpptr(), len, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(tmpcptr(), len)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcptr(), len)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcptr(), len).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcptr(), len).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcptr(), len).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcptr(), len).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcptr(), len).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpcptr(), len).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpcptr(), len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcptr(), len, &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcptr(), len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcptr(), len, &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcptr(), len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcptr(), len, &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcptr(), len, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpcptr(), len, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(tmpptr(), tmplen())), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpptr(), tmplen())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpptr(), tmplen()).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpptr(), tmplen()).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpptr(), tmplen()).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpptr(), tmplen()).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpptr(), tmplen()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpptr(), tmplen()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpptr(), tmplen(), &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpptr(), tmplen(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpptr(), tmplen(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpptr(), tmplen(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpptr(), tmplen(), &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpptr(), tmplen(), &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpptr(), tmplen(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpptr(), tmplen(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(tmpcptr(), tmplen())), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcptr(), tmplen())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcptr(), tmplen()).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcptr(), tmplen()).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcptr(), tmplen()).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcptr(), tmplen()).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcptr(), tmplen()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(tmpcptr(), tmplen()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(tmpcptr(), tmplen(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(tmpcptr(), tmplen(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(tmpcptr(), tmplen(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt::cbase64(tmpcptr(), tmplen(), &reqsize).data.buf, (void const*)&c);
    CHECK_EQ(fmt:: base64(tmpcptr(), tmplen(), &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt::cbase64(tmpcptr(), tmplen(), &reqsize).data.len, len * sizeof(T));
    CHECK_EQ(fmt:: base64(tmpcptr(), tmplen(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(tmpcptr(), tmplen(), &reqsize).required_size, &reqsize);
    //
    const std::vector<T> cvec(len);
    std::vector<T> vec(len);
    std::vector<T> &vec_ref = vec;
    const std::vector<T> &cvec_ref = vec;
    static_assert(fmt::detail::has_resize<std::vector<T>>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(cvec)>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(vec)>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(vec_ref)>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(cvec_ref)>::value, "??");
    static_assert(std::is_same<decltype(fmt:: base64(&vec[0], vec.size())), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&vec[0], vec.size())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&vec[0], vec.size()).data.buf, (void const*)vec.data());
    CHECK_EQ(fmt::cbase64(&vec[0], vec.size()).data.buf, (void const*)vec.data());
    CHECK_EQ(fmt:: base64(&vec[0], vec.size()).data.len, vec.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&vec[0], vec.size()).data.len, vec.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&vec[0], vec.size()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(&vec[0], vec.size()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(&vec[0], vec.size(), &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&vec[0], vec.size(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&vec[0], vec.size(), &reqsize).data.buf, (void const*)vec.data());
    CHECK_EQ(fmt::cbase64(&vec[0], vec.size(), &reqsize).data.buf, (void const*)vec.data());
    CHECK_EQ(fmt:: base64(&vec[0], vec.size(), &reqsize).data.len, vec.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&vec[0], vec.size(), &reqsize).data.len, vec.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&vec[0], vec.size(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(&vec[0], vec.size(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(&cvec[0], cvec.size())), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&cvec[0], cvec.size())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&cvec[0], cvec.size()).data.buf, (void const*)cvec.data());
    CHECK_EQ(fmt::cbase64(&cvec[0], cvec.size()).data.buf, (void const*)cvec.data());
    CHECK_EQ(fmt:: base64(&cvec[0], cvec.size()).data.len, cvec.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&cvec[0], cvec.size()).data.len, cvec.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&cvec[0], cvec.size()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(&cvec[0], cvec.size()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(&cvec[0], cvec.size(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&cvec[0], cvec.size(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&cvec[0], cvec.size(), &reqsize).data.buf, (void const*)cvec.data());
    CHECK_EQ(fmt::cbase64(&cvec[0], cvec.size(), &reqsize).data.buf, (void const*)cvec.data());
    CHECK_EQ(fmt:: base64(&cvec[0], cvec.size(), &reqsize).data.len, cvec.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&cvec[0], cvec.size(), &reqsize).data.len, cvec.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&cvec[0], cvec.size(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(&cvec[0], cvec.size(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(&vec_ref[0], vec_ref.size())), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&vec_ref[0], vec_ref.size())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&vec_ref[0], vec_ref.size()).data.buf, (void const*)vec_ref.data());
    CHECK_EQ(fmt::cbase64(&vec_ref[0], vec_ref.size()).data.buf, (void const*)vec_ref.data());
    CHECK_EQ(fmt:: base64(&vec_ref[0], vec_ref.size()).data.len, vec_ref.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&vec_ref[0], vec_ref.size()).data.len, vec_ref.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&vec_ref[0], vec_ref.size()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(&vec_ref[0], vec_ref.size()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(&vec_ref[0], vec_ref.size(), &reqsize)), fmt::      base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&vec_ref[0], vec_ref.size(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&vec_ref[0], vec_ref.size(), &reqsize).data.buf, (void const*)vec_ref.data());
    CHECK_EQ(fmt::cbase64(&vec_ref[0], vec_ref.size(), &reqsize).data.buf, (void const*)vec_ref.data());
    CHECK_EQ(fmt:: base64(&vec_ref[0], vec_ref.size(), &reqsize).data.len, vec_ref.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&vec_ref[0], vec_ref.size(), &reqsize).data.len, vec_ref.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&vec_ref[0], vec_ref.size(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(&vec_ref[0], vec_ref.size(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(&cvec_ref[0], cvec_ref.size())), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&cvec_ref[0], cvec_ref.size())), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&cvec_ref[0], cvec_ref.size()).data.buf, (void const*)cvec_ref.data());
    CHECK_EQ(fmt::cbase64(&cvec_ref[0], cvec_ref.size()).data.buf, (void const*)cvec_ref.data());
    CHECK_EQ(fmt:: base64(&cvec_ref[0], cvec_ref.size()).data.len, cvec_ref.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&cvec_ref[0], cvec_ref.size()).data.len, cvec_ref.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&cvec_ref[0], cvec_ref.size()).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(&cvec_ref[0], cvec_ref.size()).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(&cvec_ref[0], cvec_ref.size(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(&cvec_ref[0], cvec_ref.size(), &reqsize)), fmt::const_base64_wrapper>::value, "??");
    CHECK_EQ(fmt:: base64(&cvec_ref[0], cvec_ref.size(), &reqsize).data.buf, (void const*)cvec_ref.data());
    CHECK_EQ(fmt::cbase64(&cvec_ref[0], cvec_ref.size(), &reqsize).data.buf, (void const*)cvec_ref.data());
    CHECK_EQ(fmt:: base64(&cvec_ref[0], cvec_ref.size(), &reqsize).data.len, cvec_ref.size() * sizeof(T));
    CHECK_EQ(fmt::cbase64(&cvec_ref[0], cvec_ref.size(), &reqsize).data.len, cvec_ref.size() * sizeof(T));
    CHECK_EQ(fmt:: base64(&cvec_ref[0], cvec_ref.size(), &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(&cvec_ref[0], cvec_ref.size(), &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(vec)), fmt::      base64_container_wrapper<std::vector<T>>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(vec)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    CHECK_EQ(fmt:: base64(vec).container, (void const*)&vec);
    CHECK_EQ(fmt::cbase64(vec).container, (void const*)&vec);
    CHECK_EQ(fmt:: base64(vec).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(vec).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(vec, &reqsize)), fmt::      base64_container_wrapper<std::vector<T>>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(vec, &reqsize)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    CHECK_EQ(fmt:: base64(vec, &reqsize).container, (void const*)&vec);
    CHECK_EQ(fmt::cbase64(vec, &reqsize).container, (void const*)&vec);
    CHECK_EQ(fmt:: base64(vec, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(vec, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(cvec)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cvec)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    CHECK_EQ(fmt:: base64(cvec).container, (void const*)&cvec);
    CHECK_EQ(fmt::cbase64(cvec).container, (void const*)&cvec);
    CHECK_EQ(fmt:: base64(cvec).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(cvec).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(cvec, &reqsize)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cvec, &reqsize)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    CHECK_EQ(fmt:: base64(cvec, &reqsize).container, (void const*)&cvec);
    CHECK_EQ(fmt::cbase64(cvec, &reqsize).container, (void const*)&cvec);
    CHECK_EQ(fmt:: base64(cvec, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(cvec, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(vec_ref)), fmt::      base64_container_wrapper<std::vector<T>>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(vec_ref)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    CHECK_EQ(fmt:: base64(vec_ref).container, (void const*)&vec);
    CHECK_EQ(fmt::cbase64(vec_ref).container, (void const*)&vec);
    CHECK_EQ(fmt:: base64(vec_ref).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(vec_ref).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(cvec_ref, &reqsize)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(cvec_ref, &reqsize)), fmt::const_base64_container_wrapper<std::vector<T>>>::value, "??");
    CHECK_EQ(fmt:: base64(cvec_ref, &reqsize).container, (void const*)&vec);
    CHECK_EQ(fmt::cbase64(cvec_ref, &reqsize).container, (void const*)&vec);
    CHECK_EQ(fmt:: base64(cvec_ref, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(cvec_ref, &reqsize).required_size, &reqsize);
}

TEST_CASE("base64.fmt.tag_functions")
{
    test_base64_tag_functions<char>();
    test_base64_tag_functions<int>();
    size_t len = 10;
    std::string str;
    str.resize(len);
    std::string &str_ref = str;
    const std::string const_str;
    const std::string &const_str_ref = str;
    size_t reqsize = 0;
    static_assert(fmt::detail::has_resize<std::string>::value, "??");
    static_assert(fmt::detail::has_resize<std::string &>::value, "??");
    static_assert(fmt::detail::has_resize<std::string &&>::value, "??");
    static_assert(fmt::detail::has_resize<std::string const&>::value, "??");
    static_assert(fmt::detail::has_resize<std::string const&&>::value, "??");
    static_assert(fmt::detail::has_resize<const std::string>::value, "??");
    static_assert(fmt::detail::has_resize<const std::string&>::value, "??");
    static_assert(fmt::detail::has_resize<const std::string&&>::value, "??");
    static_assert(fmt::detail::has_resize<const std::string&>::value, "??");
    static_assert(fmt::detail::has_resize<const std::string&&>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(str)>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(str_ref)>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(const_str)>::value, "??");
    static_assert(fmt::detail::has_resize<decltype(const_str_ref)>::value, "??");
    static_assert(std::is_same<decltype(fmt:: base64(str)), fmt::      base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(str)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(str).container, &str);
    CHECK_EQ(fmt::cbase64(str).container, &str);
    CHECK_EQ(fmt:: base64(str).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(str).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(str, &reqsize)), fmt::      base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(str, &reqsize)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(str, &reqsize).container, &str);
    CHECK_EQ(fmt::cbase64(str, &reqsize).container, &str);
    CHECK_EQ(fmt:: base64(str, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(str, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(str_ref)), fmt::      base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(str_ref)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(str_ref).container, &str);
    CHECK_EQ(fmt::cbase64(str_ref).container, &str);
    CHECK_EQ(fmt:: base64(str_ref).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(str_ref).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(str_ref, &reqsize)), fmt::      base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(str_ref, &reqsize)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(str_ref, &reqsize).container, &str);
    CHECK_EQ(fmt::cbase64(str_ref, &reqsize).container, &str);
    CHECK_EQ(fmt:: base64(str_ref, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(str_ref, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(const_str)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(const_str)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(const_str).container, &const_str);
    CHECK_EQ(fmt::cbase64(const_str).container, &const_str);
    CHECK_EQ(fmt:: base64(const_str).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(const_str).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(const_str, &reqsize)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(const_str, &reqsize)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(const_str, &reqsize).container, &const_str);
    CHECK_EQ(fmt::cbase64(const_str, &reqsize).container, &const_str);
    CHECK_EQ(fmt:: base64(const_str, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(const_str, &reqsize).required_size, &reqsize);
    //
    static_assert(std::is_same<decltype(fmt:: base64(const_str_ref)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(const_str_ref)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(const_str_ref).container, &str);
    CHECK_EQ(fmt::cbase64(const_str_ref).container, &str);
    CHECK_EQ(fmt:: base64(const_str_ref).required_size, nullptr);
    CHECK_EQ(fmt::cbase64(const_str_ref).required_size, nullptr);
    static_assert(std::is_same<decltype(fmt:: base64(const_str_ref, &reqsize)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    static_assert(std::is_same<decltype(fmt::cbase64(const_str_ref, &reqsize)), fmt::const_base64_container_wrapper<std::string>>::value, "??");
    CHECK_EQ(fmt:: base64(const_str_ref, &reqsize).container, &str);
    CHECK_EQ(fmt::cbase64(const_str_ref, &reqsize).container, &str);
    CHECK_EQ(fmt:: base64(const_str_ref, &reqsize).required_size, &reqsize);
    CHECK_EQ(fmt::cbase64(const_str_ref, &reqsize).required_size, &reqsize);
}

TEST_CASE("base64.invalid")
{
    CHECK_EQ(base64_valid(nullptr, size_t(0)), true);
    csubstr enc = "YW55IGNhcm5hbCBwbGVhc3VyZS4=";
    CHECK_EQ(base64_valid(enc.first(0).str, 0), true);
    CHECK_EQ(base64_valid(enc.first(1).str, 1), false);
    CHECK_EQ(base64_valid(enc.first(2).str, 2), false);
    CHECK_EQ(base64_valid(enc.first(3).str, 3), false);
    CHECK_EQ(base64_valid(enc.first(4).str, 4), true);
    CHECK_EQ(base64_valid(enc.first(5).str, 5), false);
    CHECK_EQ(base64_valid(enc.first(6).str, 6), false);
    CHECK_EQ(base64_valid(enc.first(7).str, 7), false);
    CHECK_EQ(base64_valid(enc.first(8).str, 8), true);
    enc = "........?";
    CHECK_EQ(base64_valid(enc.first(0).str, 0), true);
    CHECK_EQ(base64_valid(enc.first(1).str, 1), false);
    CHECK_EQ(base64_valid(enc.first(2).str, 2), false);
    CHECK_EQ(base64_valid(enc.first(3).str, 3), false);
    CHECK_EQ(base64_valid(enc.first(4).str, 4), false);
    CHECK_EQ(base64_valid(enc.first(5).str, 5), false);
    CHECK_EQ(base64_valid(enc.first(6).str, 6), false);
    CHECK_EQ(base64_valid(enc.first(7).str, 7), false);
    CHECK_EQ(base64_valid(enc.first(8).str, 8), false);
    //
    enc = "MA"; CHECK_EQ(base64_valid(enc.str, enc.len), false);
    enc = "MA="; CHECK_EQ(base64_valid(enc.str, enc.len), false);
    enc = "M=A"; CHECK_EQ(base64_valid(enc.str, enc.len), false);
    enc = "=MAA"; CHECK_EQ(base64_valid(enc.str, enc.len), false);
    enc = "==MA"; CHECK_EQ(base64_valid(enc.str, enc.len), false);
}

TEST_CASE("base64.decode.invalid")
{
    char buf_[512];
    substr buf(buf_);
    size_t reqsize = 0;
    // wrong size
    {
        csubstr enc = "YW55IGNhcm5hbCBwbGVhc3VyZS4=";
        CHECK( base64_valid(enc.str, enc.len));
        CHECK(!base64_valid(enc.str, enc.len - 1));
        CHECK(!base64_valid(enc.str, enc.len - 2));
        CHECK(!base64_valid(enc.str, enc.len - 3));
        CHECK( base64_valid(enc.str, enc.len - 4));
        CHECK( base64_decode(enc.str, enc.len    , buf.str, buf.len, &reqsize));
        CHECK(!base64_decode(enc.str, enc.len - 1, buf.str, buf.len, &reqsize));
        CHECK(!base64_decode(enc.str, enc.len - 2, buf.str, buf.len, &reqsize));
        CHECK(!base64_decode(enc.str, enc.len - 3, buf.str, buf.len, &reqsize));
        CHECK( base64_decode(enc.str, enc.len - 4, buf.str, buf.len, &reqsize));
    }
    // wrong characters
    {
        for(std::string enc_ : {
                "Mw==",
                "cGxlYXN1cmUu",
                "ZWFzdXJlLg==",
                "YW55IGNhcm5hbCBwbGVhc3VyZS4=",
            })
        {
            INFO("enc: '" << enc_ << "'");
            substr enc = to_substr(enc_);
            {
                char repl = '?';
                for(size_t i = 0; i < enc.len; ++i)
                {
                    INFO("i=" << i << "  enc.len=" << enc.len);
                    CHECK(base64_valid(enc.str, enc.len));
                    CHECK(base64_decode(enc.str, enc.len, buf.str, buf.len, &reqsize));
                    char c = enc[i];
                    enc[i] = repl;
                    CHECK(!base64_valid(enc.str, enc.len));
                    CHECK(!base64_decode(enc.str, enc.len, buf.str, buf.len, &reqsize));
                    enc[i] = c;
                }
            }
            if(enc.len > 3)
            {
                char repl = '=';
                for(size_t i = 0; i < enc.len - 3; ++i)
                {
                    INFO("i=" << i << "  enc.len=" << enc.len);
                    CHECK(base64_valid(enc.str, enc.len));
                    CHECK(base64_decode(enc.str, enc.len, buf.str, buf.len, &reqsize));
                    char c = enc[i];
                    enc[i] = repl;
                    CHECK(!base64_valid(enc.str, enc.len));
                    CHECK(!base64_decode(enc.str, enc.len, buf.str, buf.len, &reqsize));
                    enc[i] = c;
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void test_base64_roundtrip_basic(void const *data, size_t len, void *data_out, size_t data_out_len, csubstr expected)
{
    char encoded_buf_[512];
    substr encoded_buf(encoded_buf_);
    csubstr encoded;

    REQUIRE_LE(expected.len, encoded_buf.len);

    // null
    {
        size_t needed = base64_encode(nullptr, 0, data, len);
        CHECK_EQ(needed, expected.len);
    }
    // too small
    {
        memset(encoded_buf.str, 0, encoded_buf.len);
        size_t needed = base64_encode(encoded_buf.str, expected.len / 2, data, len);
        CHECK_EQ(needed, expected.len);
    }
    if(expected.len)
    {
        memset(encoded_buf.str, 0, encoded_buf.len);
        size_t needed = base64_encode(encoded_buf.str, expected.len - 1, data, len);
        CHECK_EQ(needed, expected.len);
    }
    // size exact
    {
        memset(encoded_buf.str, 0, encoded_buf.len);
        size_t needed = base64_encode(encoded_buf.str, expected.len, data, len);
        CHECK_EQ(needed, expected.len);
        encoded = encoded_buf.first(expected.len);
        CHECK_EQ(encoded, expected);
        CHECK(base64_valid(expected.str, expected.len));
        CHECK(base64_valid(encoded.str, encoded.len));
    }
    // size larger
    {
        memset(encoded_buf.str, 0, encoded_buf.len);
        REQUIRE_LE(expected.len + 1, encoded_buf.len);
        size_t needed = base64_encode(encoded_buf.str, expected.len + 1, data, len);
        REQUIRE_EQ(needed, expected.len);
        encoded = encoded_buf.first(expected.len);
        CHECK_EQ(encoded, expected);
        CHECK(base64_valid(expected.str, expected.len));
        CHECK(base64_valid(encoded.str, encoded.len));
    }

    REQUIRE_LE(len, data_out_len);
    REQUIRE_GT(data_out_len, 0);

    // null
    {
        size_t needed = 0;
        bool ok = base64_decode(encoded.str, encoded.len, nullptr, 0, &needed);
        CHECK((!ok || encoded.len == 0));
        CHECK_EQ(needed, len);
    }
    // small
    {
        memset(data_out, 0, data_out_len);
        size_t needed = 0;
        bool ok = base64_decode(encoded.str, encoded.len, data_out, len / 2, &needed);
        CHECK((!ok || encoded.len == 0));
        CHECK_EQ(needed, len);
    }
    if(len)
    {
        memset(data_out, 0, data_out_len);
        size_t needed = 0;
        bool ok = base64_decode(encoded.str, encoded.len, data_out, len - 1, &needed);
        CHECK(!ok);
        CHECK_EQ(needed, len);
    }
    // size exact
    {
        memset(data_out, 0, data_out_len);
        size_t needed = 0;
        bool ok = base64_decode(encoded.str, encoded.len, data_out, len, &needed);
        CHECK(ok);
        CHECK_EQ(needed, len);
        CHECK_EQ(0, memcmp(data_out, data, needed));
    }
    // size larger
    {
        memset(data_out, 0, data_out_len);
        size_t needed = 0;
        bool ok = base64_decode(encoded.str, encoded.len, data_out, len + 1, &needed);
        CHECK(ok);
        CHECK_EQ(needed, len);
        CHECK_EQ(0, memcmp(data_out, data, needed));
    }
}

void test_base64_roundtrip_basic_str(csubstr txt, csubstr encoded, substr buf)
{
    INFO("'" << txt << "' => '" << encoded << "'");
    test_base64_roundtrip_basic(txt.str, txt.len, buf.str, buf.len, encoded);
}

template<class T>
void test_base64_roundtrip_basic_val(T const& val, T *workspace, csubstr encoded)
{
    INFO("'" << val << "' => '" << encoded << "'");
    test_base64_roundtrip_basic(&val, sizeof(T), workspace, sizeof(T), encoded);
}


template<class T, class U>
void test_base64_roundtrip_formatted_str(T const& val, csubstr expected, U *ws)
{
    char buf_[512];
    substr buf(buf_);

    // encode----------------------------------------------------------
    {
        size_t len = to_chars(substr{}, fmt::cbase64(val));
        CHECK_EQ(len, expected.len);
    }
    {
        size_t out = 0;
        size_t len = to_chars(substr{}, fmt::cbase64(val, &out));
        CHECK_EQ(len, expected.len);
        CHECK_EQ(out, expected.len);
    }
    {
        buf.fill('^');
        csubstr encoded = to_chars_sub(buf, fmt::cbase64(val));
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    {
        buf.fill('^');
        size_t len = 0;
        csubstr encoded = to_chars_sub(buf, fmt::cbase64(val, &len));
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(len, expected.len);
        CHECK_EQ(encoded, expected);
    }
    {
        buf.fill('^');
        csubstr encoded = to_chars_sub(buf, fmt::base64(val));
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    {
        buf.fill('^');
        size_t len = 0;
        csubstr encoded = to_chars_sub(buf, fmt::base64(val, &len));
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(len, expected.len);
        CHECK_EQ(encoded, expected);
    }
    {
        const std::string container(val.begin(), val.end());
        {
            buf.fill('^');
            csubstr encoded = to_chars_sub(buf, fmt::cbase64(container));
            CHECK(encoded.is_sub(buf));
            CHECK(base64_valid(encoded.str, encoded.len));
            CHECK_EQ(encoded, expected);
        }
        {
            buf.fill('^');
            size_t len = 0;
            csubstr encoded = to_chars_sub(buf, fmt::cbase64(container, &len));
            CHECK_EQ(len, expected.len);
            CHECK(encoded.is_sub(buf));
            CHECK(base64_valid(encoded.str, encoded.len));
            CHECK_EQ(encoded, expected);
        }
        {
            buf.fill('^');
            csubstr encoded = to_chars_sub(buf, fmt::base64(container));
            CHECK(encoded.is_sub(buf));
            CHECK(base64_valid(encoded.str, encoded.len));
            CHECK_EQ(encoded, expected);
        }
        {
            buf.fill('^');
            size_t len = 0;
            csubstr encoded = to_chars_sub(buf, fmt::base64(container, &len));
            CHECK_EQ(len, expected.len);
            CHECK(encoded.is_sub(buf));
            CHECK(base64_valid(encoded.str, encoded.len));
            CHECK_EQ(encoded, expected);
        }
    }
    // now read-----------------------------------------
    {
        size_t len = 0;
        bool ok = from_chars(expected, fmt::base64(ws->data(), ws->size(), &len));
        REQUIRE_LE(len, ws->size());
        CHECK(ok);
        CHECK_EQ(ws->first(len), val);
    }
    {
        size_t len = 0;
        auto req = fmt::base64(ws->data(), ws->size(), &len);
        bool ok = from_chars(expected, &req);
        REQUIRE_LE(len, ws->size());
        CHECK(ok);
        CHECK_EQ(ws->first(len), val);
    }
    {
        size_t len = 0;
        bool ok = from_chars(expected, fmt::base64(*ws, &len));
        REQUIRE_LE(len, ws->size());
        CHECK(ok);
        CHECK_EQ(ws->first(len), val);
    }
    {
        size_t len = 0;
        auto req = fmt::base64(*ws, &len);
        bool ok = from_chars(expected, &req);
        REQUIRE_LE(len, ws->size());
        CHECK(ok);
        CHECK_EQ(ws->first(len), val);
    }
    {
        std::string container;
        bool ok = from_chars(expected, fmt::base64(container));
        CHECK(ok);
        CHECK_EQ(container.size(), val.size());
        CHECK_EQ(container, val);
    }
    {
        std::vector<char> container;
        bool ok = from_chars(expected, fmt::base64(container));
        CHECK(ok);
        CHECK_EQ(container.size(), val.size());
        CHECK_EQ(container, val);
    }
    {
        std::string container;
        auto req = fmt::base64(container);
        bool ok = from_chars(expected, &req);
        CHECK(ok);
        CHECK_EQ(container.size(), val.size());
        CHECK_EQ(container, val);
    }
    {
        std::vector<char> container;
        auto req = fmt::base64(container);
        bool ok = from_chars(expected, &req);
        CHECK(ok);
        CHECK_EQ(container.size(), val.size());
        CHECK_EQ(container, val);
    }
    // ensure the final size is always the decoded size
    {
        std::vector<char> ws_;
        ws_.resize(2 * val.size());
        size_t reqsize = 0;
        CHECK(from_chars(expected, fmt::base64(ws_, &reqsize)));
        REQUIRE_EQ(ws_.size(), val.size());
        CHECK_EQ(reqsize, val.size());
        if(val.size())
            CHECK_EQ(0, memcmp(ws_.data(), val.data(), val.size()));
    }
    {
        std::string ws_;
        ws_.resize(2 * val.size());
        size_t reqsize = 0;
        CHECK(from_chars(expected, fmt::base64(ws_, &reqsize)));
        REQUIRE_EQ(ws_.size(), val.size());
        CHECK_EQ(reqsize, val.size());
        if(val.size())
            CHECK_EQ(0, memcmp(ws_.data(), val.data(), val.size()));
    }
    // fabricate a decode error
    if(val.size())
    {
        std::string invalid(expected.begin(), expected.end());
        REQUIRE_GT(invalid.size(), 0);
        REQUIRE(base64_valid(invalid.data(), invalid.size()));
        invalid.back() = '?';
        REQUIRE(!base64_valid(invalid.data(), invalid.size()));
        {
            // zero initial size
            std::vector<char> decoded;
            CHECK(!from_chars(to_csubstr(invalid), fmt::base64(decoded)));
            decoded.resize(2 * val.size());
            CHECK(!from_chars(to_csubstr(invalid), fmt::base64(decoded)));
        }
        {
            // zero initial size
            std::string decoded;
            CHECK(!from_chars(to_csubstr(invalid), fmt::base64(decoded)));
            decoded.resize(2 * val.size());
            CHECK(!from_chars(to_csubstr(invalid), fmt::base64(decoded)));
        }
    }
}

template<class T>
void test_base64_roundtrip_val(T const* val, size_t len, csubstr expected)
{
    char buf_[512];
    substr buf(buf_);
    REQUIRE(is_aligned(val, alignof(T)));
    if(len == 1)
    {
        buf.fill('^');
        csubstr encoded = to_chars_sub(buf, fmt::cbase64(*val));
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    if(len == 1)
    {
        size_t reqsize = 0;
        buf.fill('^');
        csubstr encoded = to_chars_sub(buf, fmt::cbase64(*val, &reqsize));
        CHECK_EQ(encoded.len, reqsize);
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    {
        buf.fill('^');
        csubstr encoded = to_chars_sub(buf, fmt::cbase64(val, len));
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    {
        buf.fill('^');
        size_t reqsize = 0;
        csubstr encoded = to_chars_sub(buf, fmt::cbase64(val, len, &reqsize));
        CHECK_EQ(encoded.len, reqsize);
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    if(len == 1)
    {
        buf.fill('^');
        csubstr encoded = to_chars_sub(buf, fmt::base64(*val));
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    if(len == 1)
    {
        buf.fill('^');
        size_t reqsize = 0;
        csubstr encoded = to_chars_sub(buf, fmt::base64(*val, &reqsize));
        CHECK_EQ(encoded.len, reqsize);
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    {
        buf.fill('^');
        csubstr encoded = to_chars_sub(buf, fmt::base64(val, len));
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    {
        buf.fill('^');
        size_t reqsize = 0;
        csubstr encoded = to_chars_sub(buf, fmt::cbase64(val, len, &reqsize));
        CHECK_EQ(encoded.len, reqsize);
        CHECK_LE(encoded.len, buf.len);
        CHECK(encoded.is_sub(buf));
        CHECK(base64_valid(encoded.str, encoded.len));
        CHECK_EQ(encoded, expected);
    }
    //-----------------------------------------------------------------
    if(len == 1)
    {
        size_t reqsize = 0;
        T ws = {};
        bool ok = from_chars(expected, fmt::base64(ws, &reqsize));
        CHECK(ok);
        CHECK_EQ(reqsize, sizeof(T));
        CHECK_EQ(0, memcmp(&ws, val, sizeof(T) * len));
    }
    {
        size_t reqsize = 0;
        std::vector<T> ws(len);
        bool ok = from_chars(expected, fmt::base64(&ws[0], ws.size(), &reqsize));
        CHECK(ok);
        CHECK_EQ(reqsize, sizeof(T) * len);
        CHECK_EQ(0, memcmp(ws.data(), val, sizeof(T) * len));
    }
    {
        std::vector<T> ws;
        bool ok = from_chars(expected, fmt::base64(ws));
        CHECK(ok);
        REQUIRE_EQ(ws.size(), len);
        CHECK_EQ(0, memcmp(ws.data(), val, sizeof(T) * len));
    }
    {
        std::vector<T> ws;
        size_t reqsize = 0;
        bool ok = from_chars(expected, fmt::base64(ws, &reqsize));
        CHECK(ok);
        REQUIRE_EQ(ws.size(), len);
        CHECK_EQ(0, memcmp(ws.data(), val, sizeof(T) * len));
    }
    if(len == 1)
    {
        size_t reqsize = 0;
        T ws = {};
        auto req = fmt::base64(ws, &reqsize);
        bool ok = from_chars(expected, &req);
        CHECK(ok);
        CHECK_EQ(reqsize, sizeof(T));
        CHECK_EQ(0, memcmp(&ws, val, sizeof(T) * len));
    }
    {
        size_t reqsize = 0;
        std::vector<T> ws(len);
        auto req = fmt::base64(&ws[0], ws.size(), &reqsize);
        bool ok = from_chars(expected, &req);
        CHECK(ok);
        CHECK_EQ(reqsize, sizeof(T) * len);
        CHECK_EQ(0, memcmp(ws.data(), val, sizeof(T) * len));
    }
    {
        std::vector<T> ws;
        auto req = fmt::base64(ws);
        bool ok = from_chars(expected, &req);
        CHECK(ok);
        REQUIRE_EQ(ws.size(), len);
        CHECK_EQ(0, memcmp(ws.data(), val, sizeof(T) * len));
    }
    {
        std::vector<T> ws;
        size_t reqsize = 0;
        auto req = fmt::base64(ws, &reqsize);
        bool ok = from_chars(expected, &req);
        CHECK(ok);
        REQUIRE_EQ(ws.size(), len);
        CHECK_EQ(reqsize, sizeof(T) * len);
        CHECK_EQ(0, memcmp(ws.data(), val, sizeof(T) * len));
    }
    // ensure the final size is always the decoded size
    {
        std::vector<T> ws;
        ws.resize(2 * len);
        size_t reqsize = 0;
        CHECK(from_chars(expected, fmt::base64(ws, &reqsize)));
        REQUIRE_EQ(ws.size(), len);
        CHECK_EQ(reqsize, sizeof(T) * len);
        CHECK_EQ(0, memcmp(ws.data(), val, sizeof(T) * len));
    }
    // fabricate a decode error
    if(expected.len)
    {
        std::string invalid(expected.begin(), expected.end());
        REQUIRE_GT(invalid.size(), 0);
        REQUIRE(base64_valid(invalid.data(), invalid.size()));
        invalid.back() = '?';
        REQUIRE(!base64_valid(invalid.data(), invalid.size()));
        // zero initial size
        std::vector<T> decoded;
        CHECK(!from_chars(to_csubstr(invalid), fmt::base64(decoded)));
        decoded.resize(2 * len);
        CHECK(!from_chars(to_csubstr(invalid), fmt::base64(decoded)));
    }
}
template<class T>
void test_base64_roundtrip_val(T const& val_, csubstr expected)
{
    alignas(alignof(T)) T val = val_; // this is needed in some windows 32 bit builds (!)
    test_base64_roundtrip_val(&val, 1, expected);
}


//-----------------------------------------------------------------------------

template<class T>
struct base64_test_pair
{
    // in older gcc versions compiling to 32 bits, int64 and uint64
    // integers up misaligned!
    // so force it here:
    alignas(alignof(T)) T val; // NOLINT
    csubstr encoded;
    int line;
};

template<class T>
void test_val_pairs(cspan<base64_test_pair<T>> pairs)
{
    for(size_t i = 0; i < pairs.size(); ++i)
    {
        base64_test_pair<T> const& p = pairs[i];
        INFO("\n" << __FILE__ << ":" << p.line << ": case["<<i<<"]: val=" << p.val << " expected=" << p.encoded);
        test_base64_roundtrip_val(p.val, p.encoded);
    }
}

template<class T>
void test_str_pairs(cspan<base64_test_pair<T>> pairs)
{
    char buf_[512];
    substr buf(buf_);
    for(size_t i = 0; i < pairs.size(); ++i)
    {
        base64_test_pair<T> const& p = pairs[i];
        INFO("\n" << __FILE__ << ":" << p.line << ": case["<<i<<"]: val=" << p.val << " expected=" << p.encoded);
        {
            INFO("basic");
            test_base64_roundtrip_basic_str(p.val, p.encoded, buf);
        }
        {
            INFO("formatted");
            test_base64_roundtrip_formatted_str(p.val, p.encoded, &buf);
        }
    }
}
template<class T, size_t N>
void test_val_pairs(base64_test_pair<T> const (&pairs)[N])
{
    test_val_pairs(cspan<base64_test_pair<T>>(pairs));
}
template<class T, size_t N>
void test_str_pairs(base64_test_pair<T> const (&pairs)[N])
{
    test_str_pairs(cspan<base64_test_pair<T>>(pairs));
}


//-----------------------------------------------------------------------------

TEST_CASE("base64.str")
{
    const base64_test_pair<csubstr> base64_str_pairs[] = {
        #define __(val, expected) {csubstr(val), csubstr(expected), __LINE__}
        __(       "Hello, World!", "SGVsbG8sIFdvcmxkIQ=="        ),
        __(""                    , ""                            ),
        __("0"                   , "MA=="                        ),
        __("1"                   , "MQ=="                        ),
        __("2"                   , "Mg=="                        ),
        __("3"                   , "Mw=="                        ),
        __("4"                   , "NA=="                        ),
        __("5"                   , "NQ=="                        ),
        __("6"                   , "Ng=="                        ),
        __("7"                   , "Nw=="                        ),
        __("8"                   , "OA=="                        ),
        __("9"                   , "OQ=="                        ),
        __("10"                  , "MTA="                        ),
        __("123"                 , "MTIz"                        ),
        __("1234"                , "MTIzNA=="                    ),
        __("1235"                , "MTIzNQ=="                    ),
        __("Man"                 , "TWFu"                        ),
        __("Ma"                  , "TWE="                        ),
        __("M"                   , "TQ=="                        ),
        __("deadbeef"            , "ZGVhZGJlZWY="                ),
        __("any carnal pleasure.", "YW55IGNhcm5hbCBwbGVhc3VyZS4="),
        __("any carnal pleasure" , "YW55IGNhcm5hbCBwbGVhc3VyZQ=="),
        __("any carnal pleasur"  , "YW55IGNhcm5hbCBwbGVhc3Vy"    ),
        __("any carnal pleasu"   , "YW55IGNhcm5hbCBwbGVhc3U="    ),
        __("any carnal pleas"    , "YW55IGNhcm5hbCBwbGVhcw=="    ),
        __(           "pleasure.", "cGxlYXN1cmUu"                ),
        __(            "leasure.", "bGVhc3VyZS4="                ),
        __(             "easure.", "ZWFzdXJlLg=="                ),
        __(              "asure.", "YXN1cmUu"                    ),
        __(               "sure.", "c3VyZS4="                    ),
        #undef __
    };
    test_str_pairs(base64_str_pairs);
}


//-----------------------------------------------------------------------------


TEST_CASE_TEMPLATE("base64.8bit", T, int8_t, uint8_t)
{
    const base64_test_pair<T> pairs[] = {
        {(T)  0, csubstr("AA=="), __LINE__},
        {(T)  1, csubstr("AQ=="), __LINE__},
        {(T)  2, csubstr("Ag=="), __LINE__},
        {(T)  3, csubstr("Aw=="), __LINE__},
        {(T)  4, csubstr("BA=="), __LINE__},
        {(T)  5, csubstr("BQ=="), __LINE__},
        {(T)  6, csubstr("Bg=="), __LINE__},
        {(T)  7, csubstr("Bw=="), __LINE__},
        {(T)  8, csubstr("CA=="), __LINE__},
        {(T)  9, csubstr("CQ=="), __LINE__},
        {(T) 10, csubstr("Cg=="), __LINE__},
        {(T) 11, csubstr("Cw=="), __LINE__},
        {(T) 12, csubstr("DA=="), __LINE__},
        {(T) 13, csubstr("DQ=="), __LINE__},
        {(T) 14, csubstr("Dg=="), __LINE__},
        {(T) 15, csubstr("Dw=="), __LINE__},
        {(T) 16, csubstr("EA=="), __LINE__},
        {(T) 17, csubstr("EQ=="), __LINE__},
        {(T) 18, csubstr("Eg=="), __LINE__},
        {(T) 19, csubstr("Ew=="), __LINE__},
        {(T) 20, csubstr("FA=="), __LINE__},
        {(T)127, csubstr("fw=="), __LINE__},
        {(T)128, csubstr("gA=="), __LINE__},
        {(T)254, csubstr("/g=="), __LINE__},
        {(T)255, csubstr("/w=="), __LINE__},
    };
    test_val_pairs(pairs);
}

TEST_CASE_TEMPLATE("base64.16bit", T, int16_t, uint16_t)
{
    const base64_test_pair<T> pairs[] = {
        {   0, native("AAA=", "AAA="), __LINE__},
        {   1, native("AQA=", "AAE="), __LINE__},
        {   2, native("AgA=", "AAI="), __LINE__},
        {   3, native("AwA=", "AAM="), __LINE__},
        {   4, native("BAA=", "AAQ="), __LINE__},
        {   5, native("BQA=", "AAU="), __LINE__},
        {   6, native("BgA=", "AAY="), __LINE__},
        {   7, native("BwA=", "AAc="), __LINE__},
        {   8, native("CAA=", "AAg="), __LINE__},
        {   9, native("CQA=", "AAk="), __LINE__},
        {  10, native("CgA=", "AAo="), __LINE__},
        {1234, native("0gQ=", "BNI="), __LINE__},
    };
    test_val_pairs(pairs);
}

TEST_CASE_TEMPLATE("base64.32bit", T, int32_t, uint32_t)
{
    const base64_test_pair<T> pairs[] = {
        {   0, native("AAAAAA==", "AAAAAA=="), __LINE__},
        {   1, native("AQAAAA==", "AAAAAQ=="), __LINE__},
        {   2, native("AgAAAA==", "AAAAAg=="), __LINE__},
        {   3, native("AwAAAA==", "AAAAAw=="), __LINE__},
        {   4, native("BAAAAA==", "AAAABA=="), __LINE__},
        {   5, native("BQAAAA==", "AAAABQ=="), __LINE__},
        {   6, native("BgAAAA==", "AAAABg=="), __LINE__},
        {   7, native("BwAAAA==", "AAAABw=="), __LINE__},
        {   8, native("CAAAAA==", "AAAACA=="), __LINE__},
        {   9, native("CQAAAA==", "AAAACQ=="), __LINE__},
        {  10, native("CgAAAA==", "AAAACg=="), __LINE__},
        {1234, native("0gQAAA==", "AAAE0g=="), __LINE__},
    };
    test_val_pairs(pairs);
}

TEST_CASE_TEMPLATE("base64.64bit", T, int64_t, uint64_t)
{
    const base64_test_pair<T> pairs[] = {
        {   0, native("AAAAAAAAAAA=", "AAAAAAAAAAA="), __LINE__},
        {   1, native("AQAAAAAAAAA=", "AAAAAAAAAAE="), __LINE__},
        {   2, native("AgAAAAAAAAA=", "AAAAAAAAAAI="), __LINE__},
        {   3, native("AwAAAAAAAAA=", "AAAAAAAAAAM="), __LINE__},
        {   4, native("BAAAAAAAAAA=", "AAAAAAAAAAQ="), __LINE__},
        {   5, native("BQAAAAAAAAA=", "AAAAAAAAAAU="), __LINE__},
        {   6, native("BgAAAAAAAAA=", "AAAAAAAAAAY="), __LINE__},
        {   7, native("BwAAAAAAAAA=", "AAAAAAAAAAc="), __LINE__},
        {   8, native("CAAAAAAAAAA=", "AAAAAAAAAAg="), __LINE__},
        {   9, native("CQAAAAAAAAA=", "AAAAAAAAAAk="), __LINE__},
        {  10, native("CgAAAAAAAAA=", "AAAAAAAAAAo="), __LINE__},
        {1234, native("0gQAAAAAAAA=", "AAAAAAAABNI="), __LINE__},
        {0xdeadbeef, native("776t3gAAAAA=", "AAAAAN6tvu8="), __LINE__},
    };
    test_val_pairs(pairs);
}

TEST_CASE("base64.float32")
{
    const base64_test_pair<float> pairs[] = {
        {   0.f,     native("AAAAAA==", "AAAAAA=="), __LINE__},
        {   1.f,     native("AACAPw==", "P4AAAA=="), __LINE__},
        {   123.45f, native("Zub2Qg==", "QvbmZg=="), __LINE__},
        {   543.21f, native("cc0HRA==", "RAfNcQ=="), __LINE__},
    };
    test_val_pairs(pairs);
}

TEST_CASE("base64.double")
{
    const base64_test_pair<double> pairs[] = {
        {   0.0,     native("AAAAAAAAAAA=", "AAAAAAAAAAA="), __LINE__},
        {   1.0,     native("AAAAAAAA8D8=", "P/AAAAAAAAA="), __LINE__},
        {   123.45,  native("zczMzMzcXkA=", "QF7czMzMzM0="), __LINE__},
        {   543.21,  native("SOF6FK75gEA=", "QID5rhR64Ug="), __LINE__},
    };
    test_val_pairs(pairs);
}

TEST_CASE("base64.high_bits_u32")
{
    test_base64_roundtrip_val(UINT32_C(0xdeadbeef), native("776t3g==", "3q2+7w=="));
    test_base64_roundtrip_val(UINT32_MAX          , native("/////w==", "/////w=="));
}

TEST_CASE("base64.high_bits_i32")
{
    test_base64_roundtrip_val(INT32_C(0x7fffffff), native("////fw==", "f////w=="));
    test_base64_roundtrip_val(INT32_MAX          , native("////fw==", "f////w=="));
}

TEST_CASE("base64.high_bits_u64")
{
    test_base64_roundtrip_val(UINT64_MAX, native("//////////8=", "//////////8="));
}

TEST_CASE("base64.high_bits_i64")
{
    test_base64_roundtrip_val(INT64_MAX, native("/////////38=", "f/////////8="));
}


//-----------------------------------------------------------------------------

template<class Container>
void write_1(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    catrs(&buf, fmt::cbase64(&c[0], c.size()));
};
template<class Container>
void write_1_len(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    size_t len = 0;
    catrs(&buf, fmt::cbase64(&c[0], c.size(), &len));
    CHECK_EQ(len, buf.size());
};
template<class Container>
void write_2(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    catrs(&buf, fmt::base64(&c[0], c.size()));
};
template<class Container>
void write_2_len(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    size_t len = 0;
    catrs(&buf, fmt::base64(&c[0], c.size(), &len));
    CHECK_EQ(len, buf.size());
};
template<class Container>
void write_3(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    catrs(&buf, fmt::cbase64(c));
};
template<class Container>
void write_3_len(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    size_t len = 0;
    catrs(&buf, fmt::cbase64(c, &len));
    CHECK_EQ(len, buf.size());
};
template<class Container>
void write_4(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    catrs(&buf, fmt::base64(c));
};
template<class Container>
void write_4_len(Container const& c, std::vector<char> &buf){
    CHECK(buf.empty());
    size_t len = 0;
    catrs(&buf, fmt::base64(c, &len));
    CHECK_EQ(len, buf.size());
};
template<class Container>
void read_1(csubstr encoded, Container &ws){
    using T = typename Container::value_type;
    size_t written = 0;
    T * n = nullptr;
    bool ok = from_chars(encoded, fmt::base64(n, size_t(0), &written));
    REQUIRE_NE(written, 0);
    REQUIRE_EQ(written % sizeof(T), 0);
    ws.resize(written / sizeof(T));
    CHECK(!ws.empty());
    ok = from_chars(encoded, fmt::base64(&ws[0], ws.size(), &written));
    CHECK(ok);
    CHECK_EQ(written, sizeof(T) * ws.size());
};
template<class Container>
void read_2(csubstr encoded, Container &ws){
    bool ok = from_chars(encoded, fmt::base64(ws));
    CHECK(ok);
};
template<class Container>
void read_2_len(csubstr encoded, Container &ws){
    size_t len = 0;
    bool ok = from_chars(encoded, fmt::base64(ws, &len));
    CHECK(ok);
    CHECK_EQ(len, ws.size() * sizeof(typename Container::value_type));
};

template<class Container>
void test_roundtrip(Container const& data)
{
    REQUIRE(!data.empty());
    using T = typename Container::value_type;
    using WriteFn = void(*)(Container const&, std::vector<char> &);
    using ReadFn = void(*)(csubstr, Container &);
    struct WriteFnSpec { csubstr first; WriteFn second; };
    struct ReadFnSpec { csubstr first; ReadFn second; };
    const WriteFnSpec write_functions[] = {
        WriteFnSpec{"write_1"    , write_1    <Container>},
        WriteFnSpec{"write_1_len", write_1_len<Container>},
        WriteFnSpec{"write_2"    , write_2    <Container>},
        WriteFnSpec{"write_2_len", write_2_len<Container>},
        WriteFnSpec{"write_3"    , write_3    <Container>},
        WriteFnSpec{"write_3_len", write_3_len<Container>},
        WriteFnSpec{"write_4"    , write_4    <Container>},
        WriteFnSpec{"write_4_len", write_4_len<Container>},
    };
    const ReadFnSpec read_functions[] = {
        ReadFnSpec{"read_1"    , read_1    <Container>},
        ReadFnSpec{"read_2"    , read_2    <Container>},
        ReadFnSpec{"read_2_len", read_2_len<Container>},
    };
    std::vector<char> buf;
    Container after_roundtrip;
    for(auto const& writefn : write_functions)
    {
        INFO("write_fn="<<writefn.first);
        buf.clear();
        writefn.second(data, buf);
        csubstr encoded = to_csubstr(buf);
        CHECK_EQ(encoded.len, buf.size());
        CHECK(encoded.is_sub(to_csubstr(buf)));
        CHECK(base64_valid(encoded.str, encoded.len));
        INFO("encoded=" << encoded);
        for(auto const& readfn : read_functions)
        {
            INFO("read_fn=" << readfn.first);
            after_roundtrip.clear();
            readfn.second(encoded, after_roundtrip);
            CHECK_EQ(after_roundtrip.size(), data.size());
            CHECK_EQ(after_roundtrip, data);
        }
    }
}
template<class Container>
void test_container_roundtrip()
{
    using T = typename Container::value_type;
    const T values[] = {
        (T)0, (T)1, (T)2, (T)3, (T)4, (T)5, (T)6, (T)7, (T)8, (T)9,
        (T)10, (T)11, (T)12, (T)13, (T)14, (T)15, (T)16, (T)17, (T)18, (T)19,
        (T)0xdeadbeef, (T)0xff, (T)0xffff, (T)0xffffffff, (T)0xffffffffffffffff
    };
    const size_t num_reps[] = {1, 2, 4, 8, 10};
    Container vec;
    vec.reserve(C4_COUNTOF(values) * 10);
    for(size_t reps : num_reps)
    {
        INFO("reps=" << reps);
        vec.clear();
        for(T value : values)
            for(size_t i = 0; i < reps; ++i)
                vec.push_back(value);
        test_roundtrip(vec);
    }

}

TEST_CASE_TEMPLATE("base64.container_roundtrip", T, char, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    test_container_roundtrip<std::vector<T>>();
}

TEST_CASE("base64.str_roundtrip")
{
    test_container_roundtrip<std::string>();
}


} // namespace c4

C4_SUPPRESS_WARNING_MSVC_POP

#include "c4/libtest/supprwarn_pop.hpp"
