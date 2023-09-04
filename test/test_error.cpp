#ifndef C4CORE_SINGLE_HEADER
#include "c4/error.hpp"
#endif
#include "c4/test.hpp"
#include "c4/libtest/supprwarn_push.hpp"
#ifndef C4_EXCEPTIONS
#include <csetjmp>
#endif
C4_SUPPRESS_WARNING_MSVC_WITH_PUSH(4611) // interaction between '_setjmp' and C++ object destruction is non-portable

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

namespace c4 {

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

} // namespace c4

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


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// WIP: new error handling code


#include <string>    // temporary; just for the exception example
#include <iostream>  // temporary; just for the exception example

C4_IF_EXCEPTIONS_( , static std::jmp_buf s_jmp_buf; static std::string s_jmp_errmsg; );

namespace c4 {

#define C4_ERR_FMT_BUFFER_SIZE 256

using locref = c4::srcloc const& C4_RESTRICT;

using pfn_err       = void (*)(locref loc, void *data);
using pfn_warn      = void (*)(locref loc, void *data);
using pfn_msg_begin = void (*)(locref loc, void *data);
using pfn_msg_part  = void (*)(const char* msg, size_t size, void *data);
using pfn_msg_end   = void (*)(void *data);

struct ErrorCallbacks
{
    void          *user_data;

    pfn_err        err;
    pfn_warn       warn;
    pfn_msg_begin  msg_begin;
    pfn_msg_part   msg_part;
    pfn_msg_end    msg_end;

    bool msg_enabled() const { return msg_begin != nullptr; }

