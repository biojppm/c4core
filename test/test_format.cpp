#include "c4/std/std.hpp"
#include "c4/substr.hpp"
#include "c4/format.hpp"

#include <c4/test.hpp>
#include "c4/libtest/supprwarn_push.hpp"

#ifdef __clang__
#   pragma clang diagnostic push
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

namespace c4 {


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void test_to_chars_fmt_basic(T f, int precision, const char* flt, T fltv, const char *scient, T scientv)
{
    char bufc[64];
    substr buf(bufc);
    substr r;
    T copy;

    INFO("num=" << f);

    r = to_chars_sub(buf, fmt::fmt(f, precision));
    CHECK_EQ(r, to_csubstr(flt));
    from_chars(r, &copy);
    if(sizeof(T) == sizeof(float))
    {
        CHECK_FLOAT_EQ((float)fltv, (float)copy);
    }
    else
    {
        CHECK_FLOAT_EQ(fltv, copy);
    }

    r = to_chars_sub(buf, fmt::fmt(f, precision, FTOA_SCIENT));
    CHECK_EQ(r, to_csubstr(scient));
    from_chars(r, &copy);
    if(sizeof(T) == sizeof(float))
    {
        CHECK_FLOAT_EQ((float)scientv, (float)copy);
    }
    else
    {
        CHECK_FLOAT_EQ(scientv, copy);
    }
}

TEST_CASE("to_chars.fmt_basic")
{
    char bufc[128];
    substr buf(bufc);

    CHECK_EQ(to_chars_sub(buf, fmt::hex( int8_t(0x7f))), "0x7f");
    CHECK_EQ(to_chars_sub(buf, fmt::hex(uint8_t(0xff))), "0xff");

    #ifdef FIXME_READFLOAT
    float f = 256.064f;
    test_to_chars_fmt_basic<float>(f, 0, "256", 256.f, "3e+02", 300.f);
    test_to_chars_fmt_basic<float>(f, 1, "256.1", 256.1f, "2.6e+02", 260.f);
    test_to_chars_fmt_basic<float>(f, 2, "256.06", 256.06f, "2.56e+02", 256.f);
    test_to_chars_fmt_basic<float>(f, 3, "256.064", 256.064f, "2.561e+02", 256.1f);
    test_to_chars_fmt_basic<float>(f, 4, "256.0640", 256.0640f, "2.5606e+02", 256.06f);
    test_to_chars_fmt_basic<float>(f, 5, "256.06400", 256.06400f, "2.56064e+02", 256.064f);

    double d = 256.064;
    test_to_chars_fmt_basic<double>(d, 0, "256", 256., "3e+02", 300.);
    test_to_chars_fmt_basic<double>(d, 1, "256.1", 256.1, "2.6e+02", 260.);
    test_to_chars_fmt_basic<double>(d, 2, "256.06", 256.06, "2.56e+02", 256.);
    test_to_chars_fmt_basic<double>(d, 3, "256.064", 256.064, "2.561e+02", 256.1);
    test_to_chars_fmt_basic<double>(d, 4, "256.0640", 256.0640, "2.5606e+02", 256.06);
    test_to_chars_fmt_basic<double>(d, 5, "256.06400", 256.06400, "2.56064e+02", 256.064);
    #endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE("to_chars.boolalpha")
{
    char bufc[128];
    substr buf(bufc);
    
    CHECK_EQ(to_chars_sub(buf, true), "1");
    CHECK_EQ(to_chars_sub(buf, false), "0");
    CHECK_EQ(to_chars_sub(buf, fmt::boolalpha(true)), "true");
    CHECK_EQ(to_chars_sub(buf, 1), "1");
    CHECK_EQ(to_chars_sub(buf, fmt::boolalpha(1)), "true");
    CHECK_EQ(to_chars_sub(buf, fmt::boolalpha(10)), "true");
    CHECK_EQ(to_chars_sub(buf, fmt::boolalpha(false)), "false");
    CHECK_EQ(to_chars_sub(buf, fmt::boolalpha(0)), "false");
    CHECK_EQ(to_chars_sub(buf, fmt::boolalpha(0u)), "false");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE("cat.vars")
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = cat(buf, 1, ' ', 2, ' ', 3, ' ', 4);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 3 4");
}

#ifdef C4_TUPLE_TO_STR
TEST_CASE("cat.tuple")
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = cat(buf, std::forward_as_tuple(1, ' ', 2, ' ', 3, ' ', 4));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 3 4");
}
#endif // C4_TUPLE_TO_STR

TEST_CASE("uncat.vars")
{
    size_t sz;
    size_t npos = csubstr::npos;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    sz = uncat("1 2 3 4", v1, v2, v3, v4);
    CHECK_NE(sz, npos);
    CHECK_EQ(sz, 7);
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);
    CHECK_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST_CASE("uncat.tuple")
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = uncat("1 2 3 4", tp);
    CHECK_NE(sz, csubstr::npos);
    CHECK_EQ(sz, 7);
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);
    CHECK_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR

