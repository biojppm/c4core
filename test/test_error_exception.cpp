#include "c4/error.hpp"
#include "c4/test.hpp"
#include <stdexcept>
#include <string>


C4_BEGIN_HIDDEN_NAMESPACE
bool got_an_error = false;
bool got_an_exception = false;
C4_END_HIDDEN_NAMESPACE

void error_callback_throwing_exception(const char *msg_, size_t msg_sz)
{
    got_an_error = true;
    c4::csubstr s(msg_, msg_sz);
    if     (s == "err1") throw 1;
    else if(s == "err2") throw 2;
    else if(s == "err3") throw 3;
    else if(s == "err4") throw 4;
    else throw std::runtime_error({msg_, msg_+msg_sz});
}

inline c4::ScopedErrorSettings tmp_err()
{
    got_an_error = false;
    return c4::ScopedErrorSettings(c4::ON_ERROR_CALLBACK, error_callback_throwing_exception);
}


void test_exception(int which)
{
    if(which == 0) return;
    EXPECT_FALSE(got_an_exception);
    EXPECT_EQ(c4::get_error_callback() == error_callback_throwing_exception, false);
    {
        auto tmp = tmp_err();
        EXPECT_EQ(got_an_error, false);
        EXPECT_EQ(c4::get_error_callback() == error_callback_throwing_exception, true);
        try
        {
            if     (which == 1) { C4_ERROR("err1"); }
            else if(which == 2) { C4_ERROR("err2"); }
            else if(which == 3) { C4_ERROR("err3"); }
            else if(which == 4) { C4_ERROR("err4"); }
            else                { C4_ERROR("unknown error"); }
        }
        catch(int i)
        {
            got_an_exception = true;
            EXPECT_EQ(got_an_error, true);
            EXPECT_EQ(i, which);
            throw;
        }
        catch(std::runtime_error const& e)
        {
            got_an_exception = true;
            EXPECT_EQ(got_an_error, true);
            EXPECT_STREQ(e.what(), "unknown error");
            throw;
        }
        // if we get here it means no exception was thrown
        // so the test failed
        ADD_FAILURE();
    }
    EXPECT_EQ(c4::get_error_callback() == error_callback_throwing_exception, false);
}


// Although c4core does not use exceptions by default (*), you can have your
// error callback throw an exception which you can then catch on your code.
// This test covers that possibility.
//
// (*) Note that you can also configure c4core to throw an exception on error.

TEST(error, exception_from_callback)
{
    // works!
    got_an_exception = false;
    EXPECT_THROW(test_exception(-1), std::runtime_error);
    EXPECT_TRUE(got_an_exception);

    got_an_exception = false;
    EXPECT_NO_THROW(test_exception(0));
    EXPECT_FALSE(got_an_exception);

    got_an_exception = false;
    EXPECT_THROW(test_exception(1), int);
    EXPECT_TRUE(got_an_exception);

    got_an_exception = false;
    EXPECT_THROW(test_exception(2), int);
    EXPECT_TRUE(got_an_exception);

    got_an_exception = false;
    EXPECT_THROW(test_exception(3), int);
    EXPECT_TRUE(got_an_exception);

    got_an_exception = false;
    EXPECT_THROW(test_exception(4), int);
    EXPECT_TRUE(got_an_exception);

    got_an_exception = false;
    EXPECT_THROW(test_exception(6), std::runtime_error);
    EXPECT_TRUE(got_an_exception);
}