    template<size_t N>
    void msg(const char (&s)[N])
    {
        msg_part(s, N-1, user_data);
    }
    void msg(const char *msg, size_t sz)
    {
        msg_part(msg, sz, user_data);
    }
    void msg(char c)
    {
        msg_part(&c, 1, user_data);
    }
};

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 8 && __GNUC__ < 5
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
TEST_CASE("ErrorCallbacks.default_obj")
{
    ErrorCallbacks cb {};
    CHECK_EQ(cb.user_data, nullptr);
    CHECK_EQ(cb.err, nullptr);
    CHECK_EQ(cb.warn, nullptr);
    CHECK_EQ(cb.msg_begin, nullptr);
    CHECK_EQ(cb.msg_part, nullptr);
    CHECK_EQ(cb.msg_end, nullptr);
}
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 8 && __GNUC__ < 5
#pragma GCC diagnostic pop
#endif

template<class ErrBhv>
struct ErrorCallbacksBridgeFull
{
    ErrorCallbacks callbacks() const
    {
        C4_SUPPRESS_WARNING_GCC_CLANG_WITH_PUSH("-Wcast-qual")
        return {
            (ErrBhv*)this,
            ErrorCallbacksBridgeFull<ErrBhv>::on_err,
            ErrorCallbacksBridgeFull<ErrBhv>::on_warn,
            ErrorCallbacksBridgeFull<ErrBhv>::on_msg_begin,
            ErrorCallbacksBridgeFull<ErrBhv>::on_msg_part,
            ErrorCallbacksBridgeFull<ErrBhv>::on_msg_end,
        };
        C4_SUPPRESS_WARNING_GCC_CLANG_POP
    }
    static void on_err(locref loc, void *data)
    {
        ((ErrBhv*)data)->err(loc);
    }
    static void on_warn(locref loc, void *data)
    {
        ((ErrBhv*)data)->warn(loc);
    }
    static void on_msg_begin(locref loc, void *data)
    {
        ((ErrBhv*)data)->msg_begin(loc);
    }
    static void on_msg_part(const char *part, size_t size, void *data)
    {
        ((ErrBhv*)data)->msg_part(part, size);
    }
    static void on_msg_end(void *data)
    {
        ((ErrBhv*)data)->msg_end();
    }
};

template<class ErrBhv>
struct ErrorCallbacksBridge
{
    ErrorCallbacks callbacks() const
    {
        C4_SUPPRESS_WARNING_GCC_CLANG_WITH_PUSH("-Wcast-qual")
        return {
            (ErrBhv*)this,
            ErrorCallbacksBridge<ErrBhv>::on_err,
            ErrorCallbacksBridge<ErrBhv>::on_warn,
            (pfn_msg_begin)nullptr,
            (pfn_msg_part)nullptr,
            (pfn_msg_end)nullptr
        };
        C4_SUPPRESS_WARNING_GCC_CLANG_POP
    }
    static void on_err(locref loc, void *data)
    {
        ((ErrBhv*)data)->err(loc);
    }
    static void on_warn(locref loc, void *data)
    {
        ((ErrBhv*)data)->warn(loc);
    }
};


void fputi(int val, FILE *f);

void _errmsg(locref loc)
{
    fputs(loc.file, stderr);
    fputc(':', stderr);
    fputi(loc.line, stderr);
    fputs(": ", stderr);
    fflush(stderr);
}

void _errmsg(const char *part, size_t part_size)
{
    fwrite(part, 1u, part_size, stderr);
    fflush(stderr);
}

/** example implementation using old-style abort */
struct ErrorBehaviorAbort : public ErrorCallbacksBridgeFull<ErrorBehaviorAbort>
{
    static void msg_begin(locref loc)
    {
        fputc('\n', stderr);
        _errmsg(loc);
    }
    static void msg_part(const char *part, size_t part_size)
    {
        _errmsg(part, part_size);
    }
    static void msg_end()
    {
        fputc('\n', stderr);
        fflush(stderr);
    }
    static void err(locref)
    {
        abort();
    }
    static void warn(locref)
    {
        // nothing to do
    }
};


TEST_CASE("ErrorBehaviorAbort.default_obj")
{
    ErrorBehaviorAbort bhv;
    auto cb = bhv.callbacks();
    CHECK_NE(cb.user_data, nullptr);
    CHECK_NE(cb.err, nullptr);
    CHECK_NE(cb.warn, nullptr);
    CHECK_NE(cb.msg_begin, nullptr);
    CHECK_NE(cb.msg_part, nullptr);
    CHECK_NE(cb.msg_end, nullptr);
}


void fputi(int val, FILE *f);
void _append(std::string *s, int line);

// there is a false-positive use-of-uninitialized-value in the clang
// memory sanitizer. It finds that some of the bytes between
// std::string.size() and std::string.capacity() are not initialized,
// because it presumably scans all of those bytes. So we set them
// here, ensuring that strlen() is not called:
void _sanitizestr(std::string *s)
{
    // ensure that there are bytes to be set to 0:
    if(s->capacity() == s->size())
        s->reserve(s->capacity() + 1);
    // now set them!
    memset(&(*s)[0] + s->size(), 0, s->capacity() - s->size());
}
std::string _sanitize_for_strlen(const char *s)
{
    // do not call strlen! clang's msan will fail!
    size_t len = 0;
    for( ; s[len] != '\0'; ++len)
        ;
    std::string str(s, len);
    // sanitize this as well:
    _sanitizestr(&str);
    return str;
}
struct MyRunTimeError : public std::exception
{
    std::string msg;
    MyRunTimeError(std::string const& s) : msg(s) { _sanitizestr(&msg); }
    const char *what() const noexcept override { return msg.c_str(); }
};


/** example implementation using vanilla c++ std::runtime_error (or setjmp when exceptions are disabled) */
struct ErrorBehaviorRuntimeError : public ErrorCallbacksBridgeFull<ErrorBehaviorRuntimeError>
{
    std::string exc_msg;
    ErrorBehaviorRuntimeError() : exc_msg() { _sanitizestr(&exc_msg); }
    void msg_begin(locref loc)
    {
        exc_msg.resize(strlen(loc.file) + 32);
        exc_msg.clear();
        exc_msg = loc.file;
        exc_msg += ":";
        _append(&exc_msg, loc.line);
        exc_msg += ": ";
        _sanitizestr(&exc_msg);
    }
    void msg_part(const char *part, size_t part_size)
    {
        _sanitizestr(&exc_msg);
        exc_msg.append(part, part_size);
    }
    void msg_end()
    {
        _sanitizestr(&exc_msg);
        //std::cerr << "\n" << exc_msg << "\n";
    }
    void err(locref)
    {
        _sanitizestr(&exc_msg);
        C4_IF_EXCEPTIONS_(throw MyRunTimeError(exc_msg), { s_jmp_errmsg = exc_msg; std::longjmp(s_jmp_buf, 1); });
    }
    void warn(locref)
    {
        // nothing to do
    }
};



TEST_CASE("ErrorBehaviorRuntimeError.default_obj")
{
    ErrorBehaviorRuntimeError bhv;
    auto cb = bhv.callbacks();
    CHECK_NE(cb.user_data, nullptr);
    CHECK_NE(cb.err, nullptr);
    CHECK_NE(cb.warn, nullptr);
    CHECK_NE(cb.msg_begin, nullptr);
    CHECK_NE(cb.msg_part, nullptr);
    CHECK_NE(cb.msg_end, nullptr);
}




ErrorBehaviorAbort s_err_abort = ErrorBehaviorAbort();
ErrorCallbacks s_err_callbacks = s_err_abort.callbacks();


void new_handle_error(locref loc, const char *msg, size_t msg_size)
{
    if(s_err_callbacks.msg_enabled())
    {
        s_err_callbacks.msg_begin(loc, s_err_callbacks.user_data);
        s_err_callbacks.msg("ERROR: ");
        s_err_callbacks.msg(msg, msg_size);
        s_err_callbacks.msg_end(s_err_callbacks.user_data);
    }
    s_err_callbacks.err(loc, s_err_callbacks.user_data);
}

void new_handle_warning(locref loc, const char *msg, size_t msg_size)
{
    if(s_err_callbacks.msg_enabled())
    {
        s_err_callbacks.msg_begin(loc, s_err_callbacks.user_data);
        s_err_callbacks.msg("WARNING: ");
        s_err_callbacks.msg(msg, msg_size);
        s_err_callbacks.msg_end(s_err_callbacks.user_data);
    }
    s_err_callbacks.warn(loc, s_err_callbacks.user_data);
}

template<size_t N>
C4_ALWAYS_INLINE void new_handle_error(locref loc, const char (&msg)[N])
{
    new_handle_error(loc, msg, N-1);
}

template<size_t N>
C4_ALWAYS_INLINE void new_handle_warning(locref loc, const char (&msg)[N])
{
    new_handle_warning(loc, msg, N-1);
}


#define C4_ERROR_NEW(msg) c4::new_handle_error(C4_SRCLOC(), msg)
#define C4_WARNING_NEW(msg) c4::new_handle_warning(C4_SRCLOC(), msg)

#define C4_ERROR_NEW_SZ(msg, msglen) c4::new_handle_error(C4_SRCLOC(), msg, msglen)
#define C4_WARNING_NEW_SZ(msg, msglen) c4::new_handle_warning(C4_SRCLOC(), msg, msglen)

} // namespace c4

