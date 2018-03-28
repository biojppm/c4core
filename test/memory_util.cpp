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

C4_END_NAMESPACE(c4)
