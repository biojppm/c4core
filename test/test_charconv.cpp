#include "c4/std/std.hpp"
#include "c4/charconv.hpp"
#include "c4/format.hpp"

#include "c4/libtest/supprwarn_push.hpp"

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wfloat-equal"
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wuseless-cast"
#   pragma GCC diagnostic ignored "-Wfloat-equal"
#endif

#include <c4/test.hpp>

namespace c4 {

TEST_CASE("itoa.int8_t")
{
    char bufc[64];
    substr buf = bufc;
    int8_t val = -128;
    size_t ret = itoa(buf, val);
    CHECK_EQ(buf.first(ret), "-128");
    val = 127;
    ret = itoa(buf, val);
    CHECK_EQ(buf.first(ret), "127");
}

TEST_CASE("itoa.int16_t")
{
    char bufc[64];
    substr buf = bufc;
    int16_t val = -32768;
    size_t ret = itoa(buf, val);
    CHECK_EQ(buf.first(ret), "-32768");
    val = 32767;
    ret = itoa(buf, val);
    CHECK_EQ(buf.first(ret), "32767");
}

TEST_CASE("itoa.shortbuf")
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
    CHECK_EQ(buf0.len, 0);
    CHECK_EQ(buf1.len, 1);
    CHECK_EQ(buf2.len, 2);
    CHECK_EQ(buf3.len, 3);
    CHECK_EQ(buf4.len, 4);

#define _chktoa(fn, in, expected_)                          \
    {                                                       \
        csubstr expected = expected_;                       \
        auto rdx = decltype(in)(16);                        \
                                                            \
        buf0.fill('?');                                     \
        size_t ret0 = fn(buf0, in, rdx);                    \
        CHECK_EQ(ret0, expected.len);                       \
                                                            \
        buf1.fill('?');                                     \
        CHECK_EQ(buf1, "?");                                \
        size_t ret1 = fn(buf1, in, rdx);                    \
        CHECK_EQ(ret1, expected.len);                       \
        if(ret1 <= buf1.len && buf1.len >= expected.len)    \
        {                                                   \
            CHECK_EQ(buf1.first(ret1), expected);           \
        }                                                   \
                                                            \
        buf2.fill('?');                                     \
        CHECK_EQ(buf2, "??");                               \
        size_t ret2 = fn(buf2, in, rdx);                    \
        CHECK_EQ(ret2, expected.len);                       \
        if(ret2 <= buf2.len && buf2.len >= expected.len)    \
        {                                                   \
            CHECK_EQ(buf2.first(ret2), expected);           \
        }                                                   \
                                                            \
        buf3.fill('?');                                     \
        CHECK_EQ(buf3, "???");                              \
        size_t ret3 = fn(buf3, in, rdx);                    \
        CHECK_EQ(ret3, expected.len);                       \
        if(ret3 <= buf3.len && buf3.len >= expected.len)    \
        {                                                   \
            CHECK_EQ(buf3.first(ret3), expected);           \
        }                                                   \
                                                            \
        buf4.fill('?');                                     \
        CHECK_EQ(buf4, "????");                             \
        size_t ret4 = fn(buf4, in, rdx);                    \
        CHECK_EQ(ret4, expected.len);                       \
        if(ret4 <= buf4.len && buf4.len >= expected.len)    \
        {                                                   \
            CHECK_EQ(buf4.first(ret4), expected);           \
        }                                                   \
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

    INFO("num=" << num);
    _c4clbuf();
    ret = rfn(emp, num, 2);
    CHECK_EQ(ret, ss2.len);
    CHECK_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 2);
    CHECK_EQ(buf.first(ret), ss2);

    _c4clbuf();
    ret = rfn(emp, num, 8);
    CHECK_EQ(ret, ss8.len);
    CHECK_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 8);
    CHECK_EQ(buf.first(ret), ss8);

    _c4clbuf();
    ret = rfn(emp, num, 10);
    CHECK_EQ(ret, ss10.len);
    CHECK_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 10);
    CHECK_EQ(buf.first(ret), ss10);

    _c4clbuf();
    ret = fn(emp, num);
    CHECK_EQ(ret, ss10.len);
    CHECK_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = fn(buf, num);
    CHECK_EQ(buf.first(ret), ss10);

    _c4clbuf();
    ret = rfn(emp, num, 16);
    CHECK_EQ(ret, ss16.len);
    CHECK_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 16);
    CHECK_EQ(buf.first(ret), ss16);

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

TEST_CASE("itoa.prefixed_number_on_empty_buffer")
{
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,   0,      "0b0",   "0o0",   "0",   "0x0");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix, -10,  "-0b1010", "-0o12", "-10",  "-0xa");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,  10,   "0b1010",  "0o12",  "10",   "0xa");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix, -20, "-0b10100", "-0o24", "-20", "-0x14");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,  20,  "0b10100",  "0o24",  "20",  "0x14");
}

TEST_CASE("utoa.prefixed_number_on_empty_buffer")
{
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix,  0u, "0b0"    ,  "0o0",  "0",  "0x0");
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix, 10u, "0b1010" , "0o12", "10",  "0xa");
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix, 20u, "0b10100", "0o24", "20", "0x14");
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE("read_dec.fail")
{
    int dec = 1;
    CHECK_UNARY_FALSE(detail::read_dec("zzzz", &dec));
    CHECK_UNARY(detail::read_dec("00000", &dec));
    CHECK_EQ(dec, 0);
    dec = 1;
    CHECK_UNARY(atoi("00000", &dec));
    CHECK_EQ(dec, 0);
    CHECK_UNARY(atoi("00010", &dec));
    CHECK_EQ(dec, 10);
}

TEST_CASE("read_hex.fail")
{
    int dec = 1;
    CHECK_UNARY_FALSE(detail::read_hex("zzzz", &dec));
    CHECK_UNARY(detail::read_hex("00000", &dec));
    CHECK_EQ(dec, 0);
    dec = 1;
    CHECK_UNARY(atoi("0x00000", &dec));
    CHECK_EQ(dec, 0);
    CHECK_UNARY(atoi("0x00010", &dec));
    CHECK_EQ(dec, 16);
    dec = 1;
    CHECK_UNARY(atoi("0X00000", &dec));
    CHECK_EQ(dec, 0);
    CHECK_UNARY(atoi("0X00010", &dec));
    CHECK_EQ(dec, 16);
}

TEST_CASE("read_oct.fail")
{
    int dec;
    CHECK_UNARY_FALSE(detail::read_oct("zzzz", &dec));
    CHECK_UNARY(detail::read_oct("00000", &dec));
    CHECK_EQ(dec, 0);
    dec = 1;
    CHECK_UNARY(atoi("0o00000", &dec));
    CHECK_EQ(dec, 0);
    CHECK_UNARY(atoi("0o00010", &dec));
    CHECK_EQ(dec, 8);
    dec = 1;
    CHECK_UNARY(atoi("0O00000", &dec));
    CHECK_EQ(dec, 0);
    CHECK_UNARY(atoi("0O00010", &dec));
    CHECK_EQ(dec, 8);
}

TEST_CASE("read_bin.fail")
{
    int dec;
    CHECK_UNARY_FALSE(detail::read_bin("zzzz", &dec));
    CHECK_UNARY(detail::read_bin("00000", &dec));
    CHECK_EQ(dec, 0);
    dec = 1;
    CHECK_UNARY(atoi("0b00000", &dec));
    CHECK_EQ(dec, 0);
    CHECK_UNARY(atoi("0b00010", &dec));
    CHECK_EQ(dec, 2);
    dec = 1;
    CHECK_UNARY(atoi("0B00000", &dec));
    CHECK_EQ(dec, 0);
    CHECK_UNARY(atoi("0B00010", &dec));
    CHECK_EQ(dec, 2);
}

