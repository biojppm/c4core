// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "c4/memory_resource.hpp"

#include <stdlib.h>
#include <string.h>
#ifdef C4_POSIX
#   include <errno.h>
#endif

C4_BEGIN_NAMESPACE(c4)

thread_local AllocationCounts MemoryResourceCounts::s_counts = AllocationCounts();
C4_BEGIN_NAMESPACE(detail)

#ifdef C4_NO_ALLOC_DEFAULTS
alloc_type s_alloc = nullptr;
aalloc_type s_aalloc = nullptr;
free_type s_free = nullptr;
free_type s_afree = nullptr;
realloc_type s_realloc = nullptr;
arealloc_type s_arealloc = nullptr;
#else

void free_impl(void *ptr)
{
    ::free(ptr);
}
void afree_impl(void *ptr)
{
#if defined(C4_WIN) || defined(C4_XBOX)
    ::_aligned_free(ptr);
#else
    ::free(ptr);
#endif
}
void* alloc_impl(size_t size)
{
    void* mem = ::malloc(size);
    C4_CHECK(mem != nullptr || size == 0);
    return mem;
}
void* aalloc_impl(size_t size, size_t alignment)
{
    void *mem;
#if defined(C4_WIN) || defined(C4_XBOX)
    mem = ::_aligned_malloc(size, alignment);
    C4_CHECK(mem != nullptr || size == 0);
#elif defined(C4_POSIX)
    // NOTE: alignment needs to be sized in multiples of sizeof(void*)
    size_t amult = alignment;
    if(C4_UNLIKELY(alignment < sizeof(void*)))
    {
        amult = sizeof(void*);
    }
    int ret = ::posix_memalign(&mem, amult, size);
    if(C4_UNLIKELY(ret))
    {
        if(ret == EINVAL)
        {
            C4_ERROR("The alignment argument %lu was not a power of two, "
                     "or was not a multiple of sizeof(void*)",
                     (uint64_t)alignment);
        }
        else if(ret == ENOMEM)
        {
            C4_ERROR("There was insufficient memory to fulfill the "
                     "allocation request of %lu bytes (alignment=%lu)",
                     (uint64_t)size, (uint64_t)size);
        }
        if(mem)
        {
            afree(mem);
        }
        return nullptr;
    }
#else
    C4_NOT_IMPLEMENTED_MSG("need to implement an aligned allocation for this platform");
#endif
    C4_ASSERT_MSG((size_t(mem) & (alignment-1)) == 0, "address %p is not aligned to %lu boundary", mem, (uint64_t)alignment);
    return mem;
}
void* realloc_impl(void* ptr, size_t oldsz, size_t newsz)
{
    C4_UNUSED(oldsz);
    void *nptr = ::realloc(ptr, newsz);
    return nptr;
}
void* arealloc_impl(void* ptr, size_t oldsz, size_t newsz, size_t alignment)
{
    /** @todo make this more efficient
     * @see http://stackoverflow.com/a/9078627/5875572
     * @see look for qReallocAligned() in http://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qmalloc.cpp
     */
    void *tmp = aalloc(newsz, alignment);
    size_t min = newsz < oldsz ? newsz : oldsz;
    ::memcpy(tmp, ptr, min);
    afree(ptr);
    return tmp;
}

alloc_type s_alloc = alloc_impl;
aalloc_type s_aalloc = aalloc_impl;
free_type s_free = free_impl;
free_type s_afree = afree_impl;
realloc_type s_realloc = realloc_impl;
arealloc_type s_arealloc = arealloc_impl;

#endif // C4_NO_ALLOC_DEFAULTS

C4_END_NAMESPACE(detail)

alloc_type get_alloc()
{
    return detail::s_alloc;
}
void set_alloc(alloc_type fn)
{
    detail::s_alloc = fn;
}

aalloc_type get_aalloc()
{
    return detail::s_aalloc;
}
void set_aalloc(aalloc_type fn)
{
    detail::s_aalloc = fn;
}

free_type get_free()
{
    return detail::s_free;
}
void set_free(free_type fn)
{
    detail::s_free = fn;
}

free_type get_afree()
{
    return detail::s_afree;
}
void set_afree(free_type fn)
{
    detail::s_afree = fn;
}

realloc_type get_realloc()
{
    return detail::s_realloc;
}
void set_realloc(realloc_type fn)
{
    detail::s_realloc = fn;
}

arealloc_type get_arealloc()
{
    return detail::s_arealloc;
}
void set_arealloc(arealloc_type fn)
{
    detail::s_arealloc = fn;
}


void* alloc(size_t sz)
{
    C4_ASSERT_MSG(c4::get_alloc() != nullptr, "did you forget to call set_alloc()?");
    auto fn = c4::get_alloc();
    void* ptr = fn(sz);
    return ptr;
}
void* aalloc(size_t sz, size_t alignment)
{
    C4_ASSERT_MSG(c4::get_aalloc() != nullptr, "did you forget to call set_aalloc()?");
    auto fn = c4::get_aalloc();
    void* ptr = fn(sz, alignment);
    return ptr;
}
void free(void* ptr)
{
    C4_ASSERT_MSG(c4::get_free() != nullptr, "did you forget to call set_free()?");
    auto fn = c4::get_free();
    fn(ptr);
}
void afree(void* ptr)
{
    C4_ASSERT_MSG(c4::get_afree() != nullptr, "did you forget to call set_afree()?");
    auto fn = c4::get_afree();
    fn(ptr);
}

void* realloc(void *ptr, size_t oldsz, size_t newsz)
{
    C4_ASSERT_MSG(c4::get_realloc() != nullptr, "did you forget to call set_realloc()?");
    auto fn = c4::get_realloc();
    void* nptr = fn(ptr, oldsz, newsz);
    return nptr;
}
void* arealloc(void *ptr, size_t oldsz, size_t newsz, size_t alignment)
{
    C4_ASSERT_MSG(c4::get_arealloc() != nullptr, "did you forget to call set_arealloc()?");
    auto fn = c4::get_arealloc();
    void* nptr = fn(ptr, oldsz, newsz, alignment);
    return nptr;
}

C4_END_NAMESPACE(c4)

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifdef C4_REDEFINE_CPPNEW
#include <new>
void* operator new(size_t size)
{
    auto *mr = ::c4::get_memory_resource();
    return mr->allocate(size);
}
void operator delete(void *p) noexcept
{
    C4_NEVER_REACH();
}
void operator delete(void *p, size_t size)
{
    auto *mr = ::c4::get_memory_resource();
    mr->deallocate(p, size);
}
void* operator new[](size_t size)
{
    return operator new(size);
}
void operator delete[](void *p) noexcept
{
    operator delete(p);
}
void operator delete[](void *p, size_t size)
{
    operator delete(p, size);
}
void* operator new(size_t size, std::nothrow_t)
{
    return operator new(size);
}
void operator delete(void *p, std::nothrow_t)
{
    operator delete(p);
}
void operator delete(void *p, size_t size, std::nothrow_t)
{
    operator delete(p, size);
}
void* operator new[](size_t size, std::nothrow_t)
{
    return operator new(size);
}
void operator delete[](void *p, std::nothrow_t)
{
    operator delete(p);
}
void operator delete[](void *p, size_t, std::nothrow_t)
{
    operator delete(p, size);
}
#endif // C4_REDEFINE_CPPNEW
