#include <benchmark/benchmark.h>
#include <string>
#include <sstream>
#include <c4/c4_push.hpp>
#include <c4/substr.hpp>
#include <c4/to_chars.hpp>

namespace bm = benchmark;


namespace to_chars_alternatives {


template<class StreamType, class T>
std::string sstream_naive(T const& C4_RESTRICT val)
{
    StreamType ss;
    ss << val;
    return ss.str();
}


template<class StreamType, class T>
std::string sstream_naive_reuse(StreamType &ss, T const& C4_RESTRICT val)
{
    ss.clear();
    ss.str("");
    ss << val;
    return ss.str();
}


} // to_chars_alternatives


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class IntegralType>
void c4_itoa(benchmark::State& st)
{
    char buf_[100];
    c4::substr buf(buf_);
    IntegralType i = 0;
    for(auto _ : st)
    {
        ++i;
        c4::itoa(buf, i);
    }
    st.SetBytesProcessed(st.iterations() * sizeof(IntegralType));
}

template<class IntegralType>
void c4_utoa(benchmark::State& st)
{
    char buf_[100];
    c4::substr buf(buf_);
    IntegralType i = 0;
    for(auto _ : st)
    {
        ++i;
        c4::utoa(buf, i);
    }
    st.SetBytesProcessed(st.iterations() * sizeof(IntegralType));
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class IntegralType, class StreamType>
void sstream_naive(benchmark::State& st)
{
    IntegralType i = 0;
    for(auto _ : st)
    {
        ++i;
        std::string out = to_chars_alternatives::sstream_naive<StreamType>(i);
        C4_UNUSED(out);
    }
    st.SetBytesProcessed(st.iterations() * sizeof(IntegralType));
}


//-----------------------------------------------------------------------------

template<class IntegralType, class StreamType>
void sstream_naive_reuse(benchmark::State& st)
{
    IntegralType i = 0;
    StreamType ss;
    for(auto _ : st)
    {
        ++i;
        std::string out = to_chars_alternatives::sstream_naive_reuse(ss, i);
        C4_UNUSED(out);
    }
    st.SetBytesProcessed(st.iterations() * sizeof(IntegralType));
}

BENCHMARK_TEMPLATE(c4_utoa,  uint8_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  uint8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  uint8_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive,  uint8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive,  uint8_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_itoa,   int8_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse,   int8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse,   int8_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive,   int8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive,   int8_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_utoa, uint16_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse, uint16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse, uint16_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive, uint16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive, uint16_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_itoa,  int16_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  int16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  int16_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive,  int16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive,  int16_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_utoa, uint32_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse, uint32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse, uint32_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive, uint32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive, uint32_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_itoa,  int32_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  int32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  int32_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive,  int32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive,  int32_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_utoa, uint64_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse, uint64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse, uint64_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive, uint64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive, uint64_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_itoa,  int64_t);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  int64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive_reuse,  int64_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream_naive,  int64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_naive,  int64_t, std::stringstream);


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
