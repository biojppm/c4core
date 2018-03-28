#include "c4/test.hpp"
#include "c4/to_str.hpp"
#include "c4/std/string.hpp"

namespace c4 {

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
}

TEST(to_str, trimmed_fit_int)
{
    // test that no characters are trimmed at the end of
    // the number due to printf-based implementations
    // needing space for the \0
    int v = 12345678;
    char buf[128];
    substr sp(buf);
    size_t sz = to_str(sp, v);
    sp = sp.left_of(sz);
    EXPECT_EQ(sp, "12345678"); // ehemm.
    char buf2[8+1];
    C4_ASSERT(sizeof(buf2) == sz+1);
    substr sp2(buf2, sizeof(buf2)); // make sure it spans the whole buffer
    sp2 = to_str_substr(sp2, v);
    EXPECT_EQ(sp2, sp); // ehemm.
    std::string str;
    catrs(&str, v);
    EXPECT_EQ(sp, to_csubstr(str)); // ehemm.
}

TEST(to_str, trimmed_fit_float)
{
    // test that no characters are trimmed at the end of
    // the number due to printf-based implementations
    // needing space for the \0
    float v = 1024.1568f;
    char buf[128];
    substr sp(buf);
    size_t sz = to_str(sp, v);
    sp = sp.left_of(sz);
    EXPECT_EQ(sp, "1024.16"); // ehemm.
    char buf2[7 + 1];
    C4_ASSERT(sizeof(buf2) == sz+1);
    substr sp2(buf2, sizeof(buf2)); // make sure it spans the whole buffer
    sp2 = to_str_substr(sp2, v);
    EXPECT_EQ(sp2, sp); // ehemm.
    std::string str;
    catrs(&str, v);
    EXPECT_EQ(sp, to_csubstr(str)); // ehemm.
}

TEST(to_str, trimmed_fit_double)
{
    // test that no characters are trimmed at the end of
    // the number due to printf-based implementations
    // needing space for the \0
    double v = 1024.1568;
    char buf[128];
    substr sp(buf);
    size_t sz = to_str(sp, v);
    sp = sp.left_of(sz);
    EXPECT_EQ(sp, "1024.16"); // ehemm.
    char buf2[7 + 1];
    C4_ASSERT(sizeof(buf2) == sz+1);
    substr sp2(buf2, sizeof(buf2)); // make sure it spans the whole buffer
    sp2 = to_str_substr(sp2, v);
    EXPECT_EQ(sp2, sp); // ehemm.
    std::string str;
    catrs(&str, v);
    EXPECT_EQ(sp, to_csubstr(str)); // ehemm.
}

} // namespace c4
