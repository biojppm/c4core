c4core_setup_testing()

set(testsrc
    c4/test.hpp
    c4/libtest/test.cpp
    c4/libtest/archetypes.cpp
    c4/libtest/archetypes.hpp
    c4/libtest/supprwarn_push.hpp
    c4/libtest/supprwarn_pop.hpp
    )
c4core_add_library(c4core-libtest
    SOURCES ${testsrc}
    LIBS c4core gtest
    INC_DIRS ${CMAKE_CURRENT_LIST_DIR}
    FOLDER c4core-test
    )

function(c4core_test name)
    c4core_add_executable(c4core-test-${name}
        SOURCES ${ARGN} main.cpp
        INC_DIRS ${CMAKE_CURRENT_LIST_DIR}
        LIBS c4core c4core-libtest gtest
        FOLDER c4core-test)
    c4core_add_test(c4core-test-${name} ON)
endfunction(c4core_test)

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


c4core_add_install_include_test(c4core "c4core::")
c4core_add_install_link_test(c4core "c4core::" "
#include <c4/substr.hpp>
int main()
{
    c4::csubstr s = \"12345\"\;
    return s.len == decltype(s.len)(5)\;
}
")
