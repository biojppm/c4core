#include "c4/allocator.hpp"
#include <vector>

C4_BEGIN_NAMESPACE(c4)

TEST(allocator, std_compatibility)
{
    using vec = std::vector< int, c4::Allocator<int> >;

    vec v(1024);
}

TEST(allocator, construct)
{

}

C4_END_NAMESPACE(c4)
