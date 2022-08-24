#ifndef C4CORE_SINGLE_HEADER
#include "c4/memory_util.hpp"
#endif

#include "c4/libtest/supprwarn_push.hpp"

#include <c4/test.hpp>
#include <limits>

namespace c4 {

TEST_CASE("mem_overlaps")
{
    csubstr buf = "0123456789012345678901234567890123456789";
    CHECK_EQ(buf.len, 40);
    auto overlaps = [](csubstr lhs, csubstr rhs){
        bool res =  mem_overlaps(lhs.str, rhs.str, lhs.len, rhs.len);
        CHECK(res == lhs.overlaps(rhs));
        return res;
    };
    CHECK(!overlaps(buf.first(0), buf.last(0)));
    CHECK(!overlaps(buf.first(5), buf.last(5)));
    CHECK(!overlaps(buf.first(10), buf.last(10)));
    CHECK(!overlaps(buf.first(20), buf.last(20)));
    CHECK(overlaps(buf.first(21), buf.last(20)));
    CHECK(overlaps(buf.first(20), buf.last(21)));
    CHECK(!overlaps(buf.first(0), buf));
    CHECK(overlaps(buf.first(1), buf));
    CHECK(!overlaps(buf, buf.last(0)));
    CHECK(overlaps(buf, buf.last(1)));
    CHECK(!overlaps(buf.first(20), buf.last(20)));
    CHECK(overlaps(buf.first(21), buf.last(20)));
    CHECK(overlaps(buf.first(20), buf.first(21)));
}

TEST_CASE("mem_repeatT.one_repetition")
{
    char buf[120] = {0};

    mem_repeat(buf, "123", 2, 1);
    CHECK_EQ(strcmp(buf, "12"), 0);

    mem_repeat(buf, "123", 3, 1);
    CHECK_EQ(strcmp(buf, "123"), 0);
}

TEST_CASE("mem_repeatT.basic")
{
    char buf[120] = {0};

    mem_zero(buf);

    mem_repeat(buf, "123", 2, 2);
    CHECK_EQ(strcmp(buf, "1212"), 0);
    CHECK_EQ(buf[4], '\0');

    mem_zero(buf);

    mem_repeat(buf, "123", 3, 2);
    CHECK_EQ(strcmp(buf, "123123"), 0);
    CHECK_EQ(buf[6], '\0');

    mem_zero(buf);

    mem_repeat(buf, "123", 2, 3);
    CHECK_EQ(strcmp(buf, "121212"), 0);
    CHECK_EQ(buf[6], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 3);
    CHECK_EQ(strcmp(buf, "123123123"), 0);
    CHECK_EQ(buf[9], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 2, 4);
    CHECK_EQ(strcmp(buf, "12121212"), 0);
    CHECK_EQ(buf[8], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 4);
    CHECK_EQ(strcmp(buf, "123123123123"), 0);
    CHECK_EQ(buf[12], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 2, 5);
    CHECK_EQ(strcmp(buf, "1212121212"), 0);
    CHECK_EQ(buf[10], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 5);
    CHECK_EQ(strcmp(buf, "123123123123123"), 0);
    CHECK_EQ(buf[15], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 2, 6);
    CHECK_EQ(strcmp(buf, "121212121212"), 0);
    CHECK_EQ(buf[12], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 6);
    CHECK_EQ(strcmp(buf, "123123123123123123"), 0);
    CHECK_EQ(buf[18], '\0');
}


//-----------------------------------------------------------------------------

TEST_CASE("is_aligned.basic")
{
    CHECK(is_aligned<int>((int*)0x0));
    CHECK_FALSE(is_aligned<int>((int*)0x1));
    CHECK_FALSE(is_aligned<int>((int*)0x2));
    CHECK_FALSE(is_aligned<int>((int*)0x3));
    CHECK_FALSE(is_aligned<int>((int*)0x3));
    CHECK(is_aligned<int>((int*)0x4));
}


//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("lsb.basic", T, uint8_t, uint16_t, uint32_t, uint64_t)
{
    //CHECK_EQ(lsb<T>( 0), T(0));
    CHECK_EQ(lsb<T>( 1), T(0));
    CHECK_EQ(lsb<T>( 2), T(1));
    CHECK_EQ(lsb<T>( 3), T(0));
    CHECK_EQ(lsb<T>( 4), T(2));
    CHECK_EQ(lsb<T>( 5), T(0));
    CHECK_EQ(lsb<T>( 6), T(1));
    CHECK_EQ(lsb<T>( 7), T(0));
    CHECK_EQ(lsb<T>( 8), T(3));
    CHECK_EQ(lsb<T>( 9), T(0));
    CHECK_EQ(lsb<T>(10), T(1));
    CHECK_EQ(lsb<T>(11), T(0));
    CHECK_EQ(lsb<T>(12), T(2));
    CHECK_EQ(lsb<T>(13), T(0));
    CHECK_EQ(lsb<T>(14), T(1));
    CHECK_EQ(lsb<T>(15), T(0));
    CHECK_EQ(lsb<T>(16), T(4));
}

TEST_CASE_TEMPLATE("lsb11.basic", T, uint8_t, uint16_t, uint32_t, uint64_t)
{
    //CHECK_EQ((lsb11<T, 0>::value), T(0));
    CHECK_EQ((lsb11<T, 1>::value), T(0));
    CHECK_EQ((lsb11<T, 2>::value), T(1));
    CHECK_EQ((lsb11<T, 3>::value), T(0));
    CHECK_EQ((lsb11<T, 4>::value), T(2));
    CHECK_EQ((lsb11<T, 5>::value), T(0));
    CHECK_EQ((lsb11<T, 6>::value), T(1));
    CHECK_EQ((lsb11<T, 7>::value), T(0));
    CHECK_EQ((lsb11<T, 8>::value), T(3));
    CHECK_EQ((lsb11<T, 9>::value), T(0));
    CHECK_EQ((lsb11<T,10>::value), T(1));
    CHECK_EQ((lsb11<T,11>::value), T(0));
    CHECK_EQ((lsb11<T,12>::value), T(2));
    CHECK_EQ((lsb11<T,13>::value), T(0));
    CHECK_EQ((lsb11<T,14>::value), T(1));
    CHECK_EQ((lsb11<T,15>::value), T(0));
    CHECK_EQ((lsb11<T,16>::value), T(4));
}


//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("ipow.float", T, float, double)
{
    SUBCASE("base 1, signed exponent")
    {
        CHECK_FLOAT_EQ(ipow(T(1), int(0)), T(1));
        CHECK_FLOAT_EQ(ipow(T(1), int(1)), T(1));
        CHECK_FLOAT_EQ(ipow(T(1), int(2)), T(1));
        CHECK_FLOAT_EQ(ipow(T(1), -int(1)), T(1));
        CHECK_FLOAT_EQ(ipow(T(1), -int(2)), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(int(0))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(int(1))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(int(2))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(-int(1))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(-int(2))), T(1));
    }
    SUBCASE("base 1, unsigned exponent")
    {
        CHECK_FLOAT_EQ(ipow(T(1), unsigned(0)), T(1));
        CHECK_FLOAT_EQ(ipow(T(1), unsigned(1)), T(1));
        CHECK_FLOAT_EQ(ipow(T(1), unsigned(2)), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(unsigned(0))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(unsigned(1))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 1>(unsigned(2))), T(1));
    }
    SUBCASE("base 2, signed exponent")
    {
        CHECK_FLOAT_EQ(ipow(T(2), int(0)), T(1));
        CHECK_FLOAT_EQ(ipow(T(2), int(1)), T(2));
        CHECK_FLOAT_EQ(ipow(T(2), int(2)), T(4));
        CHECK_FLOAT_EQ(ipow(T(2), int(7)), T(128));
        CHECK_FLOAT_EQ(ipow(T(2), -int(1)), T(0.5));
        CHECK_FLOAT_EQ(ipow(T(2), -int(2)), T(0.25));
        CHECK_FLOAT_EQ(ipow(T(2), -int(3)), T(0.125));
        CHECK_FLOAT_EQ(ipow(T(2), -int(4)), T(0.0625));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(int(0))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(int(1))), T(2));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(int(2))), T(4));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(int(7))), T(128));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(-int(1))), T(0.5));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(-int(2))), T(0.25));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(-int(3))), T(0.125));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(-int(4))), T(0.0625));
    }
    SUBCASE("base 2, unsigned exponent")
    {
        CHECK_FLOAT_EQ(ipow(T(2), unsigned(0)), T(1));
        CHECK_FLOAT_EQ(ipow(T(2), unsigned(1)), T(2));
        CHECK_FLOAT_EQ(ipow(T(2), unsigned(2)), T(4));
        CHECK_FLOAT_EQ(ipow(T(2), unsigned(6)), T(64));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(unsigned(0))), T(1));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(unsigned(1))), T(2));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(unsigned(2))), T(4));
        CHECK_FLOAT_EQ((ipow<T, int, 2>(unsigned(6))), T(64));
    }
}

