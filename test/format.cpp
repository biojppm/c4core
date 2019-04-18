#include "c4/std/string.hpp"
#include "c4/std/vector.hpp"
#include "c4/test.hpp"
#include "c4/format.hpp"

namespace c4 {
    
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template< class T >
void test_to_chars_fmt_basic(T f, int precision, const char* flt, T fltv, const char *scient, T scientv)
{
    char bufc[64];
    substr buf(bufc);
    substr r;
    T copy;

    r = to_chars_sub(buf, fmt(f, precision));
    EXPECT_EQ(r, to_csubstr(flt)) << "num=" << f;
    from_chars(r, &copy);
    if(sizeof(T) == sizeof(float))
    {
        EXPECT_FLOAT_EQ((float)fltv, (float)copy);
    }
    else
    {
        EXPECT_DOUBLE_EQ(fltv, copy);
    }

    r = to_chars_sub(buf, fmt(f, precision, FTOA_SCIENT));
    EXPECT_EQ(r, to_csubstr(scient)) << "num=" << f;
    from_chars(r, &copy);
    if(sizeof(T) == sizeof(float))
    {
        EXPECT_FLOAT_EQ((float)scientv, (float)copy);
    }
    else
    {
        EXPECT_DOUBLE_EQ(scientv, copy);
    }
}

TEST(to_chars, fmt_basic)
{
    char bufc[128];
    substr buf(bufc);

    size_t s = 0xff;
    EXPECT_EQ(to_chars_sub(buf, fmt( int8_t(0x7f), 16)), "0x7f");
    EXPECT_EQ(to_chars_sub(buf, fmt(uint8_t(0xff), 16)), "0xff");

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
}

  
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T, class U>
void test_base64_str(T const& val, csubstr expected, U *ws)
{
    char buf_[512];
    substr buf(buf_);

    csubstr encoded = to_chars_sub(buf, base64(val));
    EXPECT_EQ(encoded, expected);
    EXPECT_EQ(encoded.len % 4, 0);

    auto req = base64(*ws);
    size_t written = from_chars(encoded, &req);
    EXPECT_EQ(ws->first(written), val);
}

template<class T>
void test_base64(T const& val, csubstr expected, T *ws)
{
    char buf_[512];
    substr buf(buf_);

    csubstr encoded = to_chars_sub(buf, base64(val));
    EXPECT_EQ(encoded, expected);
    EXPECT_EQ(encoded.len % 4, 0);

    auto req = base64(*ws);
    size_t written = from_chars(encoded, &req);
    EXPECT_EQ(written, sizeof(T));
    EXPECT_EQ(*ws, val);
}

template<class T>
struct base64_test_pair
{
    T val;
    csubstr encoded;
};

base64_test_pair<csubstr> base64_str_pairs[] = {
    {""                    , ""                            },
    {"0"                   , "MA=="                        },
    {"1"                   , "MQ=="                        },
    {"2"                   , "Mg=="                        },
    {"3"                   , "Mw=="                        },
    {"4"                   , "NA=="                        },
    {"5"                   , "NQ=="                        },
    {"6"                   , "Ng=="                        },
    {"7"                   , "Nw=="                        },
    {"8"                   , "OA=="                        },
    {"9"                   , "OQ=="                        },
    {"10"                  , "MTA="                        },
    {"123"                 , "MTIz"                        },
    {"1234"                , "MTIzNA=="                    },
    {"1235"                , "MTIzNQ=="                    },
    {"Man"                 , "TWFu"                        },
    {"Ma"                  , "TWE="                        },
    {"M"                   , "TQ=="                        },
    {"any carnal pleasure.", "YW55IGNhcm5hbCBwbGVhc3VyZS4="},
    {"any carnal pleasure" , "YW55IGNhcm5hbCBwbGVhc3VyZQ=="},
    {"any carnal pleasur"  , "YW55IGNhcm5hbCBwbGVhc3Vy"    },
    {"any carnal pleasu"   , "YW55IGNhcm5hbCBwbGVhc3U="    },
    {"any carnal pleas"    , "YW55IGNhcm5hbCBwbGVhcw=="    },
    {"pleasure."           , "cGxlYXN1cmUu"                },
    { "leasure."           , "bGVhc3VyZS4="                },
    {  "easure."           , "ZWFzdXJlLg=="                },
    {   "asure."           , "YXN1cmUu"                    },
    {    "sure."           , "c3VyZS4="                    },
};

base64_test_pair<int> base64_int_pairs[] = {
    {   0, "AAAAAA=="},
    {   1, "AQAAAA=="},
    {   2, "AgAAAA=="},
    {   3, "AwAAAA=="},
    {   4, "BAAAAA=="},
    {   5, "BQAAAA=="},
    {   6, "BgAAAA=="},
    {   7, "BwAAAA=="},
    {   8, "CAAAAA=="},
    {   9, "CQAAAA=="},
    {  10, "CgAAAA=="},
    {1234, "0gQAAA=="},
};

TEST(base64, str)
{
    char buf_[512];
    substr buf(buf_);

    for(auto p : base64_str_pairs)
    {
        SCOPED_TRACE(p.val);
        test_base64_str(p.val, p.encoded, &buf);
    }
}

TEST(base64, int)
{
    int val;

    for(auto p : base64_int_pairs)
    {
        SCOPED_TRACE(p.val);
        test_base64(p.val, p.encoded, &val);
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST(cat, vars)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = cat(buf, 1, ' ', 2, ' ', 3, ' ', 4);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");
}

#ifdef C4_TUPLE_TO_STR
TEST(cat, tuple)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = cat(buf, std::forward_as_tuple(1, ' ', 2, ' ', 3, ' ', 4));
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");
}
#endif // C4_TUPLE_TO_STR

TEST(uncat, vars)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    sz = uncat("1 2 3 4", v1, v2, v3, v4);
    EXPECT_NE(sz, csubstr::npos);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST(uncat, tuple)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = uncat("1 2 3 4", tp);
    EXPECT_NE(sz, csubstr::npos);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR

TEST(catsep, vars)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = catsep(buf, ' ', 1, 2);
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2");

