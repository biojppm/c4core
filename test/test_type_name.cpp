#include "c4/test.hpp"

#ifndef C4CORE_SINGLE_HEADER
#include "c4/type_name.hpp"
#include "c4/substr.hpp"
#endif

class  SomeTypeName {};
struct SomeStructName {};

namespace c4 {

class  SomeTypeNameInsideANamespace {};
struct SomeStructNameInsideANamespace {};

template<class T>
void test_type_name(csubstr expected)
{
    {
        TypeNameStr result = type_name<T>();
        csubstr actual{result.str, result.len};
        CHECK_EQ(actual, expected);
    }
    {
        T val{};
        TypeNameStr result = type_name(val);
        csubstr actual{result.str, result.len};
        CHECK_EQ(actual, expected);
    }
}

TEST_CASE("type_name.intrinsic_types")
{
    test_type_name<int>("int");
    test_type_name<float>("float");
    test_type_name<double>("double");
}
TEST_CASE("type_name.classes")
{
    test_type_name<SomeTypeName>("SomeTypeName");
    test_type_name<::SomeTypeName>("SomeTypeName");
}
TEST_CASE("type_name.structs")
{
    test_type_name<SomeStructName>("SomeStructName");
    test_type_name<::SomeStructName>("SomeStructName");
}
TEST_CASE("type_name.inside_namespace")
{
    test_type_name<SomeTypeNameInsideANamespace>("c4::SomeTypeNameInsideANamespace");
    test_type_name<c4::SomeTypeNameInsideANamespace>("c4::SomeTypeNameInsideANamespace");
    test_type_name<::c4::SomeTypeNameInsideANamespace>("c4::SomeTypeNameInsideANamespace");

    test_type_name<SomeStructNameInsideANamespace>("c4::SomeStructNameInsideANamespace");
    test_type_name<c4::SomeStructNameInsideANamespace>("c4::SomeStructNameInsideANamespace");
    test_type_name<::c4::SomeStructNameInsideANamespace>("c4::SomeStructNameInsideANamespace");
}

} // namespace c4
