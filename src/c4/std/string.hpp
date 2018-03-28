#ifndef _C4_STD_STRING_HPP_
#define _C4_STD_STRING_HPP_

#include "../substr.hpp"
#include "../to_str.hpp"

#include <string>

namespace std {

//-----------------------------------------------------------------------------
// converting std::string to yml::substr / c4::csubstr
// This is not required, but is handy.

inline c4::substr to_substr(std::string &s)
{
    char* data = s.empty() ? nullptr : &s[0];
    return c4::substr(data, s.size());
}

inline c4::csubstr to_csubstr(std::string const& s)
{
    const char* data = s.empty() ? nullptr : &s[0];
    return c4::csubstr(data, s.size());
}

//-----------------------------------------------------------------------------

inline bool operator== (c4::csubstr ss, std::string const & s) { return ss == to_csubstr(s); }
inline bool operator>= (c4::csubstr ss, std::string const & s) { return ss >= to_csubstr(s); }
inline bool operator>  (c4::csubstr ss, std::string const & s) { return ss >  to_csubstr(s); }
inline bool operator<= (c4::csubstr ss, std::string const & s) { return ss <= to_csubstr(s); }
inline bool operator<  (c4::csubstr ss, std::string const & s) { return ss <  to_csubstr(s); }

inline bool operator== (std::string const & s, c4::csubstr ss) { return ss == to_csubstr(s); }
inline bool operator>= (std::string const & s, c4::csubstr ss) { return ss <= to_csubstr(s); }
inline bool operator>  (std::string const & s, c4::csubstr ss) { return ss <  to_csubstr(s); }
inline bool operator<= (std::string const & s, c4::csubstr ss) { return ss >= to_csubstr(s); }
inline bool operator<  (std::string const & s, c4::csubstr ss) { return ss >  to_csubstr(s); }

//-----------------------------------------------------------------------------

// std::string is a value-like type, and it will be a leaf node
// in the YAML data tree hierarchy (a VAL node in ryml parlance).
// So it should be serialized via to_str()/from_str().

inline size_t to_str(c4::substr buf, std::string const& s)
{
    return to_str(buf, to_csubstr(s));
}

inline bool from_str(c4::csubstr buf, std::string * s)
{
    s->resize(buf.len);
    c4::substr v = to_substr(*s);
    return from_str(buf, &v);
}

} // namespace std

#endif // _C4_STD_STRING_HPP_
