#ifndef _C4_TO_STR_HPP_
#define _C4_TO_STR_HPP_

#include <stdio.h>
#include <inttypes.h>
#include <type_traits>
#include <utility>
#include <tuple>

#include "c4/substr.hpp"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4800) //'int': forcing value to bool 'true' or 'false' (performance warning)
#   pragma warning(disable: 4996) // snprintf/scanf: this function or variable may be unsafe
#endif

namespace c4 {

typedef enum {
    /** print the real number in floating point format (like %f) */
    FTOA_FLOAT,
    /** print the real number in scientific format (like %e) */
    FTOA_SCIENT,
    /** print the real number in flexible format (like %g) */
    FTOA_FLEX,
    /** print the real number in hexadecimal format (like %a) */
    FTOA_HEXA
} RealFormat_e;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Helper macros, undefined below

// append chars to the buffer, without overflow. Always counts.
#define _c4append(c) { if(pos < buf.len) { buf.str[pos++] = (c); } else { ++pos; } }
#define _c4appendrdx(i) { if(pos < buf.len) { buf.str[pos++] = (radix == 16 ? hexchars[i] : (char)(i) + '0'); } else { ++pos; } }

template< class T >
size_t itoa(substr buf, T v)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    size_t pos = 0;
    if(v < 0)
    {
        _c4append('-');
        v = -v;
        do {
            _c4append((v % 10) + '0');
            v /= 10;
        } while(v);
        if(buf.len > 0)
        {
            buf.reverse_range(1, pos <= buf.len ? pos : buf.len);
        }
    }
    else
    {
        do {
            _c4append((v % 10) + '0');
            v /= 10;
        } while(v);
        buf.reverse_range(0, pos <= buf.len ? pos : buf.len);
    }
    return pos;
}


template< class T >
size_t itoa(substr buf, T v, T radix)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    constexpr static const char hexchars[] = "0123456789abcdef";
    size_t pos = 0;

    // write the sign prefix
    if(v < 0)
    {
        v = -v;
        _c4append('-');
    }

    // write the radix prefix
    C4_ASSERT(radix == 2 || radix == 8 || radix == 10 || radix == 16);
    switch(radix)
    {
    case 2 : _c4append('0'); _c4append('b'); break;
    case 8 : _c4append('0');                 break;
    case 16: _c4append('0'); _c4append('x'); break;
    }

    // write the number
    size_t pfx = pos;
    do {
        _c4appendrdx(v % radix);
        v /= radix;
    } while(v);
    if(buf.len)
    {
        buf.reverse_range(pfx, pos <= buf.len ? pos : buf.len);
    }

    return pos;
}

//-----------------------------------------------------------------------------
template< class T >
size_t utoa(substr buf, T v)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    static_assert(std::is_unsigned<T>::value, "must be unsigned type");
    size_t pos = 0;
    do {
        _c4append((char)(v % 10) + '0');
        v /= 10;
    } while(v);
    buf.reverse_range(0, pos <= buf.len ? pos : buf.len);
    return pos;
}

template< class T >
size_t utoa(substr buf, T v, T radix)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    static_assert(std::is_unsigned<T>::value, "must be unsigned type");
    constexpr static const char hexchars[] = "0123456789abcdef";
    size_t pos = 0;

    // write the radix prefix
    C4_ASSERT(radix == 2 || radix == 8 || radix == 10 || radix == 16);
    switch(radix)
    {
    case 2 : _c4append('0'); _c4append('b'); break;
    case 8 : _c4append('0');                 break;
    case 16: _c4append('0'); _c4append('x'); break;
    }

    // write the number
    size_t pfx = pos;
    do {
        _c4appendrdx(v % radix);
        v /= radix;
    } while(v);
    if(buf.len)
    {
        buf.reverse_range(pfx, pos <= buf.len ? pos : buf.len);
    }

    return pos;
}

#undef _c4getrdxchar
#undef _c4append


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** @see atou */
template< class T >
inline bool atoi(csubstr str, T *v)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    C4_ASSERT(str.len > 0);
    C4_ASSERT(!str.begins_with(' '));
    C4_ASSERT(!str.ends_with(' '));
    T n = 0;
    if(str[0] != '-')
    {
        for(size_t i = 0; i < str.len; ++i)
        {
            char c = str.str[i];
            if(c < '0' || c > '9') return false;
            n = n*10 + (c-'0');
        }
    }
    else
    {
        for(size_t i = 1; i < str.len; ++i)
        {
            char c = str.str[i];
            if(c < '0' || c > '9') return false;
            n = n*10 + (c-'0');
        }
        n = -n;
    }
    *v = n;
    return true;
}

template< class T >
inline size_t atoi_trim(csubstr str, T *v)
{
    csubstr trimmed = str.first_int_span();
    if(trimmed.len == 0) return csubstr::npos;
    if(atoi(trimmed, v)) return trimmed.end() - str.begin();
    return csubstr::npos;
}

