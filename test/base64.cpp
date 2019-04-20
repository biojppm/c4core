#include "c4/std/string.hpp"
#include "c4/std/vector.hpp"
#include "c4/test.hpp"
#include "c4/format.hpp"
#include "c4/base64.hpp"

namespace c4 {

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T, class U>
void test_base64_str(T const& val, csubstr expected, U *ws)
{
    char buf_[512];
    substr buf(buf_);

    csubstr encoded = to_chars_sub(buf, cbase64(val));
    EXPECT_TRUE(base64_valid(encoded));
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

    csubstr encoded = to_chars_sub(buf, cbase64(&val));
    EXPECT_TRUE(base64_valid(encoded));
    EXPECT_EQ(encoded, expected);
    EXPECT_EQ(encoded.len % 4, 0);

    auto req = base64(ws);
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

} // namespace c4
