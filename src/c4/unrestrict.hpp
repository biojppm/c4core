#ifdef _C4_RESTRICT_HPP_ // must match the include guard from restrict.hpp

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

#undef _C4_RESTRICT_HPP_

#endif /* _C4_RESTRICT_HPP_ */
