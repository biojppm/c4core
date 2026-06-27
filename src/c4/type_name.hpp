#ifndef C4_TYPENAME_HPP_
#define C4_TYPENAME_HPP_

/** @file type_name.hpp compile-time type name */

#include <stddef.h>
#include "c4/language.hpp"

/// @cond dev
// this is an abbreviated way of getting the type name,
// hence the terse and non-namespaced names
struct c4t_
{
    const char *str;
    size_t sz;
    template<size_t N> C4_ALWAYS_INLINE constexpr
    c4t_(const char (&s)[N]) noexcept : str(s), sz(N-1) {} // take off the \0
};
template<class T>
#if !defined(__GNUC__) || (__GNUC__ >= 5) || (C4_CPP >= 14)
constexpr
#endif
C4_ALWAYS_INLINE c4t_ c4tn_()
{
    return c4t_(C4_PRETTY_FUNC);
}
/// @endcond


namespace c4 {

/** A non-zero terminated typename string. We're not using csubstr or
 * span to spare the heavy include. */
struct C4CORE_EXPORT TypeNameStr
{
    const char* str;
    size_t len;
};

/** compile-time type name
 * @see http://stackoverflow.com/a/20170989/5875572 */
template<class T>
C4_CONSTEXPR14 TypeNameStr type_name() noexcept
{
    const c4t_ p(c4tn_<T>());

#if (0) // enable this to debug and find the offsets
    for(size_t index = 0; index < p.sz; ++index)
        printf(" %2c", p.str[index]);
    printf("\n");
    for(size_t index = 0; index < p.sz; ++index)
        printf(" %2zu", index);
    printf("\n");
    for(size_t index = 0; index < p.sz; ++index)
        printf(" %2zu", p.sz - index);
    printf("\n");
#endif

#if defined(_MSC_VER)
#   if defined(__clang__) // Visual Studio has the clang toolset
#       if (_MSC_VER >= 1930) // do not use this: defined(C4_MSVC_2022)
    // ..............................xxx.
    // c4t_ __cdecl c4tn_(void) [T = int]
    enum : size_t { tstart = 30, tend = 1};
#       else
    // example:
    // ..........................xxx.
    // c4t_ __cdecl c4tn_() [T = int]
    enum : size_t { tstart = 26, tend = 1};
#       endif
#   elif (_MSC_VER >= 1900)
    // Note: subtract 7 at the end because the function terminates with ">(void)" in VS2015+
    size_t tstart = 26, tend = 7;

    const char *C4_RESTRICT s = p.str + tstart; // look at the start
    size_t sz = p.sz - tstart;

    // we're not using strcmp() or memcmp() to spare the #include

    // does it start with 'class '?
    if(sz > 6 && s[0] == 'c' && s[1] == 'l' && s[2] == 'a' && s[3] == 's' && s[4] == 's' && s[5] == ' ')
    {
        tstart += 6;
    }
    // does it start with 'struct '?
    else if(sz > 7 && s[0] == 's' && s[1] == 't' && s[2] == 'r' && s[3] == 'u' && s[4] == 'c' && s[5] == 't' && s[6] == ' ')
    {
        tstart += 7;
    }

#   else
    #error unknown visual studio
#   endif

#elif defined(__ICC)
    // example:
    // ........................xxx.
    // "c4t_ c4tn_() [with T = int]"
    enum : size_t { tstart = 23, tend = 1};

#elif defined(__clang__)
    // example:
    // ...................xxx.
    // "c4t_ c4tn_() [T = int]"
    enum : size_t { tstart = 18, tend = 1};
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wuninitialized"

#elif defined(__GNUC__)
    #if (__GNUC__ >= 5) || (C4_CPP >= 14)
        // example:
        // ..................................xxx.
        // "constexpr _c4t _c4tn() [with T = int]"
        enum : size_t { tstart = 33, tend = 1 };
    #else
        // example:
        // ........................xxx.
        // "_c4t _c4tn() [with T = int]"
        enum : size_t { tstart = 23, tend = 1 };
    #endif
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wuninitialized"
#else
    #error not implemented
#endif

    TypeNameStr o{p.str + tstart, p.sz - tstart - tend};

#if defined(_MSC_VER)
#elif defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

    return o;
}

/** compile-time type name
 * @overload */
template<class T>
C4_CONSTEXPR14 C4_ALWAYS_INLINE TypeNameStr type_name(T const&) noexcept
{
    return type_name<T>();
}

} // namespace c4

#endif // C4_TYPENAME_HPP_
