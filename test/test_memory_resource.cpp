#ifndef C4CORE_SINGLE_HEADER
#include "c4/memory_resource.hpp"
#endif

#include "c4/libtest/supprwarn_push.hpp"

#include <limits>

#include "c4/test.hpp"

namespace c4 {

C4_SUPPRESS_WARNING_GCC_CLANG_WITH_PUSH("-Wold-style-cast")


TEST_CASE("get_memory_resource_malloc()")
{
    CHECK_NE(get_memory_resource_malloc(), nullptr);
}


//-----------------------------------------------------------------------------

void do_memreslinear_realloc_test(MemoryResourceLinear &mr)
{
    C4_ASSERT(mr.capacity() >= 128); // this is needed for the tests below

    char * mem = (char*) mr.allocate(32);
    CHECK_EQ(mem-(const char*)mr.mem(), 0);
    CHECK_EQ(mr.size(), 32);
    CHECK_EQ(mr.slack(), mr.capacity() - 32);

    mem = (char*) mr.reallocate(mem, 32, 16);
    CHECK_EQ(mem-(const char*)mr.mem(), 0);
    CHECK_EQ(mr.size(), 16);
    CHECK_EQ(mr.slack(), mr.capacity() - 16);

    mem = (char*) mr.reallocate(mem, 16, 64);
    CHECK_EQ(mem-(const char*)mr.mem(), 0);
    CHECK_EQ(mr.size(), 64);
    CHECK_EQ(mr.slack(), mr.capacity() - 64);

    mem = (char*) mr.reallocate(mem, 64, 32);
    CHECK_EQ(mem-(const char*)mr.mem(), 0);
    CHECK_EQ(mr.size(), 32);
    CHECK_EQ(mr.slack(), mr.capacity() - 32);


    char *mem2 = (char*) mr.allocate(32);
    CHECK_EQ(mem-(const char*)mr.mem(), 0);
    CHECK_EQ(mem2-(const char*)mr.mem(), 32);
    CHECK_EQ(mr.size(), 64);
    CHECK_EQ(mr.slack(), mr.capacity() - 64);

    mem = (char*) mr.reallocate(mem, 32, 16);
    CHECK_EQ(mem-(const char*)mr.mem(), 0);
    CHECK_EQ(mr.size(), 64);
    CHECK_EQ(mr.slack(), mr.capacity() - 64);
}

TEST_CASE("MemoryResourceLinear.reallocate")
{
    MemoryResourceLinear mr(128);
    do_memreslinear_realloc_test(mr);
}

TEST_CASE("MemoryResourceLinearArr.reallocate")
{
    MemoryResourceLinearArr<128> mr;
    do_memreslinear_realloc_test(mr);
}


//-----------------------------------------------------------------------------

TEST_CASE("MemoryResourceLinear.error_out_of_mem")
{
    {
        MemoryResourceLinear mr(8);
        mr.allocate(2);
    }

    {
        MemoryResourceLinear mr(8);
        C4_EXPECT_ERROR_OCCURS([&]{
            mr.allocate(9);
        });
    }
}

TEST_CASE("MemoryResourceLinearArr.error_out_of_mem")
{
    {
        MemoryResourceLinearArr<8> mr;
        mr.allocate(2);
    }

    {
        MemoryResourceLinearArr<8> mr;
        C4_EXPECT_ERROR_OCCURS([&]{
            mr.allocate(9);
        });
    }
}


//-----------------------------------------------------------------------------

TEST_CASE("ScopedMemoryResource.basic")
{
    auto *before = get_memory_resource();
    {
        MemoryResourceCounts mrc;
        CHECK_NE(get_memory_resource(), &mrc);
        {
            ScopedMemoryResource smr(&mrc);
            CHECK(get_memory_resource() == &mrc);
        }
        CHECK_NE(get_memory_resource(), &mrc);
    }
    CHECK_EQ(get_memory_resource(), before);
}

TEST_CASE("ScopedMemoryResourceCounts.basic")
{
    auto *before = get_memory_resource();
    {
        auto sac = ScopedMemoryResourceCounts{};
        CHECK_EQ(get_memory_resource(), &sac.mr);
    }
    CHECK_EQ(get_memory_resource(), before);
}

TEST_CASE("ScopedMemoryResourceCounts.counts")
{
    auto *before = get_memory_resource();
    C4_UNUSED(before);

    {
        auto checker = AllocationCountsChecker();
        auto *mr = &checker.mr;

        for(size_t sz : {16u, 32u, 64u, 128u})
        {
            void *mem = mr->allocate(sz);
            checker.check_all_delta(1, static_cast<ssize_t>(sz), static_cast<ssize_t>(sz));
            mr->deallocate(mem, sz);
            checker.reset();
        }
        checker.check_curr(0, 0);
        checker.check_total(4, 240);
        checker.check_max(1, 128);
    }

    {
        auto checker = AllocationCountsChecker();
        auto *mr = &checker.mr;

        std::pair<void *, size_t> mem[4] = {{0,16}, {0,32}, {0,64}, {0,128}};
        for(auto& m : mem)
        {
            m.first = mr->allocate(m.second);
            checker.check_curr_delta(1, static_cast<ssize_t>(m.second));
            checker.reset();
        }
        checker.check_curr(4, 240);
        checker.check_total(4, 240);
        checker.check_max(4, 240);
        for(auto& m : mem)
        {
            mr->deallocate(m.first, m.second);
        }
        checker.check_curr(0, 0);
        checker.check_total(4, 240);
        checker.check_max(4, 240);
    }

}

C4_SUPPRESS_WARNING_GCC_CLANG_POP

} // namespace c4

#include "c4/libtest/supprwarn_pop.hpp"
