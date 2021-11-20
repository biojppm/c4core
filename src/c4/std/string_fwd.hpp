#ifndef _C4_STD_STRING_FWD_HPP_
#define _C4_STD_STRING_FWD_HPP_

/** @file string_fwd.hpp */

#ifndef DOXYGEN

#ifndef C4CORE_SINGLE_HEADER
#include "c4/substr_fwd.hpp"
#endif

#include <cstddef>

namespace std {
#if defined(_MSC_VER)
template<typename> struct char_traits;
template<typename> class allocator;
template<typename _CharT,
         typename _Traits,
         typename _Alloc>
class basic_string;
using string = basic_string<char, char_traits<char>, allocator<char>>;
#elif defined(__GLIBCXX__) || defined(__GLIBCPP__)
template<typename> class char_traits;
template<typename> class allocator;
namespace __cxx11 {
template<typename _CharT,
         typename _Traits,
         typename _Alloc>
class basic_string;
} // namespace __cxx11
using string = __cxx11::basic_string<char, char_traits<char>, allocator<char>>;
#else
#error "unknown standard library"
#endif
} /* namespace std */

namespace c4 {

c4::substr to_substr(std::string &s);
c4::csubstr to_csubstr(std::string const& s);

bool operator== (c4::csubstr ss, std::string const& s);
bool operator!= (c4::csubstr ss, std::string const& s);
bool operator>= (c4::csubstr ss, std::string const& s);
bool operator>  (c4::csubstr ss, std::string const& s);
bool operator<= (c4::csubstr ss, std::string const& s);
bool operator<  (c4::csubstr ss, std::string const& s);

bool operator== (std::string const& s, c4::csubstr ss);
bool operator!= (std::string const& s, c4::csubstr ss);
bool operator>= (std::string const& s, c4::csubstr ss);
bool operator>  (std::string const& s, c4::csubstr ss);
bool operator<= (std::string const& s, c4::csubstr ss);
bool operator<  (std::string const& s, c4::csubstr ss);

size_t to_chars(c4::substr buf, std::string const& s);
bool from_chars(c4::csubstr buf, std::string * s);

} // namespace c4

#endif // DOXYGEN
#endif // _C4_STD_STRING_FWD_HPP_
