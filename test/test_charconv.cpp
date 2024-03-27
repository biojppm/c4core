#ifdef C4CORE_SINGLE_HEADER
#include "c4/c4core_all.hpp"
#else
#include "c4/std/std.hpp"
#include "c4/charconv.hpp"
#include "c4/format.hpp"
#include "c4/type_name.hpp"
#endif

#include "c4/libtest/supprwarn_push.hpp"

C4_SUPPRESS_WARNING_GCC_PUSH
C4_SUPPRESS_WARNING_GCC("-Wfloat-equal")
C4_SUPPRESS_WARNING_GCC("-Wuseless-cast")
C4_SUPPRESS_WARNING_GCC("-Wconversion")
C4_SUPPRESS_WARNING_GCC("-Wtype-limits")
C4_SUPPRESS_WARNING_GCC("-Wfloat-equal")
#if defined (__GNUC__) && __GNUC__ >= 9
C4_SUPPRESS_WARNING_GCC("-Wno-noexcept-type")
#endif
C4_SUPPRESS_WARNING_CLANG_PUSH
C4_SUPPRESS_WARNING_CLANG("-Wfloat-equal")

#include <c4/test.hpp>

#include "./test_numbers.hpp"

namespace c4 {

// skip the radix prefix: 0x, -0x, 0X, -0X, 0b, -0B, etc
csubstr nopfx(csubstr num)
{
    if(num.begins_with('-'))
        num = num.sub(1);
    if(num.len >= 2 && num[0] == '0')
    {
        switch(num[1])
        {
        case 'x': case 'X':
        case 'o': case 'O':
        case 'b': case 'B':
            num = num.sub(2);
        }
    }
    return num;
}

// filter out the radix prefix from anywhere: 0x, -0x, 0X, -0X, 0b, -0B, etc
csubstr nopfx(substr buf, csubstr num)
{
    REQUIRE_GE(buf.len, num.len);
    if(num.begins_with('-'))
        num = num.sub(1);
    size_t pos = 0;
    for(size_t i = 0; i < num.len; ++i)
    {
        const char c = num.str[i];
        if(c == '0')
        {
            const char n = i+1 < num.len ? num.str[i+1] : '\0';
            switch(n)
            {
            case 'x': case 'X':
            case 'o': case 'O':
            case 'b': case 'B':
                ++i;
                break;
            default:
                buf[pos++] = c;
                break;
            }
        }
        else
        {
            buf[pos++] = c;
        }
    }
    return buf.first(pos);
}

// capitalize the alphabetical characters
// eg 0xdeadbeef --> 0XDEADBEEF
substr capitalize(substr buf, csubstr str)
{
    C4_ASSERT(!buf.overlaps(str));
    if(str.len)
        memcpy(buf.str, str.str, str.len);
    substr ret = buf.first(str.len);
    ret.toupper();
    return ret;
}

// prepend zeroes to the left of the number:
// eg 1234 --> 00001234
// eg -1234 --> -00001234
// eg 0x1234 --> 0x00001234
// eg -0x1234 --> -0x00001234
substr zpad(substr buf, csubstr str, size_t num_zeroes)
{
    C4_ASSERT(!buf.overlaps(str));
    size_t pos = 0;
    if(str.len > 0 && str[0] == '-')
        buf.str[pos++] = '-';
    if(str.len >= pos+2 && str[pos] == '0')
    {
        switch(str[pos+1])
        {
        case 'x': case 'X':
        case 'o': case 'O':
        case 'b': case 'B':
            memcpy(buf.str + pos, str.str + pos, 2);
            pos += 2;
        }
    }
    memset(buf.str + pos, '0', num_zeroes);
    csubstr rem = str.sub(pos);
    if(rem.len)
        memcpy(buf.str + pos + num_zeroes, rem.str, rem.len);
    return buf.first(str.len + num_zeroes);
}

// get the front element of the type's test numbers
template<class T>
number_case<T> const& front(size_t skip=0)
{
    return *(numbers<T>::vals + skip);
}

// get the back element of the type's test numbers
template<class T>
number_case<T> const& back(size_t skip=0)
{
    static_assert(std::is_array<decltype(numbers<T>::vals)>::value, "must be array");
    return *(numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 1 - skip);
}

// given an element, get the n-th element previous to that
template<class T>
number_case<T> const& prev(number_case<T> const& curr, size_t less=1)
{
    C4_ASSERT(less >= 0);
    size_t num = C4_COUNTOF(numbers<T>::vals);
    C4_ASSERT(&curr >= numbers<T>::vals);
    C4_ASSERT(&curr < numbers<T>::vals + num);
    size_t icurr = (size_t)(&curr - numbers<T>::vals);
    size_t prev = (icurr + num - less) % num;
    return *(numbers<T>::vals + prev);
}

// given an element, get the n-th element after that
template<class T>
number_case<T> const& next(number_case<T> const& curr, size_t more=1)
{
    C4_ASSERT(more >= 0);
    size_t num = C4_COUNTOF(numbers<T>::vals);
    C4_ASSERT(&curr >= numbers<T>::vals);
    C4_ASSERT(&curr < numbers<T>::vals + num);
    size_t icurr = (size_t)(&curr - numbers<T>::vals);
    size_t next = (icurr + more) % num;
    return *(numbers<T>::vals + next);
}

// construct a string of a value such that it overflows an original value by a given amount
template<class T>
csubstr overflow_by(substr buf, T val, T how_much, T radix)
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_STATIC_ASSERT(sizeof(T) < sizeof(int64_t));
    using upcast_t = typename std::conditional<std::is_signed<T>::value, int64_t, uint64_t>::type;
    upcast_t uval = (upcast_t) val;
    uval += (upcast_t) how_much;
    size_t len = xtoa(buf, uval, (upcast_t)radix);
    REQUIRE_GE(buf.len, len);
    return buf.first(len);
}

// construct a string of a value such that it underflows an original value by a given amount
template<class T>
csubstr underflow_by(substr buf, T val, T how_much, T radix)
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_STATIC_ASSERT(sizeof(T) < sizeof(int64_t));
    using upcast_t = typename std::conditional<std::is_signed<T>::value, int64_t, uint64_t>::type;
    upcast_t uval = (upcast_t) val;
    uval -= (upcast_t) how_much;
    size_t len = xtoa(buf, uval, (upcast_t)radix);
    REQUIRE_GE(buf.len, len);
    return buf.first(len);
}

TEST_CASE("charconv.to_chars_format")
{
#if C4CORE_HAVE_STD_TO_CHARS
    CHECK(FTOA_FLOAT == static_cast<std::underlying_type<std::chars_format>::type>(std::chars_format::fixed));
    CHECK(FTOA_SCIENT == static_cast<std::underlying_type<std::chars_format>::type>(std::chars_format::scientific));
    CHECK(FTOA_FLEX == static_cast<std::underlying_type<std::chars_format>::type>(std::chars_format::general));
    CHECK(FTOA_HEXA == static_cast<std::underlying_type<std::chars_format>::type>(std::chars_format::hex));
#elif !C4CORE_HAVE_FAST_FLOAT
    CHECK(FTOA_FLOAT == 'f');
    CHECK(FTOA_SCIENT == 'e');
    CHECK(FTOA_FLEX == 'g');
    CHECK(FTOA_HEXA == 'a');
#endif
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("test_util.number_cases", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        CHECK_GT(number.dec.len, 0);
        CHECK_GT(number.hex.len, 2);
        CHECK_GT(number.oct.len, 2);
        CHECK_GT(number.bin.len, 2);
        CHECK_UNARY(number.hex.begins_with("0x"));
        CHECK_UNARY(number.oct.begins_with("0o"));
        CHECK_UNARY(number.bin.begins_with("0b"));
    }
    REQUIRE_GT(C4_COUNTOF(numbers<T>::vals), 2);
    //
    CHECK_EQ(&front<T>(), numbers<T>::vals + 0);
    CHECK_EQ(&front<T>(0), numbers<T>::vals + 0);
    CHECK_EQ(&front<T>(1), numbers<T>::vals + 1);
    //
    CHECK_EQ(&back<T>(), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 1);
    CHECK_EQ(&back<T>(0), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 1);
    CHECK_EQ(&back<T>(1), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 2);
    //
    CHECK_EQ(&next(front<T>()      ), numbers<T>::vals + 1);
    CHECK_EQ(&next(front<T>(), T(1)), numbers<T>::vals + 1);
    CHECK_EQ(&next(front<T>(), T(2)), numbers<T>::vals + 2);
    //
    CHECK_EQ(&next(back<T>()   ), numbers<T>::vals + 0);
    CHECK_EQ(&next(back<T>(), T(1)), numbers<T>::vals + 0);
    CHECK_EQ(&next(back<T>(), T(2)), numbers<T>::vals + 1);
    CHECK_EQ(&next(back<T>(), T(3)), numbers<T>::vals + 2);
    //
    CHECK_EQ(&prev(front<T>()), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 1);
    CHECK_EQ(&prev(front<T>(), T(1)), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 1);
    CHECK_EQ(&prev(front<T>(), T(2)), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 2);
    //
    CHECK_EQ(&prev(back<T>()), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 2);
    CHECK_EQ(&prev(back<T>(), T(1)), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 2);
    CHECK_EQ(&prev(back<T>(), T(2)), numbers<T>::vals + C4_COUNTOF(numbers<T>::vals) - 3);
}

