#ifndef C4CORE_SINGLE_HEADER
#include "c4/substr.hpp"
#include "c4/std/std.hpp"
#include "c4/format.hpp"
#endif

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


TEST_CASE_TEMPLATE("to_chars.fmt.bin", T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t)
{
    char bufc[128];
    substr buf(bufc);

    CHECK_EQ(to_chars_sub(buf, fmt::integral(T(21), T(2))), "0b10101");
    CHECK_EQ(to_chars_sub(buf, fmt::integral((T*)21, T(2))), "0b10101");
    CHECK_EQ(to_chars_sub(buf, fmt::integral((T const*)21, T(2))), "0b10101");
    CHECK_EQ(to_chars_sub(buf, fmt::integral(nullptr, T(2))), "0b0");
    CHECK_EQ(to_chars_sub(buf, fmt::bin(T(21))), "0b10101");
    CHECK_EQ(to_chars_sub(buf, fmt::bin((T*)21)), "0b10101");
    CHECK_EQ(to_chars_sub(buf, fmt::bin((T const*)21)), "0b10101");
    CHECK_EQ(to_chars_sub(buf, fmt::bin(nullptr)), "0b0");
}

TEST_CASE_TEMPLATE("to_chars.fmt.zpad.bin", T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t)
{
    char bufc[128];
    substr buf(bufc);
    using namespace fmt;
    CHECK_EQ(to_chars_sub(buf, zpad(integral(T(21), T(2)), 8u)), "0b00010101");
    CHECK_EQ(to_chars_sub(buf, zpad(integral((T*)21, T(2)), 8u)), "0b00010101");
    CHECK_EQ(to_chars_sub(buf, zpad(integral((T const*)21, T(2)), 8u)), "0b00010101");
    CHECK_EQ(to_chars_sub(buf, zpad(bin(T(21)), 8u)), "0b00010101");
    CHECK_EQ(to_chars_sub(buf, zpad(bin((T*)21), 8u)), "0b00010101");
    CHECK_EQ(to_chars_sub(buf, zpad(bin((T const*)21), 8u)), "0b00010101");
}

TEST_CASE_TEMPLATE("to_chars.fmt.oct", T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t)
{
    char bufc[128];
    substr buf(bufc);

    CHECK_EQ(to_chars_sub(buf, fmt::integral(T(65), T(8))), "0o101");
    CHECK_EQ(to_chars_sub(buf, fmt::integral((T*)65, T(8))), "0o101");
    CHECK_EQ(to_chars_sub(buf, fmt::integral((T const*)65, T(8))), "0o101");
    CHECK_EQ(to_chars_sub(buf, fmt::integral(nullptr, T(8))), "0o0");
    CHECK_EQ(to_chars_sub(buf, fmt::oct(T(65))), "0o101");
    CHECK_EQ(to_chars_sub(buf, fmt::oct((T*)65)), "0o101");
    CHECK_EQ(to_chars_sub(buf, fmt::oct((T const*)65)), "0o101");
    CHECK_EQ(to_chars_sub(buf, fmt::oct(nullptr)), "0o0");
}

TEST_CASE_TEMPLATE("to_chars.fmt.zpad.oct", T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t)
{
    char bufc[128];
    substr buf(bufc);
    using namespace fmt;
    CHECK_EQ(to_chars_sub(buf, zpad(integral(T(65), T(8)), 5u)), "0o00101");
    CHECK_EQ(to_chars_sub(buf, zpad(integral((T*)65, T(8)), 5u)), "0o00101");
    CHECK_EQ(to_chars_sub(buf, zpad(integral((T const*)65, T(8)), 5u)), "0o00101");
    CHECK_EQ(to_chars_sub(buf, zpad(oct(T(65)), 5u)), "0o00101");
    CHECK_EQ(to_chars_sub(buf, zpad(oct((T*)65), 5u)), "0o00101");
    CHECK_EQ(to_chars_sub(buf, zpad(oct((T const*)65), 5u)), "0o00101");
}