    sz = catsep(buf, '/', 1, 2);
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2");

    sz = catsep(buf, ' ', 1, 2, 3, 4);
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");

    sz = catsep(buf, '/', 1, 2, 3, 4);
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2/3/4");
}

#ifdef C4_TUPLE_TO_STR
TEST(catsep, tuple)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = catsep(buf, ' ', std::forward_as_tuple(1, 2));
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2");

    sz = catsep(buf, '/', std::forward_as_tuple(1, 2));
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2");

    sz = catsep(buf, ' ', std::forward_as_tuple(1, 2, 3, 4));
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");

    sz = catsep(buf, '/', std::forward_as_tuple(1, 2, 3, 4));
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2/3/4");
}
#endif // C4_TUPLE_TO_STR

TEST(uncatsep, vars)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    char sep;

    sz = uncatsep("1 2 3 4", sep, v1, v2, v3, v4);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST(uncatsep, tuple)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    char sep;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = uncatsep("1 2 3 4", sep, tp);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR

TEST(format, vars)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4);
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4, 5, 6, 7);
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3);
    EXPECT_EQ(sz, 20);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and {}");

    sz = format(buf, "{} and {} and {} and {}", 1, 2);
    EXPECT_EQ(sz, 21);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and {} and {}");

    sz = format(buf, "{} and {} and {} and {}", 1);
    EXPECT_EQ(sz, 22);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and {} and {} and {}");

    sz = format(buf, "{} and {} and {} and {}");
    EXPECT_EQ(sz, 23);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "{} and {} and {} and {}");
}

#ifdef C4_TUPLE_TO_STR
TEST(format, tuple)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3, 4));
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3, 4, 5, 6, 7));
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3));
    EXPECT_EQ(sz, 20);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and {}");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2));
    EXPECT_EQ(sz, 21);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and {} and {}");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1));
    EXPECT_EQ(sz, 22);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and {} and {} and {}");

    sz = format(buf, "{} and {} and {} and {}");
    EXPECT_EQ(sz, 23);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "{} and {} and {} and {}");
}
#endif // C4_TUPLE_TO_STR

TEST(unformat, vars)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    sz = unformat("1 and 2 and 3 and 4", "{} and {} and {} and {}", v1, v2, v3, v4);
    EXPECT_EQ(sz, 19);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST(unformat, tuple)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = unformat("1 and 2 and 3 and 4", "{} and {} and {} and {}", tp);
    EXPECT_EQ(sz, 19);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR


TEST(printf, basic)
{
    char bufc[128];
    substr buf = bufc;
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = sprintf(buf, "hello world");
    EXPECT_EQ(buf.first(ret), "hello world");

    memset(buf.str, 0, buf.len);
    ret = sprintf(buf, "%d %d %d %d", 1, 2, 3, 4);
    EXPECT_EQ(buf.first(ret), "1 2 3 4");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(catrs, basic)
{
    std::vector<char> buf;

    catrs(&buf, 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1234");
    catrs(&buf, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5678");
}

TEST(catrs, basic_append)
{
    std::vector<char> buf;

    catrs(append, &buf, 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1234");
    catrs(append, &buf, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "12345678");
    catrs(append, &buf, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "123456789012345678");
}

TEST(catseprs, basic)
{
    std::vector<char> buf;

    catseprs(&buf, ' ', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 4");
    catseprs(&buf, ' ', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5 6 7 8");

    catseprs(&buf, ',', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1,2,3,4");
    catseprs(&buf, ',', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5,6,7,8");

    catseprs(&buf, '/', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1/2/3/4");
    catseprs(&buf, '/', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5/6/7/8");

    catseprs(&buf, "///", 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1///2///3///4");
    catseprs(&buf, "///", 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5///6///7///8");
}

TEST(catseprs, basic_append)
{
    std::vector<char> buf;

    catseprs(append, &buf, ' ', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 4");
    catseprs(append, &buf, ' ', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 45 6 7 8");
    catseprs(append, &buf, ' ', 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 45 6 7 89 0 1 2 3 4 5 6 7 8");
}

TEST(formatrs, basic)
{
    std::vector<char> buf;

    formatrs(&buf, "{} goes with food, {} goes with heat, {} anytime", "wine", "beer", "coffee");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime");
}

TEST(formatrs, basic_append)
{
    std::vector<char> buf;

    formatrs(append, &buf, "{} goes with food", "wine");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food");
    formatrs(append, &buf, ", {} goes with heat", "beer");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat");
    formatrs(append, &buf, ", {} anytime", "coffee");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime");

    formatrs(append, &buf, ". And water. {} glass of {}cl in the morning clears you up for the day", 1, 40);
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime. And water. 1 glass of 40cl in the morning clears you up for the day");
}

} // namespace c4
