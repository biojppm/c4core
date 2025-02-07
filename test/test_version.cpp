#include "c4/test.hpp"
#ifndef C4CORE_SINGLE_HEADER
#include "c4/version.hpp"
#endif

TEST_CASE("version.major")
{
    int v = c4::version_major();
    CHECK_GE(v, 0);
    CHECK_EQ(v, C4CORE_VERSION_MAJOR);
}

TEST_CASE("version.minor")
{
    int v = c4::version_minor();
    CHECK_GE(v, 0);
    CHECK_EQ(v, C4CORE_VERSION_MINOR);
}

TEST_CASE("version.patch")
{
    int v = c4::version_patch();
    CHECK_GE(v, 0);
    CHECK_EQ(v, C4CORE_VERSION_PATCH);
}

TEST_CASE("version.str")
{
    c4::csubstr v = c4::to_csubstr(c4::version());
    CHECK_GE(v.len, 5);
    CHECK_EQ(v, C4_XQUOTE(C4CORE_VERSION_MAJOR) "." C4_XQUOTE(C4CORE_VERSION_MINOR) "." C4_XQUOTE(C4CORE_VERSION_PATCH) "");
}