TEST_CASE_TEMPLATE("to_chars.fmt.hex", T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t)
{
    char bufc[128];
    substr buf(bufc);
    CHECK_EQ(to_chars_sub(buf, fmt::integral(T(0x7f), T(16))), "0x7f");
    CHECK_EQ(to_chars_sub(buf, fmt::integral((T*)0x7f, T(16))), "0x7f");
    CHECK_EQ(to_chars_sub(buf, fmt::integral((T const*)0x7f, T(16))), "0x7f");
    CHECK_EQ(to_chars_sub(buf, fmt::integral(nullptr, T(16))), "0x0");
    CHECK_EQ(to_chars_sub(buf, fmt::hex(T(0x7f))), "0x7f");
    CHECK_EQ(to_chars_sub(buf, fmt::hex((T*)0x7f)), "0x7f");
    CHECK_EQ(to_chars_sub(buf, fmt::hex((T const*)0x7f)), "0x7f");
    CHECK_EQ(to_chars_sub(buf, fmt::hex(nullptr)), "0x0");
}

TEST_CASE_TEMPLATE("to_chars.fmt.zpad.hex", T, uint8_t, int8_t)
{
    char bufc[128];
    substr buf(bufc);
    using namespace fmt;
    buf.fill('?');
    CHECK_EQ(to_chars_sub(buf, zpad(integral(T(0x7f), T(16)), 5u)), "0x0007f");
    CHECK_EQ(to_chars_sub(buf, zpad(integral((T*)0x7f, T(16)), 5u)), "0x0007f");
    CHECK_EQ(to_chars_sub(buf, zpad(integral((T const*)0x7f, T(16)), 5u)), "0x0007f");
    CHECK_EQ(to_chars_sub(buf, zpad(hex(T(0x7f)), 5u)), "0x0007f");
    CHECK_EQ(to_chars_sub(buf, zpad(hex((T*)0x7f), 5u)), "0x0007f");
    CHECK_EQ(to_chars_sub(buf, zpad(hex((T const*)0x7f), 5u)), "0x0007f");
}


