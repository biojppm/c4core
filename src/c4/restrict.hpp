#ifndef _C4_RESTRICT_HPP_
#define _C4_RESTRICT_HPP_

/** @file restrict.hpp macros defining shorthand symbols for restricted
 * pointers and references
 *
 * @see unrestrict.hpp
 */

#ifdef _MSC_VER
#   ifndef __restrict__
#       define _c4_RESTRICT
#       define __restrict__ __restrict
#   endif
#endif

#define   $      * __restrict__ //!< a restricted pointer
#define  $$      & __restrict__ //!< a restricted reference
#define  c$ const* __restrict__ //!< a restricted pointer to const data
#define c$$ const& __restrict__ //!< a restricted reference to const data

#endif /* _C4_RESTRICT_HPP_ */
