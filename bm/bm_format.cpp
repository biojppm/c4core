#include <string>
#include <c4/c4_push.hpp>
#include <c4/std/std.hpp>
#include <c4/format.hpp>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <benchmark/benchmark.h>

namespace bm = benchmark;

double getmax(std::vector<double> const& v)
{
    return *(std::max_element(std::begin(v), std::end(v)));
}
double getmin(std::vector<double> const& v)
{
    return *(std::min_element(std::begin(v), std::end(v)));
}
double getrange(std::vector<double> const& v)
{
    auto min_max = std::minmax_element(std::begin(v), std::end(v));
    return *min_max.second - *min_max.first;
}

#define _c4bm_stats                                                     \
    /*->Repetitions(20)*/                                               \
    ->DisplayAggregatesOnly(true)                                       \
    ->ComputeStatistics("range", &getrange)                             \
    ->ComputeStatistics("max", &getmax)                                 \
    ->ComputeStatistics("min", &getmin)

#define C4BM(fn) BENCHMARK(fn) _c4bm_stats

/** convenience wrapper to avoid boilerplate code */
void report(bm::State &st, size_t sz)
{
    st.SetBytesProcessed(static_cast<int64_t>(st.iterations() * sz));
    st.SetItemsProcessed(static_cast<int64_t>(st.iterations()));
}


#define _c4argbundle \
    1, 2, 3, 4, 5, 6, 7, 8, 9, size_t(283482349),\
    " ", "haha", std::string("hehe"),\
    std::string("asdlklkasdlkjasd asdlkjasdlkjasdlkjasdoiasdlkjasldkj")

#define _c4argbundle_lshift \
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << size_t(283482349)\
      << " " << "haha" << std::string("hehe")\
      << std::string("asdlklkasdlkjasd asdlkjasdlkjasdlkjasdoiasdlkjasldkj")


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void cat_c4cat_substr(bm::State &st)
{
    char buf_[256];
    c4::substr buf(buf_);
    size_t sz = 0;
    for(auto _ : st)
    {
        sz = cat(buf, _c4argbundle);
    }
    report(st, sz);
}

void cat_c4catrs_reuse(bm::State &st)
{
    std::string buf;
    size_t sz = 0;
    for(auto _ : st)
    {
        c4::catrs(&buf, _c4argbundle);
        sz = buf.size();
    }
    report(st, sz);
}

void cat_c4catrs_no_reuse(bm::State &st)
{
    size_t sz = 0;
    for(auto _ : st)
    {
        auto buf = c4::catrs<std::string>(_c4argbundle);
        sz = buf.size();
    }
    report(st, sz);
}

void cat_stdsstream_reuse(bm::State &st)
{
    size_t sz = 0;
    std::stringstream ss;
    for(auto _ : st)
    {
        ss.str("");
        ss << _c4argbundle_lshift;
        sz = ss.str().size();
    }
    report(st, sz);
}

void cat_stdsstream(bm::State &st)
{
    size_t sz = 0;
    for(auto _ : st)
    {
        std::stringstream ss;
        ss << _c4argbundle_lshift;
        sz = ss.str().size();
    }
    report(st, sz);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

C4BM(cat_c4cat_substr);
C4BM(cat_c4catrs_reuse);
C4BM(cat_c4catrs_no_reuse);
C4BM(cat_stdsstream_reuse);
C4BM(cat_stdsstream);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    bm::Initialize(&argc, argv);
    bm::RunSpecifiedBenchmarks();
    return 0;
}


#include <c4/c4_pop.hpp>