#ifndef C4CORE_SINGLE_HEADER
#include <c4/substr.hpp>
#include <c4/charconv.hpp>
#endif

namespace c4 {

void fputi(int val, FILE *f)
{
    char buf[16];
    size_t ret = c4::itoa(buf, val);
    ret = ret < sizeof(buf) ? ret : sizeof(buf);
    fwrite(buf, 1u, ret, f);
}

// to avoid using std::to_string()
void _append(std::string *s, int line)
{
    auto sz = s->size();
    s->resize(sz + 16);
    substr rem = substr(&((*s)[0]) + sz, 16u);
    size_t ret = itoa(rem, line);
    s->resize(sz + ret);
    if(ret >= rem.len)
    {
        rem = substr(&((*s)[0]) + sz, ret);
        itoa(rem, line);
    }
}

} // namespace c4
template<class ErrorBehavior>
struct ScopedErrorBehavior
{
    c4::ErrorCallbacks m_prev;
    ErrorBehavior m_tmp;
    const char *m_name;
    ScopedErrorBehavior(const char* name) : m_prev(c4::s_err_callbacks), m_tmp(), m_name(name)
    {
        c4::s_err_callbacks = m_tmp.callbacks();
    }
    ~ScopedErrorBehavior()
    {
        c4::s_err_callbacks = m_prev;
    }
};
#define C4_TMP_ERR_BHV(bhv_ty) ScopedErrorBehavior<c4::bhv_ty>(#bhv_ty)

template<size_t N>
void test_error_exception(const char (&msg)[N])
{
    bool got_exc = false;
    auto check_msg_found = [&](const char* s_){
        std::string sanitized = c4::_sanitize_for_strlen(s_);
        c4::csubstr errmsg = c4::csubstr(sanitized.data(), sanitized.size());
        INFO("full message: '''" << errmsg << "'''");
        size_t pos = errmsg.find(msg);
        CHECK_NE(pos, c4::csubstr::npos);
        got_exc = (pos != c4::csubstr::npos);
    };
    INFO(msg);
    {
        auto tmp1 = C4_TMP_ERR_BHV(ErrorBehaviorAbort);
        {
            auto tmp2 = C4_TMP_ERR_BHV(ErrorBehaviorRuntimeError);
            {
                got_exc = false;
                C4_IF_EXCEPTIONS_(try, if(setjmp(s_jmp_buf) == 0))
                {
                    C4_ERROR_NEW(msg);
                }
                C4_IF_EXCEPTIONS_(catch(c4::MyRunTimeError const& e), else)
                {
                    const char* s = C4_IF_EXCEPTIONS_(e.what(), s_jmp_errmsg.c_str());
                    // check that the given message is found verbatim on the error message
                    check_msg_found(s);
                }
                CHECK(got_exc);
            }
            {
                got_exc = false;
                C4_IF_EXCEPTIONS_(try, if(setjmp(s_jmp_buf) == 0))
                {
                    C4_ERROR_NEW_SZ(msg, N-1);
                }
                C4_IF_EXCEPTIONS_(catch(c4::MyRunTimeError const& e), else)
                {
                    const char* s = C4_IF_EXCEPTIONS_(e.what(), s_jmp_errmsg.c_str());
                    // check that the given message is found verbatim on the error message
                    check_msg_found(s);
                }
                CHECK(got_exc);
            }
        }
    }
}

