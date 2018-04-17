// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "c4/memory_util.hpp"

#include "c4/test.hpp"

C4_BEGIN_NAMESPACE(c4)

TEST(mem_repeatT, one_repetition)
{
    char buf[120] = {0};

    mem_repeat(buf, "123", 2, 1);
    EXPECT_EQ(strcmp(buf, "12"), 0);

    mem_repeat(buf, "123", 3, 1);
    EXPECT_EQ(strcmp(buf, "123"), 0);
}

TEST(mem_repeatT, basic)
{
    char buf[120] = {0};

    mem_zero(buf);

    mem_repeat(buf, "123", 2, 2);
    EXPECT_EQ(strcmp(buf, "1212"), 0);
    EXPECT_EQ(buf[4], '\0');

    mem_zero(buf);

    mem_repeat(buf, "123", 3, 2);
    EXPECT_EQ(strcmp(buf, "123123"), 0);
    EXPECT_EQ(buf[6], '\0');

    mem_zero(buf);

    mem_repeat(buf, "123", 2, 3);
    EXPECT_EQ(strcmp(buf, "121212"), 0);
    EXPECT_EQ(buf[6], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 3);
    EXPECT_EQ(strcmp(buf, "123123123"), 0);
    EXPECT_EQ(buf[9], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 2, 4);
    EXPECT_EQ(strcmp(buf, "12121212"), 0);
    EXPECT_EQ(buf[8], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 4);
    EXPECT_EQ(strcmp(buf, "123123123123"), 0);
    EXPECT_EQ(buf[12], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 2, 5);
    EXPECT_EQ(strcmp(buf, "1212121212"), 0);
    EXPECT_EQ(buf[10], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 5);
    EXPECT_EQ(strcmp(buf, "123123123123123"), 0);
    EXPECT_EQ(buf[15], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 2, 6);
    EXPECT_EQ(strcmp(buf, "121212121212"), 0);
    EXPECT_EQ(buf[12], '\0');

    mem_zero(buf, sizeof(buf));

    mem_repeat(buf, "123", 3, 6);
    EXPECT_EQ(strcmp(buf, "123123123123123123"), 0);
    EXPECT_EQ(buf[18], '\0');
}


//-----------------------------------------------------------------------------

TEST(msb, basic)
{
    EXPECT_EQ(msb( 0), 0);
    EXPECT_EQ(msb( 1), 1);
    EXPECT_EQ(msb( 2), 2);
    EXPECT_EQ(msb( 3), 2);
    EXPECT_EQ(msb( 4), 3);
    EXPECT_EQ(msb( 5), 3);
    EXPECT_EQ(msb( 6), 3);
    EXPECT_EQ(msb( 7), 3);
    EXPECT_EQ(msb( 8), 4);
    EXPECT_EQ(msb( 9), 4);
    EXPECT_EQ(msb(10), 4);
    EXPECT_EQ(msb(11), 4);
    EXPECT_EQ(msb(12), 4);
    EXPECT_EQ(msb(13), 4);
    EXPECT_EQ(msb(14), 4);
    EXPECT_EQ(msb(15), 4);
    EXPECT_EQ(msb(16), 5);
}

TEST(msb11, basic)
{
    EXPECT_EQ((msb11<int, 0>::value), 0);
    EXPECT_EQ((msb11<int, 1>::value), 1);
    EXPECT_EQ((msb11<int, 2>::value), 2);
    EXPECT_EQ((msb11<int, 3>::value), 2);
    EXPECT_EQ((msb11<int, 4>::value), 3);
    EXPECT_EQ((msb11<int, 5>::value), 3);
    EXPECT_EQ((msb11<int, 6>::value), 3);
    EXPECT_EQ((msb11<int, 7>::value), 3);
    EXPECT_EQ((msb11<int, 8>::value), 4);
    EXPECT_EQ((msb11<int, 9>::value), 4);
    EXPECT_EQ((msb11<int,10>::value), 4);
    EXPECT_EQ((msb11<int,11>::value), 4);
    EXPECT_EQ((msb11<int,12>::value), 4);
    EXPECT_EQ((msb11<int,13>::value), 4);
    EXPECT_EQ((msb11<int,14>::value), 4);
    EXPECT_EQ((msb11<int,15>::value), 4);
    EXPECT_EQ((msb11<int,16>::value), 5);
}

C4_END_NAMESPACE(c4)
