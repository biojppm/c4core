#ifndef _C4_SUBSTR_FWD_HPP_
#define _C4_SUBSTR_FWD_HPP_

/** @file substr_fwd.hpp Forward declaration for substr.hpp */

namespace c4 {

template<class C>
struct basic_substring;

/** ConstantSUBSTRing: a non-owning read-only string view
 * @see to_csubstr() */
using csubstr = basic_substring<const char>;

/** SUBSTRing: a non-owning read-write string view
 * @see to_substr()
 * @see to_csubstr() */
using substr = basic_substring<char>;

} // namespace c4

#endif /* _C4_SUBSTR_FWD_HPP_ */
