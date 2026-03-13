#include "c4/test.hpp"
#ifndef C4CORE_SINGLE_HEADER
#include "c4/std/string_fwd.hpp"
#include "c4/std/string.hpp"
#include "c4/std/string_view.hpp"
#include "c4/std/vector.hpp"
#include "c4/std/span.hpp"
#endif

#if (C4_CPP >= 20)
#   if defined(__cpp_lib_string_view)
#       define TEST_CASE_TEMPLATE_RO(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>, std::string_view, std::span<char>, std::span<const char>)
#       define TEST_CASE_TEMPLATE_RW(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>, std::span<char>)
#   else
#       define TEST_CASE_TEMPLATE_RO(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>, std::span<char>, std::span<const char>)
#       define TEST_CASE_TEMPLATE_RW(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>, std::span<char>)
#   endif
#elif (C4_CPP >= 17)
#   if defined(__cpp_lib_string_view)
#       define TEST_CASE_TEMPLATE_RO(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>, std::string_view)
#       define TEST_CASE_TEMPLATE_RW(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>)
#   else
#       define TEST_CASE_TEMPLATE_RO(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>)
#       define TEST_CASE_TEMPLATE_RW(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>)
#   endif
#else
#   define TEST_CASE_TEMPLATE_RO(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>)
#   define TEST_CASE_TEMPLATE_RW(name, ty) TEST_CASE_TEMPLATE(name, ty, std::string, std::vector<char>)
#endif