TEST_CASE("test_util.overflow_by")
{
    char buf_[128];
    substr buf = buf_;
    REQUIRE_EQ(overflow_by<int8_t>(buf, INT8_C(127), INT8_C(0), INT8_C(10)), "127");
    REQUIRE_EQ(overflow_by<int8_t>(buf, INT8_C(127), INT8_C(0), INT8_C(16)), "0x7f");
    REQUIRE_EQ(overflow_by<int8_t>(buf, INT8_C(127), INT8_C(1), INT8_C(10)), "128");
    REQUIRE_EQ(overflow_by<int8_t>(buf, INT8_C(127), INT8_C(1), INT8_C(16)), "0x80");
    REQUIRE_EQ(overflow_by<int8_t>(buf, INT8_C(127), INT8_C(2), INT8_C(10)), "129");
    REQUIRE_EQ(overflow_by<int8_t>(buf, INT8_C(127), INT8_C(2), INT8_C(16)), "0x81");
}

TEST_CASE("test_util.underflow_by")
{
    char buf_[128];
    substr buf = buf_;
    REQUIRE_EQ(underflow_by<int8_t>(buf, INT8_C(-128), INT8_C(0), INT8_C(10)), "-128");
    REQUIRE_EQ(underflow_by<int8_t>(buf, INT8_C(-128), INT8_C(0), INT8_C(16)), "-0x80");
    REQUIRE_EQ(underflow_by<int8_t>(buf, INT8_C(-128), INT8_C(1), INT8_C(10)), "-129");
    REQUIRE_EQ(underflow_by<int8_t>(buf, INT8_C(-128), INT8_C(1), INT8_C(16)), "-0x81");
    REQUIRE_EQ(underflow_by<int8_t>(buf, INT8_C(-128), INT8_C(2), INT8_C(10)), "-130");
    REQUIRE_EQ(underflow_by<int8_t>(buf, INT8_C(-128), INT8_C(2), INT8_C(16)), "-0x82");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("digits_dec", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        CHECK_EQ(digits_dec(number.val), nopfx(number.dec).len);
    }
}

TEST_CASE_TEMPLATE("digits_hex", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        CHECK_EQ(digits_hex(number.val), nopfx(number.hex).len);
    }
}

TEST_CASE_TEMPLATE("digits_oct", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        CHECK_EQ(digits_oct(number.val), nopfx(number.oct).len);
    }
}


TEST_CASE_TEMPLATE("digits_bin", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        CHECK_EQ(digits_bin(number.val), nopfx(number.bin).len);
    }
}


//-----------------------------------------------------------------------------
TEST_CASE_TEMPLATE("write_dec_unchecked", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        unsigned digits = digits_dec(number.val);
        REQUIRE_GE(buf.len, digits);
        write_dec_unchecked(buf, number.val, digits);
        CHECK_EQ(buf.first(digits), nopfx(number.dec));
    }
}

TEST_CASE_TEMPLATE("write_hex_unchecked", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        unsigned digits = digits_hex(number.val);
        REQUIRE_GE(buf.len, digits);
        write_hex_unchecked(buf, number.val, digits);
        CHECK_EQ(buf.first(digits), nopfx(number.hex));
    }
}

TEST_CASE_TEMPLATE("write_oct_unchecked", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        unsigned digits = digits_hex(number.val);
        REQUIRE_GE(buf.len, digits);
        write_hex_unchecked(buf, number.val, digits);
        CHECK_EQ(buf.first(digits), nopfx(number.hex));
    }
}

TEST_CASE_TEMPLATE("write_bin_unchecked", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        unsigned digits = digits_bin(number.val);
        REQUIRE_GE(buf.len, digits);
        write_bin_unchecked(buf, number.val, digits);
        CHECK_EQ(buf.first(digits), nopfx(number.bin));
    }
}


//-----------------------------------------------------------------------------
TEST_CASE_TEMPLATE("write_dec", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        REQUIRE_GE(buf.len, number.dec.len);
        size_t retn = write_dec(substr{}, number.val);
        CHECK_EQ(retn, number.dec.len);
        size_t retb = write_dec(buf, number.val);
        CHECK_EQ(retb, retn);
        REQUIRE_EQ(retb, number.dec.len);
        CHECK_EQ(buf.first(retb), number.dec);
    }
}

TEST_CASE_TEMPLATE("write_hex", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        REQUIRE_GE(buf.len, number.hex.sub(2).len);
        size_t retn = write_hex(substr{}, number.val);
        CHECK_EQ(retn, number.hex.sub(2).len);
        size_t retb = write_hex(buf, number.val);
        CHECK_EQ(retb, retn);
        REQUIRE_EQ(retb, number.hex.sub(2).len);
        CHECK_EQ(buf.first(retb), number.hex.sub(2));
    }
}

TEST_CASE_TEMPLATE("write_oct", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        REQUIRE_GE(buf.len, number.oct.sub(2).len);
        size_t retn = write_oct(substr{}, number.val);
        CHECK_EQ(retn, number.oct.sub(2).len);
        size_t retb = write_oct(buf, number.val);
        CHECK_EQ(retb, retn);
        REQUIRE_EQ(retb, number.oct.sub(2).len);
        CHECK_EQ(buf.first(retb), nopfx(number.oct));
    }
}

TEST_CASE_TEMPLATE("write_bin", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        if(number.val < 0)
            continue;
        INFO(number);
        REQUIRE_GE(buf.len, number.bin.sub(2).len);
        size_t retb = write_bin(substr{}, number.val);
        CHECK_EQ(retb, number.bin.sub(2).len);
        size_t retn = write_bin(buf, number.val);
        CHECK_EQ(retb, retn);
        REQUIRE_EQ(retb, number.bin.sub(2).len);
        CHECK_EQ(buf.first(retb), nopfx(number.bin));
    }
}


//-----------------------------------------------------------------------------
TEST_CASE_TEMPLATE("write_dec_digits", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("num digits smaller than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int less_ : {0, 1, 2, 4, 8, (int)number.dec.len})
            {
                size_t less = (size_t) less_;
                if(less > number.dec.len)
                    continue;
                size_t num_digits = number.dec.len - less;
                INFO("less=" << less << "  num_digits=" << num_digits);
                size_t retn = write_dec(substr{}, number.val, num_digits);
                CHECK_EQ(retn, number.dec.len); // the number must always be written
                size_t retb = write_dec(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retb, number.dec.len);
                CHECK_EQ(buf.first(retb), number.dec);
            }
        }
    }
    SUBCASE("num digits larger than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int more_ : {1, 2, 4, 8})
            {
                size_t more = (size_t) more_;
                size_t num_digits = number.dec.len + more;
                INFO("more=" << more << "  num_digits=" << num_digits);
                size_t retn = write_dec(substr{}, number.val, num_digits);
                CHECK_EQ(retn, num_digits);
                size_t retb = write_dec(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retb, num_digits);
                csubstr result = buf.first(retb);
                CHECK_EQ(result.last(number.dec.len), number.dec);
                if(number.val)
                {
                    CHECK_EQ(result.triml('0'), number.dec);
                    CHECK_EQ(result.first_not_of('0'), more);
                }
                else
                {
                    CHECK(result.begins_with('0'));
                    CHECK_EQ(result.first_not_of('0'), csubstr::npos);
                }
            }
        }
    }
}

TEST_CASE_TEMPLATE("write_hex_digits", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("num digits smaller than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int less_ : {0, 1, 2, 4, 8, (int)number.hex.len})
            {
                const csubstr hex = nopfx(number.hex);
                size_t less = (size_t) less_;
                if(less > hex.len)
                    continue;
                size_t num_digits = hex.len - less;
                INFO("less=" << less << "  num_digits=" << num_digits);
                size_t retn = write_hex(substr{}, number.val, num_digits);
                CHECK_EQ(retn, hex.len); // the number must always be written
                size_t retb = write_hex(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retb, hex.len);
                CHECK_EQ(buf.first(retb), hex);
            }
        }
    }
    SUBCASE("num digits larger than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int more_ : {1, 2, 4, 8})
            {
                const csubstr hex = nopfx(number.hex);
                size_t more = (size_t) more_;
                size_t num_digits = hex.len + more;
                INFO("more=" << more << "  num_digits=" << num_digits);
                size_t retn = write_hex(substr{}, number.val, num_digits);
                CHECK_EQ(retn, num_digits);
                size_t retb = write_hex(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retn, num_digits);
                csubstr result = buf.first(retn);
                CHECK_EQ(result.last(hex.len), hex);
                if(number.val)
                {
                    CHECK_EQ(result.triml('0'), hex);
                    CHECK_EQ(result.first_not_of('0'), more);
                }
                else
                {
                    CHECK(result.begins_with('0'));
                    CHECK_EQ(result.first_not_of('0'), csubstr::npos);
                }
            }
        }
    }
}

