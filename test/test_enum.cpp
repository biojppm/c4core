#include <vector>

#include <c4/enum.hpp>
#include <c4/test.hpp>

#include "./test_enum_common.hpp"
#include "c4/libtest/supprwarn_push.hpp"


TEST(eoffs, simple_enum)
{
    using namespace c4;
    EXPECT_EQ(eoffs_cls<MyEnum>(), 0);
    EXPECT_EQ(eoffs_pfx<MyEnum>(), 0);
}

TEST(eoffs, scoped_enum)
{
    using namespace c4;
    EXPECT_EQ(eoffs_cls<MyEnumClass>(), strlen("MyEnumClass::"));
    EXPECT_EQ(eoffs_pfx<MyEnumClass>(), 0);
}

TEST(eoffs, simple_bitmask)
{
    using namespace c4;
    EXPECT_EQ(eoffs_cls<MyBitmask>(), 0);
    EXPECT_EQ(eoffs_pfx<MyBitmask>(), strlen("BM_"));
}

TEST(eoffs, scoped_bitmask)
{
    using namespace c4;
    EXPECT_EQ(eoffs_cls<MyBitmaskClass>(), strlen("MyBitmaskClass::"));
    EXPECT_EQ(eoffs_pfx<MyBitmaskClass>(), strlen("MyBitmaskClass::BM_"));
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifdef __clang__
#   pragma clang diagnostic push
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   if __GNUC__ >= 6
#       pragma GCC diagnostic ignored "-Wnull-dereference"
#   endif
#endif

template<typename Enum>
void cmp_enum(Enum lhs, Enum rhs)
{
    using I = typename std::underlying_type<Enum>::type;
    EXPECT_EQ(static_cast<I>(lhs), static_cast<I>(rhs));
}

template<class Enum>
void test_esyms()
{
    auto ss = c4::esyms<Enum>();
    EXPECT_NE(ss.size(), 0);
    EXPECT_FALSE(ss.empty());
    for(auto s : ss)
    {
        ASSERT_NE(ss.find(s.name), nullptr);
        ASSERT_NE(ss.find(s.value), nullptr);
        EXPECT_STREQ(ss.find(s.name)->name, s.name);
        EXPECT_STREQ(ss.find(s.value)->name, s.name);
        cmp_enum(ss.find(s.name)->value, s.value);
        cmp_enum(ss.find(s.value)->value, s.value);
    }
}

#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif


TEST(esyms, simple_enum)
{
    test_esyms<MyEnum>();
}

TEST(esyms, scoped_enum)
{
    test_esyms<MyEnumClass>();
}

TEST(esyms, simple_bitmask)
{
    test_esyms<MyBitmask>();
}

TEST(esyms, scoped_bitmask)
{
    test_esyms<MyBitmaskClass>();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


template<typename Enum>
void test_e2str()
{
    using namespace c4;
    using I = typename std::underlying_type<Enum>::type;
    auto ss = esyms<Enum>();
    EXPECT_NE(ss.size(), 0);
    EXPECT_FALSE(ss.empty());
    for(auto const& p : ss)
    {
        // test round trips
        cmp_enum(str2e<Enum>(e2str(p.value)), p.value);
        EXPECT_STREQ(e2str(str2e<Enum>(p.name)), p.name);
    }
}


TEST(e2str, simple_enum)
{
    test_e2str<MyEnum>();
}

TEST(e2str, scoped_enum)
{
    test_e2str<MyEnumClass>();
    cmp_enum(c4::str2e<MyEnumClass>("MyEnumClass::FOO"), MyEnumClass::FOO);
    cmp_enum(c4::str2e<MyEnumClass>("FOO"), MyEnumClass::FOO);
}

TEST(e2str, simple_bitmask)
{
    test_e2str<MyBitmask>();
    cmp_enum(c4::str2e<MyBitmask>("BM_FOO"), BM_FOO);
    cmp_enum(c4::str2e<MyBitmask>("FOO"), BM_FOO);
}

TEST(e2str, scoped_bitmask)
{
    using I = typename std::underlying_type<MyBitmaskClass>::type;
    test_e2str<MyBitmaskClass>();
    cmp_enum(c4::str2e<MyBitmaskClass>("MyBitmaskClass::BM_FOO"), MyBitmaskClass::BM_FOO);
    cmp_enum(c4::str2e<MyBitmaskClass>("BM_FOO"), MyBitmaskClass::BM_FOO);
    cmp_enum(c4::str2e<MyBitmaskClass>("FOO"), MyBitmaskClass::BM_FOO);
}

#include "c4/libtest/supprwarn_pop.hpp"