//-----------------------------------------------------------------------------
/** @see atou */
template< class T >
inline bool atou(csubstr str, T *v)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    C4_ASSERT(str.len > 0);
    C4_ASSERT(!str.begins_with(' '));
    C4_ASSERT(!str.ends_with(' '));
    C4_ASSERT_MSG(str.str[0] == '-', "must be positive");
    T n = 0;
    for(size_t i = 0; i < str.len; ++i)
    {
        char c = str.str[i];
        if(c < '0' || c > '9') return false;
        n = n*10 + (c-'0');
    }
    *v = n;
    return true;
}


template< class T >
inline size_t atou_trim(csubstr str, T *v)
{
    csubstr trimmed = str.first_uint_span();
    if(trimmed.len == 0) return csubstr::npos;
    if(atou(trimmed, v)) return trimmed.end() - str.begin();
    return csubstr::npos;
}


//-----------------------------------------------------------------------------

namespace detail {
/** @see http://www.exploringbinary.com/ for many good examples on float-str conversion */
template< size_t N >
void get_real_format_str(char (&fmt)[N], int precision, RealFormat_e formatting, const char* length_modifier="")
{
    char c;
    switch(formatting)
    {
    case FTOA_FLOAT: c = 'f'; break;
    case FTOA_SCIENT: c = 'e'; break;
    case FTOA_HEXA: c = 'a'; break;
    case FTOA_FLEX:
    default:
         c = 'g';
    }
    int iret;
    if(precision == -1)
    {
        iret = snprintf(fmt, sizeof(fmt), "%%%s%c", length_modifier, c);
    }
    else if(precision == 0)
    {
        iret = snprintf(fmt, sizeof(fmt), "%%.%s%c", length_modifier, c);
    }
    else
    {
        iret = snprintf(fmt, sizeof(fmt), "%%.%d%s%c", precision, length_modifier, c);
    }
    C4_ASSERT(iret >= 2 && size_t(iret) < sizeof(fmt));
}


/** we're depending on snprintf()/sscanf() for converting to/from
 * floating point numbers. Apparently, this increases the binary size
 * by a considerable amount. There are some lightweight printf
 * implementations:
 *
 * @see http://www.sparetimelabs.com/tinyprintf/tinyprintf.php (BSD)
 * @see https://github.com/weiss/c99-snprintf
 * @see https://github.com/nothings/stb/blob/master/stb_sprintf.h
 * */
template< class T >
size_t print_one(substr str, const char* full_fmt, T v)
{
#ifdef _MSC_VER
    /** use _snprintf() to prevent early termination of the output
     * for writing the null character at the last position
     * @see https://msdn.microsoft.com/en-us/library/2ts7cx93.aspx */
    int iret = _snprintf(str.str, str.len, full_fmt, v);
    if(iret < 0)
    {
        /* when buf.len is not enough, VS returns a negative value.
         * so call it again with a negative value for getting an
         * actual length of the string */
        iret = snprintf(nullptr, 0, full_fmt, v);
        C4_ASSERT(iret > 0);
    }
    size_t ret = (size_t) iret;
    return ret;
#else
    int iret = snprintf(str.str, str.len, full_fmt, v);
    C4_ASSERT(iret >= 0);
    size_t ret = (size_t) iret;
    if(ret >= str.len)
    {
        ++ret; /* snprintf() reserves the last character to write \0 */
    }
    return ret;
#endif
}

template< typename T >
inline size_t scan_one(csubstr str, const char *type_fmt, T *v)
{
    /* snscanf() is absolutely needed here as we must be sure that
     * str.len is strictly respected, because the span string is
     * generally not null-terminated.
     *
     * Alas, there is no snscanf().
     *
     * So we fake it by using a dynamic format with an explicit
     * field size set to the length of the given span.
     * This trick is taken from:
     * https://stackoverflow.com/a/18368910/5875572 */

    /* this is the actual format we'll use for scanning */
    char fmt[16];

    /* write the length into it. Eg "%12f".
     * Also, get the number of characters read from the string.
     * So the final format ends up as "%12f%n"*/
    int iret = snprintf(fmt, sizeof(fmt), "%%" "%zu" "%s" "%%n", str.len, type_fmt);
    /* no nasty surprises, please! */
    C4_ASSERT(iret >= 0 && size_t(iret) < sizeof(fmt));

    /* now we scan with confidence that the span length is respected */
    int num_chars;
    iret = sscanf(str.str, fmt, v, &num_chars);
    /* scanf returns the number of successful conversions */
    if(iret != 1) return csubstr::npos;
    C4_ASSERT(num_chars >= 0);
    return (size_t)(num_chars);
}

} // namespace detail

/**
 * For FTOA_FLEX, precision is the number of significand digits. Otherwise
 * precision is the number of decimals.
 */
inline size_t ftoa(substr str, float v, int precision=-1, RealFormat_e formatting=FTOA_FLEX)
{
    char fmt[16];
    detail::get_real_format_str(fmt, precision, formatting, /*length_modifier*/"");
    return detail::print_one(str, fmt, v);
}

/**
 * For FTOA_FLEX, precision is the number of significand digits. Otherwise
 * precision is the number of decimals.
 */