template<class ItoaOrUtoa, class ItoaOrUtoaRdx, class Atoi, class I>
void test_toa_radix(ItoaOrUtoa fn, ItoaOrUtoaRdx rfn, Atoi aifn, substr buf, I num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    size_t ret;
    bool ok;
    I result;

    INFO("num=" << num);

    // binary
    memset(buf.str, 0, buf.len);
    ret = rfn(buf, num, 2);
    CHECK_EQ(buf.first(ret), to_csubstr(r2));
    ok = aifn(buf.first(ret), &result);
    CHECK_UNARY(ok);
    CHECK_EQ(result, num);

    // octal
    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 8);
    CHECK_EQ(buf.first(ret), to_csubstr(r8));
    ok = aifn(buf.first(ret), &result);
    CHECK_UNARY(ok);
    CHECK_EQ(result, num);

    // decimal, explicit
    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 10);
    CHECK_EQ(buf.first(ret), to_csubstr(r10));
    ok = aifn(buf.first(ret), &result);
    CHECK_UNARY(ok);
    CHECK_EQ(result, num);

    // decimal, implicit
    memset(buf.str, 0, ret);
    ret = fn(buf, num);
    CHECK_EQ(buf.first(ret), to_csubstr(r10));
    ok = aifn(buf.first(ret), &result);
    CHECK_UNARY(ok);
    CHECK_EQ(result, num);

    // hexadecimal
    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 16);
    CHECK_EQ(buf.first(ret), to_csubstr(r16));
    ok = aifn(buf.first(ret), &result);
    CHECK_UNARY(ok);
    CHECK_EQ(result, num);
}

void test_utoa_radix(substr buf, unsigned num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    test_toa_radix(&call_utoa, &call_utoa_radix, &atou<unsigned>, buf, num, r2, r8, r10, r16);
}

void test_itoa_radix(substr buf, int num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    size_t ret;

    REQUIRE_GE(num, 0);
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
    CHECK_MESSAGE(buf.first(ret) == nbuf, "num=" << num);
    ok = atoi(buf.first(ret), &result);
    CHECK_MESSAGE(ok, "num=" << num);
    CHECK_MESSAGE(result == num, "num=" << num);

    memset(buf.str, 0, ret);
    _c4getn(r8);
    ret = itoa(buf, num, 8);
    CHECK_MESSAGE(buf.first(ret) == nbuf, "num=" << num);
    ok = atoi(buf.first(ret), &result);
    CHECK_MESSAGE(ok, "num=" << num);
    CHECK_MESSAGE(result == num, "num=" << num);

    memset(buf.str, 0, ret);
    _c4getn(r10);
    ret = itoa(buf, num, 10);
    CHECK_MESSAGE(buf.first(ret) == nbuf, "num=" << num);
    ok = atoi(buf.first(ret), &result);
    CHECK_MESSAGE(ok, "num=" << num);
    CHECK_MESSAGE(result == num, "num=" << num);

    memset(buf.str, 0, ret);
    _c4getn(r10);
    ret = itoa(buf, num);
    CHECK_MESSAGE(buf.first(ret) == nbuf, "num=" << num);
    ok = atoi(buf.first(ret), &result);
    CHECK_MESSAGE(ok, "num=" << num);
    CHECK_MESSAGE(result == num, "num=" << num);

    memset(buf.str, 0, ret);
    _c4getn(r16);
    ret = itoa(buf, num, 16);
    CHECK_MESSAGE(buf.first(ret) == nbuf, "num=" << num);
    ok = atoi(buf.first(ret), &result);
    CHECK_MESSAGE(ok, "num=" << num);
    CHECK_MESSAGE(result == num, "num=" << num);
#undef _c4getn
}

TEST_CASE("itoa.radix_basic")
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

TEST_CASE("utoa.radix_basic")
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

