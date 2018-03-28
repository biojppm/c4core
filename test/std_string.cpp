#include "c4/test.hpp"
#include "c4/std/string.hpp"

namespace c4 {

TEST(std_string, to_substr)
{
    std::string s("barnabe");
    auto ss = to_substr(s);
    EXPECT_EQ(ss.str, s.data());
    EXPECT_EQ(ss.len, s.size());
    ss[0] = 'B';
    EXPECT_EQ(s[0], 'B');
}

} // namespace c4