namespace c4 {


namespace {
template<class T>
struct factory
{
    static T make(csubstr s) { const char *begin = s.str; const char *end = s.str + s.len; return T(begin, end); }
    static T make(size_t sz) { T v{}; v.resize(sz); return v; }
    static T& get(T &v) { return v; }
};
#if C4_CPP >= 17
template<class V>
struct view_factory
{
    using T = typename V::value_type;
    using B = std::vector<std::remove_const_t<T>>;
    struct helper
    {
        B buf;
        V sp;
        helper(csubstr s) : buf(s.str, s.str+s.len), sp(buf.data(), buf.size()) {}
        helper(size_t sz) : buf(), sp() { buf.resize(sz); sp = {buf.data(), buf.size()}; }
    };
    static helper make(size_t sz) { return helper(sz); }
    static helper make(csubstr s) { return helper(s); }
    static V& get(helper& v) { return v.sp; }
};
template<> struct factory<std::string_view> : public view_factory<std::string_view> {};
#endif
#if C4_CPP >= 20
template<class T> struct factory<std::span<T>> : public view_factory<std::span<T>> {};
#endif
template<class T, class A> auto mk(A s) -> decltype(factory<T>::make(s))
{
    return factory<T>::make(s);
}
template<class T> T& get(decltype(factory<T>::make(csubstr())) &buf)
{
    return factory<T>::get(buf);
}

static csubstr cases[] = {
    csubstr(),
    csubstr(""),
    csubstr("0"),
    csubstr("01"),
    csubstr("012"),
    csubstr("0123"),
    csubstr("01234"),
    csubstr("barnabe"),
    csubstr("Barnabe"),
};
} // anon namespace


TEST_CASE_TEMPLATE_RO("to_csubstr", T)
{
    for(csubstr txt : cases)
    {
        CAPTURE(txt);
        auto buf = mk<T>(txt); // use auto because we may get the helper type (needed with spans)
        T &obj = get<T>(buf);
        csubstr ss = to_csubstr(obj);
        CHECK_EQ(ss.data(), obj.data());
        CHECK_EQ(ss.size(), obj.size());
        CHECK_EQ(ss.str, obj.data());
        CHECK_EQ(ss.len, obj.size());
        CHECK_EQ(ss, txt);
    }
}

TEST_CASE_TEMPLATE_RW("to_substr", T)
{
    for(csubstr txt : cases)
    {
        CAPTURE(txt);
        auto buf = mk<T>(txt); // use auto because we may get the helper type (needed with spans)
        T &obj = get<T>(buf);
        substr ss = to_substr(obj);
        CHECK_EQ(ss.data(), obj.data());
        CHECK_EQ(ss.size(), obj.size());
        CHECK_EQ(ss.str, obj.data());
        CHECK_EQ(ss.len, obj.size());
        CHECK_EQ(ss, txt);
    }
}

TEST_CASE_TEMPLATE_RO("compare_csubstr", T)
{
    for(csubstr ss0 : cases)
    {
        if(ss0.str == nullptr)
            continue;
        CAPTURE(ss0);
        for(csubstr ss1 : cases)
        {
            if(ss1.str == nullptr)
                continue;
            CAPTURE(ss1);
            auto buf0 = mk<T>(ss0); // use auto because we may get the helper type (needed with spans)
            auto buf1 = mk<T>(ss1); // use auto because we may get the helper type (needed with spans)
            T& s0 = get<T>(buf0);
            T& s1 = get<T>(buf1);
            //
            CHECK_NE(s0.data(), ss0.data());
            CHECK_NE(s1.data(), ss1.data());
            CHECK_EQ(s0.size(), ss0.size());
            CHECK_EQ(s1.size(), ss1.size());
            //
            CHECK_EQ(s0, ss0);
            CHECK_EQ(s1, ss1);
            CHECK_EQ(ss0, s0);
            CHECK_EQ(ss1, s1);
            //
            if(ss1 > ss0)
            {
                CHECK_GE(s1, ss0);
                CHECK_LE(s0, ss1);
                CHECK_GE(ss1, s0);
                CHECK_LE(ss0, s1);
                //
                CHECK_GT(s1, ss0);
                CHECK_LT(s0, ss1);
                CHECK_GT(ss1, s0);
                CHECK_LT(ss0, s1);
            }
            else if(ss1 < ss0)
            {
                CHECK_LE(s1, ss0);
                CHECK_GE(s0, ss1);
                CHECK_LE(ss1, s0);
                CHECK_GE(ss0, s1);
                //
                CHECK_LT(s1, ss0);
                CHECK_GT(s0, ss1);
                CHECK_LT(ss1, s0);
                CHECK_GT(ss0, s1);
            }
            else if(ss1 == ss0)
            {
                CHECK_EQ(s1, ss0);
                CHECK_EQ(s0, ss1);
                CHECK_EQ(ss1, s0);
                CHECK_EQ(ss0, s1);
                //
                CHECK_EQ(s1, ss0);
                CHECK_EQ(s0, ss1);
                CHECK_EQ(ss1, s0);
                CHECK_EQ(ss0, s1);
            }
        }
    }
}

TEST_CASE_TEMPLATE_RO("compare_substr", T)
{
    for(csubstr ss0_ : cases)
    {
        if(ss0_.str == nullptr)
            continue;
        CAPTURE(ss0_);
        std::string sbuf0(ss0_.str, ss0_.len);
        substr ss0 = to_substr(sbuf0);
        for(csubstr ss1_ : cases)
        {
            if(ss1_.str == nullptr)
                continue;
            CAPTURE(ss1_);
            std::string sbuf1(ss1_.str, ss1_.len);
            substr ss1 = to_substr(sbuf1);
            auto buf0 = mk<T>(ss0); // use auto because we may get the helper type (needed with spans)
            auto buf1 = mk<T>(ss1); // use auto because we may get the helper type (needed with spans)
            T& s0 = get<T>(buf0);
            T& s1 = get<T>(buf1);
            //
            CAPTURE(ss0);
            CAPTURE(s0);
            CAPTURE(ss1);
            CAPTURE(s1);
            //
            CHECK_EQ(s0.size(), ss0.size());
            CHECK_EQ(s1.size(), ss1.size());
            //
            CHECK_EQ(s0, ss0);
            CHECK_EQ(s1, ss1);
            CHECK_EQ(ss0, s0);
            CHECK_EQ(ss1, s1);
            //
            if(ss1 > ss0)
            {
                CHECK_GE(s1, ss0);
                CHECK_LE(s0, ss1);
                CHECK_GE(ss1, s0);
                CHECK_LE(ss0, s1);
                //
                CHECK_GT(s1, ss0);
                CHECK_LT(s0, ss1);
                CHECK_GT(ss1, s0);
                CHECK_LT(ss0, s1);
            }
            else if(ss1 < ss0)
            {
                CHECK_LE(s1, ss0);
                CHECK_GE(s0, ss1);
                CHECK_LE(ss1, s0);
                CHECK_GE(ss0, s1);
                //
                CHECK_LT(s1, ss0);
                CHECK_GT(s0, ss1);
                CHECK_LT(ss1, s0);
                CHECK_GT(ss0, s1);
            }
            else if(ss1 == ss0)
            {
                CHECK_EQ(s1, ss0);
                CHECK_EQ(s0, ss1);
                CHECK_EQ(ss1, s0);
                CHECK_EQ(ss0, s1);
                //
                CHECK_EQ(s1, ss0);
                CHECK_EQ(s0, ss1);
                CHECK_EQ(ss1, s0);
                CHECK_EQ(ss0, s1);
            }
        }
    }
}


TEST_CASE_TEMPLATE_RO("to_chars", T)
{
    for(csubstr txt : cases)
    {
        CAPTURE(txt);
        auto buf0 = mk<T>(txt); // use auto because we may get the helper type (needed with spans)
        T& s0 = get<T>(buf0);
        CAPTURE(s0);
        char buf_[100] = {};
        substr buf = buf_;
        CHECK_NE(buf.data(), s0.data());
        {
            // buffer size 0
            size_t ret = to_chars({}, s0);
            CHECK_EQ(ret, txt.len);
            CHECK_EQ(ret, s0.size());
        }
        {
            // buffer too small
            size_t ret = to_chars(buf.first(txt.len / 2), s0);
            CHECK_EQ(ret, txt.len);
            CHECK_EQ(ret, s0.size());
        }
        {
            // buffer ok
            size_t ret = to_chars(buf, s0);
            CHECK(ret == txt.len);
            CHECK(ret == s0.size());
            CAPTURE(buf.first(ret));
            CHECK(buf.first(ret) == s0);
        }
    }
}

template<class T> struct is_growable : public std::true_type {};
#if C4_CPP >= 20
template<class T> struct is_growable<std::span<T>> : public std::false_type {};
#endif

TEST_CASE_TEMPLATE_RW("from_chars", T)
{
    for(csubstr txt : cases)
    {
        CAPTURE(txt);
        {
            CAPTURE("buffer size=0");
            auto buf0 = mk<T>(size_t(0)); // use auto because we may get the helper type (needed with spans)
            T& output = get<T>(buf0);
            CHECK_EQ(output.size(), 0);
            to_substr(output).fill('\0');
            const char *orig_str = output.data();
            size_t orig_len = output.size();
            if(txt.str)
                CHECK_NE(txt.data(), output.data());
            bool ok = from_chars(txt, &output);
            if(is_growable<T>::value) // NOLINT
            {
                CHECK(ok);
                CHECK_EQ(output.size(), txt.len);
                CHECK_EQ(txt, output);
            }
            else
            {
                if(txt.len)
                {
                    CHECK(!ok);
                    CHECK_NE(txt, output);
                }
                else
                {
                    CHECK(ok);
                    CHECK_EQ(txt, output);
                }
                CHECK_EQ(output.size(), orig_len);
                CHECK_EQ(output.data(), orig_str);
            }
        }
        {
            CAPTURE("buffer size half needed");
            auto buf0 = mk<T>(txt.len / 2u); // use auto because we may get the helper type (needed with spans)
            T& output = get<T>(buf0);
            to_substr(output).fill('\0');
            const char *orig_str = output.data();
            size_t orig_len = output.size();
            if(txt.str)
                CHECK_NE(txt.data(), output.data());
            bool ok = from_chars(txt, &output);
            if(is_growable<T>::value) // NOLINT
            {
                CHECK(ok);
                CHECK_EQ(output.size(), txt.len);
                CHECK_EQ(txt, output);
            }
            else
            {
                if(txt.len)
                {
                    CHECK(!ok);
                    CHECK_NE(txt, output);
                }
                else
                {
                    CHECK(ok);
                    CHECK_EQ(txt, output);
                }
                CHECK_EQ(output.size(), orig_len);
                CHECK_EQ(output.data(), orig_str);
            }
        }
        {
            CAPTURE("buffer size ==");
            auto buf0 = mk<T>(txt.len); // use auto because we may get the helper type (needed with spans)
            T& output = get<T>(buf0);
            to_substr(output).fill('\0');
            const char *orig_str = output.data();
            size_t orig_len = output.size();
            if(txt.str)
                CHECK_NE(txt.data(), output.data());
            bool ok = from_chars(txt, &output);
            CHECK(ok);
            CHECK_EQ(output.data(), orig_str);
            CHECK_LE(output.size(), orig_len);
            CHECK_EQ(output.size(), txt.len);
            CHECK_EQ(txt, output);
        }
        {
            CAPTURE("buffer size 2*");
            auto buf0 = mk<T>(2u * txt.len); // use auto because we may get the helper type (needed with spans)
            T& output = get<T>(buf0);
            to_substr(output).fill('\0');
            const char *orig_str = output.data();
            size_t orig_len = output.size();
            if(txt.str)
                CHECK_NE(txt.data(), output.data());
            bool ok = from_chars(txt, &output);
            CHECK(ok);
            CHECK_EQ(output.data(), orig_str);
            CHECK_LE(output.size(), orig_len);
            CHECK_EQ(output.size(), txt.len);
            CHECK_EQ(txt, output);
        }
    }
}

} // namespace c4