TEST_CASE("atoi.basic")
{
    char bufc[100] = {0};
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    size_t ret;

#define _woof(val) \
    ret = itoa(buf, val); CHECK_LT(ret, buf.len); CHECK_EQ(buf.sub(0, ret), #val)
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
void test_atoi(csubstr num, T expected)
{
    INFO("num=" << num);
    T val;
    bool ok = atoi(num, &val);
    CHECK_UNARY(ok);
    CHECK_EQ(val, expected);
}

template<class T>
void test_atou(csubstr num, T expected)
{
    INFO("num=" << num_);
    T val;
    bool ok = atou(num, &val);
    CHECK_UNARY(ok);
    CHECK_EQ(val, expected);
}

TEST_CASE("atoi.bin")
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

TEST_CASE("atou.bin")
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

#define _C4PARSE_T(i_or_u, type, numstr, expected)          \
{                                                           \
    type val;                                               \
                                                            \
    bool ok = ato##i_or_u(numstr, &val);                    \
    EXPECT_TRUE(ok) << numstr;                              \
    EXPECT_EQ(val, type(expected)) << numstr;               \
                                                            \
    ok = atox(numstr, &val);                                \
    EXPECT_TRUE(ok) << numstr;                              \
    EXPECT_EQ(val, type(expected)) << "atox:" << numstr;    \
}

#define _C4PARSE_F(i_or_u, type, numstr)        \
{                                               \
    type val;                                   \
    bool ok = ato##i_or_u(numstr, &val);        \
    EXPECT_FALSE(ok) << numstr;                 \
                                                \
    ok = atox(numstr, &val);                    \
    EXPECT_FALSE(ok) << "atox:" << numstr;      \
}

TEST(atoi, empty_i8)
{
    _C4PARSE_F(i, int8_t, "");
    _C4PARSE_F(i, int8_t, "...");
    _C4PARSE_F(i, int8_t, "-");
    _C4PARSE_F(i, int8_t, "2345kjhiuy3245");
    _C4PARSE_F(i, int8_t, "02345kjhiuy3245");
    _C4PARSE_F(i, int8_t, "0x");
    _C4PARSE_F(i, int8_t, "0x12ggg");
    _C4PARSE_F(i, int8_t, "0X");
    _C4PARSE_F(i, int8_t, "0X12GGG");
    _C4PARSE_F(i, int8_t, "0o");
    _C4PARSE_F(i, int8_t, "0o12888");
    _C4PARSE_F(i, int8_t, "0O");
    _C4PARSE_F(i, int8_t, "0O12888");
    _C4PARSE_F(i, int8_t, "0b");
    _C4PARSE_F(i, int8_t, "0b12121");
    _C4PARSE_F(i, int8_t, "0B");
    _C4PARSE_F(i, int8_t, "0B12121");
    _C4PARSE_F(i, int8_t, "----");
    _C4PARSE_F(i, int8_t, "===");
    _C4PARSE_F(i, int8_t, "???");
}

TEST(atou, empty_u8)
{
    _C4PARSE_F(u, uint8_t, "");
    _C4PARSE_F(u, uint8_t, "...");
    _C4PARSE_F(u, uint8_t, "-");
    _C4PARSE_F(u, uint8_t, "2345kjhiuy3245");
    _C4PARSE_F(u, uint8_t, "02345kjhiuy3245");
    _C4PARSE_F(u, uint8_t, "0x");
    _C4PARSE_F(u, uint8_t, "0x12ggg");
    _C4PARSE_F(u, uint8_t, "0X");
    _C4PARSE_F(u, uint8_t, "0X12GGG");
    _C4PARSE_F(u, uint8_t, "0o");
    _C4PARSE_F(u, uint8_t, "0o12888");
    _C4PARSE_F(u, uint8_t, "0O");
    _C4PARSE_F(u, uint8_t, "0O12888");
    _C4PARSE_F(u, uint8_t, "0b");
    _C4PARSE_F(u, uint8_t, "0b12121");
    _C4PARSE_F(u, uint8_t, "0B");
    _C4PARSE_F(u, uint8_t, "0B12121");
    _C4PARSE_F(u, uint8_t, "----");
    _C4PARSE_F(u, uint8_t, "===");
    _C4PARSE_F(u, uint8_t, "???");
}

TEST(atoi, empty_i16)
{
    _C4PARSE_F(i, int16_t, "");
    _C4PARSE_F(i, int16_t, "...");
    _C4PARSE_F(i, int16_t, "-");
    _C4PARSE_F(i, int16_t, "2345kjhiuy3245");
    _C4PARSE_F(i, int16_t, "02345kjhiuy3245");
    _C4PARSE_F(i, int16_t, "0x");
    _C4PARSE_F(i, int16_t, "0x12ggg");
    _C4PARSE_F(i, int16_t, "0X");
    _C4PARSE_F(i, int16_t, "0X12GGG");
    _C4PARSE_F(i, int16_t, "0o");
    _C4PARSE_F(i, int16_t, "0o12888");
    _C4PARSE_F(i, int16_t, "0O");
    _C4PARSE_F(i, int16_t, "0O12888");
    _C4PARSE_F(i, int16_t, "0b");
    _C4PARSE_F(i, int16_t, "0b12121");
    _C4PARSE_F(i, int16_t, "0B");
    _C4PARSE_F(i, int16_t, "0B12121");
    _C4PARSE_F(i, int16_t, "----");
    _C4PARSE_F(i, int16_t, "===");
    _C4PARSE_F(i, int16_t, "???");
}

TEST(atou, empty_u16)
{
    _C4PARSE_F(u, uint16_t, "");
    _C4PARSE_F(u, uint16_t, "...");
    _C4PARSE_F(u, uint16_t, "-");
    _C4PARSE_F(u, uint16_t, "2345kjhiuy3245");
    _C4PARSE_F(u, uint16_t, "02345kjhiuy3245");
    _C4PARSE_F(u, uint16_t, "0x");
    _C4PARSE_F(u, uint16_t, "0x12ggg");
    _C4PARSE_F(u, uint16_t, "0X");
    _C4PARSE_F(u, uint16_t, "0X12GGG");
    _C4PARSE_F(u, uint16_t, "0o");
    _C4PARSE_F(u, uint16_t, "0o12888");
    _C4PARSE_F(u, uint16_t, "0O");
    _C4PARSE_F(u, uint16_t, "0O12888");
    _C4PARSE_F(u, uint16_t, "0b");
    _C4PARSE_F(u, uint16_t, "0b12121");
    _C4PARSE_F(u, uint16_t, "0B");
    _C4PARSE_F(u, uint16_t, "0B12121");
    _C4PARSE_F(u, uint16_t, "----");
    _C4PARSE_F(u, uint16_t, "===");
    _C4PARSE_F(u, uint16_t, "???");
}

TEST(atoi, empty_i32)
{
    _C4PARSE_F(i, int32_t, "");
    _C4PARSE_F(i, int32_t, "...");
    _C4PARSE_F(i, int32_t, "-");
    _C4PARSE_F(i, int32_t, "2345kjhiuy3245");
    _C4PARSE_F(i, int32_t, "02345kjhiuy3245");
    _C4PARSE_F(i, int32_t, "0x");
    _C4PARSE_F(i, int32_t, "0x12ggg");
    _C4PARSE_F(i, int32_t, "0X");
    _C4PARSE_F(i, int32_t, "0X12GGG");
    _C4PARSE_F(i, int32_t, "0o");
    _C4PARSE_F(i, int32_t, "0o12888");
    _C4PARSE_F(i, int32_t, "0O");
    _C4PARSE_F(i, int32_t, "0O12888");
    _C4PARSE_F(i, int32_t, "0b");
    _C4PARSE_F(i, int32_t, "0b12121");
    _C4PARSE_F(i, int32_t, "0B");
    _C4PARSE_F(i, int32_t, "0B12121");
    _C4PARSE_F(i, int32_t, "----");
    _C4PARSE_F(i, int32_t, "===");
    _C4PARSE_F(i, int32_t, "???");
}

TEST(atou, empty_u32)
{
    _C4PARSE_F(u, uint32_t, "");
    _C4PARSE_F(u, uint32_t, "...");
    _C4PARSE_F(u, uint32_t, "-");
    _C4PARSE_F(u, uint32_t, "2345kjhiuy3245");
    _C4PARSE_F(u, uint32_t, "02345kjhiuy3245");
    _C4PARSE_F(u, uint32_t, "0x");
    _C4PARSE_F(u, uint32_t, "0x12ggg");
    _C4PARSE_F(u, uint32_t, "0X");
    _C4PARSE_F(u, uint32_t, "0X12GGG");
    _C4PARSE_F(u, uint32_t, "0o");
    _C4PARSE_F(u, uint32_t, "0o12888");
    _C4PARSE_F(u, uint32_t, "0O");
    _C4PARSE_F(u, uint32_t, "0O12888");
    _C4PARSE_F(u, uint32_t, "0b");
    _C4PARSE_F(u, uint32_t, "0b12121");
    _C4PARSE_F(u, uint32_t, "0B");
    _C4PARSE_F(u, uint32_t, "0B12121");
    _C4PARSE_F(u, uint32_t, "----");
    _C4PARSE_F(u, uint32_t, "===");
    _C4PARSE_F(u, uint32_t, "???");
}

TEST(atoi, empty_i64)
{
    _C4PARSE_F(i, int64_t, "");
    _C4PARSE_F(i, int64_t, "...");
    _C4PARSE_F(i, int64_t, "-");
    _C4PARSE_F(i, int64_t, "2345kjhiuy3245");
    _C4PARSE_F(i, int64_t, "02345kjhiuy3245");
    _C4PARSE_F(i, int64_t, "0x");
    _C4PARSE_F(i, int64_t, "0x12ggg");
    _C4PARSE_F(i, int64_t, "0X");
    _C4PARSE_F(i, int64_t, "0X12GGG");
    _C4PARSE_F(i, int64_t, "0o");
    _C4PARSE_F(i, int64_t, "0o12888");
    _C4PARSE_F(i, int64_t, "0O");
    _C4PARSE_F(i, int64_t, "0O12888");
    _C4PARSE_F(i, int64_t, "0b");
    _C4PARSE_F(i, int64_t, "0b12121");
    _C4PARSE_F(i, int64_t, "0B");
    _C4PARSE_F(i, int64_t, "0B12121");
    _C4PARSE_F(i, int64_t, "----");
    _C4PARSE_F(i, int64_t, "===");
    _C4PARSE_F(i, int64_t, "???");
}

TEST(atou, empty_u64)
{
    _C4PARSE_F(u, uint64_t, "");
    _C4PARSE_F(u, uint64_t, "...");
    _C4PARSE_F(u, uint64_t, "-");
    _C4PARSE_F(u, uint64_t, "2345kjhiuy3245");
    _C4PARSE_F(u, uint64_t, "02345kjhiuy3245");
    _C4PARSE_F(u, uint64_t, "0x");
    _C4PARSE_F(u, uint64_t, "0x12ggg");
    _C4PARSE_F(u, uint64_t, "0X");
    _C4PARSE_F(u, uint64_t, "0X12GGG");
    _C4PARSE_F(u, uint64_t, "0o");
    _C4PARSE_F(u, uint64_t, "0o12888");
    _C4PARSE_F(u, uint64_t, "0O");
    _C4PARSE_F(u, uint64_t, "0O12888");
    _C4PARSE_F(u, uint64_t, "0b");
    _C4PARSE_F(u, uint64_t, "0b12121");
    _C4PARSE_F(u, uint64_t, "0B");
    _C4PARSE_F(u, uint64_t, "0B12121");
    _C4PARSE_F(u, uint64_t, "----");
    _C4PARSE_F(u, uint64_t, "===");
    _C4PARSE_F(u, uint64_t, "???");
}

#undef _C4PARSE_T
#undef _C4PARSE_F


//-----------------------------------------------------------------------------

#define _C4PARSE_T(i_or_u, type, dec, hex, oct, bin, expected)          \
{                                                                       \
    bool ok;                                                            \
    type val;                                                           \
                                                                        \
                                                                        \
    ok = ato##i_or_u(dec, &val);                                        \
    EXPECT_TRUE(ok) << "dec:" << hex;                                   \
    EXPECT_EQ(val, type(expected)) << "dec=" << dec;                    \
                                                                        \
    ok = ato##i_or_u(hex, &val);                                        \
    EXPECT_TRUE(ok) << "dec:" << dec << "  hex:" << hex;                \
    EXPECT_EQ(val, type(expected)) << "dec=" << dec << "  hex=" << hex; \
                                                                        \
    ok = ato##i_or_u(oct, &val);                                        \
    EXPECT_TRUE(ok) << "dec:" << dec << "  oct:" << oct;                \
    EXPECT_EQ(val, type(expected)) << "dec=" << dec << "  oct=" << oct; \
                                                                        \
    ok = ato##i_or_u(bin, &val);                                        \
    EXPECT_TRUE(ok) << "dec:" << dec << "  bin" << bin;                 \
    EXPECT_EQ(val, type(expected)) << "dec=" << dec << "  bin=" << bin; \
                                                                        \
                                                                        \
    ok = atox(dec, &val);                                               \
    EXPECT_TRUE(ok) << "atox: dec:" << hex;                             \
    EXPECT_EQ(val, type(expected)) << "atox: dec=" << dec;              \
                                                                        \
    ok = atox(hex, &val);                                               \
    EXPECT_TRUE(ok) << "atox: dec:" << dec << "  hex:" << hex;          \
    EXPECT_EQ(val, type(expected)) << "atox: dec=" << dec << "  hex=" << hex; \
                                                                        \
    ok = atox(oct, &val);                                               \
    EXPECT_TRUE(ok) << "atox: dec:" << dec << "  oct:" << oct;          \
    EXPECT_EQ(val, type(expected)) << "atox: dec=" << dec << "  oct=" << oct; \
                                                                        \
    ok = atox(bin, &val);                                               \
    EXPECT_TRUE(ok) << "atox: dec:" << dec << "  bin" << bin;           \
    EXPECT_EQ(val, type(expected)) << "atox: dec=" << dec << "  bin=" << bin; \
}

#define _C4PARSE_F(i_or_u, type, numstr)        \
{                                               \
    type val;                                   \
    bool ok = ato##i_or_u(numstr, &val);        \
    EXPECT_FALSE(ok) << numstr;                 \
    ok = atox(numstr, &val);                    \
    EXPECT_FALSE(ok) << "atox:" << numstr;      \
}

TEST(atoi, range_i8)
{
    _C4PARSE_T(i, int8_t, "-129", "-0x81", "-0o201", "-0b10000001",  127);
    _C4PARSE_T(i, int8_t, "-128", "-0x80", "-0o200", "-0b10000000", -128);
    _C4PARSE_T(i, int8_t, "-127", "-0x7f", "-0o177", "-0b01111111", -127);
    _C4PARSE_T(i, int8_t, "-127", "-0x7F", "-0o177", "-0b01111111", -127);
    _C4PARSE_T(i, int8_t,  "0"  ,  "0x0" ,  "0o0"  ,  "0b00000000",    0);
    _C4PARSE_T(i, int8_t, "-0"  , "-0x0" , "-0o0"  , "-0b00000000",    0);
    _C4PARSE_T(i, int8_t,  "127",  "0x7f",  "0o177",  "0b01111111",  127);
    _C4PARSE_T(i, int8_t,  "127",  "0x7F",  "0o177",  "0b01111111",  127);
    _C4PARSE_T(i, int8_t,  "128",  "0x80",  "0o200",  "0b10000000", -128);
    _C4PARSE_T(i, int8_t,  "129",  "0x81",  "0o201",  "0b10000001", -127);
}

TEST(atou, range_u8)
{
    _C4PARSE_F(u, uint8_t, "-1");
    _C4PARSE_F(u, uint8_t, "-0");
    _C4PARSE_T(u, uint8_t,  "0"   , "0x0"  , "0o0"  , "0b000000000",    0);
    _C4PARSE_T(u, uint8_t,  "127",  "0x7f" , "0o177", "0b001111111",  127);
    _C4PARSE_T(u, uint8_t,  "127",  "0x7F" , "0o177", "0b001111111",  127);
    _C4PARSE_T(u, uint8_t,  "128",  "0x80" , "0o200", "0b010000000", -128);
    _C4PARSE_T(u, uint8_t,  "255",  "0xff" , "0o377", "0b011111111",  255);
    _C4PARSE_T(u, uint8_t,  "255",  "0xFF" , "0o377", "0b011111111",  255);
    _C4PARSE_T(u, uint8_t,  "256",  "0x100", "0o400", "0b100000000",    0);
    _C4PARSE_T(u, uint8_t,  "257",  "0x101", "0o401", "0b100000001",    1);
    _C4PARSE_T(u, uint8_t,  "258",  "0x102", "0o402", "0b100000010",    2);
}

TEST(atoi, range_i16)
{
    _C4PARSE_T(i, int16_t, "-32769", "-0x8001", "-0o100001", "-0b1000000000000001",  32767);
    _C4PARSE_T(i, int16_t, "-32768", "-0x8000", "-0o100000", "-0b1000000000000000", -32768);
    _C4PARSE_T(i, int16_t, "-32767", "-0x7fff", "-0o077777", "-0b0111111111111111", -32767);
    _C4PARSE_T(i, int16_t, "-32767", "-0x7FFF", "-0o077777", "-0b0111111111111111", -32767);
    _C4PARSE_T(i, int16_t, "-0"    , "-0x0"   , "-0o0"     , "-0b0"               ,      0);
    _C4PARSE_T(i, int16_t,  "0"    ,  "0x0"   ,  "0o0"     ,  "0b0"               ,      0);
    _C4PARSE_T(i, int16_t,  "32766",  "0x7ffe",  "0o077776",  "0b0111111111111110",  32766);
    _C4PARSE_T(i, int16_t,  "32766",  "0x7FFE",  "0o077776",  "0b0111111111111110",  32766);
    _C4PARSE_T(i, int16_t,  "32767",  "0x7fff",  "0o077777",  "0b0111111111111111",  32767);
    _C4PARSE_T(i, int16_t,  "32767",  "0x7FFF",  "0o077777",  "0b0111111111111111",  32767);
    _C4PARSE_T(i, int16_t,  "32768",  "0x8000",  "0o100000",  "0b1000000000000000", -32768);
    _C4PARSE_T(i, int16_t,  "32769",  "0x8001",  "0o100001",  "0b1000000000000001", -32767);
}

TEST(atou, range_u16)
{
    _C4PARSE_F(u, uint16_t, "-1");
    _C4PARSE_F(u, uint16_t, "-0");
    _C4PARSE_T(u, uint16_t,  "0"    , "0x0"     , "0o0"     , "0b000000000"        ,      0);
    _C4PARSE_T(u, uint16_t,  "65534", "0x0fffe" , "0o177776", "0b01111111111111110",  65534);
    _C4PARSE_T(u, uint16_t,  "65534", "0x0FFFE" , "0o177776", "0b01111111111111110",  65534);
    _C4PARSE_T(u, uint16_t,  "65535", "0x0ffff" , "0o177777", "0b01111111111111111",  65535);
    _C4PARSE_T(u, uint16_t,  "65535", "0x0FFFF" , "0o177777", "0b01111111111111111",  65535);
    _C4PARSE_T(u, uint16_t,  "65536", "0x10000" , "0o200000", "0b10000000000000000",      0);
    _C4PARSE_T(u, uint16_t,  "65537", "0x10001" , "0o200001", "0b10000000000000001",      1);
}

TEST(atoi, range_i32)
{
    int32_t min = std::numeric_limits<int32_t>::min();
    int32_t max = std::numeric_limits<int32_t>::max();
    _C4PARSE_T(i, int32_t, "-2147483650", "-0x80000002", "-0o20000000002", "-0b10000000000000000000000000000010",  max-1);
    _C4PARSE_T(i, int32_t, "-2147483649", "-0x80000001", "-0o20000000001", "-0b10000000000000000000000000000001",  max  );
    _C4PARSE_T(i, int32_t, "-2147483648", "-0x80000000", "-0o20000000000", "-0b10000000000000000000000000000000",  min  );
    _C4PARSE_T(i, int32_t, "-2147483647", "-0x7fffffff", "-0o17777777777", "-0b01111111111111111111111111111111",  min+1);
    _C4PARSE_T(i, int32_t, "-2147483647", "-0x7FFFFFFF", "-0o17777777777", "-0b01111111111111111111111111111111",  min+1);
    _C4PARSE_T(i, int32_t, "-2147483646", "-0x7ffffffe", "-0o17777777776", "-0b01111111111111111111111111111110",  min+2);
    _C4PARSE_T(i, int32_t, "-2147483646", "-0x7FFFFFFE", "-0o17777777776", "-0b01111111111111111111111111111110",  min+2);
    _C4PARSE_T(i, int32_t, "-0"         , "-0x0"       , "-0o0"          , "-0b0"                               ,      0);
    _C4PARSE_T(i, int32_t,  "0"         ,  "0x0"       ,  "0o0"          ,  "0b0"                               ,      0);
    _C4PARSE_T(i, int32_t,  "2147483646",  "0x7ffffffe",  "0o17777777776",  "0b01111111111111111111111111111110",  max-1);
    _C4PARSE_T(i, int32_t,  "2147483646",  "0x7FFFFFFE",  "0o17777777776",  "0b01111111111111111111111111111110",  max-1);
    _C4PARSE_T(i, int32_t,  "2147483647",  "0x7fffffff",  "0o17777777777",  "0b01111111111111111111111111111111",  max  );
    _C4PARSE_T(i, int32_t,  "2147483647",  "0x7FFFFFFF",  "0o17777777777",  "0b01111111111111111111111111111111",  max  );
    _C4PARSE_T(i, int32_t,  "2147483648",  "0x80000000",  "0o20000000000",  "0b10000000000000000000000000000000",  min  );
    _C4PARSE_T(i, int32_t,  "2147483649",  "0x80000001",  "0o20000000001",  "0b10000000000000000000000000000001",  min+1);
}

TEST(atou, range_u32)
{
    _C4PARSE_F(u, uint32_t, "-1");
    _C4PARSE_F(u, uint32_t, "-0");
    _C4PARSE_T(u, uint32_t,  "0"         , "0x0"        , "0o0"          , "0b0"                                ,          0);
    _C4PARSE_T(u, uint32_t,  "4294967294", "0xfffffffe" , "0o37777777776", "0b011111111111111111111111111111110", 4294967294);
    _C4PARSE_T(u, uint32_t,  "4294967294", "0xFFFFFFFE" , "0o37777777776", "0b011111111111111111111111111111110", 4294967294);
    _C4PARSE_T(u, uint32_t,  "4294967295", "0xFFFFFFFF" , "0o37777777777", "0b011111111111111111111111111111111", 4294967295);
    _C4PARSE_T(u, uint32_t,  "4294967296", "0x800000000", "0o40000000000", "0b100000000000000000000000000000000",          0);
    _C4PARSE_T(u, uint32_t,  "4294967297", "0x800000001", "0o40000000001", "0b100000000000000000000000000000001",          1);
    _C4PARSE_T(u, uint32_t,  "4294967298", "0x800000002", "0o40000000002", "0b100000000000000000000000000000010",          2);
}

TEST(atoi, range_i64)
{
    int64_t min = std::numeric_limits<int64_t>::min();
    int64_t max = std::numeric_limits<int64_t>::max();
    _C4PARSE_T(i, int64_t, "-9223372036854775810", "-0x8000000000000002", "-0o1000000000000000000002", "-0b1000000000000000000000000000000000000000000000000000000000000010", max-1);
    _C4PARSE_T(i, int64_t, "-9223372036854775809", "-0x8000000000000001", "-0o1000000000000000000001", "-0b1000000000000000000000000000000000000000000000000000000000000001", max  );
    _C4PARSE_T(i, int64_t, "-9223372036854775808", "-0x8000000000000000", "-0o1000000000000000000000", "-0b1000000000000000000000000000000000000000000000000000000000000000", min  );
    _C4PARSE_T(i, int64_t, "-9223372036854775807", "-0x7fffffffffffffff", "-0o0777777777777777777777", "-0b0111111111111111111111111111111111111111111111111111111111111111", min+1);
    _C4PARSE_T(i, int64_t, "-9223372036854775807", "-0x7FFFFFFFFFFFFFFF", "-0o0777777777777777777777", "-0b0111111111111111111111111111111111111111111111111111111111111111", min+1);
    _C4PARSE_T(i, int64_t, "-9223372036854775806", "-0x7ffffffffffffffe", "-0o0777777777777777777776", "-0b0111111111111111111111111111111111111111111111111111111111111110", min+2);
    _C4PARSE_T(i, int64_t, "-9223372036854775806", "-0x7FFFFFFFFFFFFFFE", "-0o0777777777777777777776", "-0b0111111111111111111111111111111111111111111111111111111111111110", min+2);
    _C4PARSE_T(i, int64_t, "-9223372036854775805", "-0x7ffffffffffffffd", "-0o0777777777777777777775", "-0b0111111111111111111111111111111111111111111111111111111111111101", min+3);
    _C4PARSE_T(i, int64_t, "-9223372036854775805", "-0x7FFFFFFFFFFFFFFD", "-0o0777777777777777777775", "-0b0111111111111111111111111111111111111111111111111111111111111101", min+3);
    _C4PARSE_T(i, int64_t, "-1"                  , "-0x1"               , "-0o1"                     , "-0b1"                                                               ,    -1);
    _C4PARSE_T(i, int64_t, "-0"                  , "-0x0"               , "-0o0"                     , "-0b0"                                                               ,     0);
    _C4PARSE_T(i, int64_t,  "0"                  ,  "0x0"               ,  "0o0"                     ,  "0b0"                                                               ,     0);
    _C4PARSE_T(i, int64_t,  "1"                  ,  "0x1"               ,  "0o1"                     ,  "0b1"                                                               ,     1);
    _C4PARSE_T(i, int64_t,  "9223372036854775805",  "0x7ffffffffffffffd",  "0o0777777777777777777775",  "0b0111111111111111111111111111111111111111111111111111111111111101", max-2);
    _C4PARSE_T(i, int64_t,  "9223372036854775805",  "0x7FFFFFFFFFFFFFFD",  "0o0777777777777777777775",  "0b0111111111111111111111111111111111111111111111111111111111111101", max-2);
    _C4PARSE_T(i, int64_t,  "9223372036854775806",  "0x7ffffffffffffffe",  "0o0777777777777777777776",  "0b0111111111111111111111111111111111111111111111111111111111111110", max-1);
    _C4PARSE_T(i, int64_t,  "9223372036854775806",  "0x7FFFFFFFFFFFFFFE",  "0o0777777777777777777776",  "0b0111111111111111111111111111111111111111111111111111111111111110", max-1);
    _C4PARSE_T(i, int64_t,  "9223372036854775807",  "0x7fffffffffffffff",  "0o0777777777777777777777",  "0b0111111111111111111111111111111111111111111111111111111111111111", max  );
    _C4PARSE_T(i, int64_t,  "9223372036854775807",  "0x7FFFFFFFFFFFFFFF",  "0o0777777777777777777777",  "0b0111111111111111111111111111111111111111111111111111111111111111", max  );
    _C4PARSE_T(i, int64_t,  "9223372036854775808",  "0x8000000000000000",  "0o1000000000000000000000",  "0b1000000000000000000000000000000000000000000000000000000000000000", min  );
    _C4PARSE_T(i, int64_t,  "9223372036854775809",  "0x8000000000000001",  "0o1000000000000000000001",  "0b1000000000000000000000000000000000000000000000000000000000000001", min+1);
    _C4PARSE_T(i, int64_t,  "9223372036854775810",  "0x8000000000000002",  "0o1000000000000000000002",  "0b1000000000000000000000000000000000000000000000000000000000000010", min+2);
}

TEST(atou, range_u64)
{
    uint64_t max = std::numeric_limits<uint64_t>::max();
    // no range checking! B-)
    _C4PARSE_F(u, uint64_t, "-1");
    _C4PARSE_F(u, uint64_t, "-0");
    _C4PARSE_T(u, uint64_t, "0"                   , "0x0"                , "0o0"                     , "0b0"                                                                ,     0);
    _C4PARSE_T(u, uint64_t, "1"                   , "0x1"                , "0o1"                     , "0b1"                                                                ,     1);
    _C4PARSE_T(u, uint64_t, "18446744073709551613", "0x0fffffffffffffffd", "0o1777777777777777777775", "0b01111111111111111111111111111111111111111111111111111111111111101", max-2);
    _C4PARSE_T(u, uint64_t, "18446744073709551613", "0x0FFFFFFFFFFFFFFFD", "0o1777777777777777777775", "0b01111111111111111111111111111111111111111111111111111111111111101", max-2);
    _C4PARSE_T(u, uint64_t, "18446744073709551614", "0x0fffffffffffffffe", "0o1777777777777777777776", "0b01111111111111111111111111111111111111111111111111111111111111110", max-1);
    _C4PARSE_T(u, uint64_t, "18446744073709551614", "0x0FFFFFFFFFFFFFFFE", "0o1777777777777777777776", "0b01111111111111111111111111111111111111111111111111111111111111110", max-1);
    _C4PARSE_T(u, uint64_t, "18446744073709551615", "0x0ffffffffffffffff", "0o1777777777777777777777", "0b01111111111111111111111111111111111111111111111111111111111111111", max  );
    _C4PARSE_T(u, uint64_t, "18446744073709551615", "0x0FFFFFFFFFFFFFFFF", "0o1777777777777777777777", "0b01111111111111111111111111111111111111111111111111111111111111111", max  );
    _C4PARSE_T(u, uint64_t, "18446744073709551616", "0x10000000000000000", "0o2000000000000000000000", "0b10000000000000000000000000000000000000000000000000000000000000000",     0);
    _C4PARSE_T(u, uint64_t, "18446744073709551617", "0x10000000000000001", "0o2000000000000000000001", "0b10000000000000000000000000000000000000000000000000000000000000001",     1);
    _C4PARSE_T(u, uint64_t, "18446744073709551618", "0x10000000000000002", "0o2000000000000000000002", "0b10000000000000000000000000000000000000000000000000000000000000010",     2);
}

#undef _C4PARSE_T
#undef _C4PARSE_F


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void test_ftoa(substr buf, float f, int precision, const char *scient, const char *flt, const char* flex, const char *hexa, const char *hexa_alternative=nullptr)
{
    size_t ret;

    INFO("num=" << f);

    memset(buf.str, 0, buf.len);
    ret = ftoa(buf, f, precision, FTOA_SCIENT);
    CHECK_EQ(buf.left_of(ret), to_csubstr(scient));

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision, FTOA_FLOAT);
    CHECK_EQ(buf.left_of(ret), to_csubstr(flt));

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision+1, FTOA_FLEX);
    CHECK_EQ(buf.left_of(ret), to_csubstr(flex));

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision, FTOA_HEXA);
    if(!hexa_alternative) hexa_alternative = hexa;
    std::string report;
    from_chars(buf.left_of(ret), &report);
    bool ok = buf.left_of(ret) == to_csubstr(hexa) || buf.left_of(ret) == to_csubstr(hexa_alternative);
    CHECK_MESSAGE(ok, "num=" << f << "   ret='" << report
                  << "'  hexa='" << hexa
                  << "'  hexa_alternative='" << hexa_alternative << "'");
}

