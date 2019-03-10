#ifndef _C4_TO_STR_HPP_
#define _C4_TO_STR_HPP_

#include <stdio.h>
#include <inttypes.h>
#include <type_traits>
#include <utility>
#include <stdarg.h>

#include "c4/substr.hpp"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4800) //'int': forcing value to bool 'true' or 'false' (performance warning)
#   pragma warning(disable: 4996) // snprintf/scanf: this function or variable may be unsafe
#endif

namespace c4 {

/** @defgroup lowlevel_tofrom_string
 * Functions providing type-specific low-level conversion of values to and from string.
 */

/** @defgroup generic_tofrom_string
 * Lightweight generic wrappers for converting individual values to/from strings.
 * These functions generally just dispatch sto the proper low-level conversion function.
 *
 * These are the main functions:
 *
 * @code{.cpp}
 *
 * // Convert the given value, writing into the string.
 * // The resulting string will NOT be null-terminated.
 * // Return the number of characters needed.
 * // This function is safe to call when the string is too small -
 * // no writes will occur beyond the string's last character.
 * template <class T> size_t to_str(substr buf, T const& C4_RESTRICT val);
 *
 *
 * // Convert the given value to a string using to_str(), and
 * // return the resulting string, up to and including the last
 * // written character.
 * template <class T> substr to_str_sub(substr buf, T const& C4_RESTRICT val);
 *
 *
 * // read a value from the string, which must be
 * // trimmed to the value (ie, no leading/trailing whitespace).
 * // return true if the conversion succeeded
 * template <class T> bool from_str(csubstr buf, T * C4_RESTRICT val);
 *
 *
 * // read the first valid sequence of characters from the string
 * // and convert it using from_str().
 * // Return the number of characters read for converting.
 * template <class T> size_t from_str_trim(csubstr buf, T * C4_RESTRICT val);
 * @endcode
 */

/** @defgroup formatting_functions
 * Convert a sequence of values to/from a string.
 */


/** @ingroup lowlevel_tofrom_string */
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

#define _c4append(c) { if(pos < buf.len) { buf.str[pos++] = (c); } else { ++pos; } }
#define _c4appendrdx(i) { if(pos < buf.len) { buf.str[pos++] = (radix == 16 ? hexchars[i] : (char)(i) + '0'); } else { ++pos; } }


/** convert an integral signed decimal to a string.
 * The resulting string is NOT zero-terminated.
 * Writing stops at the buffer's end.
 * @return the number of characters needed for the result, even if the buffer size is insufficient
 * @ingroup lowlevel_tofrom_string */
template<class T>
size_t itoa(substr buf, T v)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    static_assert(std::is_signed<T>::value, "must be signed type");
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


/** convert an integral signed integer to a string, using a specific radix. The radix must be 2, 8, 10 or 16.
 * The resulting string is NOT zero-terminated.
 * Writing stops at the buffer's end.
 * @return the number of characters needed for the result, even if the buffer size is insufficient
 * @ingroup lowlevel_tofrom_string */
template<class T>
size_t itoa(substr buf, T v, T radix)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    static_assert(std::is_signed<T>::value, "must be signed type");
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

/** convert an integral unsigned decimal to a string.
 * The resulting string is NOT zero-terminated.
 * Writing stops at the buffer's end.
 * @return the number of characters needed for the result, even if the buffer size is insufficient
 * @ingroup lowlevel_tofrom_string */
template<class T>
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


/** convert an integral unsigned integer to a string, using a specific radix. The radix must be 2, 8, 10 or 16.
 * The resulting string is NOT zero-terminated.
 * Writing stops at the buffer's end.
 * @return the number of characters needed for the result, even if the buffer size is insufficient
 * @ingroup lowlevel_tofrom_string */
template<class T>
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

#undef _c4appendrdx
#undef _c4append


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


/** Convert a trimmed string to a signed integral value. The value can be
 * formatted as decimal, binary (prefix 0b), octal (prefix 0)
 * or hexadecimal (prefix 0x). Every character in the input string is read 
 * for the conversion; it must not contain any leading or trailing whitespace.
 * @return true if the conversion was successful.
 * @see atoi_trim() if the string is not trimmed to the value to read.
 * @ingroup lowlevel_tofrom_string
 */
template<class T>
bool atoi(csubstr str, T * C4_RESTRICT v)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    static_assert(std::is_signed<T>::value, "must be signed type");

