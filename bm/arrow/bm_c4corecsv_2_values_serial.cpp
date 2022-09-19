#include "bm_c4corecsv_common.hpp"

struct csv_data_values
{

    size_t ncols = {};
    size_t nrows = {};

    template<class T>
    struct column
    {
        csubstr name;
        buffer<T> data; // points into the data_buffer
    };
    using stringcol = column<csubstr>;
    using intcol = column<int>;

    stringcol country_region_code {};
    stringcol country_region {};
    stringcol sub_region_1 {};
    stringcol sub_region_2 {};
    stringcol metro_area {};
    stringcol iso_3166_2_code {};
    stringcol census_fips_code {};
    stringcol place_id {};
    stringcol date {};

    intcol retail_and_recreation {};
    intcol grocery_and_pharmacy {};
    intcol parks {};
    intcol transit_stations {};
    intcol workplaces {};
    intcol residential {};

    void reset(size_t num_columns, size_t num_rows) noexcept
    {
        C4_CHECK(num_columns == 15u);
        ncols = num_columns;
        nrows = num_rows;
        country_region_code.data.acquire(num_rows);
        country_region.data.acquire(num_rows);
        sub_region_1.data.acquire(num_rows);
        sub_region_2.data.acquire(num_rows);
        metro_area.data.acquire(num_rows);
        iso_3166_2_code.data.acquire(num_rows);
        census_fips_code.data.acquire(num_rows);
        place_id.data.acquire(num_rows);
        date.data.acquire(num_rows);
        retail_and_recreation.data.acquire(num_rows);
        grocery_and_pharmacy.data.acquire(num_rows);
        parks.data.acquire(num_rows);
        transit_stations.data.acquire(num_rows);
        workplaces.data.acquire(num_rows);
        residential.data.acquire(num_rows);
    }
    void reset_column(size_t col, c4::csubstr col_name) noexcept
    {
        #define setcol(index, name_, name_suffix)  \
            if(col_name == #name_ #name_suffix) {  \
                C4_CHECK(col == index);            \
                this->name_.name = col_name;       \
            }
        setcol(0u, country_region_code, )
        else setcol(1u, country_region, )
        else setcol(2u, sub_region_1, )
        else setcol(3u, sub_region_2, )
        else setcol(4u, metro_area, )
        else setcol(5u, iso_3166_2_code, )
        else setcol(6u, census_fips_code, )
        else setcol(7u, place_id, )
        else setcol(8u, date, )
        else setcol(9u, retail_and_recreation, _percent_change_from_baseline)
        else setcol(10u, grocery_and_pharmacy, _percent_change_from_baseline)
        else setcol(11u, parks, _percent_change_from_baseline)
        else setcol(12u, transit_stations, _percent_change_from_baseline)
        else setcol(13u, workplaces, _percent_change_from_baseline)
        else setcol(14u, residential, _percent_change_from_baseline)
        else C4_ERROR("unknown column: %.*s", (int)col_name.len, col_name.str);
        #undef setcol
    }
    C4_ALWAYS_INLINE csubstr readfield_s(csubstr &C4_RESTRICT line) const noexcept
    {
        csubstr field;
        if(!line.begins_with('"'))
        {
            field = line.left_of(line.first_of(','));
            line = line.right_of(field.len);
        }
        else /* field is quoted */
        {
            field = line.left_of(line.first_of('"', 1));
            line = line.right_of(field.len + 1); /* advance beyond the ending quote */
        }
        return field;
    }
    C4_ALWAYS_INLINE int readfield_i(csubstr &C4_RESTRICT line) const noexcept
    {
        C4_ASSERT(!line.begins_with('"'));
        csubstr field = line.left_of(line.first_of(','));
        line = line.right_of(field.len);
        int ret;
        if(field.len)
            C4_CHECK(c4::atox(field, &ret));
        else
            ret = 0;
        return ret;
    }
    C4_ALWAYS_INLINE int readfield_last_i(csubstr field) const noexcept
    {
        C4_ASSERT(!field.begins_with('"'));
        int ret;
        if(field.len)
            C4_CHECK(c4::atox(field, &ret));
        else
            ret = 0;
        return ret;
    }
    C4_ALWAYS_INLINE void readline(csubstr line, size_t row) noexcept
    {
        // string fields
        country_region_code.data.ptr[row]   = readfield_s(line);
        country_region.data.ptr[row]        = readfield_s(line);
        sub_region_1.data.ptr[row]          = readfield_s(line);
        sub_region_2.data.ptr[row]          = readfield_s(line);
        metro_area.data.ptr[row]            = readfield_s(line);
        iso_3166_2_code.data.ptr[row]       = readfield_s(line);
        census_fips_code.data.ptr[row]      = readfield_s(line);
        place_id.data.ptr[row]              = readfield_s(line);
        date.data.ptr[row]                  = readfield_s(line);
        // int fields
        retail_and_recreation.data.ptr[row] = readfield_i(line);
        grocery_and_pharmacy.data.ptr[row]  = readfield_i(line);
        parks.data.ptr[row]                 = readfield_i(line);
        transit_stations.data.ptr[row]      = readfield_i(line);
        workplaces.data.ptr[row]            = readfield_i(line);
        residential.data.ptr[row]           = readfield_last_i(line);
    }
};



//-----------------------------------------------------------------------------

int main(int argc, const char *argv[])
{
    using namespace c4;
    using ibuf = buffer<int>;
    C4_CHECK(argc > 1);
    buffer<char> filecontents_buf = {};
    csv_data_values data;
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
            TIMEDSECTION(readvalues);
            for(size_t row = 0; row < numlines; ++row)
            {
                csubstr line = data_lines.first(data_lines.first_of('\n'));
                C4_ASSERT(line.len);
                data.readline(line.trimr('\r'), row);
                data_lines = data_lines.right_of(line.len + 1);
            }
        }
    }
}
