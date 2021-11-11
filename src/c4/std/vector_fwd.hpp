#ifndef _C4_STD_VECTOR_FWD_HPP_
#define _C4_STD_VECTOR_FWD_HPP_

/** @file vector_fwd.hpp */

namespace std {
template<typename>
class allocator;

template<typename T, typename Alloc>
class vector;
} // namespace std

#ifndef C4CORE_SINGLE_HEADER
#include "c4/substr_fwd.hpp"
#endif

namespace c4 {

template<class Alloc>
c4::substr to_substr(std::vector<char, Alloc> &vec);

template<class Alloc>
c4::csubstr to_csubstr(std::vector<char, Alloc> const& vec);

template<class Alloc>
c4::csubstr to_csubstr(std::vector<const char, Alloc> const& vec);

template<class Alloc> bool operator== (c4::csubstr ss, std::vector<char, Alloc> const& s);
template<class Alloc> bool operator>= (c4::csubstr ss, std::vector<char, Alloc> const& s);
template<class Alloc> bool operator>  (c4::csubstr ss, std::vector<char, Alloc> const& s);
template<class Alloc> bool operator<= (c4::csubstr ss, std::vector<char, Alloc> const& s);
template<class Alloc> bool operator<  (c4::csubstr ss, std::vector<char, Alloc> const& s);

template<class Alloc> bool operator== (std::vector<char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator>= (std::vector<char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator>  (std::vector<char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator<= (std::vector<char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator<  (std::vector<char, Alloc> const& s, c4::csubstr ss);

template<class Alloc> bool operator== (c4::csubstr ss, std::vector<const char, Alloc> const& s);
template<class Alloc> bool operator>= (c4::csubstr ss, std::vector<const char, Alloc> const& s);
template<class Alloc> bool operator>  (c4::csubstr ss, std::vector<const char, Alloc> const& s);
template<class Alloc> bool operator<= (c4::csubstr ss, std::vector<const char, Alloc> const& s);
template<class Alloc> bool operator<  (c4::csubstr ss, std::vector<const char, Alloc> const& s);

template<class Alloc> bool operator== (std::vector<const char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator>= (std::vector<const char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator>  (std::vector<const char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator<= (std::vector<const char, Alloc> const& s, c4::csubstr ss);
template<class Alloc> bool operator<  (std::vector<const char, Alloc> const& s, c4::csubstr ss);

} // namespace c4

#endif // _C4_STD_VECTOR_FWD_HPP_
