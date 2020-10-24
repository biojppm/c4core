#include "c4/test.hpp"
#include "c4/charconv.hpp"
#include "c4/std/string.hpp"
#include "c4/std/vector.hpp"
#include "c4/format.hpp"
#include "c4/libtest/supprwarn_push.hpp"

#ifdef __clang__
#   pragma clang diagnostic push
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

namespace c4 {

TEST(itoa, int8_t)
{
    char bufc[64];
    substr buf = bufc;
    int8_t val = -128;
    size_t ret = itoa(buf, val);
    EXPECT_EQ(buf.first(ret), "-128");
    val = 127;
    ret = itoa(buf, val);
    EXPECT_EQ(buf.first(ret), "127");
}

TEST(itoa, int16_t)
{
    char bufc[64];
    substr buf = bufc;
    int16_t val = -32768;
    size_t ret = itoa(buf, val);
    EXPECT_EQ(buf.first(ret), "-32768");
    val = 32767;
    ret = itoa(buf, val);
    EXPECT_EQ(buf.first(ret), "32767");
}

TEST(itoa, shortbuf)
{
    char buf0_[1];
    char buf1_[2];
    char buf2_[3];
    char buf3_[4];
    char buf4_[5];
    substr buf0 = buf0_;
    substr buf1 = buf1_;
    substr buf2 = buf2_;
    substr buf3 = buf3_;
    substr buf4 = buf4_;
    EXPECT_EQ(buf0.len, 0);
    EXPECT_EQ(buf1.len, 1);
    EXPECT_EQ(buf2.len, 2);
    EXPECT_EQ(buf3.len, 3);
    EXPECT_EQ(buf4.len, 4);

#define _chktoa(fn, in, expected_)                                      \
    {                                                                   \
        csubstr expected = expected_;                                   \
        auto rdx = decltype(in)(16);                                    \
                                                                        \
        buf0.fill('?');                                                 \
        size_t ret0 = fn(buf0, in, rdx);                                \
        EXPECT_EQ(ret0, expected.len);                                  \
                                                                        \
        buf1.fill('?');                                                 \
        EXPECT_EQ(buf1, "?");                                           \
        size_t ret1 = fn(buf1, in, rdx);                                \
        EXPECT_EQ(ret1, expected.len);                                  \
        if(ret1 <= buf1.len && buf1.len >= expected.len)                \
        {                                                               \
            EXPECT_EQ(buf1.first(ret1), expected);                      \
        }                                                               \
                                                                        \
        buf2.fill('?');                                                 \
        EXPECT_EQ(buf2, "??");                                          \
        size_t ret2 = fn(buf2, in, rdx);                                \
        EXPECT_EQ(ret2, expected.len);                                  \
        if(ret2 <= buf2.len && buf2.len >= expected.len)                \
        {                                                               \
            EXPECT_EQ(buf2.first(ret2), expected);                      \
        }                                                               \
                                                                        \
        buf3.fill('?');                                                 \
        EXPECT_EQ(buf3, "???");                                         \
        size_t ret3 = fn(buf3, in, rdx);                                \
        EXPECT_EQ(ret3, expected.len);                                  \
        if(ret3 <= buf3.len && buf3.len >= expected.len)                \
        {                                                               \
            EXPECT_EQ(buf3.first(ret3), expected);                      \
        }                                                               \
                                                                        \
        buf4.fill('?');                                                 \
        EXPECT_EQ(buf4, "????");                                        \
        size_t ret4 = fn(buf4, in, rdx);                                \
        EXPECT_EQ(ret4, expected.len);                                  \
        if(ret4 <= buf4.len && buf4.len >= expected.len)                \
        {                                                               \
            EXPECT_EQ(buf4.first(ret4), expected);                      \
        }                                                               \
    }

    _chktoa(itoa, 0, "0x0");
    _chktoa(utoa, 0u, "0x0");
    _chktoa(itoa, -0, "0x0");

    _chktoa(itoa, 1, "0x1");
    _chktoa(utoa, 1u, "0x1");
    _chktoa(itoa, -1, "-0x1");

    _chktoa(itoa, 15, "0xf");
    _chktoa(utoa, 15u, "0xf");
    _chktoa(itoa, -15, "-0xf");

    _chktoa(itoa, 255, "0xff");
    _chktoa(utoa, 255u, "0xff");
    _chktoa(itoa, -255, "-0xff");

    _chktoa(itoa, 256, "0x100");
    _chktoa(utoa, 256u, "0x100");
    _chktoa(itoa, -256, "-0x100");

    _chktoa(itoa, 4096, "0x1000");
    _chktoa(utoa, 4096u, "0x1000");
    _chktoa(itoa, -4096, "-0x1000");
}


template<class ItoaOrUtoa, class ItoaOrUtoaRdx, class I>
void test_prefixed_number_on_empty_buffer(ItoaOrUtoa fn, ItoaOrUtoaRdx rfn, I num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    char bufc[64];
    size_t ret;
    substr emp; // empty
    substr buf = bufc;

    auto ss2  = to_csubstr(r2);
    auto ss8  = to_csubstr(r8);
    auto ss10 = to_csubstr(r10);
    auto ss16 = to_csubstr(r16);

#define _c4clbuf() \
    memset(buf.str, 0, buf.len);\
    buf[0] = 'a';\
    buf[1] = 'a';\
    buf[2] = '\0';

    _c4clbuf();
    ret = rfn(emp, num, 2);
    EXPECT_EQ(ret, ss2.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 2);
    EXPECT_EQ(buf.first(ret), ss2) << "num=" << num;

    _c4clbuf();
    ret = rfn(emp, num, 8);
    EXPECT_EQ(ret, ss8.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 8);
    EXPECT_EQ(buf.first(ret), ss8) << "num=" << num;

    _c4clbuf();
    ret = rfn(emp, num, 10);
    EXPECT_EQ(ret, ss10.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 10);
    EXPECT_EQ(buf.first(ret), ss10) << "num=" << num;

    _c4clbuf();
    ret = fn(emp, num);
    EXPECT_EQ(ret, ss10.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = fn(buf, num);
    EXPECT_EQ(buf.first(ret), ss10) << "num=" << num;

    _c4clbuf();
    ret = rfn(emp, num, 16);
    EXPECT_EQ(ret, ss16.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 16);
    EXPECT_EQ(buf.first(ret), ss16) << "num=" << num;

#undef _c4clbuf
}

// need these functions for overload disambiguation
size_t call_itoa(substr s, int num)
{
    return itoa(s, num);
}
size_t call_itoa_radix(substr s, int num, int radix)
{
    return itoa(s, num, radix);
}

// need these functions for overload disambiguation
size_t call_utoa(substr s, unsigned num)
{
    return utoa(s, num);
}
size_t call_utoa_radix(substr s, unsigned num, unsigned radix)
{
    return utoa(s, num, radix);
}

TEST(itoa, prefixed_number_on_empty_buffer)
{
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,   0,      "0b0",   "0o0",   "0",   "0x0");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix, -10,  "-0b1010", "-0o12", "-10",  "-0xa");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,  10,   "0b1010",  "0o12",  "10",   "0xa");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix, -20, "-0b10100", "-0o24", "-20", "-0x14");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,  20,  "0b10100",  "0o24",  "20",  "0x14");
}