    C4_ASSERT(str.len > 0);
    C4_ASSERT(str == str.first_int_span());

    T n = 0;
    T sign = 1;
    size_t start = 0;
    if(str[0] == '-')
    {
        ++start;
        sign = -1;
    }

    if(str.str[start] != '0')
    {
        for(size_t i = start; i < str.len; ++i)
        {
            char c = str.str[i];
            if(c < '0' || c > '9') return false;
            n = n*10 + (c-'0');
        }
    }
    else
    {
        if(str.len == start+1)
        {
            *v = 0; // because the first character is 0
            return true;
        }
        else if(str.str[start+1] == 'x' || str.str[start+1] == 'X') // hexadecimal
        {
            C4_ASSERT(str.len > 2);
            start += 2;
            for(size_t i = start; i < str.len; ++i)
            {
                char c = str.str[i];
                T cv;
                if(c >= '0' && c <= '9') cv = (T)(c-'0');
                else if(c >= 'a' && c <= 'f') cv = T(10) + (T)(c-'a');
                else if(c >= 'A' && c <= 'F') cv = T(10) + (T)(c-'A');
                else return false;
                n = n*16 + cv;
            }
        }
        else // octal
        {
            C4_ASSERT(str.len > 1);
            for(size_t i = start; i < str.len; ++i)
            {
                char c = str.str[i];
                if(c < '0' || c > '7') return false;
                n = n*8 + (c-'0');
            }
        }
    }
    *v = sign * n;
    return true;
}


/** Select the next range of characters in the string that can be parsed
 * as a signed integral value, and convert it using atoi(). Leading
 * whitespace (space, newline, tabs) is skipped.
 * @return the number of characters read for conversion, or csubstr::npos if the conversion failed
 * @see atoi() if the string is already trimmed to the value to read.
 * @see csubstr::first_int_span()
 * @ingroup lowlevel_tofrom_string
 */
template<class T>
inline size_t atoi_trim(csubstr str, T * C4_RESTRICT v)
{
    csubstr trimmed = str.first_int_span();
    if(trimmed.len == 0) return csubstr::npos;
    if(atoi(trimmed, v)) return trimmed.end() - str.begin();
    return csubstr::npos;
}


//-----------------------------------------------------------------------------

/** Convert a trimmed string to an unsigned integral value. The value can be
 * formatted as decimal, binary (prefix 0b), octal (prefix 0)
 * or hexadecimal (prefix 0x). Every character in the input string is read 
 * for the conversion; it must not contain any leading or trailing whitespace.
 * @return true if the conversion was successful.
 * @see atou_trim() if the string is not trimmed to the value to read.
 * @ingroup lowlevel_tofrom_string
 */
template<class T>
bool atou(csubstr str, T * C4_RESTRICT v)
{
    static_assert(std::is_integral<T>::value, "must be integral type");
    C4_ASSERT(str.len > 0);
    C4_ASSERT_MSG(str.str[0] != '-', "must be positive");
    C4_ASSERT(str == str.first_uint_span());

    T n = 0;

    if(str.str[0] != '0')
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
        if(str.len == 1)
        {
            *v = 0; // because the first character is 0
            return true;
        }
        else if(str.str[1] == 'x' || str.str[1] == 'X') // hexadecimal
        {
            C4_ASSERT(str.len > 2);
            for(size_t i = 2; i < str.len; ++i)
            {
                char c = str.str[i];
                T cv;
                if(c >= '0' && c <= '9') cv = (T)(c-'0');
                else if(c >= 'a' && c <= 'f') cv = T(10) + (T)(c-'a');
                else if(c >= 'A' && c <= 'F') cv = T(10) + (T)(c-'A');
                else return false;
                n = n*16 + cv;
            }
        }
        else // octal
        {
            C4_ASSERT(str.len > 1);
            for(size_t i = 1; i < str.len; ++i)
            {
                char c = str.str[i];
                if(c < '0' || c > '7') return false;
                n = n*8 + (c-'0');
            }
        }
    }
    *v = n;
    return true;
}


