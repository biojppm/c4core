#ifndef C4CORE_SINGLE_HEADER
#include "c4/error.hpp"
#endif

#include "c4/test.hpp"
#include <string>
#ifdef C4_EXCEPTIONS
#include <stdexcept>
#else
#include <csetjmp>
#endif
C4_SUPPRESS_WARNING_MSVC_WITH_PUSH(4611) // interaction between '_setjmp' and C++ object destruction is non-portable
C4_SUPPRESS_WARNING_MSVC_WITH_PUSH(4702) // unreachable code

C4_BEGIN_HIDDEN_NAMESPACE
bool got_an_error = false;
bool got_an_exception = false;
C4_IF_EXCEPTIONS_( , std::jmp_buf s_jmp_env; int s_jmp_val; std::string s_jmp_msg;)
C4_END_HIDDEN_NAMESPACE

void error_callback_throwing_exception(const char *msg_, size_t msg_sz)
{
    got_an_error = true;
    c4::csubstr s(msg_, msg_sz);
    if     (s == "err1") C4_IF_EXCEPTIONS(throw 1;  ,  s_jmp_val = 1; std::longjmp(s_jmp_env, s_jmp_val););
    else if(s == "err2") C4_IF_EXCEPTIONS(throw 2;  ,  s_jmp_val = 2; std::longjmp(s_jmp_env, s_jmp_val););
    else if(s == "err3") C4_IF_EXCEPTIONS(throw 3;  ,  s_jmp_val = 3; std::longjmp(s_jmp_env, s_jmp_val););
    else if(s == "err4") C4_IF_EXCEPTIONS(throw 4;  ,  s_jmp_val = 4; std::longjmp(s_jmp_env, s_jmp_val););
    else C4_IF_EXCEPTIONS(
                          throw std::runtime_error({msg_, msg_+msg_sz});
                          ,
                          s_jmp_val = 100;
                          s_jmp_msg.assign(msg_, msg_sz);
                          std::longjmp(s_jmp_env, s_jmp_val);
                          );
}

inline c4::ScopedErrorSettings tmp_err()
{
    got_an_error = false;
    return c4::ScopedErrorSettings(c4::ON_ERROR_CALLBACK, error_callback_throwing_exception);
}

C4_IF_EXCEPTIONS_( , std::jmp_buf s_jmp_env_fwd;)

void test_exception(const int which)
{
    if(which == 0)
        return;
    INFO("which=" << which);
    C4_CHECK(!got_an_exception);
    C4_CHECK(c4::get_error_callback() == error_callback_throwing_exception);
    {
        auto tmp = tmp_err();
        CHECK_EQ(got_an_error, false);
        CHECK_EQ(c4::get_error_callback() == error_callback_throwing_exception, true);
        C4_IF_EXCEPTIONS_(try, if(setjmp(s_jmp_env) == 0))
        {
            if     (which == 1) { C4_ERROR("err1"); }
            else if(which == 2) { C4_ERROR("err2"); }
            else if(which == 3) { C4_ERROR("err3"); }
            else if(which == 4) { C4_ERROR("err4"); }
            else                { C4_ERROR("unknown error"); }
        }
        C4_IF_EXCEPTIONS_(catch(int i), else if(s_jmp_val < 5))
        {
            got_an_exception = true;
            CHECK_EQ(got_an_error, true);
            CHECK_EQ(C4_IF_EXCEPTIONS_(i, s_jmp_val), which);
            C4_IF_EXCEPTIONS_(throw, std::longjmp(s_jmp_env_fwd, s_jmp_val));
        }
        C4_IF_EXCEPTIONS_(catch(std::runtime_error const& e), else)
        {
            got_an_exception = true;
            CHECK_EQ(got_an_error, true);
            const c4::csubstr expected = "unknown error";
            const c4::csubstr actual = C4_IF_EXCEPTIONS_(c4::to_csubstr(e.what()), c4::csubstr(s_jmp_msg.data(), s_jmp_msg.size()));
            CHECK_EQ(actual.size(), expected.size());
            CHECK_EQ(actual, expected);
            C4_IF_EXCEPTIONS_(throw, std::longjmp(s_jmp_env_fwd, s_jmp_val));
        }
        // if we get here it means no exception was thrown
        // so the test failed
        FAIL("an exception was thrown");
    }
    CHECK_EQ(c4::get_error_callback() == error_callback_throwing_exception, false);
}


// Although c4core does not use exceptions by default, you can have
// your error callback throw an exception which you can then catch on
// your code.
//
// This test covers that possibility.

TEST_CASE("error.exception_from_callback")
{
    // works!
    auto tmp = tmp_err();

    #ifdef C4_EXCEPTIONS
        #define CHECK_FAILURE_IS(code, failure_exc, failure_setjmp) \
        {                                                           \
            got_an_exception = false;                               \
            CHECK_THROWS_AS(code, failure_exc);                     \
            CHECK(got_an_exception);                                \
        }
    #else
        #define CHECK_FAILURE_IS(code, failure_exc, failure_setjmp) \
        {                                                           \
            INFO("expected case: " << failure_setjmp);              \
            got_an_exception = false;                               \
            if(setjmp(s_jmp_env_fwd) == 0)                          \
            {                                                       \
                code;                                               \
            }                                                       \
            CHECK_EQ(s_jmp_val, failure_setjmp);                    \
            CHECK(got_an_exception);                                \
        }
    #endif

    CHECK_FAILURE_IS(test_exception(-1), std::runtime_error, 100);
    CHECK_FAILURE_IS(test_exception(1), int, 1);
    CHECK_FAILURE_IS(test_exception(2), int, 2);
    CHECK_FAILURE_IS(test_exception(3), int, 3);
    CHECK_FAILURE_IS(test_exception(4), int, 4);
    CHECK_FAILURE_IS(test_exception(5), std::runtime_error, 100);
    CHECK_FAILURE_IS(test_exception(6), std::runtime_error, 100);
}

C4_SUPPRESS_WARNING_MSVC_POP
