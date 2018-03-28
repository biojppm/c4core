#ifndef _C4_UNRESTRICT_HPP_
#define _C4_UNRESTRICT_HPP_

/** @file unrestrict.hpp cleans up restrict macros  */

#ifdef _MSC_VER
#   ifdef _c4_RESTRICT
#       undef __restrict__
#   endif
#endif

#undef   $
#undef  $$
#undef  c$
#undef c$$

#endif /* _C4_UNRESTRICT_HPP_ */
