// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "c4/error.hpp"
#include "c4/test.hpp"

C4_BEGIN_HIDDEN_NAMESPACE
bool was_called = false;
void error_callback(const char *msg, size_t msg_sz)
{
    EXPECT_EQ(strncmp(msg, "bla bla", msg_sz), 0);
    was_called = true;
}
C4_END_HIDDEN_NAMESPACE

C4_BEGIN_NAMESPACE(c4)

TEST(Error, scoped_callback)
{
    auto orig = get_error_callback();
    {
        auto tmp = ScopedErrorSettings(ON_ERROR_CALLBACK, error_callback);
        EXPECT_EQ(get_error_callback() == error_callback, true);
        C4_ERROR("bla bla");
        EXPECT_EQ(was_called, true);
    }
    EXPECT_EQ(get_error_callback() == orig, true);
}

C4_END_NAMESPACE(c4)

TEST(Error, outside_of_c4_namespace)
{
    auto orig = c4::get_error_callback();
    {
        auto tmp = c4::ScopedErrorSettings(c4::ON_ERROR_CALLBACK, error_callback);
        EXPECT_EQ(c4::get_error_callback() == error_callback, true);
        C4_ERROR("bla bla");
        EXPECT_EQ(was_called, true);
    }
    EXPECT_EQ(c4::get_error_callback() == orig, true);
}
