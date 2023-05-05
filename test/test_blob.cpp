#ifndef C4CORE_SINGLE_HEADER
#include "c4/blob.hpp"
#endif

#include "c4/test.hpp"

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wcast-align"
#   pragma GCC diagnostic ignored "-Wold-style-cast"
#endif


namespace c4 {

TEST_CASE("blob.default_ctor")
{
    {
        blob b = {};
        CHECK_EQ(b.buf, nullptr);
        CHECK_EQ(b.len, 0u);
    }
    {
        cblob b = {};
        CHECK_EQ(b.buf, nullptr);
        CHECK_EQ(b.len, 0u);
    }
}

template<class B>
void test_blob_copy(blob const& b)
{
    {
        blob_<B> b2(b);
        CHECK_EQ(b2.buf, b.buf);
        CHECK_EQ(b2.len, b.len);
    }
    {
        blob_<B> b2 = b;
        CHECK_EQ(b2.buf, b.buf);
        CHECK_EQ(b2.len, b.len);
    }
    {
        blob_<const B> b3(b);
        CHECK_EQ(b3.buf, b.buf);
        CHECK_EQ(b3.len, b.len);
    }
    {
        blob_<const B> b3 = b;
        CHECK_EQ(b3.buf, b.buf);
        CHECK_EQ(b3.len, b.len);
    }
    {
        blob_<B> b2(b);
        CHECK_EQ(b2.buf, b.buf);
        CHECK_EQ(b2.len, b.len);
        blob_<const B> b4(b2);
        CHECK_EQ(b4.buf, b.buf);
        CHECK_EQ(b4.len, b.len);
    }
    {
        blob_<B> b2 = b;
        CHECK_EQ(b2.buf, b.buf);
        CHECK_EQ(b2.len, b.len);
        blob_<const B> b4 = b2;
        CHECK_EQ(b4.buf, b.buf);
        CHECK_EQ(b4.len, b.len);
    }
}
void test_blob_copy(blob const& b)
{
    test_blob_copy<byte>(b);
    test_blob_copy<cbyte>(b);
}
template<class T>
void test_blob(T v)
{
    blob b(v);
    CHECK_EQ((T*)b.buf, &v);
    CHECK_EQ(b.len, sizeof(T));
    test_blob_copy(b);
}

TEST_CASE("blob.fundamental_type")
{
    SUBCASE("int")
    {
        test_blob(1);
    }
    SUBCASE("u64")
    {
        test_blob(UINT64_C(1));
    }
    SUBCASE("int.arr")
    {
        int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        cblob b(arr);
        CHECK_EQ((int const*)b.buf, arr);
        CHECK_EQ(b.len, sizeof(arr));
        test_blob(arr);
    }
    SUBCASE("u64.arr")
    {
        uint64_t arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        cblob b(arr);
        CHECK_EQ((uint64_t const*)b.buf, arr);
        CHECK_EQ(b.len, sizeof(arr));
        test_blob(arr);
    }
}

} // namespace c4

#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif
