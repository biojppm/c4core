
#include "bm_c4corecsv_common.hpp"

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
        C4_ASSERT(col < ncols);
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

template<class CsvData>
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
            numlines = data_lines.count('\n');
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
            for(size_t row = 0; row < numlines; ++row)
            {
                csubstr line = data_lines.first(data_lines.first_of('\n'));
                C4_ASSERT(line.len);
                readline(line.trimr('\r'), row, &data);
                data_lines = data_lines.right_of(line.len + 1);
            }
        }
        // parse the columns to data type
        {
            TIMEDSECTION(convert_int_data);
            data.column_as(9u, &retail_and_recreation);
            data.column_as(10u, &grocery_and_pharmacy);
            data.column_as(11u, &parks);
            data.column_as(12u, &transit_stations);
            data.column_as(13u, &workplaces);
            data.column_as(14u, &residential);
        }
    }
    print_first(data);
}
