#ifndef _C4_STD_VECTOR_HPP_
#define _C4_STD_VECTOR_HPP_


#include "../substr.hpp"
#include "../to_str.hpp"

#include <vector>

namespace std {

template< class Alloc >
c4::substr to_substr(std::vector< char, Alloc > &vec)
{
    char *data = vec.empty() ? nullptr : vec.data(); // data() may or may not return a null pointer.
    return c4::substr(data, vec.size());
}

template< class Alloc >
c4::csubstr to_substr(std::vector< char, Alloc > const& vec)
{
    const char *data = vec.empty() ? nullptr : vec.data(); // data() may or may not return a null pointer.
    return c4::csubstr(data, vec.size());
}

template< class Alloc >
c4::csubstr to_substr(std::vector< const char, Alloc > const& vec)
{
    const char *data = vec.empty() ? nullptr : vec.data(); // data() may or may not return a null pointer.
    return c4::csubstr(data, vec.size());
}

template< class Alloc >
c4::csubstr to_csubstr(std::vector< char, Alloc > const& vec)
{
    const char *data = vec.empty() ? nullptr : vec.data(); // data() may or may not return a null pointer.
    return c4::csubstr(data, vec.size());
}

template< class Alloc >
c4::csubstr to_csubstr(std::vector< const char, Alloc > const& vec)
{
    const char *data = vec.empty() ? nullptr : vec.data(); // data() may or may not return a null pointer.
    return c4::csubstr(data, vec.size());
}

//-----------------------------------------------------------------------------

template< class Alloc > inline bool operator== (c4::csubstr ss, std::vector<char, Alloc> const & s) { return ss == to_csubstr(s); }
template< class Alloc > inline bool operator>= (c4::csubstr ss, std::vector<char, Alloc> const & s) { return ss >= to_csubstr(s); }
template< class Alloc > inline bool operator>  (c4::csubstr ss, std::vector<char, Alloc> const & s) { return ss >  to_csubstr(s); }
template< class Alloc > inline bool operator<= (c4::csubstr ss, std::vector<char, Alloc> const & s) { return ss <= to_csubstr(s); }
template< class Alloc > inline bool operator<  (c4::csubstr ss, std::vector<char, Alloc> const & s) { return ss <  to_csubstr(s); }

template< class Alloc > inline bool operator== (std::vector<char, Alloc> const & s, c4::csubstr ss) { return ss == to_csubstr(s); }
template< class Alloc > inline bool operator>= (std::vector<char, Alloc> const & s, c4::csubstr ss) { return ss <= to_csubstr(s); }
template< class Alloc > inline bool operator>  (std::vector<char, Alloc> const & s, c4::csubstr ss) { return ss <  to_csubstr(s); }
template< class Alloc > inline bool operator<= (std::vector<char, Alloc> const & s, c4::csubstr ss) { return ss >= to_csubstr(s); }
template< class Alloc > inline bool operator<  (std::vector<char, Alloc> const & s, c4::csubstr ss) { return ss >  to_csubstr(s); }

template< class Alloc > inline bool operator== (c4::csubstr ss, std::vector<const char, Alloc> const & s) { return ss == to_csubstr(s); }
template< class Alloc > inline bool operator>= (c4::csubstr ss, std::vector<const char, Alloc> const & s) { return ss >= to_csubstr(s); }
template< class Alloc > inline bool operator>  (c4::csubstr ss, std::vector<const char, Alloc> const & s) { return ss >  to_csubstr(s); }
template< class Alloc > inline bool operator<= (c4::csubstr ss, std::vector<const char, Alloc> const & s) { return ss <= to_csubstr(s); }
template< class Alloc > inline bool operator<  (c4::csubstr ss, std::vector<const char, Alloc> const & s) { return ss <  to_csubstr(s); }

template< class Alloc > inline bool operator== (std::vector<const char, Alloc> const & s, c4::csubstr ss) { return ss == to_csubstr(s); }
template< class Alloc > inline bool operator>= (std::vector<const char, Alloc> const & s, c4::csubstr ss) { return ss <= to_csubstr(s); }
template< class Alloc > inline bool operator>  (std::vector<const char, Alloc> const & s, c4::csubstr ss) { return ss <  to_csubstr(s); }
template< class Alloc > inline bool operator<= (std::vector<const char, Alloc> const & s, c4::csubstr ss) { return ss >= to_csubstr(s); }
template< class Alloc > inline bool operator<  (std::vector<const char, Alloc> const & s, c4::csubstr ss) { return ss >  to_csubstr(s); }

} // namespace std

#endif // _C4_STD_VECTOR_HPP_