void test_dtoa(substr buf, double f, int precision, const char *scient, const char *flt, const char* flex, const char *hexa, const char *hexa_alternative=nullptr)
{
    size_t ret;

    INFO("num=" << f);

    memset(buf.str, 0, buf.len);
    ret = dtoa(buf, f, precision, FTOA_SCIENT);
    CHECK_EQ(buf.left_of(ret), to_csubstr(scient));

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision, FTOA_FLOAT);
    CHECK_EQ(buf.left_of(ret), to_csubstr(flt));

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision+1, FTOA_FLEX);
    CHECK_EQ(buf.left_of(ret), to_csubstr(flex));

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision, FTOA_HEXA);
    if(!hexa_alternative) hexa_alternative = hexa;
    std::string report;
    from_chars(buf.left_of(ret), &report);
    bool ok = buf.left_of(ret) == to_csubstr(hexa) || buf.left_of(ret) == to_csubstr(hexa_alternative);
    CHECK_MESSAGE(ok, "num=" << f << "   ret='" << report
                  << "'  hexa='" << hexa
                  << "'  hexa_alternative='" << hexa_alternative << "'");
}


TEST_CASE("ftoa.basic")
{
    char bufc[128];
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    float f = 1.1234123f;
    double d = 1.1234123;

    {
        INFO("precision 0");
        test_ftoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
        test_dtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    }

    {
        INFO("precision 1");
        test_ftoa(buf, f, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");
        test_dtoa(buf, d, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");
    }

    {
        INFO("precision 2");
        test_ftoa(buf, f, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0");
        test_dtoa(buf, d, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0");
    }

    {
        INFO("precision 3");
        test_ftoa(buf, f, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0");
    }

    {
        INFO("precision 4");
        test_ftoa(buf, f, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");
        test_dtoa(buf, d, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");
    }

    f = 1.01234123f;
    d = 1.01234123;

    {
        INFO("precision 0");
        test_ftoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
        test_dtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    }

    {
        INFO("precision 1");
        test_ftoa(buf, f, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
        test_dtoa(buf, d, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
    }

    {
        INFO("precision 2");
        test_ftoa(buf, f, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");
        test_dtoa(buf, d, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");
    }

    {
        INFO("precision 3");
        #if defined(_MSC_VER) || defined(C4_MACOS) || defined(C4_IOS) // there are differences in the hexa formatting
        test_ftoa(buf, f, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0", /*hexa*/"0x1.032p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0", /*hexa*/"0x1.032p+0");
        #else
        test_ftoa(buf, f, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0");
        #endif
    }

    {
        INFO("precision 4");
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
        CHECK_EQ(fret, buf.len);                       \
        CHECK_NEAR(f, float(expected), float(eps));    \
        double d;                                       \
        size_t dret = detail::scan_one_real(buf, &d);   \
        CHECK_EQ(dret, buf.len);                       \
        CHECK_NEAR(d, double(expected), float(eps));   \
    }

using rfloat  = detail::real<float>;
using rdouble = detail::real<double>;

template<class T>
std::string strbin(T val)
{
    using rtype = detail::real<T>;
    rtype r(val);
    auto s = catrs<std::string>(fmt::bin(r.buf));
    size_t sz = (size_t)sizeof(T)*CHAR_BIT + size_t(2); // add 2 because of the leading 0b
    // insert leading zeros
    if(s.size() != sz)
    {
        C4_ASSERT_MSG(sz > s.size(), "sz=%zu s.size()=%zu", sz, s.size());
        s.insert(2, sz - s.size(), '0');
    }
    // insert separators
    s.insert(2, 1, ' ');
    s.insert(s.size() - rtype::sign_bit, 1, ' ');
    s.insert(s.size() - rtype::exp_start, 1, ' ');
    catrs(append, &s, " = ", fmt::hex(r.buf), "  =  ", val);
    return s;
}

void prf(double d)
{
    std::cout << strbin((float)d) << "\n";
    std::cout << strbin(d) << "\n";
}

TEST_CASE("scan_one_real.sign_representation")
{
    CHECK_EQ(rfloat( 1.f).get_sign(), 0);
    CHECK_EQ(rfloat(+1.f).get_sign(), 0);
    CHECK_EQ(rfloat(-1.f).get_sign(), 1);
    CHECK_EQ(rdouble( 1.f).get_sign(), 0);
    CHECK_EQ(rdouble(+1.f).get_sign(), 0);
    CHECK_EQ(rdouble(-1.f).get_sign(), 1);
}

TEST_CASE("scan_one_real.exponent_representation")
{
    std::cout << formatrs<std::string>("exp: [{},{}[\n", rfloat::exp_start, rfloat::exp_end);
    std::cout << formatrs<std::string>("mant: [{},{}[\n", rfloat::mant_start, rfloat::mant_end);
    prf( 0.15625);
    prf( 0.) ;
    prf( 1.) ;
    prf(-1.) ;
    prf( 0.5);
    prf( 2.) ;
    prf(-2.) ;
    prf( 0e1);
    prf( 1e1);
    prf( 2e1);
    prf( 0e2);
    prf( 1e2);
    prf( 2e2);
    prf( 0e3);
    prf( 1e3);
    prf( 2e3);
    prf( 0e4);
    prf( 1e4);
    prf( 2e4);
    prf( 0e5);
    prf( 1e5);
    prf( 2e5);
    prf( 1e-1);
    prf( 2e-1);
    prf( 1e-2);
    prf( 2e-2);
    prf( 1e-3);
    prf( 2e-3);
    prf( 1e-4);
    prf( 2e-4);
    prf(12.375);

    EXPECT_EQ(rfloat::num_exp_bits, 8);
    EXPECT_EQ(rdouble::num_exp_bits, 11);

    EXPECT_EQ(rfloat::exp_bias, 127);
    EXPECT_EQ(rdouble::exp_bias, 1023);

    //EXPECT_EQ(rfloat(0.f).get_exp(), 0);
    //EXPECT_EQ(rfloat(1.f).get_exp(), 127);
    //EXPECT_EQ(rfloat(1.f).get_exp_r(), 1.f);
    //EXPECT_EQ(rfloat(-1.f).get_exp(), 127);
    //EXPECT_EQ(rfloat(-1.f).get_exp_r(), 1.f);
    //EXPECT_EQ(rfloat(1e1f).get_exp(), 130);
    //EXPECT_EQ(rfloat(1e1f).get_exp_r(), 8.f);
    //EXPECT_EQ(rfloat(0.15625f).get_exp(), 124);
}


void test_scan_one_real_(csubstr num, float fval, double dval, float eps)
{
    float f;
    size_t fret = detail::scan_one_real(num, &f);
    {
        INFO("fval=" << fval);
        CHECK_EQ(fret, num.len);
        CHECK_NEAR(f, fval, eps);
    }
    detail::real<float> fref(fval);
    detail::real<float> fcmp(f);
    CHECK_EQ(fref.get_sign(), fcmp.get_sign());
    CHECK_EQ(fref.get_exp(), fcmp.get_exp());

    double d;
    size_t dret = detail::scan_one_real(num, &d);
    {
        INFO("dval=" << dval);
        CHECK_EQ(dret, num.len);
        CHECK_NEAR(d, dval, double(eps));
    }
    detail::real<double> dref(dval);
    detail::real<double> dcmp(d);
    CHECK_EQ(dref.get_sign(), dcmp.get_sign());
    CHECK_EQ(dref.get_exp(), dcmp.get_exp());
}


TEST_CASE("scan_one_real.zeros")
{
    char buf_[128];
    substr buf(buf_);

#define ztest(number, expected, eps)                                    \
{                                                                       \
    {                                                                   \
        INFO(number " - exponent e0");                          \
        csubstr r = cat_sub(buf, number, "e0");                         \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent E0");                          \
        csubstr r = cat_sub(buf, number, "E0");                         \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent e1");                          \
        csubstr r = cat_sub(buf, number, "e1");                         \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent E1");                          \
        csubstr r = cat_sub(buf, number, "E1");                         \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent e-1");                         \
        csubstr r = cat_sub(buf, number, "e-1");                        \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent E-1");                         \
        csubstr r = cat_sub(buf, number, "E-1");                        \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent e+1");                         \
        csubstr r = cat_sub(buf, number, "e+1");                        \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent E+1");                         \
        csubstr r = cat_sub(buf, number, "E+1");                        \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent e10");                         \
        csubstr r = cat_sub(buf, number, "e10");                        \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent E10");                         \
        csubstr r = cat_sub(buf, number, "E10");                        \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent e-10");                        \
        csubstr r = cat_sub(buf, number, "e-10");                       \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent E-10");                        \
        csubstr r = cat_sub(buf, number, "E-10");                       \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent e+10");                        \
        csubstr r = cat_sub(buf, number, "e+10");                       \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
    {                                                                   \
        INFO(number " - exponent E+10");                        \
        csubstr r = cat_sub(buf, number, "E+10");                       \
        test_scan_one_real_(r, float(expected), double(expected), eps); \
    }                                                                   \
}

    ztest( "0", 0.0, 0.0);
    ztest("-0", 0.0, 0.0);
    ztest("+0", 0.0, 0.0);
    ztest(".0", 0.0, 0.0);
    ztest("0.0", 0.0, 0.0);

    ztest("0", 0.0, 0.0);
    ztest("00", 0.0, 0.0);
    ztest("000", 0.0, 0.0);
    ztest("0000", 0.0, 0.0);
    ztest(".0", 0.0, 0.0);
    ztest(".00", 0.0, 0.0);
    ztest(".000", 0.0, 0.0);
    ztest(".0000", 0.0, 0.0);
    ztest("0.", 0.0, 0.0);
    ztest("0.0", 0.0, 0.0);
    ztest("0.00", 0.0, 0.0);
    ztest("0.000", 0.0, 0.0);
    ztest("0.0000", 0.0, 0.0);
    ztest("00.", 0.0, 0.0);
    ztest("00.0", 0.0, 0.0);
    ztest("00.00", 0.0, 0.0);
    ztest("00.000", 0.0, 0.0);
    ztest("00.0000", 0.0, 0.0);
    ztest("000.", 0.0, 0.0);
    ztest("000.0", 0.0, 0.0);
    ztest("000.00", 0.0, 0.0);
    ztest("000.000", 0.0, 0.0);
    ztest("000.0000", 0.0, 0.0);
    ztest("0000.", 0.0, 0.0);
    ztest("0000.0", 0.0, 0.0);
    ztest("0000.00", 0.0, 0.0);
    ztest("0000.000", 0.0, 0.0);
    ztest("0000.0000", 0.0, 0.0);

    ztest("-0", 0.0, 0.0);
    ztest("-00", 0.0, 0.0);
    ztest("-000", 0.0, 0.0);
    ztest("-0000", 0.0, 0.0);
    ztest("-.0", 0.0, 0.0);
    ztest("-.00", 0.0, 0.0);
    ztest("-.000", 0.0, 0.0);
    ztest("-.0000", 0.0, 0.0);
    ztest("-0.", 0.0, 0.0);
    ztest("-0.0", 0.0, 0.0);
    ztest("-0.00", 0.0, 0.0);
    ztest("-0.000", 0.0, 0.0);
    ztest("-0.0000", 0.0, 0.0);
    ztest("-00.", 0.0, 0.0);
    ztest("-00.0", 0.0, 0.0);
    ztest("-00.00", 0.0, 0.0);
    ztest("-00.000", 0.0, 0.0);
    ztest("-00.0000", 0.0, 0.0);
    ztest("-000.", 0.0, 0.0);
    ztest("-000.0", 0.0, 0.0);
    ztest("-000.00", 0.0, 0.0);
    ztest("-000.000", 0.0, 0.0);
    ztest("-000.0000", 0.0, 0.0);
    ztest("-0000.", 0.0, 0.0);
    ztest("-0000.0", 0.0, 0.0);
    ztest("-0000.00", 0.0, 0.0);
    ztest("-0000.000", 0.0, 0.0);
    ztest("-0000.0000", 0.0, 0.0);

    ztest("+0", 0.0, 0.0);
    ztest("+00", 0.0, 0.0);
    ztest("+000", 0.0, 0.0);
    ztest("+0000", 0.0, 0.0);
    ztest("+.0", 0.0, 0.0);
    ztest("+.00", 0.0, 0.0);
    ztest("+.000", 0.0, 0.0);
    ztest("+.0000", 0.0, 0.0);
    ztest("+0.", 0.0, 0.0);
    ztest("+0.0", 0.0, 0.0);
    ztest("+0.00", 0.0, 0.0);
    ztest("+0.000", 0.0, 0.0);
    ztest("+0.0000", 0.0, 0.0);
    ztest("+00.", 0.0, 0.0);
    ztest("+00.0", 0.0, 0.0);
    ztest("+00.00", 0.0, 0.0);
    ztest("+00.000", 0.0, 0.0);
    ztest("+00.0000", 0.0, 0.0);
    ztest("+000.", 0.0, 0.0);
    ztest("+000.0", 0.0, 0.0);
    ztest("+000.00", 0.0, 0.0);
    ztest("+000.000", 0.0, 0.0);
    ztest("+000.0000", 0.0, 0.0);
    ztest("+0000.", 0.0, 0.0);
    ztest("+0000.0", 0.0, 0.0);
    ztest("+0000.00", 0.0, 0.0);
    ztest("+0000.000", 0.0, 0.0);
    ztest("+0000.0000", 0.0, 0.0);
}

#define test_scan_one_real(r, eps) \
    test_scan_one_real_(#r, float(r), double(r), eps)

TEST(scan_one_real, basic)
{
    //prf(8388612.);
    //prf(8388611.);
    //prf(8388610.);
    //prf(8388609.);
    //prf(8388608.);
    //prf(8388607.);
    //prf(-8388607.);
    //prf(8388605.);
    //prf(8388606.);
    //prf(0.);
    //prf(1.);
    //prf(12.375);
    //prf(-12.375);
    //prf(0.375);
    //prf(-0.375);
    //prf(1.375);
    //prf(-1.375);

    test_scan_one_real(8388607, 0.0);
    test_scan_one_real(-8388607, 0.0);
    test_scan_one_real(1, 0.0);
    test_scan_one_real(-1, 0.0);
    test_scan_one_real(2, 0.0);
    test_scan_one_real(-2, 0.0);
    test_scan_one_real(4, 0.0);
    test_scan_one_real(-4, 0.0);
    test_scan_one_real(12.375, 0.0);
    test_scan_one_real(-12.375, 0.0);
    test_scan_one_real(0.375, 0.0);
    test_scan_one_real(-0.375, 0.0);
    test_scan_one_real(0.5, 0.0);
    test_scan_one_real(0.1, 0.0);
    test_scan_one_real(0.01, 0.0);
}

TEST(scan_one_real, exact_integers_float)
{
    #ifdef FIXME_READFLOAT
    char buf_[128];
    float result;
    // Integers in [−16777216, 16777216] can be exactly represented
    for(int32_t i = -16777216; i <= 16777216; ++i)
    {
        // roundtrip the unchanged int number
        csubstr s = cat_sub(buf_, i);
        size_t sz = detail::scan_one_real(s, &result);
        CHECK_EQ(sz, s.len);
        CHECK_MESSAGE(result, float(i), i);
        if(!(i%1000)) std::cout<<i<<"\n";
        //
        // try several combinations:
        // eg: 16777216, 0.16777216e8, 1.6777216e7, 16.777216e6, 167.77216e7
        // ....16777216e0, 167772160e-1, 1677721600e-2, 16777216000e-3
    }
    #endif
}

TEST_CASE("scan_one_real.hexadecimal")
{

}

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


TEST_CASE("to_chars.std_string")
{
    std::string foo("foo");
    char buf_[32];
    substr buf(buf_);
    size_t result = to_chars(buf, foo);
    CHECK_EQ(result, 3);
    CHECK_EQ(buf.first(3), "foo");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE("to_chars.bool")
{
    char buf_[32];
    substr buf(buf_);
    csubstr result = to_chars_sub(buf, true);
    CHECK_EQ(result, "1");
    result = to_chars_sub(buf, false);
    CHECK_EQ(result, "0");
}

TEST_CASE("from_chars.bool")
{
    bool result = false;
    for(const char *s : {"1", "true", "True", "TRUE"})
    {
        INFO("s='" << s << "'");
        bool ok = from_chars(to_csubstr(s), &result);
        CHECK_UNARY(ok);
        CHECK_UNARY(result);
    }
    for(const char *s : {"0", "false", "False", "FALSE"})
    {
        INFO("s='" << s << "'");
        bool ok = from_chars(to_csubstr(s), &result);
        CHECK_UNARY(ok);
        CHECK_UNARY_FALSE(result);
    }
}

TEST_CASE("from_chars_first.bool")
{
    bool result = false;
    for(const char *s : {"1", "10000", "2", "3", "10", "010", "001", "0001", "true", "True", "TRUE"})
    {
        INFO("s='" << s << "'");
        bool ok = from_chars(to_csubstr(s), &result);
        CHECK_UNARY(ok);
        CHECK_UNARY(result);
    }
    for(const char *s : {"0", "00", "000", "0000", "false", "False", "FALSE"})
    {
        INFO("s='" << s << "'");
        bool ok = from_chars(to_csubstr(s), &result);
        CHECK_UNARY(ok);
        CHECK_UNARY_FALSE(result);
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
    REQUIRE_GE(buf.len, expected.len);
    REQUIRE_GE(buf2.len, expected.len);
    csubstr result = to_chars_sub(buf, v);
    CHECK_EQ(result, expected);
    csubstr result2 = to_chars_sub(buf2.sub(result.len), v);
    CHECK_EQ(result2, result);
    std::string str;
    catrs(&str, v);
    CHECK_EQ(result, to_csubstr(str));
}

TEST_CASE("to_chars.trimmed_fit_int")
{
    test_trimmed_fit(12345678, "12345678");
}

TEST_CASE("to_chars.trimmed_fit_float")
{
    test_trimmed_fit(0.375f, "0.375");
    test_trimmed_fit(12.375f, "12.375");
}

TEST_CASE("to_chars.trimmed_fit_double")
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
    INFO("val=" << val);
    csubstr res = to_chars_sub(buf, val);
    CHECK_EQ(res, expected);
    bool ok = from_chars(res, &cp);
    CHECK_UNARY(ok);
    CHECK_EQ(cp, val);
}

template<size_t N>
void to_chars_roundtrip(char (&buf)[N], csubstr val)
{
    char cp_[N];
    substr cp(cp_);
    INFO("val=" << val);
    REQUIRE_LE(val.len, N);
    csubstr res = to_chars_sub(buf, val);
    CHECK_EQ(res.len, val.len);
    CHECK_EQ(res, val);
    bool ok = from_chars(res, &cp);
    CHECK_UNARY(ok);
    CHECK_EQ(cp, val);
}


TEST_CASE("to_chars.roundtrip_bool")
{
    char buf[128];
    to_chars_roundtrip<bool>(buf, false, "0");
    to_chars_roundtrip<bool>(buf,  true, "1");
}


TEST_CASE("to_chars.roundtrip_char")
{
    char buf[128];
    to_chars_roundtrip<char>(buf, 'a', "a");
    to_chars_roundtrip<char>(buf, 'b', "b");
    to_chars_roundtrip<char>(buf, 'c', "c");
    to_chars_roundtrip<char>(buf, 'd', "d");
}

#define C4_TEST_ROUNDTRIP_INT(ty) \
TEST_CASE("to_chars.roundtrip_" #ty)\
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
// some of the following types are not the same as above:
using ulong = unsigned long;
using uint = unsigned int;
C4_TEST_ROUNDTRIP_INT(int)
C4_TEST_ROUNDTRIP_INT(uint)
C4_TEST_ROUNDTRIP_INT(long)
C4_TEST_ROUNDTRIP_INT(ulong)
C4_TEST_ROUNDTRIP_INT(size_t)
C4_TEST_ROUNDTRIP_INT(intptr_t)
C4_TEST_ROUNDTRIP_INT(uintptr_t)

#define C4_TEST_ROUNDTRIP_REAL(ty) \
TEST_CASE("to_chars.roundtrip_" #ty)\
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

TEST_CASE("to_chars.roundtrip_substr")
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

TEST(to_chars, substr_enough_size)
{
    char orig_[] = "0123456789";
    substr orig = orig_;
    char result_[20];
    substr result = result_;
    size_t len = to_chars(result, orig);
    EXPECT_EQ(len, orig.len);
    EXPECT_NE(result.str, orig.str);
    EXPECT_EQ(result.first(10), orig);
}

TEST(to_chars, substr_insufficient_size)
{
    char orig_[] = "0123456789";
    substr orig = orig_;
    char result_[11] = {};
    substr result = result_;
    result.len = 5;
    size_t len = to_chars(result, orig);
    EXPECT_EQ(len, orig.len);
    EXPECT_NE(result.str, orig.str);
    EXPECT_EQ(result.first(5), "01234");
    EXPECT_EQ(substr(result_).last(5), "\0\0\0\0\0");
}

TEST(from_chars, csubstr)
{
    csubstr orig = "0123456789";
    csubstr result;
    EXPECT_NE(result.str, orig.str);
    EXPECT_NE(result.len, orig.len);
    bool ok = from_chars(orig, &result);
    EXPECT_TRUE(ok);
    EXPECT_EQ(result.str, orig.str);
    EXPECT_EQ(result.len, orig.len);
}

TEST(from_chars, substr_enough_size)
{
    char buf_[128] = {};
    substr result = buf_;
    for(char r : result)
    {
        EXPECT_EQ(r, '\0');
    }
    bool ok = from_chars("0123456789", &result);
    EXPECT_TRUE(ok);
    EXPECT_EQ(result.len, 10);
    EXPECT_EQ(result.str, buf_);
    EXPECT_EQ(result, "0123456789");
}

TEST(from_chars, substr_insufficient_size)
{
    char buf_[128] = {};
    substr buf = buf_;
    buf.len = 0;
    bool ok = from_chars("0123456789", &buf);
    EXPECT_FALSE(ok);
    for(size_t i = 0; i < 10; ++i)
    {
        EXPECT_EQ(buf_[i], '\0');
    }
}

} // namespace c4

#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif

#include "c4/libtest/supprwarn_pop.hpp"