TEST_CASE_TEMPLATE("ipow", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    SUBCASE("base 1, signed exponent")
    {
        CHECK_EQ(ipow(T(1), int(0)), T(1));
        CHECK_EQ(ipow(T(1), int(1)), T(1));
        CHECK_EQ(ipow(T(1), int(2)), T(1));
        CHECK_EQ(ipow(T(1), -int(1)), T(1));
        CHECK_EQ(ipow(T(1), -int(2)), T(1));
        CHECK_EQ(ipow<T, T(1)>(int(0)), T(1));
        CHECK_EQ(ipow<T, T(1)>(int(1)), T(1));
        CHECK_EQ(ipow<T, T(1)>(int(2)), T(1));
        CHECK_EQ(ipow<T, T(1)>(-int(1)), T(1));
        CHECK_EQ(ipow<T, T(1)>(-int(2)), T(1));
    }
    SUBCASE("base 1, unsigned exponent")
    {
        CHECK_EQ(ipow(T(1), unsigned(0)), T(1));
        CHECK_EQ(ipow(T(1), unsigned(1)), T(1));
        CHECK_EQ(ipow(T(1), unsigned(2)), T(1));
        CHECK_EQ(ipow<T, T(1)>(unsigned(0)), T(1));
        CHECK_EQ(ipow<T, T(1)>(unsigned(1)), T(1));
        CHECK_EQ(ipow<T, T(1)>(unsigned(2)), T(1));
    }
    SUBCASE("base 2, signed exponent")
    {
        CHECK_EQ(ipow(T(2), int(0)), T(1));
        CHECK_EQ(ipow(T(2), int(1)), T(2));
        CHECK_EQ(ipow(T(2), int(2)), T(4));
        CHECK_EQ(ipow(T(2), int(6)), T(64));
        CHECK_EQ(ipow(T(2), -int(1)), T(0));
        CHECK_EQ(ipow(T(2), -int(2)), T(0));
        CHECK_EQ(ipow(T(2), -int(6)), T(0));
        CHECK_EQ(ipow<T, T(2)>(int(0)), T(1));
        CHECK_EQ(ipow<T, T(2)>(int(1)), T(2));
        CHECK_EQ(ipow<T, T(2)>(int(2)), T(4));
        CHECK_EQ(ipow<T, T(2)>(int(6)), T(64));
        CHECK_EQ(ipow<T, T(2)>(-int(1)), T(0));
        CHECK_EQ(ipow<T, T(2)>(-int(2)), T(0));
        CHECK_EQ(ipow<T, T(2)>(-int(7)), T(0));
    }
    SUBCASE("base 2, unsigned exponent")
    {
        CHECK_EQ(ipow(T(2), unsigned(0)), T(1));
        CHECK_EQ(ipow(T(2), unsigned(1)), T(2));
        CHECK_EQ(ipow(T(2), unsigned(2)), T(4));
        CHECK_EQ(ipow(T(2), unsigned(6)), T(64));
        CHECK_EQ(ipow<T, T(2)>(unsigned(0)), T(1));
        CHECK_EQ(ipow<T, T(2)>(unsigned(1)), T(2));
        CHECK_EQ(ipow<T, T(2)>(unsigned(2)), T(4));
        CHECK_EQ(ipow<T, T(2)>(unsigned(6)), T(64));
    }
}