TEST(utoa, prefixed_number_on_empty_buffer)
{
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix,  0u, "0b0"    ,  "0o0",  "0",  "0x0");
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix, 10u, "0b1010" , "0o12", "10",  "0xa");
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix, 20u, "0b10100", "0o24", "20", "0x14");
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST(read_dec, fail)
{
    int dec = 1;
    EXPECT_FALSE(detail::read_dec("zzzz", &dec));
    EXPECT_TRUE(detail::read_dec("00000", &dec));
    EXPECT_EQ(dec, 0);
    dec = 1;
    EXPECT_TRUE(atoi("00000", &dec));
    EXPECT_EQ(dec, 0);
    EXPECT_TRUE(atoi("00010", &dec));
    EXPECT_EQ(dec, 10);
}

TEST(read_hex, fail)
{
    int dec = 1;
    EXPECT_FALSE(detail::read_hex("zzzz", &dec));
    EXPECT_TRUE(detail::read_hex("00000", &dec));
    EXPECT_EQ(dec, 0);
    dec = 1;
    EXPECT_TRUE(atoi("0x00000", &dec));
    EXPECT_EQ(dec, 0);
    EXPECT_TRUE(atoi("0x00010", &dec));
    EXPECT_EQ(dec, 16);
    dec = 1;
    EXPECT_TRUE(atoi("0X00000", &dec));
    EXPECT_EQ(dec, 0);
    EXPECT_TRUE(atoi("0X00010", &dec));
    EXPECT_EQ(dec, 16);
}

TEST(read_oct, fail)
{
    int dec;
    EXPECT_FALSE(detail::read_oct("zzzz", &dec));
    EXPECT_TRUE(detail::read_oct("00000", &dec));
    EXPECT_EQ(dec, 0);
    dec = 1;
    EXPECT_TRUE(atoi("0o00000", &dec));
    EXPECT_EQ(dec, 0);
    EXPECT_TRUE(atoi("0o00010", &dec));
    EXPECT_EQ(dec, 8);
    dec = 1;
    EXPECT_TRUE(atoi("0O00000", &dec));
    EXPECT_EQ(dec, 0);
    EXPECT_TRUE(atoi("0O00010", &dec));
    EXPECT_EQ(dec, 8);
}

