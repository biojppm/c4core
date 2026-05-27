
set(substr_counter_failbuild 0)
function(c4core_add_failbuild_substr code)
    c4core_test_failbuild(substr ${substr_counter_failbuild} "#include <c4/substr.hpp>
#include <c4/std/std.hpp>
using namespace c4;
int main()
{
    ${code}
}
")
    math(EXPR substr_counter_failbuild "${substr_counter_failbuild}+1")
    set(substr_counter_failbuild ${substr_counter_failbuild} PARENT_SCOPE)
endfunction()

c4core_add_failbuild_substr("substr dst = \"const literal\";")
c4core_add_failbuild_substr("csubstr src; substr dst = src;")
c4core_add_failbuild_substr("const char buf[20]; substr bad(buf);")
c4core_add_failbuild_substr("const char buf[20]; substr bad = buf;")
c4core_add_failbuild_substr("char buf[20]; csubstr(buf).toupper(); ")
c4core_add_failbuild_substr("char buf[20]; csubstr(buf).tolower(); ")
c4core_add_failbuild_substr("char buf[20]; csubstr(buf).fill(0); ")
c4core_add_failbuild_substr("char buf[20]; csubstr(buf).copy_from(\"asdasd\"); ")
c4core_add_failbuild_substr("char buf[20]; csubstr(buf).reverse(); ")
c4core_add_failbuild_substr("char buf[20]; csubstr(buf).reverse_sub(0, 10); ")

c4core_add_failbuild_substr("const char* s = \"a\"; to_substr(s); ")
c4core_add_failbuild_substr("const char* s = \"a\"; substr ss; ss.assign(s); ")
c4core_add_failbuild_substr("const char* s = \"a\"; substr ss; ss = s; ")

c4core_add_failbuild_substr("const std::string s; to_substr(s); ")
c4core_add_failbuild_substr("const std::vector<char> s; to_substr(s); ")

c4core_add_failbuild_substr("int* s = nullptr; substr ss(s); ")
c4core_add_failbuild_substr("int* s = nullptr; substr ss; ss = s; ")
c4core_add_failbuild_substr("int* s = nullptr; substr ss; ss.assign(s); ")
c4core_add_failbuild_substr("int* s = nullptr; to_substr(s); ")

c4core_add_failbuild_substr("const int* s = nullptr; csubstr ss(s); ")
c4core_add_failbuild_substr("const int* s = nullptr; csubstr ss; ss = s; ")
c4core_add_failbuild_substr("const int* s = nullptr; csubstr ss; ss.assign(s); ")
c4core_add_failbuild_substr("const int* s = nullptr; to_csubstr(s); ")