inline size_t dtoa(substr str, double v, int precision=-1, RealFormat_e formatting=FTOA_FLEX)
{
    char fmt[16];
    detail::get_real_format_str(fmt, precision, formatting, /*length_modifier*/"l");
    return detail::print_one(str, fmt, v);
}

inline size_t atof(csubstr str, float *v)
{
    return detail::scan_one(str, "g", v);
}

inline size_t atod(csubstr str, double *v)
{
    return detail::scan_one(str, "lg", v);
}

inline size_t atof_trim(csubstr str, float *v)
{
    csubstr trimmed = str.first_real_span();
    if(trimmed.len == 0) return csubstr::npos;
    if(atof(trimmed, v)) return trimmed.end() - str.begin();
    return csubstr::npos;
}

inline size_t atod_trim(csubstr str, double *v)
{
    csubstr trimmed = str.first_real_span();
    if(trimmed.len == 0) return csubstr::npos;
    if(atod(trimmed, v)) return trimmed.end() - str.begin();
    return csubstr::npos;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template< class T >
inline substr to_str_sub(substr buf, T const& v)
{
    size_t sz = to_str(buf, v);
    return buf.left_of(sz <= buf.len ? sz : buf.len);
}

#define _C4_DEFINE_TO_FROM_STR_TOA(ty, id)              \
                                                        \
inline size_t to_str(substr buf, ty v)                  \
{                                                       \
    return id##toa(buf, v);                             \
}                                                       \
                                                        \
inline bool from_str(csubstr buf, ty *v)                \
{                                                       \
    return ato##id(buf, v);                             \
}                                                       \
                                                        \
inline size_t from_str_trim(csubstr buf, ty *v)         \
{                                                       \
    return ato##id##_trim(buf, v);                      \
}

#ifdef _MSC_VER
#define _C4_DEFINE_TO_STR(ty, pri_fmt)                                  \
inline size_t to_str(substr buf, ty v)                                  \
{                                                                       \
    /** use _snprintf() to prevent early termination of the output      \
     * for writing the null character at the last position              \
     * @see https://msdn.microsoft.com/en-us/library/2ts7cx93.aspx */   \
    int iret = _snprintf(buf.str, buf.len, "%" pri_fmt, v);             \
    if(iret < 0)                                                        \
    {                                                                   \
        /* when buf.len is not enough, VS returns a negative value.     \
         * so call it again with a negative value for getting an        \
         * actual length of the string */                               \
        iret = snprintf(nullptr, 0, "%" pri_fmt, v);                    \
        C4_ASSERT(iret > 0);                                            \
    }                                                                   \
    size_t ret = (size_t) iret;                                         \
    return ret;                                                         \
}
#else
#define _C4_DEFINE_TO_STR(ty, pri_fmt)\
inline size_t to_str(substr buf, ty v)                                  \
{                                                                       \
    int iret = snprintf(buf.str, buf.len, "%" pri_fmt, v);              \
    C4_ASSERT(iret >= 0);                                               \
    size_t ret = (size_t) iret;                                         \
    if(ret >= buf.len)                                                  \
    {                                                                   \
        ++ret; /* snprintf() reserves the last character to write \0 */ \
    }                                                                   \
    return ret;                                                         \
}
#endif

/** this macro defines to_str()/from_str() pairs for intrinsic types. */
#define _C4_DEFINE_TO_FROM_STR(ty, pri_fmt, scn_fmt)                    \
                                                                        \
_C4_DEFINE_TO_STR(ty, pri_fmt)                                          \
                                                                        \
inline size_t from_str_trim(csubstr buf, ty *v)                         \
{                                                                       \
    /* snscanf() is absolutely needed here as we must be sure that      \
     * buf.len is strictly respected, because the span string is        \
     * generally not null-terminated.                                   \
     *                                                                  \
     * Alas, there is no snscanf().                                     \
     *                                                                  \
     * So we fake it by using a dynamic format with an explicit         \
     * field size set to the length of the given span.                  \
     * This trick is taken from:                                        \
     * https://stackoverflow.com/a/18368910/5875572 */                  \
                                                                        \
    /* this is the actual format we'll use for scanning */              \
    char fmt[12];                                                       \
    /* write the length into it. Eg "%12d" for an int (scn_fmt="d").    \
     * Also, get the number of characters read from the string.         \
     * So the final format ends up as "%12d%n"*/                        \
    int ret = snprintf(fmt, sizeof(fmt), "%%""%zu" scn_fmt "%%n", buf.len);\
    /* no nasty surprises, please! */                                   \
    C4_ASSERT(size_t(ret) < sizeof(fmt));                               \
    /* now we scan with confidence that the span length is respected */ \
    int num_chars;                                                      \
    ret = sscanf(buf.str, fmt, v, &num_chars);                          \
    /* scanf returns the number of successful conversions */            \
    if(ret != 1) return csubstr::npos;                                  \
    return (size_t)(num_chars);                                         \
}                                                                       \
                                                                        \
inline bool from_str(csubstr buf, ty *v)                                \
{                                                                       \
    size_t num = from_str_trim(buf, v);                                 \
    return (num != csubstr::npos);                                      \
}

_C4_DEFINE_TO_FROM_STR(void*   , "p"             , "p"             )
//_C4_DEFINE_TO_FROM_STR(double  , "lg"            , "lg"            )
//_C4_DEFINE_TO_FROM_STR(float   , "g"             , "g"             )
//_C4_DEFINE_TO_FROM_STR(char    , "c"             , "c"             )
//_C4_DEFINE_TO_FROM_STR(  int8_t, PRId8 /*"%hhd"*/, SCNd8 /*"%hhd"*/)
//_C4_DEFINE_TO_FROM_STR( uint8_t, PRIu8 /*"%hhu"*/, SCNu8 /*"%hhu"*/)
//_C4_DEFINE_TO_FROM_STR( int16_t, PRId16/*"%hd" */, SCNd16/*"%hd" */)
//_C4_DEFINE_TO_FROM_STR(uint16_t, PRIu16/*"%hu" */, SCNu16/*"%hu" */)
//_C4_DEFINE_TO_FROM_STR( int32_t, PRId32/*"%d"  */, SCNd32/*"%d"  */)
//_C4_DEFINE_TO_FROM_STR(uint32_t, PRIu32/*"%u"  */, SCNu32/*"%u"  */)
//_C4_DEFINE_TO_FROM_STR( int64_t, PRId64/*"%lld"*/, SCNd64/*"%lld"*/)
//_C4_DEFINE_TO_FROM_STR(uint64_t, PRIu64/*"%llu"*/, SCNu64/*"%llu"*/)
_C4_DEFINE_TO_FROM_STR_TOA(   float, f)
_C4_DEFINE_TO_FROM_STR_TOA(  double, d)
_C4_DEFINE_TO_FROM_STR_TOA(  int8_t, i)
_C4_DEFINE_TO_FROM_STR_TOA( int16_t, i)
_C4_DEFINE_TO_FROM_STR_TOA( int32_t, i)
_C4_DEFINE_TO_FROM_STR_TOA( int64_t, i)
_C4_DEFINE_TO_FROM_STR_TOA( uint8_t, u)
_C4_DEFINE_TO_FROM_STR_TOA(uint16_t, u)
_C4_DEFINE_TO_FROM_STR_TOA(uint32_t, u)
_C4_DEFINE_TO_FROM_STR_TOA(uint64_t, u)

#undef _C4_DEFINE_TO_FROM_STR
#undef _C4_DEFINE_TO_FROM_STR_TOA


//-----------------------------------------------------------------------------
inline size_t to_str(substr buf, bool v)
{
    int val = v;
    return to_str(buf, val);
}

inline bool from_str(csubstr buf, bool *v)
{
    int val;
    bool ret = from_str(buf, &val);
    *v = val != 0;
    return ret;
}

inline size_t from_str_trim(csubstr buf, bool *v)
{
    int val;
    size_t ret = from_str_trim(buf, &val);
    *v = val != 0;
    return ret;
}

//-----------------------------------------------------------------------------
inline size_t to_str(substr buf, char v)
{
    if(buf.len > 0) buf[0] = v;
    return 1;
}

inline bool from_str(csubstr buf, char *v)
{
    if(buf.len != 1) return false;
    *v = buf[0];
    return true;
}

inline size_t from_str_trim(csubstr buf, char *v)
{
    if(buf.len < 1) return csubstr::npos;
    *v = buf[0];
    return 1;
}

//-----------------------------------------------------------------------------
inline size_t to_str(substr buf, csubstr v)
{
    size_t len = buf.len < v.len ? buf.len : v.len;
    memcpy(buf.str, v.str, len);
    return v.len;
}

inline bool from_str(csubstr buf, csubstr *v)
{
    *v = buf;
    return true;
}

inline size_t from_str_trim(substr buf, csubstr *v)
{
    csubstr trimmed = buf.first_non_empty_span();
    if(trimmed.len == 0) return csubstr::npos;
    *v = trimmed;
    return trimmed.end() - buf.begin();
}

//-----------------------------------------------------------------------------
inline size_t to_str(substr buf, substr const& v)
{
    size_t len = buf.len < v.len ? buf.len : v.len;
    memcpy(buf.str, v.str, len);
    return v.len;
}

inline bool from_str(csubstr buf, substr *v)
{
    size_t len = buf.len > v->len ? v->len : buf.len;
    memcpy(v->str, buf.str, len);
    return buf.len <= v->len;
}

inline size_t from_str_trim(csubstr buf, substr *v)
{
    csubstr trimmed = buf.first_non_empty_span();
    if(trimmed.len == 0) return csubstr::npos;
    size_t len = trimmed.len > v->len ? v->len : trimmed.len;
    memcpy(v->str, trimmed.str, len);
    if(trimmed.len > v->len) return csubstr::npos;
    return trimmed.end() - buf.begin();
}

//-----------------------------------------------------------------------------

template< size_t N >
inline size_t to_str(substr buf, const char (&v)[N])
{
    csubstr sp(v);
    return to_str(buf, sp);
}

inline size_t to_str(substr buf, const char *v)
{
    return to_str(buf, to_csubstr(v));
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


namespace detail {
template< class T >
struct float_formatter
{
    T val;
    int precision;
    RealFormat_e fmt;
    float_formatter(T v, int prec=-1, RealFormat_e f=FTOA_FLOAT) : val(v), precision(prec), fmt(f)  {}
};

template< class T >
struct int_formatter
{
    T val;
    uint8_t radix;
    int_formatter(T val_, uint8_t radix_=10) : val(val_), radix(radix_) {}
};
} // namespace detail

template< class T >
struct fmt_wrapper;

// define format settings for intrinsic types.
// The references are needed for reading, in case a format_wrapper
// is used to read

template<> struct fmt_wrapper<   float > : public detail::float_formatter< float > { using detail::float_formatter< float >::float_formatter; };
template<> struct fmt_wrapper<   float&> : public detail::float_formatter< float&> { using detail::float_formatter< float&>::float_formatter; };
template<> struct fmt_wrapper<  double > : public detail::float_formatter<double > { using detail::float_formatter<double >::float_formatter; };
template<> struct fmt_wrapper<  double&> : public detail::float_formatter<double&> { using detail::float_formatter<double&>::float_formatter; };
template<> struct fmt_wrapper<  int8_t > : public detail::int_formatter<  int8_t > { using detail::int_formatter<  int8_t >::int_formatter; };
template<> struct fmt_wrapper<  int8_t&> : public detail::int_formatter<  int8_t&> { using detail::int_formatter<  int8_t&>::int_formatter; };
template<> struct fmt_wrapper< uint8_t > : public detail::int_formatter< uint8_t > { using detail::int_formatter< uint8_t >::int_formatter; };
template<> struct fmt_wrapper< uint8_t&> : public detail::int_formatter< uint8_t&> { using detail::int_formatter< uint8_t&>::int_formatter; };
template<> struct fmt_wrapper< int16_t > : public detail::int_formatter< int16_t > { using detail::int_formatter< int16_t >::int_formatter; };
template<> struct fmt_wrapper< int16_t&> : public detail::int_formatter< int16_t&> { using detail::int_formatter< int16_t&>::int_formatter; };
template<> struct fmt_wrapper<uint16_t > : public detail::int_formatter<uint16_t > { using detail::int_formatter<uint16_t >::int_formatter; };
template<> struct fmt_wrapper<uint16_t&> : public detail::int_formatter<uint16_t&> { using detail::int_formatter<uint16_t&>::int_formatter; };
template<> struct fmt_wrapper< int32_t > : public detail::int_formatter< int32_t > { using detail::int_formatter< int32_t >::int_formatter; };
template<> struct fmt_wrapper< int32_t&> : public detail::int_formatter< int32_t&> { using detail::int_formatter< int32_t&>::int_formatter; };
template<> struct fmt_wrapper<uint32_t > : public detail::int_formatter<uint32_t > { using detail::int_formatter<uint32_t >::int_formatter; };
template<> struct fmt_wrapper<uint32_t&> : public detail::int_formatter<uint32_t&> { using detail::int_formatter<uint32_t&>::int_formatter; };
template<> struct fmt_wrapper< int64_t > : public detail::int_formatter< int64_t > { using detail::int_formatter< int64_t >::int_formatter; };
template<> struct fmt_wrapper< int64_t&> : public detail::int_formatter< int64_t&> { using detail::int_formatter< int64_t&>::int_formatter; };
template<> struct fmt_wrapper<uint64_t > : public detail::int_formatter<uint64_t > { using detail::int_formatter<uint64_t >::int_formatter; };
template<> struct fmt_wrapper<uint64_t&> : public detail::int_formatter<uint64_t&> { using detail::int_formatter<uint64_t&>::int_formatter; };



/** mark a variable to be written in custom format */
template< class T, class... Args >
inline fmt_wrapper<T> fmt(T v, Args && ...args)
{
    return fmt_wrapper<T>(std::ref(v), std::forward<Args>(args)...);
}

inline size_t to_str(substr buf, fmt_wrapper<   float> fmt) { return ftoa(buf, fmt.val, fmt.precision, fmt.fmt); }
inline size_t to_str(substr buf, fmt_wrapper<  double> fmt) { return dtoa(buf, fmt.val, fmt.precision, fmt.fmt); }
inline size_t to_str(substr buf, fmt_wrapper<  int8_t> fmt) { return itoa(buf, fmt.val, (  int8_t)fmt.radix); }
inline size_t to_str(substr buf, fmt_wrapper< uint8_t> fmt) { return utoa(buf, fmt.val, ( uint8_t)fmt.radix); }
inline size_t to_str(substr buf, fmt_wrapper< int16_t> fmt) { return itoa(buf, fmt.val, ( int16_t)fmt.radix); }
inline size_t to_str(substr buf, fmt_wrapper<uint16_t> fmt) { return utoa(buf, fmt.val, (uint16_t)fmt.radix); }
inline size_t to_str(substr buf, fmt_wrapper< int32_t> fmt) { return itoa(buf, fmt.val, ( int32_t)fmt.radix); }
inline size_t to_str(substr buf, fmt_wrapper<uint32_t> fmt) { return utoa(buf, fmt.val, (uint32_t)fmt.radix); }
inline size_t to_str(substr buf, fmt_wrapper< int64_t> fmt) { return itoa(buf, fmt.val, ( int64_t)fmt.radix); }
inline size_t to_str(substr buf, fmt_wrapper<uint64_t> fmt) { return utoa(buf, fmt.val, (uint64_t)fmt.radix); }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** @todo add custom alignment */
template< class T >
struct binary_wrapper
{
    T& val;
    binary_wrapper(T& v) : val(v) {}
};

/** mark a variable to be written in binary format */
template< class T >
inline binary_wrapper<T> fmtbin(T &v)
{
    return binary_wrapper<T>(std::ref(v));
}

/** write a variable in binary format */
template< class T >
inline size_t to_str(substr buf, binary_wrapper<T> b)
{
    if(sizeof(T) > buf.len) return buf.len;
    memcpy(buf.str, &b.val, sizeof(T));
    return sizeof(T);
}

/** read a variable in binary format */
template< class T >
inline size_t from_str(csubstr buf, binary_wrapper<T> *b)
{
    if(sizeof(T) > buf.len) return csubstr::npos;
    memcpy(&b->val, buf.str, sizeof(T));
    return sizeof(T);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

inline size_t cat(substr /*buf*/)
{
    return 0;
}

/** serialize the arguments to the given span.
 * @return the number of characters written to the buffer. */
template< class Arg, class... Args >
size_t cat(substr buf, Arg const& a, Args const& ...more)
{
    size_t num = to_str(buf, a);
    buf  = buf.len >= num ? buf.sub(num) : substr{};
    num += cat(buf, more...);
    return num;
}

inline size_t uncat(csubstr /*buf*/)
{
    return 0;
}

/** deserialize the arguments from the given span.
 *
 * @return the number of characters read from the buffer. If a
 * conversion was not successful, return npos. */
template< class Arg, class... Args >
size_t uncat(csubstr buf, Arg & a, Args & ...more)
{
    size_t num = from_str_trim(buf, &a);
    if(num == csubstr::npos) return csubstr::npos;
    buf  = buf.len >= num ? buf.sub(num) : substr{};
    num += uncat(buf, more...);
    return num;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace detail {

template< class Sep >
inline size_t catsep_more(substr /*buf*/, Sep const& /*sep*/)
{
    return 0;
}

template< class Sep, class Arg, class... Args >
size_t catsep_more(substr buf, Sep const& sep, Arg const& a, Args const& ...more)
{
    size_t ret = to_str(buf, sep), num = ret;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = to_str(buf, a);
    num += ret;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = catsep_more(buf, sep, more...);
    num += ret;
    return num;
}

template< class Sep >
inline size_t uncatsep_more(csubstr /*buf*/, Sep & /*sep*/)
{
    return 0;
}

template< class Sep, class Arg, class... Args >
size_t uncatsep_more(csubstr buf, Sep & sep, Arg & a, Args & ...more)
{
    size_t ret = from_str_trim(buf, &sep), num = ret;
    if(ret == csubstr::npos) return csubstr::npos;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = from_str_trim(buf, &a);
    if(ret == csubstr::npos) return csubstr::npos;
    num += ret;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = uncatsep_more(buf, sep, more...);
    if(ret == csubstr::npos) return csubstr::npos;
    num += ret;
    return num;
}

} // namespace detail


template< class Sep, class Arg, class... Args >
size_t catsep(substr buf, Sep const& sep, Arg const& a, Args const& ...more)
{
    size_t num = to_str(buf, a);
    buf  = buf.len >= num ? buf.sub(num) : substr{};
    num += detail::catsep_more(buf, sep, more...);
    return num;
}

template< class Sep, class Arg, class... Args >
size_t uncatsep(csubstr buf, Sep & sep, Arg & a, Args & ...more)
{
    size_t ret = from_str_trim(buf, &a), num = ret;
    if(ret == csubstr::npos) return csubstr::npos;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = detail::uncatsep_more(buf, sep, more...);
    if(ret == csubstr::npos) return csubstr::npos;
    num += ret;
    return num;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

inline size_t format(substr buf, csubstr fmt)
{
    return to_str(buf, fmt);
}

template< class Arg, class... Args >
size_t format(substr buf, csubstr fmt, Arg const& a, Args const& ...more)
{
    auto pos = fmt.find("{}");
    if(pos != csubstr::npos)
    {
        size_t num = to_str(buf, fmt.sub(0, pos));
        size_t out = num;
        buf  = buf.len >= num ? buf.sub(num) : substr{};
        num  = to_str(buf, a);
        out += num;
        buf  = buf.len >= num ? buf.sub(num) : substr{};
        num  = format(buf, fmt.sub(pos + 2), more...);
        out += num;
        return out;
    }
    else
    {
        return format(buf, fmt);
    }
}

inline size_t unformat(csubstr buf, csubstr fmt)
{
    return 0;
}

template< class Arg, class... Args >
size_t unformat(csubstr buf, csubstr fmt, Arg & a, Args & ...more)
{
    auto pos = fmt.find("{}");
    if(pos != csubstr::npos)
    {
        size_t num = pos;
        size_t out = num;
        buf  = buf.len >= num ? buf.sub(num) : substr{};
        num  = from_str_trim(buf, &a);
        out += num;
        buf  = buf.len >= num ? buf.sub(num) : substr{};
        num  = unformat(buf, fmt.sub(pos + 2), more...);
        out += num;
        return out;
    }
    else
    {
        return unformat(buf, fmt);
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct append_t {};
constexpr const append_t append = {};

template< class CharOwningContainer, class... Args >
inline void catrs(CharOwningContainer *cont, Args const& ...args)
{
    substr buf = to_substr(*cont);
    size_t ret = cat(buf, args...);
    cont->resize(ret);
    if(ret > buf.len)
    {
        buf = to_substr(*cont);
        ret = cat(buf, args...);
        if(ret != buf.len)
        {
            cont->resize(ret);
        }
    }
}

template< class CharOwningContainer, class... Args >
inline void catrs(append_t, CharOwningContainer *cont, Args const& ...args)
{
    size_t pos = cont->size();
    substr buf = to_substr(*cont).sub(pos);
    size_t ret = cat(buf, args...);
    cont->resize(pos + ret);
    if(ret > buf.len)
    {
        buf = to_substr(*cont).sub(pos);
        ret = cat(buf, args...);
        if(ret != buf.len)
        {
            cont->resize(pos + ret);
        }
    }
}

template< class CharOwningContainer, class Sep, class... Args >
inline void catseprs(CharOwningContainer *cont, Sep const& sep, Args const& ...args)
{
    substr buf = to_substr(*cont);
    size_t ret = catsep(buf, sep, args...);
    cont->resize(ret);
    if(ret > buf.len)
    {
        buf = to_substr(*cont);
        ret = catsep(buf, sep, args...);
        if(ret != buf.len)
        {
            cont->resize(ret);
        }
    }
}

template< class CharOwningContainer, class Sep, class... Args >
inline void catseprs(append_t, CharOwningContainer *cont, Sep const& sep, Args const& ...args)
{
    size_t pos = cont->size();
    substr buf = to_substr(*cont).sub(pos);
    size_t ret = catsep(buf, sep, args...);
    cont->resize(pos + ret);
    if(ret > buf.len)
    {
        buf = to_substr(*cont).sub(pos);
        ret = catsep(buf, sep, args...);
        if(ret != buf.len)
        {
            cont->resize(pos + ret);
        }
    }
}

template< class CharOwningContainer, class... Args >
inline void formatrs(CharOwningContainer *cont, csubstr fmt, Args const& ...args)
{
    substr buf = to_substr(*cont);
    size_t ret = format(buf, fmt, args...);
    cont->resize(ret);
    if(ret > buf.len)
    {
        buf = to_substr(*cont);
        ret = format(buf, fmt, args...);
        if(ret != buf.len)
        {
            cont->resize(ret);
        }
    }
}

template< class CharOwningContainer, class... Args >
inline void formatrs(append_t, CharOwningContainer *cont, csubstr fmt, Args const& ...args)
{
    size_t pos = cont->size();
    substr buf = to_substr(*cont).sub(pos);
    size_t ret = format(buf, fmt, args...);
    cont->resize(pos + ret);
    if(ret > buf.len)
    {
        buf = to_substr(*cont).sub(pos);
        ret = format(buf, fmt, args...);
        if(ret != buf.len)
        {
            cont->resize(pos + ret);
        }
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifdef C4_TUPLE_TO_STR
namespace detail {

template< size_t Curr, class... Types >
struct tuple_helper
{
    static size_t do_cat(substr buf, std::tuple< Types... > const& tp)
    {
        size_t num = to_str(buf, std::get<Curr>(tp));
        buf = buf.len >= num ? buf.sub(num) : substr{};
        num += tuple_helper< Curr+1, Types... >::do_cat(buf, tp);
        return num;
    }

    static size_t do_uncat(csubstr buf, std::tuple< Types... > & tp)
    {
        size_t num = from_str_trim(buf, &std::get<Curr>(tp));
        if(num == csubstr::npos) return csubstr::npos;
        buf = buf.len >= num ? buf.sub(num) : substr{};
        num += tuple_helper< Curr+1, Types... >::do_uncat(buf, tp);
        return num;
    }

    template< class Sep >
    static size_t do_catsep_more(substr buf, Sep const& sep, std::tuple< Types... > const& tp)
    {
        size_t ret = to_str(buf, sep), num = ret;
        buf  = buf.len >= ret ? buf.sub(ret) : substr{};
        ret  = to_str(buf, std::get<Curr>(tp));
        num += ret;
        buf  = buf.len >= ret ? buf.sub(ret) : substr{};
        ret  = tuple_helper< Curr+1, Types... >::do_catsep_more(buf, sep, tp);
        num += ret;
        return num;
    }

    template< class Sep >
    static size_t do_uncatsep_more(csubstr buf, Sep & sep, std::tuple< Types... > & tp)
    {
        size_t ret = from_str_trim(buf, &sep), num = ret;
        if(ret == csubstr::npos) return csubstr::npos;
        buf  = buf.len >= ret ? buf.sub(ret) : substr{};
        ret  = from_str_trim(buf, &std::get<Curr>(tp));
        if(ret == csubstr::npos) return csubstr::npos;
        num += ret;
        buf  = buf.len >= ret ? buf.sub(ret) : substr{};
        ret  = tuple_helper< Curr+1, Types... >::do_uncatsep_more(buf, sep, tp);
        if(ret == csubstr::npos) return csubstr::npos;
        num += ret;
        return num;
    }

    static size_t do_format(substr buf, csubstr fmt, std::tuple< Types... > const& tp)
    {
        auto pos = fmt.find("{}");
        if(pos != csubstr::npos)
        {
            size_t num = to_str(buf, fmt.sub(0, pos));
            size_t out = num;
            buf  = buf.len >= num ? buf.sub(num) : substr{};
            num  = to_str(buf, std::get<Curr>(tp));
            out += num;
            buf  = buf.len >= num ? buf.sub(num) : substr{};
            num  = tuple_helper< Curr+1, Types... >::do_format(buf, fmt.sub(pos + 2), tp);
            out += num;
            return out;
        }
        else
        {
            return format(buf, fmt);
        }
    }

    static size_t do_unformat(csubstr buf, csubstr fmt, std::tuple< Types... > & tp)
    {
        auto pos = fmt.find("{}");
        if(pos != csubstr::npos)
        {
            size_t num = pos;
            size_t out = num;
            buf  = buf.len >= num ? buf.sub(num) : substr{};
            num  = from_str_trim(buf, &std::get<Curr>(tp));
            out += num;
            buf  = buf.len >= num ? buf.sub(num) : substr{};
            num  = tuple_helper< Curr+1, Types... >::do_unformat(buf, fmt.sub(pos + 2), tp);
            out += num;
            return out;
        }
        else
        {
            return tuple_helper< sizeof...(Types), Types... >::do_unformat(buf, fmt, tp);
        }
    }

};

/** @todo VS compilation fails for this class */
template< class... Types >
struct tuple_helper< sizeof...(Types), Types... >
{
    static size_t do_cat(substr /*buf*/, std::tuple<Types...> const& /*tp*/) { return 0; }
    static size_t do_uncat(csubstr /*buf*/, std::tuple<Types...> & /*tp*/) { return 0; }

    template< class Sep > static size_t do_catsep_more(substr /*buf*/, Sep const& /*sep*/, std::tuple<Types...> const& /*tp*/) { return 0; }
    template< class Sep > static size_t do_uncatsep_more(csubstr /*buf*/, Sep & /*sep*/, std::tuple<Types...> & /*tp*/) { return 0; }

    static size_t do_format(substr buf, csubstr fmt, std::tuple<Types...> const& /*tp*/)
    {
        return to_str(buf, fmt);
    }

    static size_t do_unformat(csubstr buf, csubstr fmt, std::tuple<Types...> const& /*tp*/)
    {
        return 0;
    }
};

} // namespace detail

template< class... Types >
inline size_t cat(substr buf, std::tuple< Types... > const& tp)
{
    return detail::tuple_helper< 0, Types... >::do_cat(buf, tp);
}

template< class... Types >
inline size_t uncat(csubstr buf, std::tuple< Types... > & tp)
{
    return detail::tuple_helper< 0, Types... >::do_uncat(buf, tp);
}

template< class Sep, class... Types >
inline size_t catsep(substr buf, Sep const& sep, std::tuple< Types... > const& tp)
{
    size_t num = to_str(buf, std::cref(std::get<0>(tp)));
    buf  = buf.len >= num ? buf.sub(num) : substr{};
    num += detail::tuple_helper< 1, Types... >::do_catsep_more(buf, sep, tp);
    return num;
}

template< class Sep, class... Types >
inline size_t uncatsep(csubstr buf, Sep & sep, std::tuple< Types... > & tp)
{
    size_t ret = from_str_trim(buf, &std::get<0>(tp)), num = ret;
    if(ret == csubstr::npos) return csubstr::npos;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = detail::tuple_helper< 1, Types... >::do_uncatsep_more(buf, sep, tp);
    if(ret == csubstr::npos) return csubstr::npos;
    num += ret;
    return num;
}

template< class... Types >
inline size_t format(substr buf, csubstr fmt, std::tuple< Types... > const& tp)
{
    return detail::tuple_helper< 0, Types... >::do_format(buf, fmt, tp);
}

template< class... Types >
inline size_t unformat(csubstr buf, csubstr fmt, std::tuple< Types... > & tp)
{
    return detail::tuple_helper< 0, Types... >::do_unformat(buf, fmt, tp);
}
#endif // C4_TUPLE_TO_STR

} // namespace c4

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

#endif /* _C4_TO_STR_HPP_ */
