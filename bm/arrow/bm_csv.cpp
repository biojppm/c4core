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

struct csv_data
{
    struct column
    {
        csubstr name;
        csubstr *C4_RESTRICT data; // points into the data_buffer
    };
    buffer<csubstr> data_buffer {};
    buffer<column> columns {};
    size_t ncols = {};
    size_t nrows = {};
    void reset(size_t num_columns, size_t num_rows) noexcept
    {
        ncols = num_columns;
        nrows = num_rows;
        data_buffer.acquire(ncols * nrows);
        columns.acquire(ncols);
        for(size_t col = 0; col < ncols; ++col)
            columns.ptr[col].data = &data_buffer.ptr[col * nrows];
    }
    void reset_column(size_t col, c4::csubstr col_name) noexcept
    {
        columns.ptr[col].name = col_name;
    }
    C4_ALWAYS_INLINE void set(size_t row, size_t col, csubstr value) noexcept
    {
        C4_ASSERT_MSG(col < ncols, "row=%zu col=%zu ncols=%zu value='%.*s'", row, col, ncols, (int)value.len, value.str);
        columns.ptr[col].data[row] = value;
    }
    C4_ALWAYS_INLINE csubstr get(size_t row, size_t col) const noexcept
    {
        C4_ASSERT_MSG(col < ncols, "row=%zu col=%zu ncols=%zu value='%.*s'", row, col, ncols);
        return columns.ptr[col].data[row];
    }
    template<class T>
    void column_as(size_t col, buffer<T> *buf) const noexcept
    {
        if(buf->dim != nrows)
        {
            buf->release();
            buf->acquire(nrows);
        }
        column const& c = columns.ptr[col];
        for(size_t row = 0; row < nrows; ++row)
        {
            csubstr s = c.data[row];
            if(s.len)
                C4_CHECK_MSG(c4::atox(s, &buf->ptr[row]), "row=%zu col=%zu value='%.*s'", row, col, (int)c.name.len, c.name.str, (int)s.len, s.str);
            else
                buf->ptr[row] = {};
        }
    }
    template<class T>
    void column_as(csubstr name, buffer<T> *buf) const noexcept
    {
        size_t col = lookup_column(name);
        C4_CHECK_MSG(col != c4::csubstr::npos, "name='%.*s'", (int)name.len, name.str);
        column_as(col, buf);
    }
    size_t lookup_column(csubstr name) const noexcept
    {
        // just do a linear search, it's fast because the number of
        // cols is generally small
        for(size_t col = 0; col < ncols; ++col)
            if(columns.ptr[col].name == name)
                return col;
        return csubstr::npos;
    }
};

C4_ALWAYS_INLINE void readline(csubstr line, size_t row, csv_data *C4_RESTRICT data) noexcept
{
    // we cannot use line.split(',') because fields may be quoted
    // and contain a comma inside the quotes
    for(size_t col = 0; col < data->ncols; ++col)
    {
        csubstr field;
        if(!line.begins_with('"'))
        {
            field = line.left_of(line.first_of(','));
            line = line.right_of(field);
        }
        else // field is quoted
        {
            field = line.left_of(line.first_of('"', 1));
            line = line.right_of(field).sub(1); // advance beyond the ending quote
        }
        if(line.len)
        {
            C4_ASSERT(line[0] == ',');
            line = line.sub(1); // advance beyond the comma
        }
        data->set(row, col, field);
    }
}

void print_first(csv_data const& C4_RESTRICT data, size_t num=20)
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


//-----------------------------------------------------------------------------

int main(int argc, const char *argv[])
{
    using namespace c4;
    using ibuf = buffer<int>;
    C4_CHECK(argc > 1);
    buffer<char> filecontents_buf = {};
    csv_data data;
    ibuf retail_and_recreation;
    ibuf grocery_and_pharmacy;
    ibuf parks;
    ibuf transit_stations;
    ibuf workplaces;
    ibuf residential;
    {
        TIMEDSECTION(all);
        {
            TIMEDSECTION(readfile);
            file_get_contents(argv[1], &filecontents_buf);
        }
        // split the contents into header line and the data lines
        csubstr filecontents = {filecontents_buf.ptr, filecontents_buf.dim};
        size_t first_linebreak = filecontents.first_of('\n');
        C4_CHECK(first_linebreak != csubstr::npos);
        csubstr header_line = filecontents.left_of(first_linebreak).trimr('\r');
        csubstr data_lines = filecontents.right_of(first_linebreak);
        // prepare by counting columns and lines
        size_t numcols, numlines;
        {
            TIMEDSECTION(count);
            numcols = 1 + header_line.count(',');
            numlines = 1 + data_lines.count('\n');
            printf("cols=%zu rows=%zu\n", numcols, numlines);
        }
        // size the data as needed
        {
            TIMEDSECTION(prepare_mem);
            C4_CHECK(numlines > 0);
            data.reset(numcols, numlines);
            size_t col = 0;
            for(csubstr field : header_line.split(',')) // assumes no quotes and no commas as part of the col name
                data.reset_column(col++, field);
            C4_ASSERT(col == numcols);
        }
        // read all the fields to the proper place
        {
            TIMEDSECTION(populate_string_views);
            size_t row = 0;
            for(csubstr line : data_lines.split('\n'))
                readline(line.trimr('\r'), row++, &data);
            C4_ASSERT(row == numlines);
        }
        // parse the columns to data type
        {
            TIMEDSECTION(convert_int_data);
            data.column_as<int>("retail_and_recreation_percent_change_from_baseline", &retail_and_recreation);
            data.column_as<int>("grocery_and_pharmacy_percent_change_from_baseline", &grocery_and_pharmacy);
            data.column_as<int>("parks_percent_change_from_baseline", &parks);
            data.column_as<int>("transit_stations_percent_change_from_baseline", &transit_stations);
            data.column_as<int>("workplaces_percent_change_from_baseline", &workplaces);
            data.column_as<int>("residential_percent_change_from_baseline", &residential);
        }
    }
    print_first(data);
}