TEST_CASE_TEMPLATE("write_oct_digits", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("num digits smaller than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int less_ : {0, 1, 2, 4, 8, (int)number.oct.len})
            {
                const csubstr oct = nopfx(number.oct);
                size_t less = (size_t) less_;
                if(less > oct.len)
                    continue;
                size_t num_digits = oct.len - less;
                INFO("less=" << less << "  num_digits=" << num_digits);
                size_t retn = write_oct(substr{}, number.val, num_digits);
                CHECK_EQ(retn, oct.len); // the number must always be written
                size_t retb = write_oct(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retb, oct.len);
                CHECK_EQ(buf.first(retb), oct);
            }
        }
    }
    SUBCASE("num digits larger than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int more_ : {1, 2, 4, 8})
            {
                const csubstr oct = nopfx(number.oct);
                size_t more = (size_t) more_;
                size_t num_digits = oct.len + more;
                INFO("more=" << more << "  num_digits=" << num_digits);
                size_t retn = write_oct(substr{}, number.val, num_digits);
                CHECK_EQ(retn, num_digits);
                size_t retb = write_oct(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retb, num_digits);
                csubstr result = buf.first(retb);
                CHECK_EQ(result.last(oct.len), oct);
                if(number.val)
                {
                    CHECK_EQ(result.triml('0'), oct);
                    CHECK_EQ(result.first_not_of('0'), more);
                }
                else
                {
                    CHECK(result.begins_with('0'));
                    CHECK_EQ(result.first_not_of('0'), csubstr::npos);
                }
            }
        }
    }
}