TEST_CASE_TEMPLATE("to_chars.fmt.zpad", T, uint8_t, int8_t)
{
    char bufc[128];
    substr buf(bufc);
    using namespace fmt;
    CHECK_EQ(to_chars_sub(buf, zpad(T(10), 0)), "10");
    CHECK_EQ(to_chars_sub(buf, zpad(T(10), 1)), "10");
    CHECK_EQ(to_chars_sub(buf, zpad(T(10), 2)), "10");
    CHECK_EQ(to_chars_sub(buf, zpad(T(10), 3)), "010");
    CHECK_EQ(to_chars_sub(buf, zpad(T(10), 4)), "0010");
    CHECK_EQ(to_chars_sub(buf, zpad((T const*)17, 0)), "0x11");
    CHECK_EQ(to_chars_sub(buf, zpad((T const*)17, 1)), "0x11");
    CHECK_EQ(to_chars_sub(buf, zpad((T const*)17, 2)), "0x11");
    CHECK_EQ(to_chars_sub(buf, zpad((T const*)17, 3)), "0x011");
    CHECK_EQ(to_chars_sub(buf, zpad((T const*)17, 4)), "0x0011");
    CHECK_EQ(to_chars_sub(buf, zpad((T *)17, 0)), "0x11");
    CHECK_EQ(to_chars_sub(buf, zpad((T *)17, 1)), "0x11");
    CHECK_EQ(to_chars_sub(buf, zpad((T *)17, 2)), "0x11");
    CHECK_EQ(to_chars_sub(buf, zpad((T *)17, 3)), "0x011");
    CHECK_EQ(to_chars_sub(buf, zpad((T *)17, 4)), "0x0011");
    CHECK_EQ(to_chars_sub(buf, zpad(nullptr, 0)), "0x0");
    CHECK_EQ(to_chars_sub(buf, zpad(nullptr, 1)), "0x0");
    CHECK_EQ(to_chars_sub(buf, zpad(nullptr, 2)), "0x00");
    CHECK_EQ(to_chars_sub(buf, zpad(nullptr, 3)), "0x000");
    CHECK_EQ(to_chars_sub(buf, zpad(nullptr, 4)), "0x0000");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(10)), 0u)), "0");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(16)), 0u)), "0x0");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(2)), 0u)), "0b0");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(8)), 0u)), "0o0");
    CHECK_EQ(to_chars_sub(buf, zpad(hex(nullptr), 0u)), "0x0");
    CHECK_EQ(to_chars_sub(buf, zpad(bin(nullptr), 0u)), "0b0");
    CHECK_EQ(to_chars_sub(buf, zpad(oct(nullptr), 0u)), "0o0");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(10)), 5u)), "00000");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(16)), 5u)), "0x00000");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(2)), 5u)), "0b00000");
    CHECK_EQ(to_chars_sub(buf, zpad(integral(nullptr, T(8)), 5u)), "0o00000");
    CHECK_EQ(to_chars_sub(buf, zpad(hex(nullptr), 5u)), "0x00000");
    CHECK_EQ(to_chars_sub(buf, zpad(bin(nullptr), 5u)), "0b00000");
    CHECK_EQ(to_chars_sub(buf, zpad(oct(nullptr), 5u)), "0o00000");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void test_to_chars_real(T f, int precision, const char* flt, T fltv, const char *scient, T scientv)
{
    char bufc[64];
    substr buf(bufc);
    substr r;
    T copy;

    INFO("num=" << f);

    r = to_chars_sub(buf, fmt::real(f, precision));
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

    r = to_chars_sub(buf, fmt::real(f, precision, FTOA_SCIENT));
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

TEST_CASE_TEMPLATE("to_chars.fmt.real", T, float, double)
{
    char bufc[128];
    substr buf(bufc);

    T f = static_cast<T>(256.064);
    test_to_chars_real<T>(f, 0, "256", T(256.), "3e+02", T(300.));
    test_to_chars_real<T>(f, 1, "256.1", T(256.1), "2.6e+02", T(260.));
    test_to_chars_real<T>(f, 2, "256.06", T(256.06), "2.56e+02", T(256.));
    test_to_chars_real<T>(f, 3, "256.064", T(256.064), "2.561e+02", T(256.1));
    test_to_chars_real<T>(f, 4, "256.0640", T(256.0640), "2.5606e+02", T(256.06));
    test_to_chars_real<T>(f, 5, "256.06400", T(256.06400), "2.56064e+02", T(256.064));
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE("to_chars.fmt.boolalpha")
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

TEST_CASE("align.left.overflow")
{
    CHECK_EQ(to_chars(substr(), fmt::left(' ', 91u)), 91u);
    CHECK_EQ(to_chars(substr(), fmt::left("0123456789.123456789.123456789.123456789", 91u)), 91u);
    CHECK_EQ(to_chars(substr(), fmt::left("0123456789.123456789.123456789.123456789", 30u)), 40u);
}

TEST_CASE("align.right.overflow")
{
    CHECK_EQ(to_chars(substr(), fmt::right(' ', 91u)), 91u);
    CHECK_EQ(to_chars(substr(), fmt::right("0123456789.123456789.123456789.123456789", 91u)), 91u);
    CHECK_EQ(to_chars(substr(), fmt::right("0123456789.123456789.123456789.123456789", 30u)), 40u);
}

TEST_CASE("align.left")
{
    char buf[128] = {};
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 1)), "1");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 2)), "1 ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 3)), "1  ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 4)), "1   ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 5)), "1    ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 6)), "1     ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 7)), "1      ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 8)), "1       ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 9)), "1        ");

    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 1, '+')), "1");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 2, '+')), "1+");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 3, '+')), "1++");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 4, '+')), "1+++");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 5, '+')), "1++++");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 6, '+')), "1+++++");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 7, '+')), "1++++++");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 8, '+')), "1+++++++");
    CHECK_EQ(to_chars_sub(buf, fmt::left("1", 9, '+')), "1++++++++");

    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 0)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 1)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 2)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 3)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 4)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 5)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 6)), "01234 ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 7)), "01234  ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 8)), "01234   ");
    CHECK_EQ(to_chars_sub(buf, fmt::left("01234", 9)), "01234    ");

    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 0)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 1)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 2)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 3)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 4)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 5)), "1234 ");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 6)), "1234  ");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 7)), "1234   ");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 8)), "1234    ");
    CHECK_EQ(to_chars_sub(buf, fmt::left(1234, 9)), "1234     ");
}


