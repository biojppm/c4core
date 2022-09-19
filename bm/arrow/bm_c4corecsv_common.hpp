#ifndef BM_ARROWCSV_COMMON_H
#define BM_ARROWCSV_COMMON_H

#include <cstdio>
#include <chrono>
#include <c4/substr.hpp>
#include <c4/charconv.hpp>
#include <c4/memory_resource.hpp>

using csubstr = c4::csubstr;


//-----------------------------------------------------------------------------

struct timed_section
{
    using myclock = std::chrono::steady_clock;
    using msecs = std::chrono::duration<double, std::milli>;

    csubstr name;
    myclock::time_point start;

    msecs since() const { return myclock::now() - start; }
    timed_section(csubstr n) : name(n), start(myclock::now()) {}
    ~timed_section()
    {
        fprintf(stderr, "%.6fms: %.*s\n", since().count(), (int)name.len, name.str);
        fflush(stderr);
    }
};

#define TIMEDSECTION(name) timed_section C4_XCAT(time_section__, C4_XCAT(name, __LINE__))(#name)


//-----------------------------------------------------------------------------

template<class T>
struct buffer
{
    buffer() = default;
    ~buffer() { release(); }
    C4_NO_COPY_OR_MOVE(buffer); // good enough for now
    void acquire(size_t sz)
    {
        ptr = (T *)c4::aalloc(sz * sizeof(T), alignof(max_align_t));
        dim = sz;
    }
    void release()
    {
        if(ptr)
            c4::afree(ptr);
        ptr = nullptr;
        dim = 0;
    }

    T *C4_RESTRICT ptr = nullptr;
    size_t dim = 0;
};


//-----------------------------------------------------------------------------

/** load a file from disk to a buffer */
size_t file_get_contents(const char *filename, buffer<char> *v)
{
    C4_SUPPRESS_WARNING_MSVC_WITH_PUSH(4996) // fopen: this function or variable may be unsafe
    ::FILE *fp = ::fopen(filename, "rb");
    C4_CHECK_MSG(fp != nullptr, "could not open file");
    ::fseek(fp, 0, SEEK_END);
    long sz = ::ftell(fp);
    if(sz)
    {
        v->acquire(static_cast<size_t>(sz));
        ::rewind(fp);
        size_t ret = ::fread(v->ptr, 1, v->dim, fp);
        C4_CHECK(ret == (size_t)sz);
    }
    else
    {
        v->release();
    }
    ::fclose(fp);
    return v->dim;
    C4_SUPPRESS_WARNING_MSVC_POP
}


//-----------------------------------------------------------------------------

template<class CsvData>
void print_first(CsvData const& C4_RESTRICT data, size_t num=20)
{
    for(size_t col = 0; col < data.ncols; ++col)
    {
        auto const &c = data.columns.ptr[col];
        printf("col[%zu]=%.*s  ", col, (int)c.name.len, c.name.str);
        size_t n = 0;
        for(size_t row = 0; row < data.nrows; ++row)
        {
            csubstr v = c.data[row];
            if(!v.len)
                continue;
            if(n == 0)
                printf(" [first=%zu] ", row);
            fwrite(v.str, 1, v.len, stdout);
            putchar(',');
            if(n++ >= num)
                break;
        }
        putchar('\n');
    }
}

#endif /* BM_ARROWCSV_COMMON_H */
