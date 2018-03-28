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
// least significant bit

/** least significant bit; this function is constexpr-14 because of the local
 * variable */
template< class I >
C4_CONSTEXPR14 I lsb(I v)
{
    if(!v) return 0;
    I b = 0;
    while(!(v & 1))
    {
        v >>= 1;
        ++b;
    }
    return b;
}

C4_BEGIN_NAMESPACE(detail)
template< size_t num_, size_t v_, bool bit1 > struct _lsb;
template< size_t num_, size_t v_ >
struct _lsb< num_, v_, true >
{
    enum : size_t { num = num_ };
};
template< size_t num_, size_t v_ >
struct _lsb< num_, v_, false >
{
    enum : size_t { num = _lsb< num_+1, (v_>>1), ((v_>>1)&1) >::num };
};
C4_END_NAMESPACE(detail)

/** TMP version of lsb(); this needs to be implemented with template
 * meta-programming because C++11 cannot use a constexpr function with
 * local variables
 * @see lsb */
template< class I, size_t v >
struct lsb11
{
    C4_STATIC_ASSERT(v != 0);
    enum : I { value = (I)detail::_lsb< 0, v, (v&1) >::num };
};

C4_END_NAMESPACE(c4)

#endif /* _C4_MEMORY_UTIL_HPP_ */
