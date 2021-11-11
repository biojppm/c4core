#include "c4/test.hpp"
#ifndef C4CORE_SINGLE_HEADER
#include "c4/std/vector.hpp"
#endif

namespace c4 {

template< size_t N >
std::vector<char> ctor(const char (&s)[N])
{
    return std::vector<char>(s, s+N-1);
}

TEST_CASE("std_vector.to_substr")
{
    std::vector<char> s = ctor("barnabe");
    auto ss = to_substr(s);
    CHECK_EQ(ss.str, s.data());
    CHECK_EQ(ss.len, s.size());
    ss[0] = 'B';
    CHECK_EQ(s[0], 'B');
}

} // namespace c4
