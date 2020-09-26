#include "c4/test.hpp"
#include "c4/blob.hpp"


#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wcast-align"
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wcast-align"
#endif


namespace c4 {

template<class T>
void test_blob()
{
    T v;
    blob b(v);
    EXPECT_EQ((T*)b.buf, &v);
    EXPECT_EQ(b.len, sizeof(T));

    blob b2 = b;
    EXPECT_EQ((T*)b2.buf, &v);
    EXPECT_EQ(b2.len, sizeof(T));
}

TEST(blob, basic)
{
    test_blob<int>();
}


#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif

} // namespace c4