TEST_CASE("catsep.vars")
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = catsep(buf, ' ', 1, 2);
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2");

    sz = catsep(buf, '/', 1, 2);
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1/2");

    sz = catsep(buf, ' ', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 3 4");

    sz = catsep(buf, '/', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1/2/3/4");
}

#ifdef C4_TUPLE_TO_STR
TEST_CASE("catsep.tuple")
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = catsep(buf, ' ', std::forward_as_tuple(1, 2));
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2");

    sz = catsep(buf, '/', std::forward_as_tuple(1, 2));
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1/2");

    sz = catsep(buf, ' ', std::forward_as_tuple(1, 2, 3, 4));
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 3 4");

    sz = catsep(buf, '/', std::forward_as_tuple(1, 2, 3, 4));
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1/2/3/4");
}
#endif // C4_TUPLE_TO_STR

TEST_CASE("uncatsep.vars")
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    char sep;

    sz = uncatsep("1 2 3 4", sep, v1, v2, v3, v4);
    CHECK_EQ(sz, 7);
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);
    CHECK_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST_CASE("uncatsep.tuple")
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    char sep;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = uncatsep("1 2 3 4", sep, tp);
    CHECK_EQ(sz, 7);
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);
    CHECK_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR

TEST_CASE("format.vars")
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, 19);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4, 5, 6, 7);
    CHECK_EQ(sz, 19);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3);
    CHECK_EQ(sz, 20);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and {}");

    sz = format(buf, "{} and {} and {} and {}", 1, 2);
    CHECK_EQ(sz, 21);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and {} and {}");

    sz = format(buf, "{} and {} and {} and {}", 1);
    CHECK_EQ(sz, 22);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and {} and {} and {}");

    sz = format(buf, "{} and {} and {} and {}");
    CHECK_EQ(sz, 23);
    result = sp.left_of(sz);
    CHECK_EQ(result, "{} and {} and {} and {}");
}

#ifdef C4_TUPLE_TO_STR
TEST_CASE("format.tuple")
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3, 4));
    CHECK_EQ(sz, 19);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3, 4, 5, 6, 7));
    CHECK_EQ(sz, 19);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3));
    CHECK_EQ(sz, 20);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and {}");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2));
    CHECK_EQ(sz, 21);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and {} and {}");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1));
    CHECK_EQ(sz, 22);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and {} and {} and {}");

    sz = format(buf, "{} and {} and {} and {}");
    CHECK_EQ(sz, 23);
    result = sp.left_of(sz);
    CHECK_EQ(result, "{} and {} and {} and {}");
}
#endif // C4_TUPLE_TO_STR

TEST_CASE("unformat.vars")
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    sz = unformat("1 and 2 and 3 and 4", "{} and {} and {} and {}", v1, v2, v3, v4);
    CHECK_EQ(sz, 19);
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);
    CHECK_EQ(v4, 4);

    sz = unformat("1 and 2 and 3 and 4" , "3", v1);
    CHECK_EQ(sz, 0);
    CHECK_EQ(v1, 1);
}

#ifdef C4_TUPLE_TO_STR
TEST_CASE("unformat.tuple")
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = unformat("1 and 2 and 3 and 4", "{} and {} and {} and {}", tp);
    CHECK_EQ(sz, 19);
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);
    CHECK_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR


