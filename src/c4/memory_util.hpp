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
// most significant bit

/** most significant bit; this function is constexpr-14 because of the local
 * variable */
template< class I >
C4_CONSTEXPR14 I msb(I v)
{
    if(!v) return 0;
    I b = 0;
    while(v)
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

C4_END_NAMESPACE(c4)

#endif /* _C4_MEMORY_UTIL_HPP_ */
