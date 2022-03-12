#ifndef C4CORE_SINGLE_HEADER
#include "c4/memory_util.hpp"
#endif

#include "c4/libtest/supprwarn_push.hpp"

#include <c4/test.hpp>
#include <limits>

namespace c4 {

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

TEST_CASE("lsb.basic")
{
    CHECK_EQ(lsb( 0), 0);
    CHECK_EQ(lsb( 1), 0);
    CHECK_EQ(lsb( 2), 1);
    CHECK_EQ(lsb( 3), 0);
    CHECK_EQ(lsb( 4), 2);
    CHECK_EQ(lsb( 5), 0);
    CHECK_EQ(lsb( 6), 1);
    CHECK_EQ(lsb( 7), 0);
    CHECK_EQ(lsb( 8), 3);
    CHECK_EQ(lsb( 9), 0);
    CHECK_EQ(lsb(10), 1);
    CHECK_EQ(lsb(11), 0);
    CHECK_EQ(lsb(12), 2);
    CHECK_EQ(lsb(13), 0);
    CHECK_EQ(lsb(14), 1);
    CHECK_EQ(lsb(15), 0);
    CHECK_EQ(lsb(16), 4);
}

TEST_CASE("lsb11.basic")
{
    //CHECK_EQ((lsb11<int, 0>::value), 0);
    CHECK_EQ((lsb11<int, 1>::value), 0);
    CHECK_EQ((lsb11<int, 2>::value), 1);
    CHECK_EQ((lsb11<int, 3>::value), 0);
    CHECK_EQ((lsb11<int, 4>::value), 2);
    CHECK_EQ((lsb11<int, 5>::value), 0);
    CHECK_EQ((lsb11<int, 6>::value), 1);
    CHECK_EQ((lsb11<int, 7>::value), 0);
    CHECK_EQ((lsb11<int, 8>::value), 3);
    CHECK_EQ((lsb11<int, 9>::value), 0);
    CHECK_EQ((lsb11<int,10>::value), 1);
    CHECK_EQ((lsb11<int,11>::value), 0);
    CHECK_EQ((lsb11<int,12>::value), 2);
    CHECK_EQ((lsb11<int,13>::value), 0);
    CHECK_EQ((lsb11<int,14>::value), 1);
    CHECK_EQ((lsb11<int,15>::value), 0);
    CHECK_EQ((lsb11<int,16>::value), 4);
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
// tight pair

TEST_CASE("contiguous_mask.basic")
{
    CHECK_EQ(contiguous_mask(0, 0), 0);
    CHECK_EQ(contiguous_mask(0, 1), 1);
    CHECK_EQ(contiguous_mask(0, 2), 3);
    CHECK_EQ(contiguous_mask(0, 3), 7);
    CHECK_EQ(contiguous_mask(0, 4), 15);
    CHECK_EQ(contiguous_mask(1, 4), 14);
    CHECK_EQ(contiguous_mask(2, 4), 12);
    CHECK_EQ(contiguous_mask(3, 4), 8);
    CHECK_EQ(contiguous_mask(4, 4), 0);
}

TEST_CASE("contiguous_mask11.basic")
{
    CHECK_EQ((contiguous_mask11<int, 0, 0>::value), 0);
    CHECK_EQ((contiguous_mask11<int, 0, 1>::value), 1);
    CHECK_EQ((contiguous_mask11<int, 0, 2>::value), 3);
    CHECK_EQ((contiguous_mask11<int, 0, 3>::value), 7);
    CHECK_EQ((contiguous_mask11<int, 0, 4>::value), 15);
    CHECK_EQ((contiguous_mask11<int, 1, 4>::value), 14);
    CHECK_EQ((contiguous_mask11<int, 2, 4>::value), 12);
    CHECK_EQ((contiguous_mask11<int, 3, 4>::value), 8);
    CHECK_EQ((contiguous_mask11<int, 4, 4>::value), 0);
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
