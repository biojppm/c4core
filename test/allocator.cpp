// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "c4/allocator.hpp"
#include "c4/test.hpp"

#include <vector>

#include <string>

#include <map>

template< class T > using stdc4vec = std::vector< T, c4::Allocator<T> >;

using stdc4string = std::basic_string< char, std::char_traits<char>, c4::Allocator<char> >;

template< class K, class V, class Compare=std::less<K> >
using stdc4map = std::map< K, V, Compare, c4::Allocator<std::pair<K,V>> >;

C4_BEGIN_NAMESPACE(c4)

TEST(allocator, std_containers)
{
    {
        stdc4vec< int > v(1024);
    }

    {
        stdc4vec< std::string > v(1024);
    }

    {
        stdc4string v("adskjhsdfkjdflkjsdfkjhsdfkjh");
    }

    {
        stdc4vec< stdc4string > v({"foo", "bar", "baz", "bat", "bax"});
    }

    {
        stdc4map< stdc4string, int > v({{"foo", 0}, {"bar", 1}, {"baz", 2}, {"bat", 3}});
    }
}

//-----------------------------------------------------------------------------
template< class T >
void test_traits_compat_construct(T const& val)
{
    using atraits = std::allocator_traits< c4::Allocator<T> >;

    c4::Allocator<T> a;

    T *mem = a.allocate(1);
    atraits::construct(a, mem, val);
    EXPECT_EQ(*mem, val);

    atraits::destroy(a, mem);
    a.deallocate(mem, 1);
}

TEST(allocator, traits_compat_construct)
{
    test_traits_compat_construct<int>(1);
}

C4_END_NAMESPACE(c4)