/** Select the next range of characters in the string that can be parsed
 * as an unsigned integral value, and convert it using atou(). Leading
 * whitespace (space, newline, tabs) is skipped.
 * @return the number of characters read for conversion, or csubstr::npos if the conversion faileds
 * @see atou() if the string is already trimmed to the value to read.
 * @see csubstr::first_uint_span()
 * @ingroup lowlevel_tofrom_string
 */
template<class T>
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
template<size_t N>
void get_real_format_str(char (& C4_RESTRICT fmt)[N], int precision, RealFormat_e formatting, const char* length_modifier="")
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
 */
template<class T>
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

/** scans a string using the given type format, while at the same time
 * allowing non-null-terminated strings AND guaranteeing that the given
 * string length is strictly respected, so that no buffer overflows
 * might occur. */
template<typename T>
inline size_t scan_one(csubstr str, const char *type_fmt, T *v)
{
    /* snscanf() is absolutely needed here as we must be sure that
     * str.len is strictly respected, because substr is
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


/** Convert a single precision real number to string.
 * The string will in general be NOT null-terminated.
 * For FTOA_FLEX, \p precision is the number of significand digits. Otherwise
 * \p precision is the number of decimals.
 * @ingroup lowlevel_tofrom_string
 */
inline size_t ftoa(substr str, float v, int precision=-1, RealFormat_e formatting=FTOA_FLEX)
{
    char fmt[16];
    detail::get_real_format_str(fmt, precision, formatting, /*length_modifier*/"");
    return detail::print_one(str, fmt, v);
}


/** Convert a double precision real number to string.
 * The string will in general be NOT null-terminated.
 * For FTOA_FLEX, \p precision is the number of significand digits. Otherwise
 * \p precision is the number of decimals.
 * @return the number of characters written.
 * @ingroup lowlevel_tofrom_string
 */
inline size_t dtoa(substr str, double v, int precision=-1, RealFormat_e formatting=FTOA_FLEX)
{
    char fmt[16];
    detail::get_real_format_str(fmt, precision, formatting, /*length_modifier*/"l");
    return detail::print_one(str, fmt, v);
}


/** Convert a string to a single precision real number.
 * The input string must be trimmed to the value.
 * @return true iff the conversion succeeded
 * @ingroup lowlevel_tofrom_string
 * @see atof_trim() if the string is not trimmed
 */
inline bool atof(csubstr str, float * C4_RESTRICT v)
{
    C4_ASSERT(str == str.first_real_span());
    size_t ret = detail::scan_one(str, "g", v);
    return ret != csubstr::npos;
}


/** Convert a string to a double precision real number.
 * The input string must be trimmed to the value.
 * @return true iff the conversion succeeded
 * @ingroup lowlevel_tofrom_string
 * @see atod_trim() if the string is not trimmed
 */
inline bool atod(csubstr str, double * C4_RESTRICT v)
{
    C4_ASSERT(str == str.first_real_span());
    size_t ret = detail::scan_one(str, "lg", v);
    return ret != csubstr::npos;
}


/** Convert a string to a single precision real number.
 * Leading whitespace is skipped until valid characters are found.
 * @return true iff the conversion succeeded
 * @ingroup lowlevel_tofrom_string
 */
inline size_t atof_trim(csubstr str, float * C4_RESTRICT v)
{
    csubstr trimmed = str.first_real_span();
    if(trimmed.len == 0) return csubstr::npos;
    if(atof(trimmed, v)) return trimmed.end() - str.begin();
    return csubstr::npos;
}


/** Convert a string to a double precision real number.
 * Leading whitespace is skipped until valid characters are found.
 * @return true iff the conversion succeeded
 * @ingroup lowlevel_tofrom_string
 */
inline size_t atod_trim(csubstr str, double * C4_RESTRICT v)
{
    csubstr trimmed = str.first_real_span();
    if(trimmed.len == 0) return csubstr::npos;
    if(atod(trimmed, v)) return trimmed.end() - str.begin();
    return csubstr::npos;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** @ingroup generic_tofrom_string */
template<class T>
inline substr to_str_sub(substr buf, T const& C4_RESTRICT v)
{
    size_t sz = to_str(buf, v);
    return buf.left_of(sz <= buf.len ? sz : buf.len);
}

//-----------------------------------------------------------------------------
#define _C4_DEFINE_TO_FROM_STR_TOA(ty, id)              \
                                                        \
/** @ingroup generic_tofrom_string */                   \
inline size_t to_str(substr buf, ty v)                  \
{                                                       \
    return id##toa(buf, v);                             \
}                                                       \
                                                        \
/** @ingroup generic_tofrom_string */                   \
inline bool from_str(csubstr buf, ty *C4_RESTRICT v)    \
{                                                       \
    return ato##id(buf, v);                             \
}                                                       \
                                                        \
/** @ingroup generic_tofrom_string */                   \
inline size_t from_str_trim(csubstr buf, ty *C4_RESTRICT v) \
{                                                       \
    return ato##id##_trim(buf, v);                      \
}

#ifdef _MSC_VER
#define _C4_DEFINE_TO_STR(ty, pri_fmt)                                  \
/** @ingroup generic_tofrom_string */                                   \
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
#define _C4_DEFINE_TO_STR(ty, pri_fmt)                                  \
/** @ingroup generic_tofrom_string */                                   \
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
/** @ingroup generic_tofrom_string */                                   \
inline size_t from_str_trim(csubstr buf, ty * C4_RESTRICT v)                         \
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
/** @ingroup generic_tofrom_string */                                   \
inline bool from_str(csubstr buf, ty * C4_RESTRICT v)                                \
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
// bool implementation

/** @ingroup generic_tofrom_string */
inline size_t to_str(substr buf, bool v)
{
    int val = v;
    return to_str(buf, val);
}

/** @ingroup generic_tofrom_string */
inline bool from_str(csubstr buf, bool * C4_RESTRICT v)
{
    int val = 0;
    bool ret = from_str(buf, &val);
    *v = (val != 0);
    return ret;
}

/** @ingroup generic_tofrom_string */
inline size_t from_str_trim(csubstr buf, bool * C4_RESTRICT v)
{
    int val = 0;
    size_t ret = from_str_trim(buf, &val);
    *v = (val != 0);
    return ret;
}


//-----------------------------------------------------------------------------
// single-char implementation

/** @ingroup generic_tofrom_string */
inline size_t to_str(substr buf, char v)
{
    if(buf.len > 0) buf[0] = v;
    return 1;
}

/** extract a single character from a substring
 * @note to extract a string instead and not just a single character, use the csubstr overload
 * @ingroup generic_tofrom_string */
inline bool from_str(csubstr buf, char * C4_RESTRICT v)
{
    if(buf.len != 1) return false;
    *v = buf[0];
    return true;
}

/** @ingroup generic_tofrom_string */
inline size_t from_str_trim(csubstr buf, char * C4_RESTRICT v)
{
    if(buf.len < 1) return csubstr::npos;
    *v = buf[0];
    return 1;
}


//-----------------------------------------------------------------------------
// csubstr implementation

/** @ingroup generic_tofrom_string */
inline size_t to_str(substr buf, csubstr v)
{
    C4_ASSERT(!buf.contains(v) && !v.contains(buf));
    size_t len = buf.len < v.len ? buf.len : v.len;
    memcpy(buf.str, v.str, len);
    return v.len;
}

/** @ingroup generic_tofrom_string */
inline bool from_str(csubstr buf, csubstr *C4_RESTRICT v)
{
    *v = buf;
    return true;
}

/** @ingroup generic_tofrom_string */
inline size_t from_str_trim(substr buf, csubstr * C4_RESTRICT v)
{
    csubstr trimmed = buf.first_non_empty_span();
    if(trimmed.len == 0) return csubstr::npos;
    *v = trimmed;
    return trimmed.end() - buf.begin();
}


//-----------------------------------------------------------------------------
// substr

/** @ingroup generic_tofrom_string */
inline size_t to_str(substr buf, substr v)
{
    C4_ASSERT(!buf.overlaps(v));
    size_t len = buf.len < v.len ? buf.len : v.len;
    memcpy(buf.str, v.str, len);
    return v.len;
}

/** @ingroup generic_tofrom_string */
inline bool from_str(csubstr buf, substr * C4_RESTRICT v)
{
    size_t len = buf.len > v->len ? v->len : buf.len;
    memcpy(v->str, buf.str, len);
    return buf.len <= v->len;
}

/** @ingroup generic_tofrom_string */
inline size_t from_str_trim(csubstr buf, substr * C4_RESTRICT v)
{
    csubstr trimmed = buf.first_non_empty_span();
    if(C4_UNLIKELY(trimmed.len == 0)) return csubstr::npos;
    size_t len = trimmed.len > v->len ? v->len : trimmed.len;
    memcpy(v->str, trimmed.str, len);
    if(C4_UNLIKELY(trimmed.len > v->len)) return csubstr::npos;
    return trimmed.end() - buf.begin();
}

//-----------------------------------------------------------------------------

/** @ingroup generic_tofrom_string */
template<size_t N>
inline size_t to_str(substr buf, const char (& C4_RESTRICT v)[N])
{
    csubstr sp(v);
    return to_str(buf, sp);
}

/** @ingroup generic_tofrom_string */
inline size_t to_str(substr buf, const char * C4_RESTRICT v)
{
    return to_str(buf, to_csubstr(v));
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// formatting floats and ints

namespace detail {
template<class T>
struct float_formatter
{
    T val;
    int precision;
    RealFormat_e fmt;
    float_formatter(T v, int prec=-1, RealFormat_e f=FTOA_FLOAT) : val(v), precision(prec), fmt(f)  {}
};

template<class T>
struct int_formatter
{
    T val;
    T radix;
    int_formatter(T val_, uint8_t radix_=10) : val(val_), radix(radix_) {}
};
} // namespace detail

/** @ingroup generic_tofrom_string */
template<class T>
struct fmt_wrapper;

// define format settings for intrinsic types.

template<> struct fmt_wrapper<   float> : public detail::float_formatter< float> { using detail::float_formatter< float>::float_formatter; };
template<> struct fmt_wrapper<  double> : public detail::float_formatter<double> { using detail::float_formatter<double>::float_formatter; };
template<> struct fmt_wrapper<  int8_t> : public detail::int_formatter<  int8_t> { using detail::int_formatter<  int8_t>::int_formatter; };
template<> struct fmt_wrapper< uint8_t> : public detail::int_formatter< uint8_t> { using detail::int_formatter< uint8_t>::int_formatter; };
template<> struct fmt_wrapper< int16_t> : public detail::int_formatter< int16_t> { using detail::int_formatter< int16_t>::int_formatter; };
template<> struct fmt_wrapper<uint16_t> : public detail::int_formatter<uint16_t> { using detail::int_formatter<uint16_t>::int_formatter; };
template<> struct fmt_wrapper< int32_t> : public detail::int_formatter< int32_t> { using detail::int_formatter< int32_t>::int_formatter; };
template<> struct fmt_wrapper<uint32_t> : public detail::int_formatter<uint32_t> { using detail::int_formatter<uint32_t>::int_formatter; };
template<> struct fmt_wrapper< int64_t> : public detail::int_formatter< int64_t> { using detail::int_formatter< int64_t>::int_formatter; };
template<> struct fmt_wrapper<uint64_t> : public detail::int_formatter<uint64_t> { using detail::int_formatter<uint64_t>::int_formatter; };

template<class T>
inline fmt_wrapper<T> fmthex(T v)
{
    return fmt_wrapper<T>(v, T(16));
}

template<class T>
inline fmt_wrapper<T> fmtoct(T v)
{
    return fmt_wrapper<T>(v, T(8));
}

template<class T>
inline fmt_wrapper<T> fmtbin(T v)
{
    return fmt_wrapper<T>(v, T(2));
}

/** mark a variable to be written in custom format
 * @ingroup generic_tofrom_string */
template<class T, class... Args>
inline fmt_wrapper<T> fmt(T v, Args && ...args)
{
    return fmt_wrapper<T>(v, std::forward<Args>(args)...);
}

inline size_t to_str(substr buf, fmt_wrapper<   float> fmt) { return ftoa(buf, fmt.val, fmt.precision, fmt.fmt); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper<  double> fmt) { return dtoa(buf, fmt.val, fmt.precision, fmt.fmt); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper<  int8_t> fmt) { return itoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper< uint8_t> fmt) { return utoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper< int16_t> fmt) { return itoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper<uint16_t> fmt) { return utoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper< int32_t> fmt) { return itoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper<uint32_t> fmt) { return utoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper< int64_t> fmt) { return itoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string
inline size_t to_str(substr buf, fmt_wrapper<uint64_t> fmt) { return utoa(buf, fmt.val, fmt.radix); } //!< @ingroup generic_tofrom_string


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// writing binary values

/** @todo add custom alignment
 * @ingroup generic_tofrom_string */
template<class T>
struct binary_wrapper
{
    T& val;
    binary_wrapper(T& v) : val(v) {}
};

/** mark a variable to be written in binary format
 * @ingroup generic_tofrom_string  */
template<class T>
inline binary_wrapper<T> bin(T &v)
{
    return binary_wrapper<T>(v);
}

/** write a variable in binary format
 * @ingroup generic_tofrom_string */
template<class T>
inline size_t to_str(substr buf, binary_wrapper<T> b)
{
    if(sizeof(T) <= buf.len) memcpy(buf.str, &b.val, sizeof(T));
    return sizeof(T);
}

/** read a variable in binary format
 * @ingroup generic_tofrom_string */
template<class T>
inline size_t from_str(csubstr buf, binary_wrapper<T> *b)
{
    if(sizeof(T) > buf.len) return csubstr::npos;
    memcpy(&b->val, buf.str, sizeof(T));
    return sizeof(T);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** terminates the variadic recursion
 * @ingroup formatting_functions */
inline size_t cat(substr /*buf*/)
{
    return 0;
}

/** serialize the arguments, concatenating them to the given fixed-size buffer.
 * The buffer size is strictly respected: no writes will occur beyond its end.
 * @return the number of characters needed to write all the arguments into the buffer.
 * @see catrs() if instead of a fixed-size buffer, a resizeable container is desired
 * @see uncat() for the inverse function
 * @see catsep() if a separator between each argument is to be used
 * @see format() if a format string is desired
 * @ingroup formatting_functions */
template<class Arg, class... Args>
size_t cat(substr buf, Arg const& C4_RESTRICT a, Args const& C4_RESTRICT ...more)
{
    size_t num = to_str(buf, a);
    buf  = buf.len >= num ? buf.sub(num) : substr{};
    num += cat(buf, more...);
    return num;
}

/** like cat but return a substr instead of a size */
template<class... Args>
substr cat_sub(substr buf, Args && ...args)
{
    size_t sz = cat(buf, std::forward<Args>(args)...);
    return {buf.str, sz <= buf.len ? sz : buf.len};
}

//-----------------------------------------------------------------------------

/** terminates the variadic recursion
 * @ingroup formatting_functions */
inline size_t uncat(csubstr /*buf*/)
{
    return 0;
}

/** deserialize the arguments from the given buffer.
 *
 * @return the number of characters read from the buffer, or csubstr::npos
 *   if a conversion was not successful.
 * @see cat(). uncat() is the inverse of cat().
 * @ingroup formatting_functions */
template<class Arg, class... Args>
size_t uncat(csubstr buf, Arg & C4_RESTRICT a, Args & C4_RESTRICT ...more)
{
    size_t out = from_str_trim(buf, &a);
    if(C4_UNLIKELY(out == csubstr::npos)) return csubstr::npos;
    buf  = buf.len >= out ? buf.sub(out) : substr{};
    size_t num = uncat(buf, more...);
    if(C4_UNLIKELY(num == csubstr::npos)) return csubstr::npos;
    return out + num;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace detail {

template<class Sep>
inline size_t catsep_more(substr /*buf*/, Sep const& C4_RESTRICT /*sep*/)
{
    return 0;
}

template<class Sep, class Arg, class... Args>
size_t catsep_more(substr buf, Sep const& C4_RESTRICT sep, Arg const& C4_RESTRICT a, Args const& C4_RESTRICT ...more)
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

template<class Sep>
inline size_t uncatsep_more(csubstr /*buf*/, Sep & /*sep*/)
{
    return 0;
}

template<class Sep, class Arg, class... Args>
size_t uncatsep_more(csubstr buf, Sep & C4_RESTRICT sep, Arg & C4_RESTRICT a, Args & C4_RESTRICT ...more)
{
    size_t ret = from_str_trim(buf, &sep), num = ret;
    if(C4_UNLIKELY(ret == csubstr::npos)) return csubstr::npos;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = from_str_trim(buf, &a);
    if(C4_UNLIKELY(ret == csubstr::npos)) return csubstr::npos;
    num += ret;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = uncatsep_more(buf, sep, more...);
    if(C4_UNLIKELY(ret == csubstr::npos)) return csubstr::npos;
    num += ret;
    return num;
}

} // namespace detail


/** serialize the arguments, concatenating them to the given fixed-size
 * buffer, using a separator between each argument.
 * The buffer size is strictly respected: no writes will occur beyond its end.
 * @return the number of characters needed to write all the arguments into the buffer.
 * @see catseprs() if instead of a fixed-size buffer, a resizeable container is desired
 * @see uncatsep() for the inverse function
 * @see cat() if no separator is needed
 * @see format() if a format string is desired
 * @ingroup formatting_functions */
template<class Sep, class Arg, class... Args>
size_t catsep(substr buf, Sep const& C4_RESTRICT sep, Arg const& C4_RESTRICT a, Args const& C4_RESTRICT ...more)
{
    size_t num = to_str(buf, a);
    buf  = buf.len >= num ? buf.sub(num) : substr{};
    num += detail::catsep_more(buf, sep, more...);
    return num;
}

/** like catsep but return a substr instead of a size */
template<class... Args>
substr catsep_sub(substr buf, Args && ...args)
{
    size_t sz = catsep(buf, std::forward<Args>(args)...);
    return {buf.str, sz <= buf.len ? sz : buf.len};
}

/** deserialize the arguments from the given buffer, using a separator.
 *
 * @return the number of characters read from the buffer, or csubstr::npos
 *   if a conversion was not successful
 * @see catsep(). uncatsep() is the inverse of catsep().
 * @ingroup formatting_functions */
template<class Sep, class Arg, class... Args>
size_t uncatsep(csubstr buf, Sep & C4_RESTRICT sep, Arg & C4_RESTRICT a, Args & C4_RESTRICT ...more)
{
    size_t ret = from_str_trim(buf, &a), num = ret;
    if(C4_UNLIKELY(ret == csubstr::npos)) return csubstr::npos;
    buf  = buf.len >= ret ? buf.sub(ret) : substr{};
    ret  = detail::uncatsep_more(buf, sep, more...);
    if(C4_UNLIKELY(ret == csubstr::npos)) return csubstr::npos;
    num += ret;
    return num;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** terminates the variadic recursion
 * @ingroup formatting_functions */
inline size_t format(substr buf, csubstr fmt)
{
    return to_str(buf, fmt);
}

/** using a format string, serialize the arguments into the given
 * fixed-size buffer.
 * The buffer size is strictly respected: no writes will occur beyond its end.
 * In the format string, each argument is marked with a compact
 * curly-bracket pair: {}. Arguments beyond the last curly bracket pair
 * are silently ignored. For example:
 * @code{.cpp}
 * c4::format(buf, "the {} drank {} {}", "partier", 5, "beers"); // the partier drank 5 beers
 * c4::format(buf, "the {} drank {} {}", "programmer", 6, "coffees"); // the programmer drank 6 coffees
 * @endcode
 * @return the number of characters needed to write into the buffer.
 * @see formatrs() if instead of a fixed-size buffer, a resizeable container is desired
 * @see unformat() for the inverse function
 * @see cat() if no format or separator is needed
 * @see catsep() if no format is needed, but a separator must be used
 * @ingroup formatting_functions */
template<class Arg, class... Args>
size_t format(substr buf, csubstr fmt, Arg const& C4_RESTRICT a, Args const& C4_RESTRICT ...more)
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

/** like format but return a substr instead of a size */
template<class... Args>
substr format_sub(substr buf, Args && ...args)
{
    size_t sz = c4::format(buf, std::forward<Args>(args)...);
    return {buf.str, sz <= buf.len ? sz : buf.len};
}


//-----------------------------------------------------------------------------

/** terminates the variadic recursion
 * @ingroup formatting_functions */
inline size_t unformat(csubstr buf, csubstr fmt)
{
    return 0;
}

/** using a format string, deserialize the arguments from the given
 * buffer.
 * @return the number of characters read from the buffer, or npos if a conversion failed.
 * @see format() this is the inverse function to format().
 * @ingroup formatting_functions */
template<class Arg, class... Args>
size_t unformat(csubstr buf, csubstr fmt, Arg & C4_RESTRICT a, Args & C4_RESTRICT ...more)
{
    auto pos = fmt.find("{}");
    if(pos != csubstr::npos)
    {
        size_t num = pos;
        size_t out = num;
        buf  = buf.len >= num ? buf.sub(num) : substr{};
        num  = from_str_trim(buf, &a);
        if(C4_UNLIKELY(num == csubstr::npos)) return csubstr::npos;
        out += num;
        buf  = buf.len >= num ? buf.sub(num) : substr{};
        num  = unformat(buf, fmt.sub(pos + 2), more...);
        if(C4_UNLIKELY(num == csubstr::npos)) return csubstr::npos;
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
/** C-style printing into a buffer
 * @ingroup formatting_functions */
size_t sprintf(substr buf, const char * C4_RESTRICT fmt, ...);
//size_t sscanf(csubstr buf, const char *fmt, ...);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** a tag type
 * @see catrs
 * @ingroup formatting_functions
 * */
struct append_t {};

/** a tag variable
 * @see catrs
 * @ingroup formatting_functions
 * */
constexpr const append_t append = {};

//-----------------------------------------------------------------------------

/** like cat(), but receives a container, and resizes it as needed to contain the result.
 * The container is overwritten. To append to it, use the append overload.
 * @see cat()
 * @ingroup formatting_functions */
template<class CharOwningContainer, class... Args>
inline void catrs(CharOwningContainer * C4_RESTRICT cont, Args const& C4_RESTRICT ...args)
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

/** like cat(), but receives a container, and appends to it instead of overwriting it. The
 * container is resized as needed to contain the result.
 * @return the region newly appended to the original container
 * @see cat()
 * @see catrs()
 * @ingroup formatting_functions */
template<class CharOwningContainer, class... Args>
inline csubstr catrs(append_t, CharOwningContainer * C4_RESTRICT cont, Args const& C4_RESTRICT ...args)
{
    const size_t pos = cont->size();
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
    return to_csubstr(*cont).range(pos, cont->size());
}

//-----------------------------------------------------------------------------

/** like catsep(), but receives a container, and resizes it as needed to contain the result.
 * The container is overwritten. To append to the container use the append overload.
 * @see catsep()
 * @ingroup formatting_functions */
template<class CharOwningContainer, class Sep, class... Args>
inline void catseprs(CharOwningContainer * C4_RESTRICT cont, Sep const& C4_RESTRICT sep, Args const& C4_RESTRICT ...args)
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

/** like catsep(), but receives a container, and appends the arguments, resizing the
 * container as needed to contain the result. The buffer is appended to.
 * @ingroup formatting_functions */
template<class CharOwningContainer, class Sep, class... Args>
inline csubstr catseprs(append_t, CharOwningContainer * C4_RESTRICT cont, Sep const& C4_RESTRICT sep, Args const& C4_RESTRICT ...args)
{
    const size_t pos = cont->size();
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
    return to_csubstr(*cont).range(pos, cont->size());
}

//-----------------------------------------------------------------------------

/** like format(), but receives a container, and resizes it as needed
 * to contain the result.  The container is overwritten. To append to
 * the container use the append overload.
 * @see format()
 * @ingroup formatting_functions */
template<class CharOwningContainer, class... Args>
inline void formatrs(CharOwningContainer * C4_RESTRICT cont, csubstr fmt, Args const&  C4_RESTRICT ...args)
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

/** like format(), but receives a container, and appends the
 * arguments, resizing the container as needed to contain the
 * result. The buffer is appended to.
 * @return the region newly appended to the original container
 * @ingroup formatting_functions */
template<class CharOwningContainer, class... Args>
inline csubstr formatrs(append_t, CharOwningContainer * C4_RESTRICT cont, csubstr fmt, Args const& C4_RESTRICT ...args)
{
    const size_t pos = cont->size();
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
    return to_csubstr(*cont).range(pos, cont->size());
}

} // namespace c4

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

#endif /* _C4_TO_STR_HPP_ */