TEST(read_bin, fail)
{
    int dec;
    EXPECT_FALSE(detail::read_bin("zzzz", &dec));
    EXPECT_TRUE(detail::read_bin("00000", &dec));
    EXPECT_EQ(dec, 0);
    dec = 1;
    EXPECT_TRUE(atoi("0b00000", &dec));
    EXPECT_EQ(dec, 0);
    EXPECT_TRUE(atoi("0b00010", &dec));
    EXPECT_EQ(dec, 2);
    dec = 1;
    EXPECT_TRUE(atoi("0B00000", &dec));
    EXPECT_EQ(dec, 0);
    EXPECT_TRUE(atoi("0B00010", &dec));
    EXPECT_EQ(dec, 2);
}

template<class ItoaOrUtoa, class ItoaOrUtoaRdx, class Atoi, class I>
void test_toa_radix(ItoaOrUtoa fn, ItoaOrUtoaRdx rfn, Atoi aifn, substr buf, I num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    size_t ret;
    bool ok;
    I result;

    // binary
    memset(buf.str, 0, buf.len);
    ret = rfn(buf, num, 2);
    EXPECT_EQ(buf.first(ret), to_csubstr(r2)) << "num=" << num;
    ok = aifn(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    // octal
    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 8);
    EXPECT_EQ(buf.first(ret), to_csubstr(r8)) << "num=" << num;
    ok = aifn(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    // decimal, explicit
    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 10);
    EXPECT_EQ(buf.first(ret), to_csubstr(r10)) << "num=" << num;
    ok = aifn(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    // decimal, implicit
    memset(buf.str, 0, ret);
    ret = fn(buf, num);
    EXPECT_EQ(buf.first(ret), to_csubstr(r10)) << "num=" << num;
    ok = aifn(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    // hexadecimal
    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 16);
    EXPECT_EQ(buf.first(ret), to_csubstr(r16)) << "num=" << num;
    ok = aifn(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;
}

void test_utoa_radix(substr buf, unsigned num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    test_toa_radix(&call_utoa, &call_utoa_radix, &atou<unsigned>, buf, num, r2, r8, r10, r16);
}

void test_itoa_radix(substr buf, int num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    size_t ret;

    ASSERT_GE(num, 0);
    test_toa_radix(&call_itoa, &call_itoa_radix, &atoi<int>, buf, num, r2, r8, r10, r16);

    if(num == 0) return;
    // test negative values
    num *= -1;
    char nbufc[128];
    csubstr nbuf;
    bool ok;
    int result;

#define _c4getn(which) \
{\
    nbufc[0] = '-'; \
    memcpy(nbufc+1, which, strlen(which)+1); \
    nbuf.assign(nbufc, 1 + strlen(which)); \
}

    memset(buf.str, 0, buf.len);
    _c4getn(r2);
    ret = itoa(buf, num, 2);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;
    ok = atoi(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r8);
    ret = itoa(buf, num, 8);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;
    ok = atoi(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r10);
    ret = itoa(buf, num, 10);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;
    ok = atoi(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r10);
    ret = itoa(buf, num);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;
    ok = atoi(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r16);
    ret = itoa(buf, num, 16);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;
    ok = atoi(buf.first(ret), &result);
    EXPECT_TRUE(ok) << "num=" << num;
    EXPECT_EQ(result, num) << "num=" << num;
#undef _c4getn
}

TEST(itoa, radix_basic)
{
    char bufc[100] = {0};
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    test_itoa_radix(buf,   0,         "0b0",   "0o0",   "0",   "0x0");
    test_itoa_radix(buf,   1,         "0b1",   "0o1",   "1",   "0x1");
    test_itoa_radix(buf,   2,        "0b10",   "0o2",   "2",   "0x2");
    test_itoa_radix(buf,   3,        "0b11",   "0o3",   "3",   "0x3");
    test_itoa_radix(buf,   4,       "0b100",   "0o4",   "4",   "0x4");
    test_itoa_radix(buf,   5,       "0b101",   "0o5",   "5",   "0x5");
    test_itoa_radix(buf,   6,       "0b110",   "0o6",   "6",   "0x6");
    test_itoa_radix(buf,   7,       "0b111",   "0o7",   "7",   "0x7");
    test_itoa_radix(buf,   8,      "0b1000",  "0o10",   "8",   "0x8");
    test_itoa_radix(buf,   9,      "0b1001",  "0o11",   "9",   "0x9");
    test_itoa_radix(buf,  10,      "0b1010",  "0o12",  "10",   "0xa");
    test_itoa_radix(buf,  11,      "0b1011",  "0o13",  "11",   "0xb");
    test_itoa_radix(buf,  12,      "0b1100",  "0o14",  "12",   "0xc");
    test_itoa_radix(buf,  13,      "0b1101",  "0o15",  "13",   "0xd");
    test_itoa_radix(buf,  14,      "0b1110",  "0o16",  "14",   "0xe");
    test_itoa_radix(buf,  15,      "0b1111",  "0o17",  "15",   "0xf");
    test_itoa_radix(buf,  16,     "0b10000",  "0o20",  "16",  "0x10");
    test_itoa_radix(buf,  17,     "0b10001",  "0o21",  "17",  "0x11");
    test_itoa_radix(buf,  31,     "0b11111",  "0o37",  "31",  "0x1f");
    test_itoa_radix(buf,  32,    "0b100000",  "0o40",  "32",  "0x20");
    test_itoa_radix(buf,  33,    "0b100001",  "0o41",  "33",  "0x21");
    test_itoa_radix(buf,  63,    "0b111111",  "0o77",  "63",  "0x3f");
    test_itoa_radix(buf,  64,   "0b1000000", "0o100",  "64",  "0x40");
    test_itoa_radix(buf,  65,   "0b1000001", "0o101",  "65",  "0x41");
    test_itoa_radix(buf, 127,   "0b1111111", "0o177", "127",  "0x7f");
    test_itoa_radix(buf, 128,  "0b10000000", "0o200", "128",  "0x80");
    test_itoa_radix(buf, 129,  "0b10000001", "0o201", "129",  "0x81");
    test_itoa_radix(buf, 255,  "0b11111111", "0o377", "255",  "0xff");
    test_itoa_radix(buf, 256, "0b100000000", "0o400", "256", "0x100");
}

TEST(utoa, radix_basic)
{
    char bufc[100] = {0};
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    test_utoa_radix(buf,   0,         "0b0",   "0o0",   "0",   "0x0");
    test_utoa_radix(buf,   1,         "0b1",   "0o1",   "1",   "0x1");
    test_utoa_radix(buf,   2,        "0b10",   "0o2",   "2",   "0x2");
    test_utoa_radix(buf,   3,        "0b11",   "0o3",   "3",   "0x3");
    test_utoa_radix(buf,   4,       "0b100",   "0o4",   "4",   "0x4");
    test_utoa_radix(buf,   5,       "0b101",   "0o5",   "5",   "0x5");
    test_utoa_radix(buf,   6,       "0b110",   "0o6",   "6",   "0x6");
    test_utoa_radix(buf,   7,       "0b111",   "0o7",   "7",   "0x7");
    test_utoa_radix(buf,   8,      "0b1000",  "0o10",   "8",   "0x8");
    test_utoa_radix(buf,   9,      "0b1001",  "0o11",   "9",   "0x9");
    test_utoa_radix(buf,  10,      "0b1010",  "0o12",  "10",   "0xa");
    test_utoa_radix(buf,  11,      "0b1011",  "0o13",  "11",   "0xb");
    test_utoa_radix(buf,  12,      "0b1100",  "0o14",  "12",   "0xc");
    test_utoa_radix(buf,  13,      "0b1101",  "0o15",  "13",   "0xd");
    test_utoa_radix(buf,  14,      "0b1110",  "0o16",  "14",   "0xe");
    test_utoa_radix(buf,  15,      "0b1111",  "0o17",  "15",   "0xf");
    test_utoa_radix(buf,  16,     "0b10000",  "0o20",  "16",  "0x10");
    test_utoa_radix(buf,  17,     "0b10001",  "0o21",  "17",  "0x11");
    test_utoa_radix(buf,  31,     "0b11111",  "0o37",  "31",  "0x1f");
    test_utoa_radix(buf,  32,    "0b100000",  "0o40",  "32",  "0x20");
    test_utoa_radix(buf,  33,    "0b100001",  "0o41",  "33",  "0x21");
    test_utoa_radix(buf,  63,    "0b111111",  "0o77",  "63",  "0x3f");
    test_utoa_radix(buf,  64,   "0b1000000", "0o100",  "64",  "0x40");
    test_utoa_radix(buf,  65,   "0b1000001", "0o101",  "65",  "0x41");
    test_utoa_radix(buf, 127,   "0b1111111", "0o177", "127",  "0x7f");
    test_utoa_radix(buf, 128,  "0b10000000", "0o200", "128",  "0x80");
    test_utoa_radix(buf, 129,  "0b10000001", "0o201", "129",  "0x81");
    test_utoa_radix(buf, 255,  "0b11111111", "0o377", "255",  "0xff");
    test_utoa_radix(buf, 256, "0b100000000", "0o400", "256", "0x100");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST(atoi, basic)
{
    char bufc[100] = {0};
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    size_t ret;

#define _woof(val) \
    ret = itoa(buf, val); EXPECT_LT(ret, buf.len); EXPECT_EQ(buf.sub(0, ret), #val)
    _woof(0);
    _woof(1);
    _woof(2);
    _woof(3);
    _woof(4);
    _woof(5);
    _woof(6);
    _woof(7);
    _woof(8);
    _woof(9);
    _woof(10);
    _woof(11);
    _woof(12);
    _woof(13);
    _woof(14);
    _woof(15);
    _woof(16);
    _woof(17);
    _woof(18);
    _woof(19);
    _woof(20);
    _woof(21);
    _woof(100);
    _woof(1000);
    _woof(11);
    _woof(101);
    _woof(1001);
    _woof(10001);
#undef _woof
}

template<class T>
void test_atoi(const char* num_, T expected)
{
    SCOPED_TRACE(num_);
    csubstr num = to_csubstr(num_);
    T val;
    bool ok = atoi(num, &val);
    EXPECT_TRUE(ok);
    EXPECT_EQ(val, expected) << num;
}

template<class T>
void test_atou(csubstr num, T expected)
{
    T val;
    bool ok = atou(num, &val);
    EXPECT_TRUE(ok);
    EXPECT_EQ(val, expected) << num;
}

TEST(atoi, bin)
{
    test_atoi("0b0", 0);
    test_atoi("0B0", 0);
    test_atoi("0b0000000000", 0);
    test_atoi("0B0000000000", 0);
    test_atoi("0b1", 1);
    test_atoi("0B1", 1);
    test_atoi("0b00000000001", 1);
    test_atoi("0B00000000001", 1);
    test_atoi("0b10", 2);
    test_atoi("0B10", 2);
    test_atoi("0b11", 3);
    test_atoi("0B11", 3);
    test_atoi("0b100", 4);
    test_atoi("0B100", 4);
    test_atoi("0b101", 5);
    test_atoi("0B101", 5);
    test_atoi("0b110", 6);
    test_atoi("0B110", 6);
    test_atoi("0b111", 7);
    test_atoi("0B111", 7);
    test_atoi("0b1000", 8);
    test_atoi("0B1000", 8);
}

TEST(atou, bin)
{
    test_atou("0b0", 0);
    test_atou("0B0", 0);
    test_atou("0b0000000000", 0);
    test_atou("0B0000000000", 0);
    test_atou("0b1", 1);
    test_atou("0B1", 1);
    test_atou("0b00000000001", 1);
    test_atou("0B00000000001", 1);
    test_atou("0b10", 2);
    test_atou("0B10", 2);
    test_atou("0b11", 3);
    test_atou("0B11", 3);
    test_atou("0b100", 4);
    test_atou("0B100", 4);
    test_atou("0b101", 5);
    test_atou("0B101", 5);
    test_atou("0b110", 6);
    test_atou("0B110", 6);
    test_atou("0b111", 7);
    test_atou("0B111", 7);
    test_atou("0b1000", 8);
    test_atou("0B1000", 8);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void test_ftoa(substr buf, float f, int precision, const char *scient, const char *flt, const char* flex, const char *hexa, const char *hexa_alternative=nullptr)
{
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = ftoa(buf, f, precision, FTOA_SCIENT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(scient)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision, FTOA_FLOAT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flt)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision+1, FTOA_FLEX);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flex)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision, FTOA_HEXA);
    if(!hexa_alternative) hexa_alternative = hexa;
    std::string report;
    from_chars(buf.left_of(ret), &report);
    EXPECT_TRUE(buf.left_of(ret) == to_csubstr(hexa) || buf.left_of(ret) == to_csubstr(hexa_alternative)) << "num=" << f << "   ret='" << report << "'  hexa='" << hexa << "'  hexa_alternative='" << hexa_alternative << "'";
}

void test_dtoa(substr buf, double f, int precision, const char *scient, const char *flt, const char* flex, const char *hexa, const char *hexa_alternative=nullptr)
{
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = dtoa(buf, f, precision, FTOA_SCIENT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(scient)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision, FTOA_FLOAT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flt)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision+1, FTOA_FLEX);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flex)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision, FTOA_HEXA);
    if(!hexa_alternative) hexa_alternative = hexa;
    std::string report;
    from_chars(buf.left_of(ret), &report);
    EXPECT_TRUE(buf.left_of(ret) == to_csubstr(hexa) || buf.left_of(ret) == to_csubstr(hexa_alternative)) << "num=" << f << "   ret='" << report << "'  hexa='" << hexa << "'  hexa_alternative='" << hexa_alternative << "'";
}