TEST_CASE_TEMPLATE("write_bin_digits", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("num digits smaller than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int less_ : {0, 1, 2, 4, 8, (int)number.bin.len})
            {
                const csubstr bin = nopfx(number.bin);
                size_t less = (size_t) less_;
                if(less > bin.len)
                    continue;
                size_t num_digits = bin.len - less;
                INFO("less=" << less << "  num_digits=" << num_digits);
                size_t retn = write_bin(substr{}, number.val, num_digits);
                CHECK_EQ(retn, bin.len); // the number must always be written
                size_t retb = write_bin(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retb, bin.len);
                CHECK_EQ(buf.first(retb), bin);
            }
        }
    }
    SUBCASE("num digits larger than length")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < 0)
                continue;
            INFO(number);
            for(int more_ : {1, 2, 4, 8})
            {
                const csubstr bin = nopfx(number.bin);
                size_t more = (size_t) more_;
                size_t num_digits = bin.len + more;
                INFO("more=" << more << "  num_digits=" << num_digits);
                size_t retn = write_bin(substr{}, number.val, num_digits);
                CHECK_EQ(retn, num_digits);
                size_t retb = write_bin(buf, number.val, num_digits);
                CHECK_EQ(retb, retn);
                REQUIRE_EQ(retn, num_digits);
                csubstr result = buf.first(retn);
                CHECK_EQ(result.last(bin.len), bin);
                if(number.val)
                {
                    CHECK_EQ(result.triml('0'), bin);
                    CHECK_EQ(result.first_not_of('0'), more);
                }
                else
                {
                    CHECK(result.begins_with('0'));
                    CHECK_EQ(result.first_not_of('0'), csubstr::npos);
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("xtoa", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        INFO(number);
        {
            buf.fill('?');
            size_t retn = xtoa(substr{}, number.val);
            CHECK_EQ(retn, number.dec.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK_EQ(buf.first(retb), number.dec);
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
    }
}


//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("xtoa_radix.dec", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        INFO(number);
        {
            buf.fill('?');
            size_t retn = xtoa(substr{}, number.val, T(10));
            CHECK_EQ(retn, number.dec.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(10));
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK_EQ(buf.first(retb), number.dec);
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        const size_t adj = size_t(number.val < 0);
        REQUIRE_LT(adj, number.dec.len);
        const size_t dec_digits = number.dec.len - adj;
        for(size_t more_digits = 0; more_digits < 6; ++more_digits)
        {
            buf.fill('?');
            size_t reqdigits = dec_digits + more_digits;
            INFO("dec_digits=" << dec_digits << "  more_digits=" << more_digits << "  req_digits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(10), reqdigits);
            CHECK_EQ(retn, reqdigits + size_t(number.val < 0));
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(10), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK(buf.first(retb).ends_with(number.dec.sub(number.val < 0)));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        for(size_t less_digits = 0; less_digits < dec_digits; ++less_digits)
        {
            buf.fill('?');
            size_t reqdigits = dec_digits - less_digits;
            INFO("dec_digits=" << dec_digits << "  less_digits=" << less_digits << "  req_digits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(10), reqdigits);
            CHECK_EQ(retn, number.dec.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(10), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK(buf.first(retb).ends_with(number.dec.sub(number.val < 0)));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
    }
}

TEST_CASE_TEMPLATE("xtoa_radix.hex", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        INFO(number);
        {
            buf.fill('?');
            size_t retn = xtoa(substr{}, number.val, T(16));
            CHECK_EQ(retn, number.hex.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(16));
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK_EQ(buf.first(retb), number.hex);
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        const size_t adj = size_t(number.val < 0) + size_t(2); // 2 for 0x
        REQUIRE_LT(adj, number.hex.len);
        const size_t hex_digits = number.hex.len - adj;
        for(size_t more_digits = 0; more_digits < 6; ++more_digits)
        {
            buf.fill('?');
            size_t reqdigits = hex_digits + more_digits;
            INFO("more_digits=" << more_digits << "  reqdigits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(16), reqdigits);
            CHECK_EQ(retn, reqdigits + adj);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(16), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            csubstr result = buf.first(retb);
            csubstr ref = number.hex.sub(adj);
            INFO("result=" << result << "  ref=" << ref);
            if(number.val < 0)
                CHECK(buf.first(retb).begins_with('-'));
            CHECK(result.ends_with(ref));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        for(size_t less_digits = 0; less_digits < hex_digits; ++less_digits)
        {
            buf.fill('?');
            size_t reqdigits = hex_digits - less_digits;
            INFO("hex_digits=" << hex_digits << "  less_digits=" << less_digits << "  req_digits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(16), reqdigits);
            CHECK_EQ(retn, number.hex.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(16), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK(buf.first(retb).ends_with(number.hex.sub(number.val < 0)));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
    }
}

TEST_CASE_TEMPLATE("xtoa_radix.oct", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        INFO(number);
        {
            buf.fill('?');
            size_t retn = xtoa(substr{}, number.val, T(8));
            CHECK_EQ(retn, number.oct.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(8));
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK_EQ(buf.first(retb), number.oct);
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        const size_t adj = size_t(number.val < 0) + size_t(2); // 2 for 0o
        REQUIRE_LT(adj, number.oct.len);
        const size_t oct_digits = number.oct.len - adj;
        for(size_t more_digits = 0; more_digits < 6; ++more_digits)
        {
            buf.fill('?');
            size_t reqdigits = oct_digits + more_digits;
            INFO("more_digits=" << more_digits << "  reqdigits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(8), reqdigits);
            CHECK_EQ(retn, reqdigits + adj);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(8), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            csubstr result = buf.first(retb);
            csubstr ref = number.oct.sub(adj);
            INFO("result=" << result << "  ref=" << ref);
            if(number.val < 0)
                CHECK(buf.first(retb).begins_with('-'));
            CHECK(result.ends_with(ref));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        for(size_t less_digits = 0; less_digits < oct_digits; ++less_digits)
        {
            buf.fill('?');
            size_t reqdigits = oct_digits - less_digits;
            INFO("oct_digits=" << oct_digits << "  less_digits=" << less_digits << "  req_digits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(8), reqdigits);
            CHECK_EQ(retn, number.oct.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(8), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK(buf.first(retb).ends_with(number.oct.sub(number.val < 0)));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
    }
}

TEST_CASE_TEMPLATE("xtoa_radix.bin", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    ITER_NUMBERS(T, number)
    {
        INFO(number);
        {
            buf.fill('?');
            size_t retn = xtoa(substr{}, number.val, T(2));
            CHECK_EQ(retn, number.bin.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(2));
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK_EQ(buf.first(retb), number.bin);
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        const size_t adj = size_t(number.val < 0) + size_t(2); // 2 for 0b
        REQUIRE_LT(adj, number.bin.len);
        const size_t bin_digits = number.bin.len - adj;
        for(size_t more_digits = 0; more_digits < 6; ++more_digits)
        {
            buf.fill('?');
            size_t reqdigits = bin_digits + more_digits;
            INFO("more_digits=" << more_digits << "  reqdigits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(2), reqdigits);
            CHECK_EQ(retn, reqdigits + adj);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(2), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            csubstr result = buf.first(retb);
            csubstr ref = number.bin.sub(adj);
            INFO("result=" << result << "  ref=" << ref);
            if(number.val < 0)
                CHECK(buf.first(retb).begins_with('-'));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
        for(size_t less_digits = 0; less_digits < bin_digits; ++less_digits)
        {
            buf.fill('?');
            size_t reqdigits = bin_digits - less_digits;
            INFO("bin_digits=" << bin_digits << "  less_digits=" << less_digits << "  req_digits=" << reqdigits);
            size_t retn = xtoa(substr{}, number.val, T(2), reqdigits);
            CHECK_EQ(retn, number.bin.len);
            CHECK_UNARY(buf.begins_with('?') && buf.first_not_of('?') == csubstr::npos);
            size_t retb = xtoa(buf, number.val, T(2), reqdigits);
            CHECK_EQ(retn, retb);
            REQUIRE_LE(retb, buf.len);
            CHECK(buf.first(retb).ends_with(number.bin.sub(number.val < 0)));
            #ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
            T after_roundtrip = number.val + T(1);
            CHECK(atox(buf.first(retb), &after_roundtrip));
            CHECK_EQ(after_roundtrip, number.val);
            #endif
        }
    }
}


//-----------------------------------------------------------------------------


TEST_CASE_TEMPLATE("overflows.in_range_does_not_overflow", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("dec")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            CHECK_FALSE(overflows<T>(number.dec));
            CHECK_FALSE(overflows<T>(capitalize(buf, number.dec)));
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, number.dec, numz);
                CHECK_FALSE(overflows<T>(buf2));
                buf2.toupper();
                CHECK_FALSE(overflows<T>(buf2));
            }
        }
    }
    SUBCASE("hex")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            CHECK_FALSE(overflows<T>(number.hex));
            CHECK_FALSE(overflows<T>(capitalize(buf, number.hex)));
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, number.hex, numz);
                CHECK_FALSE(overflows<T>(buf2));
                buf2.toupper();
                CHECK_FALSE(overflows<T>(buf2));
            }
        }
    }
    SUBCASE("oct")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            CHECK_FALSE(overflows<T>(number.oct));
            CHECK_FALSE(overflows<T>(capitalize(buf, number.oct)));
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, number.oct, numz);
                CHECK_FALSE(overflows<T>(buf2));
                buf2.toupper();
                CHECK_FALSE(overflows<T>(buf2));
            }
        }
    }
    SUBCASE("bin")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            CHECK_FALSE(overflows<T>(number.bin));
            CHECK_FALSE(overflows<T>(capitalize(buf, number.bin)));
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, number.bin, numz);
                CHECK_FALSE(overflows<T>(buf2));
                buf2.toupper();
                CHECK_FALSE(overflows<T>(buf2));
            }
        }
    }
}


//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("read_dec", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    (void)invalid_cases;
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("numbers")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < T(0))
                continue;
            INFO(number);
            {
                T val = number.val + T(1);
                CHECK(read_dec(number.dec, &val));
                CHECK_EQ(val, number.val);
            }
            // capitalize
            {
                T val = number.val + T(1);
                csubstr cbuf = capitalize(buf, number.dec);
                CHECK(read_dec(cbuf, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr buf2 = zpad(buf, number.dec, numz);
                INFO("zprefix=" << buf2);
                CHECK(read_dec(buf2, &val));
                CHECK_EQ(val, number.val);
                buf2.toupper();
                CHECK(read_dec(buf2, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
    SUBCASE("fail")
    {
        T val = {};
        for(auto ic : invalid_cases)
        {
            if(ic.dec.empty())
                continue;
            INFO(ic.dec);
            CHECK(!read_dec(ic.dec, &val));
        }
    }
#endif
}

TEST_CASE_TEMPLATE("read_hex", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("numbers")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < T(0))
                continue;
            INFO(number);
            // must not accept 0x prefix
            {
                T val = number.val + T(1);
                CHECK(!read_hex(number.hex, &val));
            }
            // must accept without prefix
            csubstr hex = nopfx(number.hex);
            INFO("nopfx(hex)=" << hex);
            {
                T val = number.val + T(1);
                CHECK(read_hex(hex, &val));
                CHECK_EQ(val, number.val);
            }
            // capitalize
            {
                csubstr cbuf = capitalize(buf, hex);
                INFO("capitalized=" << buf);
                REQUIRE_EQ(cbuf.len, hex.len);
                T val = number.val + T(1);
                CHECK(read_hex(cbuf, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr zprefix = zpad(buf, hex, numz);
                INFO("zprefix='" << zprefix << "'");
                CHECK(read_hex(zprefix, &val));
                CHECK_EQ(val, number.val);
                zprefix.toupper();
                CHECK(read_hex(zprefix, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
    SUBCASE("fail")
    {
        char buf2_[128] = {};
        substr buf2 = buf2_;
        size_t icase = 0;
        for(auto const& ic : invalid_cases)
        {
            csubstr cbuf = nopfx(buf, ic.hex);
            csubstr cbuf2 = capitalize(buf2, cbuf);
            INFO("case#=" << icase << " hex='" << ic.hex << "' nopfx(hex)='" << cbuf << "' capitalize(nopfx(hex))='" << cbuf2 << "'");
            REQUIRE_EQ(cbuf2.len, cbuf.len);
            // must not accept 0x prefix
            if(ic.hex.len)
            {
                T val = {};
                CHECK(!read_hex(ic.hex, &val));
            }
            // it is invalid; must not accept even without 0x prefix
            if(cbuf.len)
            {
                T val = {};
                CHECK(!read_hex(cbuf, &val));
            }
            // capitalize
            if(cbuf2.len)
            {
                T val = {};
                CHECK(!read_hex(cbuf2, &val));
            }
            ++icase;
        }
    }
#endif
}

TEST_CASE_TEMPLATE("read_oct", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("numbers")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < T(0))
                continue;
            INFO(number);
            // must not accept 0x prefix
            {
                T val = number.val + T(1);
                CHECK(!read_oct(number.oct, &val));
            }
            // must accept without prefix
            csubstr oct = nopfx(number.oct);
            INFO("nopfx(oct)=" << oct);
            {
                T val = number.val + T(1);
                CHECK(read_oct(oct, &val));
                CHECK_EQ(val, number.val);
            }
            // capitalize
            {
                csubstr cbuf = capitalize(buf, oct);
                INFO("capitalized=" << buf);
                REQUIRE_EQ(cbuf.len, oct.len);
                T val = number.val + T(1);
                CHECK(read_oct(cbuf, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr zprefix = zpad(buf, oct, numz);
                INFO("zprefix=" << zprefix);
                CHECK(read_oct(zprefix, &val));
                CHECK_EQ(val, number.val);
                zprefix.toupper();
                CHECK(read_oct(zprefix, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
    SUBCASE("fail")
    {
        char buf2_[128] = {};
        substr buf2 = buf2_;
        size_t icase = 0;
        for(auto const& ic : invalid_cases)
        {
            csubstr cbuf = nopfx(buf, ic.oct);
            csubstr cbuf2 = capitalize(buf2, cbuf);
            INFO("case#=" << icase << " oct='" << ic.oct << "' nopfx(oct)='" << cbuf << "' capitalize(nopfx(oct))='" << cbuf2 << "'");
            REQUIRE_EQ(cbuf2.len, cbuf.len);
            // must not accept 0x prefix
            if(ic.oct.len)
            {
                T val = {};
                CHECK(!read_oct(ic.oct, &val));
            }
            // it is invalid; must not accept even without 0x prefix
            if(cbuf.len)
            {
                T val = {};
                CHECK(!read_oct(cbuf, &val));
            }
            // capitalize
            if(cbuf2.len)
            {
                T val = {};
                CHECK(!read_oct(cbuf2, &val));
            }
            ++icase;
        }
    }
#endif
}

TEST_CASE_TEMPLATE("read_bin", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("numbers")
    {
        ITER_NUMBERS(T, number)
        {
            if(number.val < T(0))
                continue;
            INFO(number);
            // must not accept 0x prefix
            {
                T val = number.val + T(1);
                CHECK(!read_bin(number.bin, &val));
            }
            // must accept without prefix
            csubstr bin = nopfx(number.bin);
            INFO("nopfx(bin)=" << bin);
            {
                T val = number.val + T(1);
                CHECK(read_bin(bin, &val));
                CHECK_EQ(val, number.val);
            }
            // capitalize
            {
                csubstr cbuf = capitalize(buf, bin);
                INFO("capitalized=" << buf);
                REQUIRE_EQ(cbuf.len, bin.len);
                T val = number.val + T(1);
                CHECK(read_bin(cbuf, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr zprefix = zpad(buf, bin, numz);
                INFO("zprefix=" << zprefix);
                CHECK(read_bin(zprefix, &val));
                CHECK_EQ(val, number.val);
                zprefix.toupper();
                CHECK(read_bin(zprefix, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
    SUBCASE("fail")
    {
        char buf2_[128] = {};
        substr buf2 = buf2_;
        size_t icase = 0;
        for(auto const& ic : invalid_cases)
        {
            csubstr cbuf = nopfx(buf, ic.bin);
            csubstr cbuf2 = capitalize(buf2, cbuf);
            INFO("case#=" << icase << " bin='" << ic.bin << "' nopfx(bin)='" << cbuf << "' capitalize(nopfx(bin))='" << cbuf2 << "'");
            REQUIRE_EQ(cbuf2.len, cbuf.len);
            // must not accept 0x prefix
            if(ic.bin.len)
            {
                T val = {};
                CHECK(!read_bin(ic.bin, &val));
            }
            // it is invalid; must not accept even without 0x prefix
            if(cbuf.len)
            {
                T val = {};
                CHECK(!read_bin(cbuf, &val));
            }
            // capitalize
            if(cbuf2.len)
            {
                T val = {};
                CHECK(!read_bin(cbuf2, &val));
            }
            ++icase;
        }
    }
#endif
}


//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("atox", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("dec")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            {
                T val = number.val + T(1);
                CHECK(atox(number.dec, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr zprefix = zpad(buf, number.dec, numz);
                INFO("zprefix=" << zprefix);
                CHECK(atox(zprefix, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
    SUBCASE("hex")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            {
                T val = number.val + T(1);
                CHECK(atox(number.hex, &val));
                CHECK_EQ(val, number.val);
            }
            // capitalize
            {
                T val = number.val + T(1);
                csubstr cbuf = capitalize(buf, number.hex);
                CHECK(atox(cbuf, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr zprefix = zpad(buf, number.hex, numz);
                INFO("zprefix=" << zprefix);
                CHECK(atox(zprefix, &val));
                CHECK_EQ(val, number.val);
                zprefix.toupper();
                CHECK(atox(zprefix, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
    SUBCASE("oct")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            {
                T val = number.val + T(1);
                CHECK(atox(number.oct, &val));
                CHECK_EQ(val, number.val);
            }
            // capitalize
            {
                T val = number.val + T(1);
                csubstr cbuf = capitalize(buf, number.oct);
                CHECK(atox(cbuf, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr zprefix = zpad(buf, number.oct, numz);
                INFO("zprefix=" << zprefix);
                CHECK(atox(zprefix, &val));
                CHECK_EQ(val, number.val);
                zprefix.toupper();
                CHECK(atox(zprefix, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
    SUBCASE("bin")
    {
        ITER_NUMBERS(T, number)
        {
            INFO(number);
            {
                T val = number.val + T(1);
                CHECK(atox(number.bin, &val));
                CHECK_EQ(val, number.val);
            }
            // capitalize
            {
                T val = number.val + T(1);
                csubstr cbuf = capitalize(buf, number.bin);
                CHECK(atox(cbuf, &val));
                CHECK_EQ(val, number.val);
            }
            // zero-prefix
            for(size_t numz : {1u, 4u, 6u})
            {
                T val = number.val + T(1);
                substr zprefix = zpad(buf, number.oct, numz);
                INFO("zprefix=" << zprefix);
                CHECK(atox(zprefix, &val));
                CHECK_EQ(val, number.val);
                zprefix.toupper();
                CHECK(atox(zprefix, &val));
                CHECK_EQ(val, number.val);
            }
        }
    }
#endif
}

TEST_CASE_TEMPLATE("atox.fail", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128] = {};
    substr buf = buf_;
    SUBCASE("dec")
    {
        size_t icase = 0;
        for(auto const& ic : invalid_cases)
        {
            csubstr cdec = capitalize(buf, ic.dec);
            INFO("case#=" << icase << " dec='" << ic.dec << "' capitalize='" << cdec << "'");
            REQUIRE_EQ(cdec.len, ic.dec.len);
            {
                T val = {};
                CHECK(!atox(ic.dec, &val));
            }
            {
                T val = {};
                CHECK(!atox(cdec, &val));
            }
            ++icase;
        }
    }
    SUBCASE("hex")
    {
        size_t icase = 0;
        for(auto const& ic : invalid_cases)
        {
            csubstr chex = capitalize(buf, ic.hex);
            INFO("case#=" << icase << " hex='" << ic.hex << "' capitalize='" << chex << "'");
            REQUIRE_EQ(chex.len, ic.hex.len);
            {
                T val = {};
                CHECK(!atox(ic.hex, &val));
            }
            {
                T val = {};
                CHECK(!atox(chex, &val));
            }
            ++icase;
        }
    }
    SUBCASE("oct")
    {
        size_t icase = 0;
        for(auto const& ic : invalid_cases)
        {
            csubstr coct = capitalize(buf, ic.oct);
            INFO("case#=" << icase << " oct='" << ic.oct << "' capitalize='" << coct << "'");
            REQUIRE_EQ(coct.len, ic.oct.len);
            {
                T val = {};
                CHECK(!atox(ic.oct, &val));
            }
            {
                T val = {};
                CHECK(!atox(coct, &val));
            }
            ++icase;
        }
    }
    SUBCASE("bin")
    {
        size_t icase = 0;
        for(auto const& ic : invalid_cases)
        {
            csubstr cbin = capitalize(buf, ic.bin);
            INFO("case#=" << icase << " bin='" << ic.bin << "' capitalize='" << cbin << "'");
            REQUIRE_EQ(cbin.len, ic.bin.len);
            {
                T val = {};
                CHECK(!atox(ic.bin, &val));
            }
            {
                T val = {};
                CHECK(!atox(cbin, &val));
            }
            ++icase;
        }
    }
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void test_overflows(std::initializer_list<const char *> args)
{
    for(const char *s : args)
        CHECK_MESSAGE(overflows<T>(to_csubstr(s)), "num=" << s);
}

template<class T>
void test_no_overflows(std::initializer_list<const char *> args)
{
    for(const char *s : args)
        CHECK_MESSAGE(!overflows<T>(to_csubstr(s)), "num=" << s);
}

template<class T>
auto test_no_overflow_zeroes()
    -> typename std::enable_if<std::is_signed<T>::value, void>::type
{
    test_no_overflows<T>({ "-", "-0", "-000", "-0b0", "-0B0", "-0x0", "-0X0", "-0o0", "-0O0" });
    test_no_overflows<T>({ "", "0", "000", "0b0", "0B0", "0x0", "0X0", "0o0", "0O0" });
}

template<class T>
auto test_no_overflow_zeroes()
    -> typename std::enable_if<std::is_unsigned<T>::value, void>::type
{
    test_no_overflows<T>({ "", "0", "000", "0b0", "0B0", "0x0", "0X0", "0o0", "0O0" });
}


// test overflow in sizes smaller than 64 bit by upcasting
TEST_CASE_TEMPLATE("atox.overflow", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128];
    substr buf = buf_;
    auto do_test = [](bool is_overflow, number_case<T> const& num, csubstr exceeded, number_case<T> const& wrapped){
        char buf2_[128] = {};
        substr buf2 = buf2_;
        INFO("exceeded=" << exceeded << "  is_overflow=" << is_overflow);
        INFO("num=" << num);
        INFO("wrapped=" << wrapped);
        CHECK_EQ(is_overflow, overflows<T>(exceeded));
        if(is_overflow)
            CHECK_NE(&num, &wrapped);
        else
            CHECK_EQ(&num, &wrapped);
        {
            T val = num.val + T(1);
            CHECK(atox(exceeded, &val));
            CHECK_EQ(val, wrapped.val);
        }
        // capitalize
        buf2 = capitalize(buf2_, exceeded);
        INFO(buf2);
        CHECK_EQ(is_overflow, overflows<T>(buf2));
        {
            T val = num.val + T(1);
            CHECK(atox(buf2, &val));
            CHECK_EQ(val, wrapped.val);
        }
        // zero-pad on the left
        for(size_t numz : {1u, 4u, 6u})
        {
            buf2 = zpad(buf2_, exceeded, numz);
            CHECK_EQ(is_overflow, overflows<T>(buf2));
            {
                T val = num.val + T(1);
                CHECK(atox(buf2, &val));
                CHECK_EQ(val, wrapped.val);
            }
            buf2.toupper();
            CHECK_EQ(is_overflow, overflows<T>(buf2));
            {
                T val = num.val + T(1);
                CHECK(atox(buf2, &val));
                CHECK_EQ(val, wrapped.val);
            }
        }
    };
    C4_SUPPRESS_WARNING_MSVC_WITH_PUSH(4296)  // '>=': expression is always true
    auto do_test_overflow = [&](T exceed_how_much, T radix){
        REQUIRE(exceed_how_much >= 0);
        number_case<T> const& backelm = back<T>();
        number_case<T> const& wrapelm = next(backelm, (size_t)exceed_how_much);
        csubstr exceeded = overflow_by(buf, backelm.val, exceed_how_much, radix);
        do_test(exceed_how_much > 0, backelm, exceeded, wrapelm);
    };
    auto do_test_underflow = [&](T exceed_how_much, T radix){
        REQUIRE(exceed_how_much >= 0);
        number_case<T> const& frntelm = front<T>();
        number_case<T> const& wrapelm = prev(frntelm, (size_t)exceed_how_much);
        csubstr exceeded = underflow_by(buf, frntelm.val, exceed_how_much, radix);
        do_test(exceed_how_much > 0, frntelm, exceeded, wrapelm);
    };
    C4_SUPPRESS_WARNING_MSVC_POP
    SUBCASE("zeroes")
    {
        test_no_overflow_zeroes<T>();
    }
    SUBCASE("dec")
    {
        do_test_underflow(T(0), T(10));
        do_test_underflow(T(1), T(10));
        do_test_underflow(T(2), T(10));
        do_test_underflow(T(3), T(10));
        do_test_underflow(T(4), T(10));
        do_test_underflow(T(5), T(10));
        do_test_overflow(T(0), T(10));
        do_test_overflow(T(1), T(10));
        do_test_overflow(T(2), T(10));
        do_test_overflow(T(3), T(10));
        do_test_overflow(T(4), T(10));
        do_test_overflow(T(5), T(10));
    }
    SUBCASE("hex")
    {
        do_test_underflow(T(0), T(16));
        do_test_underflow(T(1), T(16));
        do_test_underflow(T(2), T(16));
        do_test_underflow(T(3), T(16));
        do_test_underflow(T(4), T(16));
        do_test_underflow(T(5), T(16));
        do_test_overflow(T(0), T(16));
        do_test_overflow(T(1), T(16));
        do_test_overflow(T(2), T(16));
        do_test_overflow(T(3), T(16));
        do_test_overflow(T(4), T(16));
        do_test_overflow(T(5), T(16));
    }
    SUBCASE("oct")
    {
        do_test_underflow(T(0), T(8));
        do_test_underflow(T(1), T(8));
        do_test_underflow(T(2), T(8));
        do_test_underflow(T(3), T(8));
        do_test_underflow(T(4), T(8));
        do_test_underflow(T(5), T(8));
        do_test_overflow(T(0), T(8));
        do_test_overflow(T(1), T(8));
        do_test_overflow(T(2), T(8));
        do_test_overflow(T(3), T(8));
        do_test_overflow(T(4), T(8));
        do_test_overflow(T(5), T(8));
    }
    SUBCASE("bin")
    {
        do_test_underflow(T(0), T(2));
        do_test_underflow(T(1), T(2));
        do_test_underflow(T(2), T(2));
        do_test_underflow(T(3), T(2));
        do_test_underflow(T(4), T(2));
        do_test_underflow(T(5), T(2));
        do_test_overflow(T(0), T(2));
        do_test_overflow(T(1), T(2));
        do_test_overflow(T(2), T(2));
        do_test_overflow(T(3), T(2));
        do_test_overflow(T(4), T(2));
        do_test_overflow(T(5), T(2));
    }
#endif
}

TEST_CASE_TEMPLATE("atox.overflow64", T, int64_t, uint64_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    char buf_[128] = {};
    substr buf = buf_;
    auto test_atox = [](csubstr s, overflow64case<T> const& c){
        INFO("s=" << s);
        T val = c.wrapped + T(1);
        if(std::is_signed<T>::value || !s.begins_with('-'))
        {
            CHECK(atox(s, &val));
            CHECK_EQ(val, c.wrapped);
        }
        else
        {
            CHECK(!atox(s, &val));
        }
    };
    SUBCASE("zeroes")
    {
        test_no_overflow_zeroes<T>();
    }
    SUBCASE("dec")
    {
        for(auto c : overflow64cases<T>::values)
        {
            INFO(c.dec);
            CHECK_EQ(c.is_overflow, overflows<T>(c.dec));
            test_atox(c.dec, c);
            csubstr capitalized = capitalize(buf, c.dec);
            CHECK_EQ(c.is_overflow, overflows<T>(capitalized));
            test_atox(capitalized, c);
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, c.dec, numz);
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
                buf2.toupper();
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
            }
        }
    }
    SUBCASE("hex")
    {
        for(auto c : overflow64cases<T>::values)
        {
            INFO(c.hex);
            CHECK_EQ(c.is_overflow, overflows<T>(c.hex));
            test_atox(c.hex, c);
            csubstr capitalized = capitalize(buf, c.hex);
            CHECK_EQ(c.is_overflow, overflows<T>(capitalized));
            test_atox(capitalized, c);
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, c.hex, numz);
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
                buf2.toupper();
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
            }
        }
    }
    SUBCASE("oct")
    {
        for(auto c : overflow64cases<T>::values)
        {
            INFO(c.oct);
            CHECK_EQ(c.is_overflow, overflows<T>(c.oct));
            test_atox(c.oct, c);
            csubstr capitalized = capitalize(buf, c.oct);
            CHECK_EQ(c.is_overflow, overflows<T>(capitalized));
            test_atox(capitalized, c);
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, c.oct, numz);
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
                buf2.toupper();
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
            }
        }
    }
    SUBCASE("bin")
    {
        for(auto c : overflow64cases<T>::values)
        {
            INFO(c.bin);
            CHECK_EQ(c.is_overflow, overflows<T>(c.bin));
            test_atox(c.bin, c);
            csubstr capitalized = capitalize(buf, c.bin);
            CHECK_EQ(c.is_overflow, overflows<T>(capitalized));
            test_atox(capitalized, c);
            for(size_t numz : {1u, 4u, 6u})
            {
                substr buf2 = zpad(buf, c.bin, numz);
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
                buf2.toupper();
                CHECK_EQ(c.is_overflow, overflows<T>(buf2));
                test_atox(buf2, c);
            }
        }
    }
#endif
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void test_overflows_hex()
{
    T x = {};
    std::string str;
    if (std::is_unsigned<T>::value)
    {
        /* with leading zeroes */
        str = "0x0" + std::string(sizeof (T) * 2, 'F');
        CHECK_MESSAGE(!overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));

        CHECK_EQ(std::numeric_limits<T>::max(), x);

        str = "0x01" + std::string(sizeof (T) * 2, '0');
        CHECK_MESSAGE(overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::min(), x);
    }
    else
    {
        /* with leading zeroes */
        str = "0x07" + std::string(sizeof (T) * 2 - 1, 'F');
        CHECK_MESSAGE(!overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::max(), x);

        str = "0x0" + std::string(sizeof (T) * 2, 'F');
        CHECK_MESSAGE(overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(-1, x);

        str = "-0x08" + std::string(sizeof (T) * 2 - 1, '0');
        CHECK_MESSAGE(!overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::min(), x);

        str = "-0x08" + std::string(sizeof (T) * 2 - 2, '0') + "1";
        CHECK_MESSAGE(overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::max(), x);
    }
}

template<class T>
void test_overflows_bin()
{
    T x = {};
    std::string str;
    if (std::is_unsigned<T>::value)
    {
        /* with leading zeroes */
        str = "0b0" + std::string(sizeof (T) * 8, '1');
        CHECK_MESSAGE(!overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::max(), x);

        str = "0b01" + std::string(sizeof (T) * 8, '0');
        CHECK_MESSAGE(overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::min(), x);
    }
    else
    {
        /* with leading zeroes */
        str = "0b0" + std::string(sizeof (T) * 8 - 1, '1');
        CHECK_MESSAGE(!overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::max(), x);

        str = "0b0" + std::string(sizeof (T) * 8, '1');
        CHECK_MESSAGE(overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(-1, x);

        str = "-0b01" + std::string(sizeof (T) * 8 - 1, '0');
        CHECK_MESSAGE(!overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::min(), x);

        str = "-0b01" + std::string(sizeof (T) * 8 - 2, '0') + "1";
        CHECK_MESSAGE(overflows<T>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<T>::max(), x);
    }
}

// TODO: test_overflows_oct

template<class T>
typename std::enable_if<std::is_unsigned<T>::value, void>::type
test_overflows()
{
    for(int radix : { 2, 8, 10, 16 })
    {
        char bufc[100] = {0};
        substr s(bufc);
        INFO("radix=" << radix << " num=" << s);

        uint64_t max = (uint64_t) std::numeric_limits<T>::max();
        size_t sz = utoa<uint64_t>(s, max, (uint64_t)radix);
        REQUIRE_LE(sz, s.size());
        CHECK(!overflows<T>(s.first(sz)));
        memset(s.str, 0, s.len);
        sz = utoa<uint64_t>(s, max + 1, (uint64_t)radix);
        REQUIRE_LE(sz, s.size());
        CHECK(overflows<T>(s.first(sz)));
    }

    test_overflows_hex<T>();
    test_overflows_bin<T>();
    // TODO: octal
}

template<class T>
typename std::enable_if<std::is_signed<T>::value, void>::type
test_overflows()
{
    for(int radix : { 2, 8, 10, 16 })
    {
        char bufc[100] = {0};
        substr s(bufc);
        INFO("radix=" << radix << " num=" << s);

        int64_t max = (int64_t) std::numeric_limits<T>::max();
        size_t sz = itoa<int64_t>(s, max, (int64_t)radix);
        REQUIRE_LE(sz, s.size());
        CHECK(!overflows<T>(s.first(sz)));
        memset(s.str, 0, s.len);
        sz = itoa<int64_t>(s, max + 1, (int64_t)radix);
        REQUIRE_LE(sz, s.size());
        CHECK(overflows<T>(s.first(sz)));

        int64_t min = (int64_t) std::numeric_limits<T>::min();
        sz = itoa<int64_t>(s, min, (int64_t)radix);
        REQUIRE_LE(sz, s.size());
        CHECK(!overflows<T>(s.first(sz)));
        memset(s.str, 0, s.len);
        sz = itoa<int64_t>(s, min - 1, (int64_t)radix);
        REQUIRE_LE(sz, s.size());
        CHECK(overflows<T>(s.first(sz)));
    }

    test_overflows_hex<T>();
    test_overflows_bin<T>();
    // TODO: octal
}

TEST_CASE_TEMPLATE("overflows.8bit_32bit", T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t)
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    test_overflows<T>();
#endif
}

TEST_CASE("overflows.u64")
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    CHECK(!overflows<uint64_t>("18446744073709551614"));
    CHECK(!overflows<uint64_t>("18446744073709551615"));
    CHECK(overflows<uint64_t>("18446744073709551616"));

    // more chars but leading zeroes
    CHECK(!overflows<uint64_t>("0018446744073709551615"));

    { /* with leading zeroes */
        std::string str;
        uint64_t x = {};
        str = "0o01" + std::string(21, '7');
        CHECK_MESSAGE(!overflows<uint64_t>(to_csubstr(str)), "num=" << str);
        str = "0o02" + std::string(21, '0');
        CHECK_MESSAGE(overflows<uint64_t>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(0, x);
    }

    test_overflows_hex<uint64_t>();
    test_overflows_bin<uint64_t>();
    // TODO: octal
#endif
}

TEST_CASE("overflows.i64")
{
#ifndef C4_UBSAN // the tests succeed, so UBSAN can take a hike.
    CHECK(!overflows<int64_t>("9223372036854775806"));
    CHECK(!overflows<int64_t>("9223372036854775807"));
    CHECK(overflows<int64_t>("9223372036854775808"));
    CHECK(!overflows<int64_t>("-9223372036854775808"));
    CHECK(overflows<int64_t>("-9223372036854775809"));

    // more chars, but leading zeroes
    CHECK(!overflows<int64_t>("0009223372036854775807"));
    CHECK(!overflows<int64_t>("-0009223372036854775807"));

    { /* with leading zeroes */
        std::string str;
        int64_t x = {};
        str = "0o0" + std::string(21, '7');
        CHECK_MESSAGE(!overflows<int64_t>(to_csubstr(str)), "num=" << str);
        str = "0o01" + std::string(21, '0');
        CHECK_MESSAGE(overflows<int64_t>(to_csubstr(str)), "num=" << str);
        CHECK(atox(to_csubstr(str), &x));
        CHECK_EQ(std::numeric_limits<int64_t>::min(), x);
    }

    test_overflows_hex<int64_t>();
    test_overflows_bin<int64_t>();
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** remove trailing digits after precision */
template<class T>
T remprec10(T val, int precision)
{
    T fprec = T(1);
    for(int i = 0; i < precision; ++i)
        fprec *= T(10);
    T rval = val * fprec;
    return ((T)((int64_t)rval)) / fprec;
}

template<class T>
T test_ator(csubstr s, T ref)
{
    INFO("str=" << s << "  ref=" << ref);
    T rval;
    CHECK(atox(s, &rval));
    INFO("rval=" << rval);
    CHECK_EQ(memcmp(&rval, &ref, sizeof(T)), 0);
    return rval;
}

template<class Real>
void test_rtoa(substr buf, Real f, int precision, const char *scient, const char *flt, const char* flex, const char *hexa, const char *hexa_alternative=nullptr)
{
    size_t ret;
    Real pf = remprec10(f, precision);

    {
        INFO("num=" << f << " precision=" << precision << "  scient=" << scient);
        memset(buf.str, 0, buf.len);
        ret = xtoa(buf, f, precision, FTOA_SCIENT);
        REQUIRE_LE(ret, buf.len);
        CHECK_EQ(buf.first(ret), to_csubstr(scient));
        test_ator(buf.first(ret), pf);
    }

    {
        INFO("num=" << f << " precision=" << precision << "  flt=" << flt);
        memset(buf.str, 0, ret);
        ret = xtoa(buf, f, precision, FTOA_FLOAT);
        REQUIRE_LE(ret, buf.len);
        CHECK_EQ(buf.first(ret), to_csubstr(flt));
        test_ator(buf.first(ret), pf);
    }

    {
        INFO("num=" << f << " precision=" << precision << "  flex=" << flex);
        memset(buf.str, 0, ret);
        ret = xtoa(buf, f, precision+1, FTOA_FLEX);
        REQUIRE_LE(ret, buf.len);
        CHECK_EQ(buf.first(ret), to_csubstr(flex));
        test_ator(buf.first(ret), pf);
    }

    {
        if(!hexa_alternative)
            hexa_alternative = hexa;
        INFO("num=" << f << " precision=" << precision << "  hexa=" << hexa << "  hexa_alternative=" << hexa_alternative);
        memset(buf.str, 0, ret);
        ret = xtoa(buf, f, precision, FTOA_HEXA);
        REQUIRE_LE(ret, buf.len);
        INFO("buf='" << buf.first(ret) << "'");

        CHECK((buf.first(ret) == to_csubstr(hexa) || buf.first(ret) == to_csubstr(hexa_alternative)));
        Real readback = {};
        CHECK(atox(buf.first(ret), &readback));
        INFO("readback=" << readback);
        REQUIRE_EQ(xtoa(buf, readback, precision, FTOA_HEXA), ret);
        Real readback2 = {};
        CHECK(atox(buf.first(ret), &readback2));
        INFO("readback2=" << readback2);
        CHECK_EQ(memcmp(&readback2, &readback, sizeof(Real)), 0);
    }
}


TEST_CASE("ftoa.basic")
{
    char bufc[128];
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    // earlier versions of emscripten's sprintf() do not respect some
    // precision values when printing in hexadecimal format.
    //
    // @see https://github.com/biojppm/c4core/pull/52
    #if defined(__EMSCRIPTEN__) && __EMSCRIPTEN_major__ < 3
    #define _c4emscripten_alt(alt) , alt
    #define _c4emscripten_alt2(alt1, alt2) , alt2
    #else
    #define _c4emscripten_alt(alt)
    #define _c4emscripten_alt2(alt1, alt2) , alt1
    #endif

    float f = 1.1234123f;
    double d = 1.1234123;

    test_rtoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    test_rtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");

    test_rtoa(buf, f, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");
    test_rtoa(buf, d, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");

    test_rtoa(buf, f, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0" _c4emscripten_alt("0x1.1f8p+0"));
    test_rtoa(buf, d, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0" _c4emscripten_alt("0x1.1f8p+0"));

    test_rtoa(buf, f, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0" _c4emscripten_alt("0x1.1f98p+0"));
    test_rtoa(buf, d, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0" _c4emscripten_alt("0x1.1f98p+0"));

    test_rtoa(buf, f, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");
    test_rtoa(buf, d, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");

    f = 1.01234123f;
    d = 1.01234123;

    test_rtoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    test_rtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");

    test_rtoa(buf, f, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
    test_rtoa(buf, d, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");

    test_rtoa(buf, f, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");
    test_rtoa(buf, d, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");

    test_rtoa(buf, f, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0" _c4emscripten_alt2("0x1.032p+0", "0x1.0328p+0"));
    test_rtoa(buf, d, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0" _c4emscripten_alt2("0x1.032p+0", "0x1.0328p+0"));

    test_rtoa(buf, f, 4, /*scient*/"1.0123e+00", /*flt*/"1.0123", /*flex*/"1.0123", /*hexa*/"0x1.0329p+0");
    test_rtoa(buf, d, 4, /*scient*/"1.0123e+00", /*flt*/"1.0123", /*flex*/"1.0123", /*hexa*/"0x1.0329p+0");

    f = 0.f;
    d = 0.;

    test_rtoa(buf, f, 0, /*scient*/"0e+00", /*flt*/"0", /*flex*/"0", /*hexa*/"0x0p+0");
    test_rtoa(buf, d, 0, /*scient*/"0e+00", /*flt*/"0", /*flex*/"0", /*hexa*/"0x0p+0");

    test_rtoa(buf, f, 1, /*scient*/"0.0e+00", /*flt*/"0.0", /*flex*/"0", /*hexa*/"0x0.0p+0");
    test_rtoa(buf, d, 1, /*scient*/"0.0e+00", /*flt*/"0.0", /*flex*/"0", /*hexa*/"0x0.0p+0");

    test_rtoa(buf, f, 2, /*scient*/"0.00e+00", /*flt*/"0.00", /*flex*/"0", /*hexa*/"0x0.00p+0");
    test_rtoa(buf, d, 2, /*scient*/"0.00e+00", /*flt*/"0.00", /*flex*/"0", /*hexa*/"0x0.00p+0");

    test_rtoa(buf, f, 3, /*scient*/"0.000e+00", /*flt*/"0.000", /*flex*/"0", /*hexa*/"0x0.000p+0" _c4emscripten_alt2("0x0.000p+0", "0x0.000p+0"));
    test_rtoa(buf, d, 3, /*scient*/"0.000e+00", /*flt*/"0.000", /*flex*/"0", /*hexa*/"0x0.000p+0" _c4emscripten_alt2("0x0.000p+0", "0x0.000p+0"));

    test_rtoa(buf, f, 4, /*scient*/"0.0000e+00", /*flt*/"0.0000", /*flex*/"0", /*hexa*/"0x0.0000p+0");
    test_rtoa(buf, d, 4, /*scient*/"0.0000e+00", /*flt*/"0.0000", /*flex*/"0", /*hexa*/"0x0.0000p+0");

    f = 1.f;
    d = 1.;

    test_rtoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    test_rtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");

    test_rtoa(buf, f, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
    test_rtoa(buf, d, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");

    test_rtoa(buf, f, 2, /*scient*/"1.00e+00", /*flt*/"1.00", /*flex*/"1", /*hexa*/"0x1.00p+0");
    test_rtoa(buf, d, 2, /*scient*/"1.00e+00", /*flt*/"1.00", /*flex*/"1", /*hexa*/"0x1.00p+0");

    test_rtoa(buf, f, 3, /*scient*/"1.000e+00", /*flt*/"1.000", /*flex*/"1", /*hexa*/"0x1.000p+0" _c4emscripten_alt2("0x1.000p+0", "0x1.000p+0"));
    test_rtoa(buf, d, 3, /*scient*/"1.000e+00", /*flt*/"1.000", /*flex*/"1", /*hexa*/"0x1.000p+0" _c4emscripten_alt2("0x1.000p+0", "0x1.000p+0"));

    test_rtoa(buf, f, 4, /*scient*/"1.0000e+00", /*flt*/"1.0000", /*flex*/"1", /*hexa*/"0x1.0000p+0");
    test_rtoa(buf, d, 4, /*scient*/"1.0000e+00", /*flt*/"1.0000", /*flex*/"1", /*hexa*/"0x1.0000p+0");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST_CASE_TEMPLATE("atof.integral", T, float, double)
{
    auto t_ = [](csubstr str, int val){
        T rval = (T)10 * (T)val;
        INFO("str=" << str);
        bool ret = atox(str, &rval);
        CHECK_EQ(ret, true);
        CHECK_EQ(static_cast<int>(rval), val);
        CHECK_EQ(rval, (T)val);
    };

    csubstr s = "12345678";
    t_(s, 12345678);
    t_(s.first(8), 12345678);
    t_(s.first(7), 1234567);
    t_(s.first(6), 123456);
    t_(s.first(5), 12345);
    t_(s.first(4), 1234);
    t_(s.first(3), 123);
    t_(s.first(2), 12);
    t_(s.first(1), 1);
}

TEST_CASE_TEMPLATE("atof.hexa", T, float, double)
{
    auto t_ = [](csubstr str, bool isok){
        T rval = {};
        INFO("str=" << str);
        CHECK_EQ(atox(str, &rval), isok);
    };
    #if C4CORE_NO_FAST_FLOAT
    #define _scanf_accepts(expected) !expected
    #else
    #define _scanf_accepts(expected) expected
    #endif
    t_("0x1.p+0", true);
    t_("0x1.p", _scanf_accepts(false));
    t_("0x1.p+", _scanf_accepts(false));
    t_("0x12p+0", true);
    t_("0x12p", _scanf_accepts(false));
    t_("0xabcdef.abcdefp+0", true);
    t_("0xABCDEF.ABCDEFp+0", true);
    t_("0x1g", _scanf_accepts(false));
    t_("0x1.2", true);
    t_("0x1.", true);
    t_("0x1.0329p+0", true);
    t_("0x1.0329P+0", true);
    t_("0x1.aAaAaAp+0", true);
    t_("0x1.agA+0", _scanf_accepts(false));
}

TEST_CASE_TEMPLATE("atof.infnan", T, float, double)
{
    T pinf = std::numeric_limits<T>::infinity();
    T ninf = -std::numeric_limits<T>::infinity();
    T nan = std::numeric_limits<T>::quiet_NaN();
    T rval = {};
    test_ator("infinity", pinf);
    test_ator("inf", pinf);
    test_ator("-infinity", ninf);
    test_ator("-inf", ninf);
    test_ator("nan", nan);
}

TEST_CASE_TEMPLATE("atof.fail_parse", T, float, double)
{
    auto t_ = [](csubstr str){
        T rval;
        INFO("str=" << str << "  rval=" << rval);
        CHECK_EQ(atox(str, &rval), false);
    };
    t_(".inf");
    t_("-.inf");
    t_(".nan");
    t_("-.nan");
    t_("not a float!");
    #ifndef C4CORE_NO_FAST_FLOAT
    t_("0xfonix!");
    #endif
    //t_("123.45not a float!");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST_CASE_TEMPLATE("to_chars.empty_buffer", T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, void*)
{
    char buf_[100];
    substr buf = buf_;
    CHECK_EQ(to_chars({}, T(101)), to_chars(buf_, T(101)));
    CHECK_EQ(to_chars({}, T(101)), to_chars(buf , T(101)));
}
// due to an implementation quirk with sprintf, for floats the empty is GE
TEST_CASE_TEMPLATE("to_chars.empty_buffer", T, float, double)
{
    char buf_[100];
    substr buf = buf_;
    CHECK_GE(to_chars({}, T(101)), to_chars(buf_, T(101)));
    CHECK_GE(to_chars({}, T(101)), to_chars(buf , T(101)));
}


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
    CHECK_EQ(expected, to_csubstr(str));
    CHECK_EQ(result, to_csubstr(str));
}

TEST_CASE("to_chars.trimmed_fit_int")
{
    test_trimmed_fit(12345678, "12345678");
}

TEST_CASE("to_chars.trimmed_fit_float")
{
    test_trimmed_fit(0.374f, "0.374");
    test_trimmed_fit(12.374f, "12.374");
}

TEST_CASE("to_chars.trimmed_fit_double")
{
    test_trimmed_fit(0.374, "0.374");
    test_trimmed_fit(12.374, "12.374");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T>
void to_chars_roundtrip(substr buf, T const& val, csubstr expected)
{
    T cp = {};
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
    char cp_[N] = {};
    substr cp = cp_;
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

TEST_CASE("to_chars.substr_enough_size")
{
    char orig_[] = "0123456789";
    substr orig = orig_;
    char result_[20];
    substr result = result_;
    size_t len = to_chars(result, orig);
    CHECK_EQ(len, orig.len);
    CHECK_NE(result.str, orig.str);
    CHECK_EQ(result.first(10), orig);
}

TEST_CASE("to_chars.substr_insufficient_size")
{
    char orig_[] = "0123456789";
    substr orig = orig_;
    char result_[11] = {};
    substr result = result_;
    result.len = 5;
    size_t len = to_chars(result, orig);
    CHECK_EQ(len, orig.len);
    CHECK_NE(result.str, orig.str);
    CHECK_EQ(result.first(5), "01234");
    CHECK_EQ(substr(result_).last(5), "\0\0\0\0\0");
}

TEST_CASE("from_chars.csubstr")
{
    csubstr orig = "0123456789";
    csubstr result;
    CHECK_NE(result.str, orig.str);
    CHECK_NE(result.len, orig.len);
    bool ok = from_chars(orig, &result);
    CHECK(ok);
    CHECK_EQ(result.str, orig.str);
    CHECK_EQ(result.len, orig.len);
}

TEST_CASE("from_chars.substr_enough_size")
{
    char buf_[128] = {};
    substr result = buf_;
    for(char r : result)
    {
        CHECK_EQ(r, '\0');
    }
    bool ok = from_chars("0123456789", &result);
    CHECK(ok);
    CHECK_EQ(result.len, 10);
    CHECK_EQ(result.str, buf_);
    CHECK_EQ(result, "0123456789");
}

TEST_CASE("from_chars.substr_insufficient_size")
{
    char buf_[128] = {};
    substr buf = buf_;
    buf.len = 0;
    bool ok = from_chars("0123456789", &buf);
    CHECK_FALSE(ok);
    for(size_t i = 0; i < 10; ++i)
    {
        CHECK_EQ(buf_[i], '\0');
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct ptr_expected { void *ptr; c4::csubstr str; };
const ptr_expected ptr_cases[] = {
    {(void*)0x0, c4::csubstr("0x0")},
    {(void*)0x1234, c4::csubstr("0x1234")},
    {(void*)-0x1234, c4::csubstr("-0x1234")},
};

template<class T>
void test_xtoa_ptr(const char *type_name)
{
    INFO("type=" << type_name);
    char buf_[128] = {};
    c4::substr buf(buf_);
    for(auto &pe : ptr_cases)
    {
        INFO("val=" << pe.str);
        size_t ret = xtoa(buf, (T const*)pe.ptr);
        CHECK_EQ(ret, pe.str.len);
        CHECK_EQ(buf.first(ret), pe.str);
    }
}

template<class T>
void test_to_chars_ptr(const char *type_name)
{
    INFO("type=" << type_name);
    char buf_[128] = {};
    c4::substr buf(buf_);
    for(auto &pe : ptr_cases)
    {
        INFO("val=" << pe.str);
        size_t ret = to_chars(buf, (T const*)pe.ptr);
        CHECK_EQ(ret, pe.str.len);
        CHECK_EQ(buf.first(ret), pe.str);
    }
}

template<class T>
void test_atox_ptr(const char *type_name)
{
    INFO("type=" << type_name);
    for(auto &pe : ptr_cases)
    {
        T *ptr = nullptr;
        INFO("val=" << pe.str);
        bool ret = atox(pe.str, &ptr);
        CHECK(ret);
        CHECK_EQ((void*)ptr, pe.ptr);
    }
}

template<class T>
void test_from_chars_ptr(const char *type_name)
{
    INFO("type=" << type_name);
    for(auto &pe : ptr_cases)
    {
        T *ptr = nullptr;
        INFO("val=" << pe.str);
        bool ret = from_chars(pe.str, &ptr);
        CHECK(ret);
        CHECK_EQ((void*)ptr, pe.ptr);
    }
}

template<class T>
void test_from_chars_first_ptr(const char *type_name)
{
    INFO("type=" << type_name);
    for(auto &pe : ptr_cases)
    {
        T *ptr = nullptr;
        INFO("val=" << pe.str);
        bool ret = from_chars(pe.str, &ptr);
        CHECK(ret);
        CHECK_EQ((void*)ptr, pe.ptr);
    }
}


TEST_CASE("xtoa.ptr")
{
    test_xtoa_ptr<void>("void");
    test_xtoa_ptr<int>("int");
    test_xtoa_ptr<std::vector<int>>("std::vector<int>");
}

TEST_CASE("atox.ptr")
{
    test_atox_ptr<void>("void");
    test_atox_ptr<int>("int");
    test_atox_ptr<std::vector<int>>("std::vector<int>");
}

TEST_CASE("to_chars.ptr")
{
    test_to_chars_ptr<void>("void");
    test_to_chars_ptr<int>("int");
    test_to_chars_ptr<std::vector<int>>("std::vector<int>");
}

TEST_CASE("from_chars.ptr")
{
    test_from_chars_ptr<void>("void");
    test_from_chars_ptr<int>("int");
    test_from_chars_ptr<std::vector<int>>("std::vector<int>");
}

TEST_CASE("from_chars_first.ptr")
{
    test_from_chars_first_ptr<void>("void");
    test_from_chars_first_ptr<int>("int");
    test_from_chars_first_ptr<std::vector<int>>("std::vector<int>");
}

} // namespace c4


C4_SUPPRESS_WARNING_GCC_POP
C4_SUPPRESS_WARNING_CLANG_POP

#include "c4/libtest/supprwarn_pop.hpp"