//-----------------------------------------------------------------------------

TEST_CASE_TEMPLATE("msb.basic", T, uint8_t, uint16_t, uint32_t, uint64_t)
{
    CHECK_EQ(msb(T( 1)), 0u);
    CHECK_EQ(msb(T( 2)), 1u);
    CHECK_EQ(msb(T( 3)), 1u);
    CHECK_EQ(msb(T( 4)), 2u);
    CHECK_EQ(msb(T( 5)), 2u);
    CHECK_EQ(msb(T( 6)), 2u);
    CHECK_EQ(msb(T( 7)), 2u);
    CHECK_EQ(msb(T( 8)), 3u);
    CHECK_EQ(msb(T( 9)), 3u);
    CHECK_EQ(msb(T(10)), 3u);
    CHECK_EQ(msb(T(11)), 3u);
    CHECK_EQ(msb(T(12)), 3u);
    CHECK_EQ(msb(T(13)), 3u);
    CHECK_EQ(msb(T(14)), 3u);
    CHECK_EQ(msb(T(15)), 3u);
    CHECK_EQ(msb(T(16)), 4u);
    CHECK_EQ(msb(std::numeric_limits<T>::max()), 8u * sizeof(T) - 1u);
}

TEST_CASE_TEMPLATE("msb11.basic", T, uint8_t, uint16_t, uint32_t, uint64_t)
{
    CHECK_EQ((msb11<T,T( 1)>::value), T(0));
    CHECK_EQ((msb11<T,T( 2)>::value), T(1));
    CHECK_EQ((msb11<T,T( 3)>::value), T(1));
    CHECK_EQ((msb11<T,T( 4)>::value), T(2));
    CHECK_EQ((msb11<T,T( 5)>::value), T(2));
    CHECK_EQ((msb11<T,T( 6)>::value), T(2));
    CHECK_EQ((msb11<T,T( 7)>::value), T(2));
    CHECK_EQ((msb11<T,T( 8)>::value), T(3));
    CHECK_EQ((msb11<T,T( 9)>::value), T(3));
    CHECK_EQ((msb11<T,T(10)>::value), T(3));
    CHECK_EQ((msb11<T,T(11)>::value), T(3));
    CHECK_EQ((msb11<T,T(12)>::value), T(3));
    CHECK_EQ((msb11<T,T(13)>::value), T(3));
    CHECK_EQ((msb11<T,T(14)>::value), T(3));
    CHECK_EQ((msb11<T,T(15)>::value), T(3));
    CHECK_EQ((msb11<T,T(16)>::value), T(4));
    CHECK_EQ((msb11<T,std::numeric_limits<T>::max()>::value), 8u * sizeof(T) - 1u);
}


