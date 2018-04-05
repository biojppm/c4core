#ifndef _C4_MEMORY_RESOURCE_HPP_
#define _C4_MEMORY_RESOURCE_HPP_

/** @file memory_resource.hpp Provides facilities to allocate typeless
 *  memory, via the memory resource model consecrated with C++17. */

/** @defgroup raw_memory_alloc Raw memory allocation
 *
 */

/** @defgroup memory_resources Memory resources
 *
 */

#include "c4/config.hpp"
#include "c4/error.hpp"

C4_BEGIN_NAMESPACE(c4)

// need these forward decls here
struct MemoryResource;
struct MemoryResourceMalloc;
struct MemoryResourceStack;
MemoryResourceMalloc* get_memory_resource_malloc();
MemoryResourceStack* get_memory_resource_stack();
namespace detail { MemoryResource*& get_memory_resource(); }


// c-style allocation ---------------------------------------------------------

// this API provides unaligned as well as aligned allocation functions.
// These functions forward the call to a modifiable function.

// aligned allocation.
void* aalloc(size_t sz, size_t alignment);
void afree(void* ptr);
void* arealloc(void* ptr, size_t oldsz, size_t newsz, size_t alignment);

// classic, unaligned allocation.
void* alloc(size_t sz);
void free(void* ptr);
void* realloc(void* ptr, size_t oldsz, size_t newsz);

// allocation setup facilities
using aalloc_type   = void* (*)(size_t size, size_t alignment);
using afree_type    = void  (*)(void *ptr);
using arealloc_type = void* (*)(void *ptr, size_t oldsz, size_t newsz, size_t alignment);

using alloc_type   = void* (*)(size_t size);
using free_type    = void  (*)(void *ptr);
using realloc_type = void* (*)(void *ptr, size_t oldsz, size_t newsz);

// set the function to be called
void set_aalloc  (aalloc_type   fn);
void set_afree   (afree_type    fn);
void set_arealloc(arealloc_type fn);

void set_alloc  (alloc_type   fn);
void set_free   (free_type    fn);
void set_realloc(realloc_type fn);

// get the function which will be called
alloc_type   get_alloc();
free_type    get_free();
realloc_type get_realloc();

aalloc_type   get_aalloc();
free_type     get_afree();
arealloc_type get_arealloc();

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// c++-style allocation -------------------------------------------------------

/** C++17-style memory_resource base class. See http://en.cppreference.com/w/cpp/experimental/memory_resource
 * @ingroup memory_resources */
struct MemoryResource
{
    const char *name = nullptr;
    virtual ~MemoryResource() {}

    void* allocate(size_t sz, size_t alignment = alignof(max_align_t))
    {
        void *mem = this->do_allocate(sz, alignment);
        C4_CHECK_MSG(mem != nullptr, "could not allocate %lu bytes", sz);
        return mem;
    }
    void* reallocate(void* ptr, size_t oldsz, size_t newsz, size_t alignment = alignof(max_align_t))
    {
        void *mem = this->do_reallocate(ptr, oldsz, newsz, alignment);
        C4_CHECK_MSG(mem != nullptr, "could not reallocate from %lu to %lu bytes", oldsz, newsz);
        return mem;
    }
    void deallocate(void* ptr, size_t sz, size_t alignment = alignof(max_align_t))
    {
        this->do_deallocate(ptr, sz, alignment);
    }

protected:

    virtual void* do_allocate(size_t sz, size_t alignment) = 0;
    virtual void* do_reallocate(void* ptr, size_t oldsz, size_t newsz, size_t alignment) = 0;
    virtual void  do_deallocate(void* ptr, size_t sz, size_t alignment) = 0;

};

/** get the current global memory resource. To avoid static initialization
 * order problems, this is implemented using a function call to ensure
 * that it is available on when first used.
 * @ingroup memory_resources */
C4_ALWAYS_INLINE MemoryResource* get_memory_resource()
{
    /* T */
    return detail::get_memory_resource();
}

/** set the global memory resource
 * @ingroup memory_resources */