TEST(ftoa, basic)
{
    char bufc[128];
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    float f = 1.1234123f;
    double d = 1.1234123;

    {
        SCOPED_TRACE("precision 0");
        test_ftoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
        test_dtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    }

    {
        SCOPED_TRACE("precision 1");
        test_ftoa(buf, f, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");
        test_dtoa(buf, d, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");
    }

    {
        SCOPED_TRACE("precision 2");
        test_ftoa(buf, f, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0");
        test_dtoa(buf, d, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0");
    }

    {
        SCOPED_TRACE("precision 3");
        test_ftoa(buf, f, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0");
    }

    {
        SCOPED_TRACE("precision 4");
        test_ftoa(buf, f, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");
        test_dtoa(buf, d, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");
    }

    f = 1.01234123f;
    d = 1.01234123;

    {
        SCOPED_TRACE("precision 0");
        test_ftoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
        test_dtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    }

    {
        SCOPED_TRACE("precision 1");
        test_ftoa(buf, f, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
        test_dtoa(buf, d, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
    }

    {
        SCOPED_TRACE("precision 2");
        test_ftoa(buf, f, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");
        test_dtoa(buf, d, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");
    }

    {
        SCOPED_TRACE("precision 3");
        #if defined(_MSC_VER) || defined(C4_MACOS) || defined(C4_IOS) // there are differences in the hexa formatting
        test_ftoa(buf, f, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0", /*hexa*/"0x1.032p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0", /*hexa*/"0x1.032p+0");
        #else
        test_ftoa(buf, f, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0");
        #endif
    }

    {
        SCOPED_TRACE("precision 4");
        test_ftoa(buf, f, 4, /*scient*/"1.0123e+00", /*flt*/"1.0123", /*flex*/"1.0123", /*hexa*/"0x1.0329p+0");
        test_dtoa(buf, d, 4, /*scient*/"1.0123e+00", /*flt*/"1.0123", /*flex*/"1.0123", /*hexa*/"0x1.0329p+0");
    }
}


//-----------------------------------------------------------------------------

#ifdef WORK_IN_PROGRESS

#define __(buf_, expected, eps)                         \
    {                                                   \
        csubstr buf(buf_);                              \
        float f;                                        \
        size_t fret = detail::scan_one_real(buf, &f);   \
        EXPECT_EQ(fret, buf.len);                       \
        EXPECT_NEAR(f, float(expected), float(eps));    \
        double d;                                       \
        size_t dret = detail::scan_one_real(buf, &d);   \
        EXPECT_EQ(dret, buf.len);                       \
        EXPECT_NEAR(d, double(expected), float(eps));   \
    }

TEST(scan_one_real, zeros)
{
    __( "0", 0.0, 0.0);
    __("-0", 0.0, 0.0);
    __("+0", 0.0, 0.0);
    __(".0", 0.0, 0.0);
    __("0.0", 0.0, 0.0);

    __("0", 0.0, 0.0);
    __("00", 0.0, 0.0);
    __("000", 0.0, 0.0);
    __("0000", 0.0, 0.0);
    __(".0", 0.0, 0.0);
    __(".00", 0.0, 0.0);
    __(".000", 0.0, 0.0);
    __(".0000", 0.0, 0.0);
    __("0.", 0.0, 0.0);
    __("0.0", 0.0, 0.0);
    __("0.00", 0.0, 0.0);
    __("0.000", 0.0, 0.0);
    __("0.0000", 0.0, 0.0);
    __("00.", 0.0, 0.0);
    __("00.0", 0.0, 0.0);
    __("00.00", 0.0, 0.0);
    __("00.000", 0.0, 0.0);
    __("00.0000", 0.0, 0.0);
    __("000.", 0.0, 0.0);
    __("000.0", 0.0, 0.0);
    __("000.00", 0.0, 0.0);
    __("000.000", 0.0, 0.0);
    __("000.0000", 0.0, 0.0);
    __("0000.", 0.0, 0.0);
    __("0000.0", 0.0, 0.0);
    __("0000.00", 0.0, 0.0);
    __("0000.000", 0.0, 0.0);
    __("0000.0000", 0.0, 0.0);

    __("-0", 0.0, 0.0);
    __("-00", 0.0, 0.0);
    __("-000", 0.0, 0.0);
    __("-0000", 0.0, 0.0);
    __("-.0", 0.0, 0.0);
    __("-.00", 0.0, 0.0);
    __("-.000", 0.0, 0.0);
    __("-.0000", 0.0, 0.0);
    __("-0.", 0.0, 0.0);
    __("-0.0", 0.0, 0.0);
    __("-0.00", 0.0, 0.0);
    __("-0.000", 0.0, 0.0);
    __("-0.0000", 0.0, 0.0);
    __("-00.", 0.0, 0.0);
    __("-00.0", 0.0, 0.0);
    __("-00.00", 0.0, 0.0);
    __("-00.000", 0.0, 0.0);
    __("-00.0000", 0.0, 0.0);
    __("-000.", 0.0, 0.0);
    __("-000.0", 0.0, 0.0);
    __("-000.00", 0.0, 0.0);
    __("-000.000", 0.0, 0.0);
    __("-000.0000", 0.0, 0.0);
    __("-0000.", 0.0, 0.0);
    __("-0000.0", 0.0, 0.0);
    __("-0000.00", 0.0, 0.0);
    __("-0000.000", 0.0, 0.0);
    __("-0000.0000", 0.0, 0.0);

    __("+0", 0.0, 0.0);
    __("+00", 0.0, 0.0);
    __("+000", 0.0, 0.0);
    __("+0000", 0.0, 0.0);
    __("+.0", 0.0, 0.0);
    __("+.00", 0.0, 0.0);
    __("+.000", 0.0, 0.0);
    __("+.0000", 0.0, 0.0);
    __("+0.", 0.0, 0.0);
    __("+0.0", 0.0, 0.0);
    __("+0.00", 0.0, 0.0);
    __("+0.000", 0.0, 0.0);
    __("+0.0000", 0.0, 0.0);
    __("+00.", 0.0, 0.0);
    __("+00.0", 0.0, 0.0);
    __("+00.00", 0.0, 0.0);
    __("+00.000", 0.0, 0.0);
    __("+00.0000", 0.0, 0.0);
    __("+000.", 0.0, 0.0);
    __("+000.0", 0.0, 0.0);
    __("+000.00", 0.0, 0.0);
    __("+000.000", 0.0, 0.0);
    __("+000.0000", 0.0, 0.0);
    __("+0000.", 0.0, 0.0);
    __("+0000.0", 0.0, 0.0);
    __("+0000.00", 0.0, 0.0);
    __("+0000.000", 0.0, 0.0);
    __("+0000.0000", 0.0, 0.0);
}

TEST(scan_one_real, hexadecimal)
{

}

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


TEST(to_chars, std_string)
{
    std::string foo("foo");
    char buf_[32];
    substr buf(buf_);
    size_t result = to_chars(buf, foo);
    EXPECT_EQ(result, 3);
    EXPECT_EQ(buf.first(3), "foo");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST(to_chars, bool)
{
    char buf_[32];
    substr buf(buf_);
    csubstr result = to_chars_sub(buf, true);
    EXPECT_EQ(result, "1");
    result = to_chars_sub(buf, false);
    EXPECT_EQ(result, "0");
}

TEST(from_chars, bool)
{
    bool result = false;
    for(const char *s : {"1", "true", "True", "TRUE"})
    {
        bool ok = from_chars(to_csubstr(s), &result);
        EXPECT_TRUE(ok) << "s='" << s << "'";
        EXPECT_TRUE(result);
    }
    for(const char *s : {"0", "false", "False", "FALSE"})
    {
        bool ok = from_chars(to_csubstr(s), &result);
        EXPECT_TRUE(ok) << "s='" << s << "'";
        EXPECT_FALSE(result);
    }
}

TEST(from_chars_first, bool)
{
    bool result = false;
    for(const char *s : {"1", "10000", "2", "3", "10", "010", "001", "0001", "true", "True", "TRUE"})
    {
        bool ok = from_chars(to_csubstr(s), &result);
        EXPECT_TRUE(ok) << "s='" << s << "'";
        EXPECT_TRUE(result);
    }
    for(const char *s : {"0", "00", "000", "0000", "false", "False", "FALSE"})
    {
        bool ok = from_chars(to_csubstr(s), &result);
        EXPECT_TRUE(ok) << "s='" << s << "'";
        EXPECT_FALSE(result);
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// test that no characters are trimmed at the end of
// the number due to printf-based implementations
// needing space for the \0
template<class T>
void test_trimmed_fit(T v, csubstr expected)
{
    char buf_[128] = {};
    char buf2_[128] = {};
    substr buf(buf_);
    substr buf2(buf_);
    ASSERT_GE(buf.len, expected.len);
    ASSERT_GE(buf2.len, expected.len);
    csubstr result = to_chars_sub(buf, v);
    EXPECT_EQ(result, expected);
    csubstr result2 = to_chars_sub(buf2.sub(result.len), v);
    EXPECT_EQ(result2, result);
    std::string str;
    catrs(&str, v);
    EXPECT_EQ(result, to_csubstr(str));
}

TEST(to_chars, trimmed_fit_int)
{
    test_trimmed_fit(12345678, "12345678");
}

TEST(to_chars, trimmed_fit_float)
{
    test_trimmed_fit(0.375f, "0.375");
    test_trimmed_fit(12.375f, "12.375");
}

TEST(to_chars, trimmed_fit_double)
{
    test_trimmed_fit(0.375, "0.375");
    test_trimmed_fit(12.375, "12.375");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void to_chars_roundtrip(substr buf, T const& val, csubstr expected)
{
    T cp;
    csubstr res = to_chars_sub(buf, val);
    EXPECT_EQ(res, expected);
    bool ok = from_chars(res, &cp);
    EXPECT_TRUE(ok) << "val=" << val;
    EXPECT_EQ(cp, val) << "val=" << val;
}

template<size_t N>
void to_chars_roundtrip(char (&buf)[N], csubstr val)
{
    char cp_[N];
    substr cp(cp_);
    ASSERT_LE(val.len, N);
    csubstr res = to_chars_sub(buf, val);
    EXPECT_EQ(res.len, val.len);
    EXPECT_EQ(res, val);
    bool ok = from_chars(res, &cp);
    EXPECT_TRUE(ok);
    EXPECT_EQ(cp, val);
}


TEST(to_chars, roundtrip_bool)
{
    char buf[128];
    to_chars_roundtrip<bool>(buf, false, "0");
    to_chars_roundtrip<bool>(buf,  true, "1");
}


TEST(to_chars, roundtrip_char)
{
    char buf[128];
    to_chars_roundtrip<char>(buf, 'a', "a");
    to_chars_roundtrip<char>(buf, 'b', "b");
    to_chars_roundtrip<char>(buf, 'c', "c");
    to_chars_roundtrip<char>(buf, 'd', "d");
}

#define C4_TEST_ROUNDTRIP_INT(ty) \
TEST(to_chars, roundtrip_##ty)\
{\
    char buf[128];\
    to_chars_roundtrip<ty>(buf, 0, "0");\
    to_chars_roundtrip<ty>(buf, 1, "1");\
    to_chars_roundtrip<ty>(buf, 2, "2");\
    to_chars_roundtrip<ty>(buf, 3, "3");\
    to_chars_roundtrip<ty>(buf, 4, "4");\
}
C4_TEST_ROUNDTRIP_INT(int8_t)
C4_TEST_ROUNDTRIP_INT(int16_t)
C4_TEST_ROUNDTRIP_INT(int32_t)
C4_TEST_ROUNDTRIP_INT(int64_t)
C4_TEST_ROUNDTRIP_INT(uint8_t)
C4_TEST_ROUNDTRIP_INT(uint16_t)
C4_TEST_ROUNDTRIP_INT(uint32_t)
C4_TEST_ROUNDTRIP_INT(uint64_t)
using ulong = unsigned long;
using uint = unsigned int;
C4_TEST_ROUNDTRIP_INT(int)
C4_TEST_ROUNDTRIP_INT(uint)
C4_TEST_ROUNDTRIP_INT(long)
C4_TEST_ROUNDTRIP_INT(ulong)
// in Mac, some of the following types are not the same as above:
C4_TEST_ROUNDTRIP_INT(size_t)
C4_TEST_ROUNDTRIP_INT(intptr_t)
C4_TEST_ROUNDTRIP_INT(uintptr_t)

#define C4_TEST_ROUNDTRIP_REAL(ty) \
TEST(to_chars, roundtrip_##ty)\
{\
    char buf[128];\
    to_chars_roundtrip<ty>(buf, ty(0.0), "0");\
    to_chars_roundtrip<ty>(buf, ty(1.0), "1");\
    to_chars_roundtrip<ty>(buf, ty(2.0), "2");\
    to_chars_roundtrip<ty>(buf, ty(3.0), "3");\
    to_chars_roundtrip<ty>(buf, ty(4.0), "4");\
}
C4_TEST_ROUNDTRIP_REAL(float)
C4_TEST_ROUNDTRIP_REAL(double)

TEST(to_chars, roundtrip_substr)
{
    char buf[128];
    to_chars_roundtrip(buf, "");
    to_chars_roundtrip(buf, "0");
    to_chars_roundtrip(buf, "1");
    to_chars_roundtrip(buf, "2");
    to_chars_roundtrip(buf, "3");
    to_chars_roundtrip(buf, "4");
    to_chars_roundtrip(buf, "zhis iz a test");
}

} // namespace c4

#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif

#include "c4/libtest/supprwarn_pop.hpp"