TEST_CASE("align.right")
{
    char buf[128] = {};
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 1)), "1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 2)), " 1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 3)), "  1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 4)), "   1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 5)), "    1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 6)), "     1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 7)), "      1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 8)), "       1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 9)), "        1");

    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 1, '+')), "1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 2, '+')), "+1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 3, '+')), "++1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 4, '+')), "+++1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 5, '+')), "++++1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 6, '+')), "+++++1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 7, '+')), "++++++1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 8, '+')), "+++++++1");
    CHECK_EQ(to_chars_sub(buf, fmt::right("1", 9, '+')), "++++++++1");

    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 0)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 1)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 2)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 3)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 4)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 5)), "01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 6)), " 01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 7)), "  01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 8)), "   01234");
    CHECK_EQ(to_chars_sub(buf, fmt::right("01234", 9)), "    01234");

    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 0)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 1)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 2)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 3)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 4)), "1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 5)), " 1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 6)), "  1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 7)), "   1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 8)), "    1234");
    CHECK_EQ(to_chars_sub(buf, fmt::right(1234, 9)), "     1234");

    CHECK_EQ(to_chars_sub(buf, fmt::real(0.124, 1)), "0.1"); // we assume this in what follows
    CHECK_EQ(to_chars_sub(buf, fmt::real(0.124, 2)), "0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::real(0.124, 3)), "0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 0)), "0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 1)), "0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 2)), "0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 3)), "0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 4)), " 0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 5)), "  0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 6)), "   0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 1), 7)), "    0.1");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 0)), "0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 1)), "0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 2)), "0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 3)), "0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 4)), "0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 5)), " 0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 6)), "  0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 2), 7)), "   0.12");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 0)), "0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 1)), "0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 2)), "0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 3)), "0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 4)), "0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 5)), "0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 6)), " 0.124");
    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(0.124, 3), 7)), "  0.124");

    CHECK_EQ(to_chars_sub(buf, fmt::right(fmt::real(1234.5222, 1), 7)), " 1234.5");
    auto r = [](double val, size_t width) { return fmt::right(fmt::real(val, 1), width); };
    CHECK_EQ(to_chars_sub(buf, r(1234.5, 7)), " 1234.5");
    c4::format(buf, "freq={}Hz\0", r(1234.5, 7));
    CHECK_EQ(to_csubstr((const char*)buf).len, to_csubstr("freq= 1234.5Hz").len);
    CHECK_EQ(to_csubstr((const char*)buf), "freq= 1234.5Hz");
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

    sz = cat({}, 1, 2, 3, 4);
    CHECK_EQ(sz, 4);
    sp.fill('+');
    sz = cat(sp.first(2), 1, 2, 3, 4);
    CHECK_EQ(sz, 4);
    CHECK_EQ(sp.right_of(sz).first(sz - 2), "++");
    sz = cat(buf, 1, 2, 3, 4);
    CHECK_EQ(sz, 4);
    result = sp.first(sz);
    CHECK_EQ(result, "1234");

    sz = cat({}, 1, 2, 3);
    CHECK_EQ(sz, 3);
    sp.fill('+');
    sz = cat(sp.first(2), 1, 2, 3);
    CHECK_EQ(sz, 3);
    CHECK_EQ(sp.right_of(sz).first(sz - 2), "+");
    sz = cat(buf, 1, 2, 3);
    CHECK_EQ(sz, 3);
    result = sp.first(sz);
    CHECK_EQ(result, "123");

    sz = cat({}, 1, 2);
    CHECK_EQ(sz, 2);
    sp.fill('+');
    sz = cat(sp.first(1), 1, 2);
    CHECK_EQ(sz, 2);
    CHECK_EQ(sp.right_of(sz).first(sz - 1), "+");
    sz = cat(buf, 1, 2);
    CHECK_EQ(sz, 2);
    result = sp.first(sz);
    CHECK_EQ(result, "12");

    sz = cat({}, 1);
    CHECK_EQ(sz, 1);
    sz = cat(buf, 1);
    CHECK_EQ(sz, 1);
    result = sp.first(sz);
    CHECK_EQ(result, "1");

    sz = cat({});
    CHECK_EQ(sz, 0);
    sz = cat(buf);
    CHECK_EQ(sz, 0);
    result = sp.first(sz);
    CHECK_EQ(result, "");

    sz = cat({}, 1, ' ', 2, ' ', 3, ' ', 4);
    CHECK_EQ(sz, 7);
    sz = cat(buf, 1, ' ', 2, ' ', 3, ' ', 4);
    CHECK_EQ(sz, 7);
    result = sp.first(sz);
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

    sz = catsep({}, ' ');
    CHECK_EQ(sz, 0);
    sz = catsep(buf, ' ');
    CHECK_EQ(sz, 0);

    sz = catsep({}, ' ', 1);
    CHECK_EQ(sz, 1);
    sz = catsep(buf, ' ', 1);
    CHECK_EQ(sz, 1);
    CHECK_EQ(sp.first(1), "1");

    sz = catsep({}, ' ', 1, 2);
    CHECK_EQ(sz, 3);
    sz = catsep(buf, ' ', 1, 2);
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2");
    sp.fill('+');
    sz = catsep(sp.first(1), ' ', 1, 2);
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1++");
    sp.fill('+');
    sz = catsep(sp.first(2), ' ', 1, 2);
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 +");

    sz = catsep({}, '/', 1, 2);
    CHECK_EQ(sz, 3);
    sz = catsep(buf, '/', 1, 2);
    CHECK_EQ(sz, 3);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1/2");

    sz = catsep({}, ' ', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    sz = catsep(buf, ' ', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 3 4");
    sp.fill('+');
    sz = catsep(sp.first(3), ' ', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2++++");
    sp.fill('+');
    sz = catsep(sp.first(4), ' ', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 +++");
    sp.fill('+');
    sz = catsep(sp.first(5), ' ', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 3++");
    sp.fill('+');
    sz = catsep(sp.first(6), ' ', 1, 2, 3, 4);
    CHECK_EQ(sz, 7);
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 2 3 +");

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

    sz = format({}, "{} and {} and {} and {}");
    CHECK_EQ(sz, strlen("{} and {} and {} and {}"));
    sz = format(buf, "{} and {} and {} and {}");
    CHECK_EQ(sz, strlen("{} and {} and {} and {}"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "{} and {} and {} and {}");

    sz = format({}, "{} and {} and {} and {}", 1);
    CHECK_EQ(sz, strlen("1 and {} and {} and {}"));
    sz = format(buf, "{} and {} and {} and {}", 1);
    CHECK_EQ(sz, strlen("1 and {} and {} and {}"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and {} and {} and {}");

    sz = format({}, "{} and {} and {} and {}", 1, 2);
    CHECK_EQ(sz, strlen("1 and 2 and {} and {}"));
    sz = format(buf, "{} and {} and {} and {}", 1, 2);
    CHECK_EQ(sz, strlen("1 and 2 and {} and {}"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and {} and {}");

    sz = format({}, "{} and {} and {} and {}", 1, 2, 3);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and {}"));
    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and {}"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and {}");

    sz = format({}, "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 and 3 and 4");
    sp.fill('+');
    sz = format(sp.first(1), "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1++++++++++++++++++");
    sp.fill('+');
    sz = format(sp.first(4), "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 an+++++++++++++++");
    sp.fill('+');
    sz = format(sp.first(6), "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and +++++++++++++");
    sp.fill('+');
    sz = format(sp.first(7), "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2++++++++++++");
    sp.fill('+');
    sz = format(sp.first(8), "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 +++++++++++");
    sp.fill('+');
    sz = format(sp.first(10), "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "1 and 2 an+++++++++");
    sp.fill('+');
    sz = format(sp.first(2), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "+++++++++++++++++++++++++++");
    sp.fill('+');
    sz = format(sp.first(3), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111++++++++++++++++++++++++");
    sp.fill('+');
    sz = format(sp.first(4), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 +++++++++++++++++++++++");
    sp.fill('+');
    sz = format(sp.first(8), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and +++++++++++++++++++");
    sp.fill('+');
    sz = format(sp.first(11), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and 222++++++++++++++++");
    sp.fill('+');
    sz = format(sp.first(16), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and 222 and +++++++++++");
    sp.fill('+');
    sz = format(sp.first(20), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and 222 and 333 +++++++");
    sp.fill('+');
    sz = format(sp.first(25), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and 222 and 333 and +++");
    sp.fill('+');
    sz = format(sp.first(26), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and 222 and 333 and +++");
    sp.fill('+');
    sz = format(sp.first(28), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and 222 and 333 and 444");
    sp.fill('+');
    sz = format(sp.first(30), "{} and {} and {} and {}", 111, 222, 333, 444);
    CHECK_EQ(sz, strlen("111 and 222 and 333 and 444"));
    result = sp.left_of(sz);
    CHECK_EQ(result, "111 and 222 and 333 and 444");

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4, 5, 6, 7);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
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

    sz = format(buf, "{} args only at the begin", 1);
    CHECK_EQ(sz, csubstr("1 args only at the begin").len);
    result = sp.left_of(sz);
    CHECK_EQ(result, csubstr("1 args only at the begin"));
}

TEST_CASE("format.empty_buffer")
{
    size_t sz = format({}, "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    char buf_[128];
    substr buf = buf_;
    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4);
    CHECK_EQ(sz, strlen("1 and 2 and 3 and 4"));
    CHECK_EQ(format(buf, "{} and {} and {} and {}", 1, 2, 3, 4),
             format({} , "{} and {} and {} and {}", 1, 2, 3, 4));
    CHECK_EQ(to_chars({}, 101), to_chars(buf, 101)); // eq for all integers
    CHECK_GE(to_chars({}, 0.1f), to_chars(buf, 0.1f)); // ge for all floats, due to a sprintf quirk
    CHECK_EQ(format(buf, "a={} foo {} {} bar {}", 101, 10, 11, 12),
             format({} , "a={} foo {} {} bar {}", 101, 10, 11, 12));
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

    v1 = 0;
    sz = unformat("1 and 2 and 3 and 4" , "3", v1);
    CHECK_EQ(sz, 1);
    CHECK_EQ(v1, 0);

    v1 = 0;
    sz = unformat("1,2,3,,,", "{},{},{}", v1, v2, v3);
    CHECK_EQ(sz, 5);
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);

    v1 = v2 = v3 = 0;
    sz = unformat("1,2,3,,,", "{},{},{},,,", v1, v2, v3);
    CHECK_EQ(sz, 8); // make sure we count the trailing characters in the format
    CHECK_EQ(v1, 1);
    CHECK_EQ(v2, 2);
    CHECK_EQ(v3, 3);
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

    catrs_append(&buf);
    CHECK_EQ(to_csubstr(buf), "");

    catrs_append(&buf, 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1234");
    catrs_append(&buf, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "12345678");
    catrs_append(&buf, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
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

    auto ret = catseprs_append(&buf, ' ');
    CHECK_EQ(to_csubstr(buf), "");
    CHECK_EQ(ret, "");

    ret = catseprs_append(&buf, ' ', 1, 2, 3, 4);
    CHECK_EQ(to_csubstr(buf), "1 2 3 4");
    CHECK_EQ(ret, "1 2 3 4");
    ret = catseprs_append(&buf, ' ', 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "1 2 3 45 6 7 8");
    CHECK_EQ(ret, "5 6 7 8");
    ret = catseprs_append(&buf, ' ', 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    CHECK_EQ(to_csubstr(buf), "1 2 3 45 6 7 89 0 1 2 3 4 5 6 7 8");
    CHECK_EQ(ret, "9 0 1 2 3 4 5 6 7 8");

    ret = catseprs_append(&buf, ' ');
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

    formatrs_append(&buf, "{} goes with food", "wine");
    CHECK_EQ(to_csubstr(buf), "wine goes with food");
    formatrs_append(&buf, ", {} goes with heat", "beer");
    CHECK_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat");
    formatrs_append(&buf, ", {} anytime", "coffee");
    CHECK_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime");

    formatrs_append(&buf, ". And water. {} glass of {}cl in the morning clears you up for the day", 1, 40);
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
    SUBCASE("charbuf")
    {
        char buf_[128] = {};
        substr buf = buf_;
        buf.fill('?'); // this will be executed before each case
        SUBCASE("sz=0")
        {
            CHECK_EQ(cat(buf.first(0), fmt::hex(in)), expected.len);
            CHECK_EQ(buf[0], '?');
            CHECK_EQ(buf.trimr('?'), "");
        }
        SUBCASE("sz=1")
        {
            CHECK_EQ(cat(buf.first(1), fmt::hex(in)), expected.len);
            CHECK_EQ(buf[0], '?');
            CHECK_EQ(buf.trimr('?'), "");
        }
        SUBCASE("sz=2")
        {
            CHECK_EQ(cat(buf.first(2), fmt::hex(in)), expected.len);
            CHECK_EQ(buf[0], '?');
            CHECK_EQ(buf.trimr('?'), "");
        }
        SUBCASE("full")
        {
            REQUIRE_EQ(cat(buf, fmt::hex(in)), expected.len);
            CHECK_EQ(buf.first(expected.len), expected);
            CHECK_EQ(buf.trimr('?'), expected);
        }
    }
    SUBCASE("vector")
    {
        std::vector<char> buf;
        catrs(&buf, fmt::hex(in));
        CHECK_EQ(buf.size(), expected.len);
        CHECK_EQ(to_csubstr(buf), expected);
    }
}

TEST_CASE("fmt.hex")
{
    test_hex(0, "0x0");
    test_hex(nullptr, "0x0");
    test_hex(254, "0xfe");
    test_hex(255, "0xff");
    test_hex(256, "0x100");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void test_raw_roundtrip(const char *valstr, T const& orig)
{
    INFO("valstr=" << valstr);
    alignas(alignof(T)) char buf_[2 * (sizeof(T) + alignof(T))] = {};
    substr buf = buf_;

    fmt::const_raw_wrapper rawwrap = fmt::raw(orig);
    REQUIRE_EQ((void const*)rawwrap.buf, (void const*)&orig);
    REQUIRE_EQ(rawwrap.len, sizeof(orig));

    for(size_t i = 0; i < alignof(T); ++i)
    {
        INFO(" i=" << i);
        // make sure to cover unaligned buffers
        substr sbuf = buf.sub(i);
        size_t szwrite = c4::to_chars(sbuf, fmt::raw(orig));
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
        szwrite = c4::to_chars(sbuf, fmt::raw(orig));
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