C4_ALWAYS_INLINE void set_memory_resource(MemoryResource* mr)
{
    C4_ASSERT(mr != nullptr);
    detail::get_memory_resource() = mr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** A c4::aalloc-based memory resource. Thread-safe if the implementation called by
 * c4::aalloc() is safe.
 * @ingroup memory_resources */
struct MemoryResourceMalloc : public MemoryResource
{

    MemoryResourceMalloc() { name = "malloc"; }
    virtual ~MemoryResourceMalloc() {}

protected:

    virtual void* do_allocate(size_t sz, size_t alignment) override
    {
        return c4::aalloc(sz, alignment);
    }

    virtual void  do_deallocate(void* ptr, size_t sz, size_t alignment) override
    {
        C4_UNUSED(sz);
        C4_UNUSED(alignment);
        c4::afree(ptr);
    }

    virtual void* do_reallocate(void* ptr, size_t oldsz, size_t newsz, size_t alignment) override
    {
        return c4::arealloc(ptr, oldsz, newsz, alignment);
    }

};

/** returns a malloc-based memory resource
 *  @ingroup memory_resources */
C4_ALWAYS_INLINE MemoryResourceMalloc* get_memory_resource_malloc()
{
    static MemoryResourceMalloc mr;
    return &mr;
}

C4_BEGIN_NAMESPACE(detail)
C4_ALWAYS_INLINE MemoryResource* & get_memory_resource()
{
    thread_local static MemoryResource* mr = get_memory_resource_malloc();
    return mr;
}
C4_END_NAMESPACE(detail)


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** Provides an alloca()-based memory resource. The usual alloca() caveats apply.
 * @ingroup memory_resources */
struct MemoryResourceStack : public MemoryResource
{

    MemoryResourceStack() { name = "stack_malloc"; }
    virtual ~MemoryResourceStack() {}

protected:

    virtual void* do_allocate(size_t sz, size_t alignment) override;
    virtual void  do_deallocate(void* ptr, size_t sz, size_t alignment) override;
    virtual void* do_reallocate(void* ptr, size_t oldsz, size_t newsz, size_t alignment) override;

};

/** returns an alloca-based memory resource
 *  @ingroup memory_resources */
C4_ALWAYS_INLINE MemoryResourceStack* get_memory_resource_stack()
{
    static MemoryResourceStack mr;
    return &mr;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** provides a linear memory resource. Deallocations happen only when the
 * resource is cleared or destroyed. The memory used by this object can be owned
 * or borrowed. */
struct MemoryResourceLinear : public MemoryResource
{

    MemoryResourceLinear() { name = "linear_malloc"; }

    C4_NO_COPY_OR_MOVE(MemoryResourceLinear);

public:

    /** initialize with owned memory, allocated from the global memory resource */
    MemoryResourceLinear(size_t sz) : MemoryResourceLinear() { acquire(sz); }
    /** initialize with borrowed memory */
    MemoryResourceLinear(void *mem, size_t sz) : MemoryResourceLinear() { acquire(mem, sz); }

    virtual ~MemoryResourceLinear() { release(); }

public:

    char  *m_mem{nullptr};
    size_t m_size{0};
    size_t m_pos{0};
    bool   m_owner;

public:

    void clear() { m_pos = 0; }

    /** initialize with owned memory, allocated from the global memory resource */
    void acquire(size_t sz);
    /** initialize with borrowed memory */
    void acquire(void *mem, size_t sz);
    /** release the memory */
    void release();

protected:

    virtual void* do_allocate(size_t sz, size_t alignment) override;
    virtual void  do_deallocate(void* ptr, size_t sz, size_t alignment) override;
    virtual void* do_reallocate(void* ptr, size_t oldsz, size_t newsz, size_t alignment) override;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct AllocationCounts
{
    struct Item
    {
        ssize_t allocs;
        ssize_t size;

        void add(size_t sz)
        {
            ++allocs;
            size += sz;
        }
        void rem(size_t sz)
        {
            --allocs;
            size -= sz;
        }
        Item max(Item const& that) const
        {
            Item r(*this);
            r.allocs = r.allocs > that.allocs ? r.allocs : that.allocs;
            r.size = r.size > that.size ? r.size : that.size;
            return r;
        }
    };

    Item curr  = {0, 0};
    Item total = {0, 0};
    Item max   = {0, 0};

    void clear_counts()
    {
        curr  = {0, 0};
        total = {0, 0};
        max   = {0, 0};
    }

    void update(AllocationCounts const& that)
    {
        curr.allocs += that.curr.allocs;
        curr.size += that.curr.size;
        total.allocs += that.total.allocs;
        total.size += that.total.size;
        max.allocs += that.max.allocs;
        max.size += that.max.size;
    }

    void add_counts(void* ptr, size_t sz)
    {
        if(ptr == nullptr) return;
        curr.add(sz);
        total.add(sz);
        max = max.max(curr);
    }

    void rem_counts(void *ptr, size_t sz)
    {
        if(ptr == nullptr) return;
        curr.rem(sz);
    }

    AllocationCounts operator- (AllocationCounts const& that) const
    {
        AllocationCounts r(*this);
        r.curr.allocs -= that.curr.allocs;
        r.curr.size -= that.curr.size;
        r.total.allocs -= that.total.allocs;
        r.total.size -= that.total.size;
        r.max.allocs -= that.max.allocs;
        r.max.size -= that.max.size;
        return r;
    }

    AllocationCounts operator+ (AllocationCounts const& that) const
    {
        AllocationCounts r(*this);
        r.curr.allocs += that.curr.allocs;
        r.curr.size += that.curr.size;
        r.total.allocs += that.total.allocs;
        r.total.size += that.total.size;
        r.max.allocs += that.max.allocs;
        r.max.size += that.max.size;
        return r;
    }
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** a MemoryResource which latches onto another MemoryResource
 * and counts allocations and sizes.
 * @ingroup memory_resources */
class MemoryResourceCounts : public MemoryResource
{
public:

    MemoryResourceCounts() : m_resource(get_memory_resource())
    {
        C4_ASSERT(m_resource != this);
        name = "MemoryResourceCounts";
    }
    MemoryResourceCounts(MemoryResource *res) : m_resource(res)
    {
        C4_ASSERT(m_resource != this);
        name = "MemoryResourceCounts";
    }
    virtual ~MemoryResourceCounts()
    {
        s_counts.update(m_counts);
    }

    MemoryResource *resource() { return m_resource; }
    AllocationCounts const& counts() const { return m_counts; }

protected:

    MemoryResource *m_resource;
    AllocationCounts m_counts;
    static thread_local AllocationCounts s_counts;

protected:

    virtual void* do_allocate(size_t sz, size_t alignment) override
    {
        void *ptr = m_resource->allocate(sz, alignment);
        m_counts.add_counts(ptr, sz);
        return ptr;
    }

    virtual void  do_deallocate(void* ptr, size_t sz, size_t alignment) override
    {
        m_counts.rem_counts(ptr, sz);
        m_resource->deallocate(ptr, sz, alignment);
    }

    virtual void* do_reallocate(void* ptr, size_t oldsz, size_t newsz, size_t alignment) override
    {
        m_counts.rem_counts(ptr, oldsz);
        void* nptr = m_resource->reallocate(ptr, oldsz, newsz, alignment);
        m_counts.add_counts(nptr, newsz);
        return nptr;
    }

};

//-----------------------------------------------------------------------------
/** RAII class which binds a memory resource with a scope duration.
 * @ingroup memory_resources */
struct ScopedMemoryResource
{
    MemoryResource *m_original;

    ScopedMemoryResource(MemoryResource *r)
    :
        m_original(get_memory_resource())
    {
        set_memory_resource(r);
    }

    ~ScopedMemoryResource()
    {
        set_memory_resource(m_original);
    }
};

//-----------------------------------------------------------------------------
/** RAII class which counts allocations and frees inside a scope. Can
 * optionally set also the memory resource to be used.
 * @ingroup memory_resources */
struct ScopedMemoryResourceCounts
{
    MemoryResourceCounts mr;

    ScopedMemoryResourceCounts() : mr()
    {
        set_memory_resource(&mr);
    }
    ScopedMemoryResourceCounts(MemoryResource *m) : mr(m)
    {
        set_memory_resource(&mr);
    }
    ~ScopedMemoryResourceCounts()
    {
        set_memory_resource(mr.resource());
    }
};

C4_END_NAMESPACE(c4)

#endif /* _C4_MEMORY_RESOURCE_HPP_ */
