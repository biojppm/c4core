c4_setup_testing()

c4_add_library(c4core-_libtest LIBRARY_TYPE STATIC
    SOURCES
        c4/test.hpp
        c4/libtest/test.cpp
        c4/libtest/archetypes.cpp
        c4/libtest/archetypes.hpp
        c4/libtest/supprwarn_push.hpp
        c4/libtest/supprwarn_pop.hpp
    LIBS c4core gtest gtest_main
    INC_DIRS ${CMAKE_CURRENT_LIST_DIR}
    FOLDER test
    )

function(c4core_test name)
    c4_add_executable(c4core-test-${name}
        SOURCES ${ARGN}
        INC_DIRS ${CMAKE_CURRENT_LIST_DIR}
        LIBS c4core-_libtest
        FOLDER test)
    c4_add_test(c4core-test-${name})
endfunction()

c4core_test(preprocessor     preprocessor.cpp)
c4core_test(type_name        type_name.cpp)
c4core_test(types            types.cpp)
c4core_test(szconv           szconv.cpp)
c4core_test(error            error.cpp)
c4core_test(error_exception  error_exception.cpp)
c4core_test(blob             blob.cpp)
c4core_test(memory_util      memory_util.cpp)
c4core_test(memory_resource  memory_resource.cpp)
c4core_test(allocator        allocator.cpp)
c4core_test(ctor_dtor        ctor_dtor.cpp)
c4core_test(chartraits       char_traits.cpp)
c4core_test(enum             enum.cpp)
c4core_test(bitmask          bitmask.cpp)
c4core_test(span             span.cpp)
c4core_test(substr           substr.cpp)
c4core_test(charconv         charconv.cpp)
c4core_test(format           format.cpp)
c4core_test(base64           base64.cpp)
c4core_test(std_string       std_string.cpp)
c4core_test(std_vector       std_vector.cpp)


c4_add_install_include_test(c4core "c4core::")
c4_add_install_link_test(c4core "c4core::" "
#include <c4/substr.hpp>
int main()
{
    c4::csubstr s = \"12345\";
    return s.len != decltype(s.len)(5);
}
")