//-----------------------------------------------------------------------------
// contiguous mask

template<class T> T _mask() { return T(0); }
template<class T, class... Bits> T _mask(int bit, Bits ...bits) { return (T)(T(1) << bit | _mask<T>(bits...)); }

TEST_CASE_TEMPLATE("contiguous_mask.basic", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    CHECK_EQ(contiguous_mask(0, 0), _mask<T>());
    CHECK_EQ(contiguous_mask(0, 1), _mask<T>(0));
    CHECK_EQ(contiguous_mask(0, 2), _mask<T>(0, 1));
    CHECK_EQ(contiguous_mask(0, 3), _mask<T>(0, 1, 2));
    CHECK_EQ(contiguous_mask(0, 4), _mask<T>(0, 1, 2, 3));
    CHECK_EQ(contiguous_mask(1, 4), _mask<T>(   1, 2, 3));
    CHECK_EQ(contiguous_mask(2, 4), _mask<T>(      2, 3));
    CHECK_EQ(contiguous_mask(3, 4), _mask<T>(         3));
    CHECK_EQ(contiguous_mask(4, 4), _mask<T>());
}

TEST_CASE_TEMPLATE("contiguous_mask11.basic", T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t)
{
    CHECK_EQ((contiguous_mask11<T, 0, 0>::value), _mask<T>());
    CHECK_EQ((contiguous_mask11<T, 0, 1>::value), _mask<T>(0));
    CHECK_EQ((contiguous_mask11<T, 0, 2>::value), _mask<T>(0, 1));
    CHECK_EQ((contiguous_mask11<T, 0, 3>::value), _mask<T>(0, 1, 2));
    CHECK_EQ((contiguous_mask11<T, 0, 4>::value), _mask<T>(0, 1, 2, 3));
    CHECK_EQ((contiguous_mask11<T, 1, 4>::value), _mask<T>(   1, 2, 3));
    CHECK_EQ((contiguous_mask11<T, 2, 4>::value), _mask<T>(      2, 3));
    CHECK_EQ((contiguous_mask11<T, 3, 4>::value), _mask<T>(         3));
    CHECK_EQ((contiguous_mask11<T, 4, 4>::value), _mask<T>());
}


//-----------------------------------------------------------------------------


template<size_t N> struct sz    { char buf[N]; };
template<        > struct sz<0> {              };
template<size_t F, size_t S> void check_tp()
{
    #if defined(__clang__)
    #   pragma clang diagnostic push
    #elif defined(__GNUC__)
    #   pragma GCC diagnostic push
    #   if __GNUC__ >= 7
    #       pragma GCC diagnostic ignored "-Wduplicated-branches"
    #   endif
    #endif
    size_t expected;
    if(F != 0 && S != 0) expected = F+S;
    else if(F == 0 && S != 0) expected = S;
    else if(F != 0 && S == 0) expected = F;   // -Wduplicated-branches: false positive here
    else /* F == 0 && S == 0)*/expected = 1;
    #if defined(__clang__)
    #   pragma clang diagnostic pop
    #elif defined(__GNUC__)
    #   pragma GCC diagnostic pop
    #endif
    INFO("F=" << F << "  S=" << S);
    CHECK_EQ(sizeof(tight_pair<sz<F>, sz<S>>), expected);
}


TEST_CASE("tight_pair.basic")
{
    check_tp<0,0>();
    check_tp<0,1>();
    check_tp<0,2>();
    check_tp<0,3>();
    check_tp<0,4>();

    check_tp<0,0>();
    check_tp<1,0>();
    check_tp<2,0>();
    check_tp<3,0>();
    check_tp<4,0>();

    check_tp<0,0>();
    check_tp<1,1>();
    check_tp<2,2>();
    check_tp<3,3>();
    check_tp<4,4>();
}

} // namespace c4

#include "c4/libtest/supprwarn_pop.hpp"
