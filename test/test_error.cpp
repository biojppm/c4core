#include "c4/error.hpp"
#include "c4/test.hpp"
#include "c4/libtest/supprwarn_push.hpp"

C4_BEGIN_HIDDEN_NAMESPACE
bool got_an_error = false;
void error_callback(const char *msg, size_t msg_sz)
{
    CHECK_EQ(strncmp(msg, "bla bla", msg_sz), 0);
    CHECK_EQ(msg_sz, 7);
    got_an_error = true;
}
inline c4::ScopedErrorSettings tmp_err()
{
    got_an_error = false;
    return c4::ScopedErrorSettings(c4::ON_ERROR_CALLBACK, error_callback);
}
C4_END_HIDDEN_NAMESPACE

C4_BEGIN_NAMESPACE(c4)

TEST_CASE("Error.scoped_callback")
{
    auto orig = get_error_callback();
    {
        auto tmp = tmp_err();
        CHECK_EQ(get_error_callback() == error_callback, true);
        C4_ERROR("bla bla");
        CHECK_EQ(got_an_error, true);
    }
    CHECK_EQ(get_error_callback() == orig, true);
}

C4_END_NAMESPACE(c4)

TEST_CASE("Error.outside_of_c4_namespace")
{
    auto orig = c4::get_error_callback();
    {
        auto tmp = tmp_err();
        CHECK_EQ(c4::get_error_callback() == error_callback, true);
        C4_ERROR("bla bla");
        CHECK_EQ(got_an_error, true);
    }
    CHECK_EQ(c4::get_error_callback() == orig, true);
}

#include "c4/libtest/supprwarn_pop.hpp"
