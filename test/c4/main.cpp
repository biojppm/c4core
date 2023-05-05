#ifdef C4CORE_SINGLE_HEADER
#include "c4/c4core_all.hpp"
#else
#include <c4/language.hpp>
#endif
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#if !defined(C4_EXCEPTIONS)
#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#endif
#include <doctest/doctest.h>
