#include "bm_c4corecsv_common.hpp"
#include <thread>
#include <atomic>
#include <vector>

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
    C4_ALWAYS_INLINE csubstr readfield_s(size_t col, csubstr line) const noexcept
    {
        if(col + 1 == ncols)
            return line.right_of(line.last_of(','));
        // look for the col,col+1 commas, starting at the beginning
        size_t ncommas = 0u;
        size_t prevcomma = 0u;
        for(size_t i = 0u; i < line.len; ++i)
        {
            const char c = line.str[i];
            if(c == ',')
            {
                ++ncommas;
                if(ncommas != col+1)
                {
                    prevcomma = i;
                }
                else
                {
                    return line.range(prevcomma+1, i);
                }
            }
            // skip everything within quotes
            else if(c == '"')
            {
                i = line.first_of('"', i + 1);
                C4_ASSERT(i != csubstr::npos);
            }
        }
        C4_ERROR("field not found. col=%zu line=%.*s", col, (int)line.len, line.str);
        return {};
    }
    C4_ALWAYS_INLINE void readcol(size_t icol, stringcol *C4_RESTRICT col, csubstr data) noexcept
    {
        for(size_t row = 0; row < nrows; ++row)
        {
            csubstr line = data.first(data.first_of('\n'));
            C4_ASSERT(line.len);
            col->data.ptr[row] = readfield_s(icol, line.trimr('\r'));
            data = data.right_of(line.len + 1);
        }
    }
    C4_ALWAYS_INLINE void readcol(size_t icol, intcol *C4_RESTRICT col, csubstr data) noexcept
    {
        for(size_t row = 0; row < nrows; ++row)
        {
            csubstr line = data.first(data.first_of('\n'));
            C4_ASSERT(line.len);
            csubstr field = readfield_s(icol, line.trimr('\r'));
            if(field.len)
                C4_CHECK_MSG(c4::atox(field, &col->data.ptr[row]), "row=%zu col=%zu field='%.*s", row, icol, (int)field.len, field.str);
            else
                col->data.ptr[row] = 0;
            data = data.right_of(line.len + 1);
        }
    }
    C4_ALWAYS_INLINE void readcols(size_t first, size_t last, csubstr data) noexcept
    {
        C4_ASSERT(first < ncols);
        C4_ASSERT(last <= ncols);
        C4_ASSERT(first < last);
        for(size_t col = first; col < last; ++col)
        {
            switch(col)
            {
                // string fields
                case  0u: readcol(0u, &country_region_code, data); break;
                case  1u: readcol(1u, &country_region, data); break;
                case  2u: readcol(2u, &sub_region_1, data); break;
                case  3u: readcol(3u, &sub_region_2, data); break;
                case  4u: readcol(4u, &metro_area, data); break;
                case  5u: readcol(5u, &iso_3166_2_code, data); break;
                case  6u: readcol(6u, &census_fips_code, data); break;
                case  7u: readcol(7u, &place_id, data); break;
                case  8u: readcol(8u, &date, data); break;
                // int fields
                case  9u: readcol(9u, &retail_and_recreation, data); break;
                case 10u: readcol(10u, &grocery_and_pharmacy, data); break;
                case 11u: readcol(11u, &parks, data); break;
                case 12u: readcol(12u, &transit_stations, data); break;
                case 13u: readcol(13u, &workplaces, data); break;
                case 14u: readcol(14u, &residential, data); break;
            }
        }
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
    size_t num_threads = 4;
    {
        TIMEDSECTION(all);
        {
            TIMEDSECTION(readfile);
            file_get_contents(argv[1], &filecontents_buf);
        }
        // split the contents into header line and the data lines
        csubstr filecontents = {filecontents_buf.ptr, filecontents_buf.dim};
        size_t first_linebreak = filecontents.first_of('\n');
        C4_CHECK(filecontents.len);
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
            std::vector<std::thread> threads;
            {
                TIMEDSECTION(launch_threads);
                const size_t num_cols_per_thread = numcols / num_threads;
                printf("numcols/thread=%zu\n", num_cols_per_thread);
                for(size_t thread = 0; thread < num_threads; ++thread)
                {
                    threads.emplace_back([&,
                                          thread_id=thread,
                                          ncpt=num_cols_per_thread]{
                        TIMEDSECTION(thread_work);
                        size_t rem = numcols % num_threads;
                        size_t first_col = thread_id * ncpt;
                        size_t last_col = first_col + ncpt;
                        if(rem)
                        {
                            if(thread_id < rem)
                            {
                                first_col += thread_id;
                                last_col += thread_id + 1;
                            }
                            else
                            {
                                first_col += rem;
                                last_col += rem;
                            }
                        }
                        printf("thread[%zu]: numcols/thread=%zu\n", thread_id, ncpt);
                        printf("thread[%zu]: %zu...%zu rem=%zu\n", thread_id, first_col, last_col, rem);
                        data.readcols(first_col, last_col, data_lines);
                    });
                }
            }
            {
                TIMEDSECTION(join_threads);
                for(std::thread &thread : threads)
                    thread.join();
            }
        }
    }
}
