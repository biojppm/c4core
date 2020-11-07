#include "c4/language.hpp"

C4_BEGIN_NAMESPACE(c4)
C4_BEGIN_NAMESPACE(detail)

#ifndef __GNUC__
void use_char_pointer(char const volatile* v)
{
    C4_UNUSED(v);
}
#else
void foo() {} // to avoid empty file warning from the linker
#endif

C4_END_NAMESPACE(detail)
C4_END_NAMESPACE(c4)