TEST_CASE("printf.basic")
{
    char bufc[128];
    substr buf = bufc;
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = sprintf(buf, "hello world");
    CHECK_EQ(buf.first(ret), "hello world");

    memset(buf.str, 0, buf.len);
    ret = sprintf(buf, "%d %d %d %d", 1, 2, 3, 4);
    CHECK_EQ(buf.first(ret), "1 2 3 4");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST_CASE("catrs.basic")
{
    std::vector<char> buf;

    catrs(&buf);
    CHECK_EQ(to_csubstr(buf), "");

    catrs(&buf, 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1234");
    catrs(&buf, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5678");
}

TEST_CASE("catrs.basic_return")
{
    auto bufv = catrs<std::vector<char>>(9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    CHECK_EQ(to_csubstr(bufv), "9876543210");
    bufv = catrs<std::vector<char>>();
    CHECK_EQ(to_csubstr(bufv), "");
    CHECK(bufv.empty());

    auto bufs = catrs<std::string>(9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    CHECK_EQ(to_csubstr(bufs), "9876543210");
}

TEST_CASE("catrs.basic_append")
{
    std::vector<char> buf;

    catrs(append, &buf);
    CHECK_EQ(to_csubstr(buf), "");

    catrs(append, &buf, 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1234");
    catrs(append, &buf, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "12345678");
    catrs(append, &buf, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "123456789012345678");
}

template<class... Args>
void catrs_perfect_fwd(Args && ...args)
{
    catrs(std::forward<Args>(args)...);
}

TEST_CASE("catrs.perfect_fwd")
{
    std::vector<char> buf;
    catrs_perfect_fwd(&buf, 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1234");
    catrs_perfect_fwd(&buf, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5678");
}

template<class... Args>
void catrs_const_fwd(Args const& ...args)
{
    catrs(args...);
}

TEST_CASE("catrs.const_fwd")
{
    std::vector<char> buf;
    catrs_const_fwd(&buf, 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1234");
    catrs_const_fwd(&buf, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5678");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE("catseprs.basic")
{
    std::vector<char> buf;

    catseprs(&buf, ' ');
    CHECK_EQ(to_csubstr(buf), "");

    catseprs(&buf, ' ', 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1 2 3 4");
    catseprs(&buf, ' ', 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5 6 7 8");

    catseprs(&buf, ',', 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1,2,3,4");
    catseprs(&buf, ',', 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5,6,7,8");

    catseprs(&buf, '/', 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1/2/3/4");
    catseprs(&buf, '/', 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5/6/7/8");

    catseprs(&buf, "///", 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1///2///3///4");
    catseprs(&buf, "///", 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5///6///7///8");

    catseprs(&buf, 5678, 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1567825678356784");
    catseprs(&buf, 1234, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5123461234712348");
}

TEST_CASE("catseprs.basic_return")
{
    auto bufv = catseprs<std::vector<char>>('a', 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    CHECK_EQ(to_csubstr(bufv), "9a8a7a6a5a4a3a2a1a0");

    auto bufs = catseprs<std::string      >('a', 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    CHECK_EQ(to_csubstr(bufs), "9a8a7a6a5a4a3a2a1a0");
}

TEST_CASE("catseprs.basic_append")
{
    std::vector<char> buf;

    auto ret = catseprs(append, &buf, ' ');
    CHECK_EQ(to_csubstr(buf), "");
    CHECK_EQ(ret, "");

    ret = catseprs(append, &buf, ' ', 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1 2 3 4");
    CHECK_EQ(ret, "1 2 3 4");
    ret = catseprs(append, &buf, ' ', 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "1 2 3 45 6 7 8");
    CHECK_EQ(ret, "5 6 7 8");
    ret = catseprs(append, &buf, ' ', 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "1 2 3 45 6 7 89 0 1 2 3 4 5 6 7 8");
    CHECK_EQ(ret, "9 0 1 2 3 4 5 6 7 8");

    ret = catseprs(append, &buf, ' ');
    CHECK_EQ(to_csubstr(buf), "1 2 3 45 6 7 89 0 1 2 3 4 5 6 7 8");
    CHECK_EQ(ret, "");
}

template<class... Args>
void catseprs_perfect_fwd(Args && ...args)
{
    catseprs(std::forward<Args>(args)...);
}

template<class... Args>
void catseprs_const_fwd(Args const& ...args)
{
    catseprs(args...);
}

TEST_CASE("catseprs.perfect_fwd")
{
    std::vector<char> buf;
    catseprs_perfect_fwd(&buf, '.', 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1.2.3.4");
    catseprs_perfect_fwd(&buf, 0, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5060708");
}

TEST_CASE("catseprs.const_fwd")
{
    std::vector<char> buf;
    catseprs_const_fwd(&buf, '.', 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1.2.3.4");
    catseprs_const_fwd(&buf, 0, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "5060708");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE("formatrs.basic")
{
    std::vector<char> buf;

    formatrs(&buf, "");
    CHECK(buf.empty());

    formatrs(&buf, "{} goes with food, {} goes with heat, {} anytime", "wine", "beer", "coffee");
    CHECK_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime");

    formatrs(&buf, "");
    CHECK(buf.empty());
}

TEST_CASE("formatrs.basic_return")
{
    auto bufv = formatrs<std::vector<char>>("{} goes with food, {} goes with heat, {} anytime", "wine", "beer", "coffee");
    CHECK_EQ(to_csubstr(bufv), "wine goes with food, beer goes with heat, coffee anytime");

    auto bufs = formatrs<std::string>("{} goes with food, {} goes with heat, {} anytime", "wine", "beer", "coffee");
    CHECK_EQ(to_csubstr(bufs), "wine goes with food, beer goes with heat, coffee anytime");
}

TEST_CASE("formatrs.basic_append")
{
    std::vector<char> buf;

    formatrs(append, &buf, "{} goes with food", "wine");
    CHECK_EQ(to_csubstr(buf), "wine goes with food");
    formatrs(append, &buf, ", {} goes with heat", "beer");
    CHECK_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat");
    formatrs(append, &buf, ", {} anytime", "coffee");
    CHECK_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime");

    formatrs(append, &buf, ". And water. {} glass of {}cl in the morning clears you up for the day", 1, 40);
    CHECK_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime. And water. 1 glass of 40cl in the morning clears you up for the day");
}

template<class... Args>
void formatrs_perfect_fwd(Args && ...args)
{
    formatrs(std::forward<Args>(args)...);
}

template<class... Args>
void formatrs_const_fwd(Args const& ...args)
{
    formatrs(args...);
}

TEST_CASE("formatrs.perfect_fwd")
{
    std::vector<char> buf;
    formatrs_perfect_fwd(&buf, "Too much of anything is bad, but too much {} is {}.", "Champagne", "just right");
    CHECK_EQ(to_csubstr(buf), "Too much of anything is bad, but too much Champagne is just right.");
    formatrs_perfect_fwd(&buf, "{}, I am tasting the {}", "Come quickly", "stars!");
    CHECK_EQ(to_csubstr(buf), "Come quickly, I am tasting the stars!");
    formatrs_perfect_fwd(&buf, "{} the only wine that leaves a {} {} after {}.", "Champagne is", "woman", "beautiful", "drinking it");
    CHECK_EQ(to_csubstr(buf), "Champagne is the only wine that leaves a woman beautiful after drinking it.");
    formatrs_perfect_fwd(&buf, "Remember {}, it's not just {} we are fighting for, it's {}", "gentlemen", "France", "Champagne!");
    CHECK_EQ(to_csubstr(buf), "Remember gentlemen, it's not just France we are fighting for, it's Champagne!");
    // https://www.townandcountrymag.com/leisure/drinks/how-to/g828/the-10-best-quotes-about-champagne/
}

TEST_CASE("formatrs.const_fwd")
{
    std::vector<char> buf;
    formatrs_const_fwd(&buf, "Too much of anything is bad, but too much {} is {}.", "Champagne", "just right");
    CHECK_EQ(to_csubstr(buf), "Too much of anything is bad, but too much Champagne is just right.");
    formatrs_const_fwd(&buf, "{}, I am tasting the {}", "Come quickly", "stars!");
    CHECK_EQ(to_csubstr(buf), "Come quickly, I am tasting the stars!");
    formatrs_const_fwd(&buf, "{} the only wine that leaves a {} {} after {}.", "Champagne is", "woman", "beautiful", "drinking it");
    CHECK_EQ(to_csubstr(buf), "Champagne is the only wine that leaves a woman beautiful after drinking it.");
    formatrs_const_fwd(&buf, "Remember {}, it's not just {} we are fighting for, it's {}", "gentlemen", "France", "Champagne!");
    CHECK_EQ(to_csubstr(buf), "Remember gentlemen, it's not just France we are fighting for, it's Champagne!");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void test_hex(T in, csubstr expected)
{
    INFO("expected=" << expected);

    std::vector<char> buf;
    char buf1[1] = {};
    char buf2[2] = {};
    char buf3[3] = {};

    CHECK_EQ(cat({}, fmt::hex(in)), expected.len);

    buf1[0] = '?';
    CHECK_EQ(cat(buf1, fmt::hex(in)), expected.len);
    CHECK_EQ(buf1[0], '?');

    buf2[0] = '?';
    buf2[1] = '?';
    CHECK_EQ(cat(buf2, fmt::hex(in)), expected.len);
    CHECK_EQ(buf2[0], '0');
    CHECK_EQ(buf2[1], '?');

    buf3[0] = '?';
    buf3[1] = '?';
    buf3[2] = '?';
    CHECK_EQ(cat(buf3, fmt::hex(in)), expected.len);
    CHECK_EQ(buf3[0], '0');
    CHECK_EQ(buf3[1], 'x');
    CHECK_EQ(buf3[2], '?');

    buf.clear();
    CHECK_EQ(cat(to_substr(buf), fmt::hex(in)), expected.len);
    CHECK_EQ(buf.size(), 0);

    catrs(&buf, fmt::hex(in));
    CHECK_EQ(buf.size(), expected.len);
    CHECK_EQ(to_csubstr(buf), expected);
}

TEST_CASE("fmt.hex")
{
    test_hex(0, "0x0");
    test_hex(nullptr, "0x0");
    test_hex(256, "0x100");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T> void test_raw_roundtrip(const char *valstr, T const& orig)
{
    INFO("valstr=" << valstr);
    alignas(alignof(T)) char buf_[2 * (sizeof(T) + alignof(T))] = {};
    substr buf = buf_;

    fmt::const_raw_wrapper rawwrap = fmt::craw(orig);
    REQUIRE_EQ((void*)rawwrap.buf, (void*)&orig);
    REQUIRE_EQ(rawwrap.len, sizeof(orig));

    for(size_t i = 0; i < alignof(T); ++i)
    {
        INFO(" i=" << i);
        // make sure to cover unaligned buffers
        substr sbuf = buf.sub(i);
        size_t szwrite = c4::to_chars(sbuf, fmt::craw(orig));
        REQUIRE_LE(szwrite, sbuf.len);
        if(i == 0)
        {
            REQUIRE_EQ(szwrite, sizeof(T));
        }
        else
        {
            REQUIRE_GT(szwrite, sizeof(T));
        }
        T copy = {};
        REQUIRE_NE(copy, orig);
        bool ok = c4::from_chars_first(sbuf, fmt::raw(copy));
        REQUIRE_EQ(ok, true);
        CHECK_EQ(copy, orig);

        // cover also insufficient buffers
        sbuf = sbuf.first(sizeof(T)-1);
        memset(buf.str, 0, buf.len);
        szwrite = c4::to_chars(sbuf, fmt::craw(orig));
        REQUIRE_GT(szwrite, sbuf.len);
        for(char c : buf)
        {
            CHECK_EQ(c, 0);
        }
    }
}

TEST_CASE("fmt.raw_int")
{
    #define _(v) test_raw_roundtrip(#v, v)

    _(int(1));
    _(int(2));
    _(int(-1));
    _(int(-2));

    #undef _
}

} // namespace c4

#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif

#include "c4/libtest/supprwarn_pop.hpp"
