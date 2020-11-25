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


//-----------------------------------------------------------------------------
#include <c4/substr.hpp>
#include <c4/charconv.hpp>

namespace c4 {

#define C4_ERR_FMT_BUFFER_SIZE 256

using locref = c4::srcloc const& C4_RESTRICT;

using pfn_err_begin = void (*)(void *data);
using pfn_err_part = void (*)(const char* msg, size_t size, void *data);
using pfn_err_end = void (*)(void *data);

struct ErrorCallbacks
{
    void         *user_data;
    pfn_err_begin err_begin;
    pfn_err_part  err_msg_part;
    pfn_err_end   err_end;

    template<size_t N>
    void _err_send(const char (&s)[N])
    {
        err_msg_part(s, N-1, user_data);
    }
    void _err_send(char c)
    {
        err_msg_part(&c, 1, user_data);
    }
    template<class T>
    void _err_send(T const& arg)
    {
        char buf[C4_ERR_FMT_BUFFER_SIZE];
        size_t num = to_chars(buf, arg);
        num = num < C4_ERR_FMT_BUFFER_SIZE ? num : C4_ERR_FMT_BUFFER_SIZE;
        err_msg_part(buf, num, user_data);
    }
};

// implementation
void on_error_begin(void *data)
{
    C4_UNUSED(data);
    fputc('\n', stderr);
    fflush(stderr);
}
void on_error_message(const char *msg, size_t size, void *data)
{
    C4_UNUSED(data);
    fwrite(msg, 1u, size, stderr);
    fflush(stderr);
}
void on_error_end(void *data)
{
    C4_UNUSED(data);
    fflush(stderr);
    //std::abort();
}

ErrorCallbacks s_err_callbacks{
    nullptr,
    &on_error_begin,
    &on_error_message,
    &on_error_end,
};

#if defined(C4_ERROR_SHOWS_FILELINE)
void _err_fileline(locref loc, ErrorCallbacks *cb)
{
    constexpr const size_t buflen = 16; // should be enough to print line numbers
    char buf[buflen];
    size_t filelen = strlen(loc.file);
    size_t numchars = c4::itoa(buf, loc.line);
    numchars = numchars < buflen ? numchars : buflen;
    cb->err_msg_part(loc.file, filelen, cb->user_data); // file name
    cb->_err_send(':');
    cb->err_msg_part(buf, numchars, cb->user_data); // line number
    cb->_err_send(": ");
}
#else
void _err_fileline(locref, ErrorCallbacks *)
{
}
#endif

void _start_err(locref loc, const char *msg_type)
{
    s_err_callbacks.err_begin(s_err_callbacks.user_data);
    _err_fileline(loc, &s_err_callbacks);
    s_err_callbacks.err_msg_part(msg_type, strlen(msg_type),  s_err_callbacks.user_data);
    s_err_callbacks._err_send(": ");
}

void err_msg(locref loc, size_t msg_size, const char *msg)
{
    _start_err(loc, "ERROR");
    s_err_callbacks.err_msg_part(msg, msg_size, s_err_callbacks.user_data);
    s_err_callbacks._err_send('\n');
    s_err_callbacks.err_end(s_err_callbacks.user_data);
}
template<size_t N>
void err_msg(locref loc, const char (&msg)[N])
{
    err_msg(loc, N-1, msg);
}

void warn_msg(locref loc, size_t msg_size, const char *msg)
{
    _start_err(loc, "WARNING");
    s_err_callbacks.err_msg_part(msg, msg_size, s_err_callbacks.user_data);
    s_err_callbacks._err_send('\n');
    s_err_callbacks.err_end(s_err_callbacks.user_data);
}
template<size_t N>
void warn_msg(locref loc, const char (&msg)[N])
{
    warn_msg(loc, N-1, msg);
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
    s_err_callbacks.err_msg_part(fmt, fmt_size, s_err_callbacks.user_data);
}

template<class Arg, class ...Args>
void _err_fmt(size_t fmt_size, const char *C4_RESTRICT fmt, Arg const& C4_RESTRICT arg, Args const& C4_RESTRICT ...args)
{
    size_t pos = _find_fmt(fmt, fmt_size);
    if(pos == (size_t)-1)
    {
        s_err_callbacks.err_msg_part(fmt, fmt_size, s_err_callbacks.user_data);
        return;
    }
    s_err_callbacks.err_msg_part(fmt, pos, s_err_callbacks.user_data);
    s_err_callbacks._err_send(arg);
    pos += 2;
    _err_fmt(fmt_size - pos, fmt + pos, args...);
}

template<class ...Args>
void err_fmt(locref loc, size_t fmt_size, const char *fmt, Args const& C4_RESTRICT ...args)
{
    _start_err(loc, "ERROR");
    _err_fmt(fmt_size, fmt, args..., '\n');
    s_err_callbacks.err_end(s_err_callbacks.user_data);
}

template<size_t N, class ...Args>
void err_fmt(locref loc, const char (&fmt)[N], Args const& C4_RESTRICT ...args)
{
    err_fmt(loc, N-1, fmt, args...);
}

template<class ...Args>
void warn_fmt(locref loc, size_t fmt_size, const char *fmt, Args const& C4_RESTRICT ...args)
{
    _start_err(loc, "WARNING");
    _err_fmt(fmt_size, fmt, args..., '\n');
    s_err_callbacks.err_end(s_err_callbacks.user_data);
}

template<size_t N, class ...Args>
void warn_fmt(locref loc, const char (&fmt)[N], Args const& C4_RESTRICT ...args)
{
    warn_fmt(loc, N-1, fmt, args...);
}

} // namespace c4

TEST_CASE("error.simple")
{
    c4::err_msg(C4_SRCLOC(), "some error with a message");
    c4::err_msg(C4_SRCLOC(), "some warning with a message");
    c4::err_fmt(C4_SRCLOC(), "{} is {} is it not?");
    c4::err_fmt(C4_SRCLOC(), "{} is {} is it not?", "foo", 1234);
    c4::warn_fmt(C4_SRCLOC(), "{} is {} you moron!");
    c4::warn_fmt(C4_SRCLOC(), "{} is {} you moron!", "foo", 15678);
}


#include "c4/libtest/supprwarn_pop.hpp"
