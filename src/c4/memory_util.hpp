#ifndef _C4_MEMORY_UTIL_HPP_
#define _C4_MEMORY_UTIL_HPP_

#include "c4/config.hpp"

#include <string.h>

/** @file memory_util.hpp Some memory utilities. */

C4_BEGIN_NAMESPACE(c4)

/** set the given memory to zero */
C4_ALWAYS_INLINE void mem_zero(void* mem, size_t num_bytes)
{
    memset(mem, 0, num_bytes);
}
/** set the given memory to zero */
template< class T >
C4_ALWAYS_INLINE void mem_zero(T* mem, size_t num_elms)
{
    memset(mem, 0, sizeof(T) * num_elms);
}
/** set the given memory to zero */
template< class T >
C4_ALWAYS_INLINE void mem_zero(T* mem)
{
    memset(mem, 0, sizeof(T));
}

bool mem_overlaps(void const* a, void const* b, size_t sza, size_t szb);

void mem_repeat(void* dest, void const* pattern, size_t pattern_size, size_t num_times);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// most significant bit

/** most significant bit; this function is constexpr-14 because of the local
 * variable */
template< class I >
C4_CONSTEXPR14 I msb(I v)
{
    if(!v) return 0;
    I b = 0;
    while(v != 0)
    {
        v >>= 1;
        ++b;
    }
    return b;
}

namespace detail {

template< class I, I val, I num_bits, bool finished >
struct _msb11;

template< class I, I val, I num_bits >
struct _msb11< I, val, num_bits, false >
{
    enum : I { num = _msb11< I, (val>>1), num_bits+I(1), ((val>>1)==I(0)) >::num };
};

template< class I, I val, I num_bits >
struct _msb11< I, val, num_bits, true >
{
    static_assert(val == 0, "bad implementation");
    enum : I { num = num_bits };
};

} // namespace detail


/** TMP version of msb(); this needs to be implemented with template
 * meta-programming because C++11 cannot use a constexpr function with
 * local variables
 * @see msb */
template< class I, I number >
struct msb11
{
    enum : I { value = detail::_msb11< I, number, 0, (number==I(0)) >::num };
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** use Empty Base Class Optimization to reduce the size of a pair of
 * potentially empty types*/

C4_BEGIN_NAMESPACE(detail)
typedef enum { tpc_same_non_empty_type, tpc_same_empty_type, tpc_general } TightPairCase_e;
template< class First, class Second >
constexpr TightPairCase_e tpc_which_case()
{
    return (std::is_same< First, Second >::value) ?
        (std::is_empty< First >::value) ? tpc_same_empty_type : tpc_same_non_empty_type
        :
        tpc_general;
}

template< class First, class Second, TightPairCase_e Case >
struct tight_pair : public First, public Second
{
    using first_type = First;
    using second_type = Second;

    tight_pair() : First(), Second() {}
    tight_pair(First const& f, Second const& s) : First(f), Second(s) {}

    C4_ALWAYS_INLINE C4_CONSTEXPR14 First       & first ()       { return static_cast< First       & >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 First  const& first () const { return static_cast< First  const& >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 Second      & second()       { return static_cast< Second      & >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 Second const& second() const { return static_cast< Second const& >(*this); }
};

template< class First, class Second >
struct tight_pair< First, Second, tpc_same_empty_type > : public First
{
    static_assert(std::is_same<First,Second>::value, "bad implementation");

    using first_type = First;
    using second_type = Second;

    tight_pair() : First() {}
    tight_pair(First const& f, Second const& s) : First(f) {}

    C4_ALWAYS_INLINE C4_CONSTEXPR14 First      & first ()       { return static_cast< First      & >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 First const& first () const { return static_cast< First const& >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 First      & second()       { return static_cast< First      & >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 First const& second() const { return static_cast< First const& >(*this); }
};

template< class First, class Second >
struct tight_pair< First, Second, tpc_same_non_empty_type > : public First
{
    Second m_second;

    using first_type = First;
    using second_type = Second;

    tight_pair() : First() {}
    tight_pair(First const& f, Second const& s) : First(f), Second(s) {}

    C4_ALWAYS_INLINE C4_CONSTEXPR14 First      & first ()       { return static_cast< First      & >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 First const& first () const { return static_cast< First const& >(*this); }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 First      & second()       { return m_second; }
    C4_ALWAYS_INLINE C4_CONSTEXPR14 First const& second() const { return m_second; }
};

C4_END_NAMESPACE(detail)

template< class First, class Second >
using tight_pair = detail::tight_pair<First, Second, detail::tpc_which_case<First,Second>()>;

C4_END_NAMESPACE(c4)

#endif /* _C4_MEMORY_UTIL_HPP_ */
