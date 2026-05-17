#ifndef C4CORE_SINGLE_HEADER
#include "c4/alloc.hpp"
#endif

#include "c4/libtest/supprwarn_push.hpp"

#include <limits>

#include "c4/test.hpp"

namespace c4 {

C4_SUPPRESS_WARNING_GCC_CLANG_WITH_PUSH("-Wold-style-cast")

TEST_CASE("set_aalloc.basic")
{
    auto a = get_aalloc();
    set_aalloc(nullptr);
    CHECK_EQ(get_aalloc(), nullptr);
    set_aalloc(a);
    CHECK_EQ(get_aalloc(), a);
}

TEST_CASE("set_afree.basic")
{
    auto a = get_afree();
    set_afree(nullptr);
    CHECK_EQ(get_afree(), nullptr);
    set_afree(a);
    CHECK_EQ(get_afree(), a);
}

TEST_CASE("set_arealloc.basic")
{
    auto a = get_arealloc();
    set_arealloc(nullptr);
    CHECK_EQ(get_arealloc(), nullptr);
    set_arealloc(a);
    CHECK_EQ(get_arealloc(), a);
}

//-----------------------------------------------------------------------------
namespace detail {
void* aalloc_impl(size_t size, size_t alignment=alignof(max_align_t));
void* arealloc_impl(void *ptr, size_t oldsz, size_t newsz, size_t alignment=alignof(max_align_t));
void  afree_impl(void *ptr);
} // namespace detail

TEST_CASE("aalloc_impl.error_bad_align")
{
#if defined(C4_POSIX)
    #if !defined(C4_ASAN) && !defined(C4_LSAN) && !defined(C4_TSAN) && !defined(C4_UBSAN)
    C4_EXPECT_ERROR_OCCURS([&]{
        auto *mem = detail::aalloc_impl(64, 9); // allocating with a non-power of two value is invalid
        (void)mem;
    });
    #endif
#endif
}

TEST_CASE("aalloc_impl.error_out_of_mem")
{
#if defined(C4_POSIX)
    #if !defined(C4_ASAN) && !defined(C4_LSAN) && !defined(C4_TSAN) && !defined(C4_UBSAN)
    if(sizeof(size_t) != 8) return; // valgrind complains that size is -1
    C4_EXPECT_ERROR_OCCURS([&]{
        size_t sz = std::numeric_limits<size_t>::max() / 2u;
        void const* mem = detail::aalloc_impl(sz);
        (void)mem;
    });
    #endif
#endif
}

//-----------------------------------------------------------------------------

void do_test_realloc(arealloc_pfn fn)
{
#define _set(dim) for(uint8_t i = 0; i < dim; ++i) { mem[i] = static_cast<char>(i); }
#define _check(dim) for(uint8_t i = 0; i < dim; ++i) { CHECK_EQ(mem[i], i); }

    char *mem = (char*) aalloc(16, alignof(max_align_t));
    _set(16u);
    _check(16u);
    mem = (char*) fn(mem, 16, 20, alignof(max_align_t));
    _check(16u);
    mem = (char*) fn(mem, 8, 20, alignof(max_align_t));
    _check(8u);
    afree(mem);

#undef _set
#undef _check
}

TEST_CASE("realloc_impl.basic")
{
    do_test_realloc(&detail::arealloc_impl);
}

TEST_CASE("realloc.basic")
{
    do_test_realloc(&arealloc);
}


C4_SUPPRESS_WARNING_GCC_CLANG_POP

} // namespace c4

#include "c4/libtest/supprwarn_pop.hpp"
