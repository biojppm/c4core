// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "c4/memory_resource.hpp"

#include "c4/test.hpp"

C4_BEGIN_NAMESPACE(c4)

TEST(ScopedMemoryResource, basic)
{
    auto *before = get_memory_resource();
    {
        MemoryResourceCounts mrc;
        ScopedMemoryResource smr(&mrc);
        EXPECT_EQ(get_memory_resource(), &mrc);
    }
    EXPECT_EQ(get_memory_resource(), before);
}

TEST(ScopedMemoryResourceCounts, basic)
{
    auto *before = get_memory_resource();
    {
        auto sac = ScopedMemoryResourceCounts{};
        EXPECT_EQ(get_memory_resource(), &sac.mr);
    }
    EXPECT_EQ(get_memory_resource(), before);
}

TEST(ScopedMemoryResourceCounts, counts)
{
    auto *before = get_memory_resource();
    C4_UNUSED(before);

    {
        auto checker = AllocationCountsChecker();
        auto *mr = &checker.mr;

        for(size_t sz : {16, 32, 64, 128})
        {
            void *mem = mr->allocate(sz);
            checker.check_all_delta(1, sz, sz);
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
            checker.check_curr_delta(1, m.second);
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

C4_END_NAMESPACE(c4)