template<size_t N>
void test_warning_exception(const char (&msg)[N])
{
    auto tmp = C4_TMP_ERR_BHV(ErrorBehaviorRuntimeError);
    C4_WARNING_NEW(msg);
    auto const& wmsg = tmp.m_tmp.exc_msg;
    REQUIRE_FALSE(wmsg.empty());
    REQUIRE_GT(wmsg.size(), N);
    auto what = c4::to_csubstr(wmsg.c_str()).last(N-1);
    CHECK_EQ(what, msg);

    C4_WARNING_NEW_SZ(msg, N-1);
    REQUIRE_FALSE(wmsg.empty());
    REQUIRE_GT(wmsg.size(), N);
    what = c4::to_csubstr(wmsg.c_str()).last(N-1);
    CHECK_EQ(what, msg);
}

TEST_CASE("error.exception")
{
    test_error_exception("some error with some message");
    test_error_exception("some error with another message");
}

TEST_CASE("warning.exception")
{
    test_warning_exception("some warning");
    test_warning_exception("some other warning");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef C4CORE_SINGLE_HEADER
#include <c4/substr.hpp>
#include <c4/charconv.hpp>
#endif

namespace c4 {

template<class T>
void _err_send(T const& arg)
{
    char buf[C4_ERR_FMT_BUFFER_SIZE];
    size_t num = to_chars(buf, arg);
    num = num < C4_ERR_FMT_BUFFER_SIZE ? num : C4_ERR_FMT_BUFFER_SIZE;
    s_err_callbacks.msg_part(buf, num, s_err_callbacks.user_data);
}


size_t _find_fmt(const char *C4_RESTRICT fmt, size_t len)
{
    for(size_t i = 0; i < len; ++i)
    {
        if(fmt[i] != '{')
        {
            continue;
        }
        if(i + 1 == len)
        {
            break;
        }
        if(fmt[i+1] == '}')
        {
            return i;
        }
    }
    return (size_t)-1;
}

void _err_fmt(size_t fmt_size, const char *C4_RESTRICT fmt)
{
    s_err_callbacks.msg_part(fmt, fmt_size, s_err_callbacks.user_data);
}

template<class Arg, class ...Args>
void _err_fmt(size_t fmt_size, const char *C4_RESTRICT fmt, Arg const& C4_RESTRICT arg, Args const& C4_RESTRICT ...args)
{
    size_t pos = _find_fmt(fmt, fmt_size);
    if(pos == (size_t)-1)
    {
        s_err_callbacks.msg_part(fmt, fmt_size, s_err_callbacks.user_data);
        return;
    }
    s_err_callbacks.msg_part(fmt, pos, s_err_callbacks.user_data);
    _err_send(arg);
    pos += 2;
    _err_fmt(fmt_size - pos, fmt + pos, args...);
}

template<class ...Args>
void err_fmt(locref loc, size_t fmt_size, const char *fmt, Args const& C4_RESTRICT ...args)
{
    s_err_callbacks.msg_begin(loc, s_err_callbacks.user_data);
    s_err_callbacks.msg("ERROR: ");
    _err_fmt(fmt_size, fmt, args...);
    s_err_callbacks.msg_end(s_err_callbacks.user_data);
    s_err_callbacks.err(loc, s_err_callbacks.user_data);
}

template<class ...Args>
void warn_fmt(locref loc, size_t fmt_size, const char *fmt, Args const& C4_RESTRICT ...args)
{
    s_err_callbacks.msg_begin(loc, s_err_callbacks.user_data);
    s_err_callbacks.msg("WARNING: ");
    _err_fmt(fmt_size, fmt, args...);
    s_err_callbacks.msg_end(s_err_callbacks.user_data);
    s_err_callbacks.warn(loc, s_err_callbacks.user_data);
}

template<size_t N, class ...Args>
void err_fmt(locref loc, const char (&fmt)[N], Args const& C4_RESTRICT ...args)
{
    err_fmt(loc, N-1, fmt, args...);
}

template<size_t N, class ...Args>
void warn_fmt(locref loc, const char (&fmt)[N], Args const& C4_RESTRICT ...args)
{
    warn_fmt(loc, N-1, fmt, args...);
}
} // namespace c4


template<size_t N, size_t M, class... Args>
void test_error_fmt_exception(const char (&expected)[M], const char (&fmt)[N], Args const& ...args)
{
    bool got_exc = false;
    auto check_msg_found = [&](const char* s_){
        std::string sanitized = c4::_sanitize_for_strlen(s_);
        c4::csubstr errmsg = c4::csubstr(sanitized.data(), sanitized.size());
        INFO("full message: '''" << errmsg << "'''");
        size_t pos = errmsg.find(expected);
        CHECK_NE(pos, c4::csubstr::npos);
        got_exc = (pos != c4::csubstr::npos);
    };
    INFO("expected is: '" << expected << "'");
    {
        auto tmp1 = C4_TMP_ERR_BHV(ErrorBehaviorAbort);
        {
            auto tmp2 = C4_TMP_ERR_BHV(ErrorBehaviorRuntimeError);
            {
                got_exc = false;
                C4_IF_EXCEPTIONS_(try, if(setjmp(s_jmp_buf) == 0))
                {
                    c4::err_fmt(C4_SRCLOC(), fmt, args...);
                }
                C4_IF_EXCEPTIONS_(catch(c4::MyRunTimeError const& e), else)
                {
                    // check that the given message is found verbatim on the error message
                    check_msg_found(C4_IF_EXCEPTIONS_(e.what(), s_jmp_errmsg.c_str()));
                }
                CHECK(got_exc);
            }

            {
                got_exc = false;
                C4_IF_EXCEPTIONS_(try, if(setjmp(s_jmp_buf) == 0))
                {
                    c4 ::err_fmt(C4_SRCLOC(), N - 1, fmt, args...);
                }
                C4_IF_EXCEPTIONS_(catch(c4::MyRunTimeError const& e), else)
                {
                    // check that the given message is found verbatim on the error message
                    check_msg_found(C4_IF_EXCEPTIONS_(e.what(), s_jmp_errmsg.c_str()));
                }
                CHECK(got_exc);
            }
        }
    }
}

template<size_t N, size_t M, class... Args>
void test_warning_fmt_exception(const char (&expected)[M], const char (&fmt)[N], Args const& ...args)
{
    INFO(expected);

    auto tmp = C4_TMP_ERR_BHV(ErrorBehaviorRuntimeError);
    auto const& wmsg = tmp.m_tmp.exc_msg;
    c4 ::warn_fmt(C4_SRCLOC(), fmt, args...);
    REQUIRE_FALSE(wmsg.empty());
    REQUIRE_GT(wmsg.size(), M);
    auto what = c4::to_csubstr(wmsg.c_str()).last(M-1);
    CHECK_EQ(what, expected);

    c4 ::warn_fmt(C4_SRCLOC(), N - 1, fmt, args...);
    REQUIRE_FALSE(wmsg.empty());
    REQUIRE_GT(wmsg.size(), M);
    what = c4::to_csubstr(wmsg.c_str()).last(M-1);
    CHECK_EQ(what, expected);
}


TEST_CASE("error.fmt")
{
    test_error_fmt_exception("abc is 2 is it not?",
                             "{} is {} is it not?", "abc", 2);
    test_error_fmt_exception("abc is bbb is it not?",
                             "{} is {} is it not?", "abc", "bbb");
    test_error_fmt_exception("abc is {} is it not?",
                             "{} is {} is it not?", "abc");
    test_error_fmt_exception("abc is {} is it not?",
                             "{} is {} is it not?", "abc");
}

TEST_CASE("warning.fmt")
{
    test_warning_fmt_exception("abc is 2 is it not?",
                               "{} is {} is it not?", "abc", 2);
    test_warning_fmt_exception("abc is bbb is it not?",
                               "{} is {} is it not?", "abc", "bbb");
    test_warning_fmt_exception("abc is {} is it not?",
                               "{} is {} is it not?", "abc");
    test_warning_fmt_exception("abc is {} is it not?",
                               "{} is {} is it not?", "abc");
}

C4_SUPPRESS_WARNING_MSVC_POP


#include "c4/libtest/supprwarn_pop.hpp"
