#include <benchmark/benchmark.h>
#include <string>
#include <sstream>
#include <c4/c4_push.hpp>
#include <c4/substr.hpp>
#include <c4/charconv.hpp>
#include <inttypes.h>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <vector>

#if C4_CPP >= 17
#include <charconv>
#endif

// benchmarks depending on c++17 features are disabled using the
// preprocessor.  google benchmark has state.SkipWithError() but that
// makes the program return a nonzero exit code when it finishes. So
// we resort to the preprocessor to conditionally disable these
// benchmarks
#if C4_CPP >= 17
#define BENCHMARK_TEMPLATE_CPP17(fn, ...) BENCHMARK_TEMPLATE(fn, __VA_ARGS__)
#else
#define BENCHMARK_TEMPLATE_CPP17(...)
#endif


namespace bm = benchmark;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// utilities for use in benchmarks below


template<class T>
void report(bm::State &st)
{
    st.SetBytesProcessed(st.iterations() * sizeof(T));
    st.SetItemsProcessed(st.iterations());
}

template<size_t Dim=128>
struct sbuf
{
    char buf_[Dim];
    c4::substr buf;
    sbuf() : buf_(), buf(buf_) {}
    inline operator c4::substr& () { return buf; }
    char* begin() { return buf.begin(); }
    char* end() { return buf.end(); }
};

template<class T>
struct ranf
{
    std::vector<T> v;
    size_t curr;
    T next() { T f = v[curr]; curr = (curr + 1) % v.size(); return f; }
    ranf(size_t sz=4096) : v(sz), curr(0) { std::generate(v.begin(), v.end(), std::rand); }
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// facilities to deuglify SFINAE
#define C4FOR(ty, condition)                            \
    template<class ty>                                  \
    typename std::enable_if<condition(ty), void>::type
#define C4FOR2(ty1, ty2, condition)                             \
    template<class ty1, class ty2>                              \
    typename std::enable_if<condition(ty1), void>::type

#define isint(ty) std::is_integral<T>::value
#define isiint(ty) std::is_integral<T>::value && !std::is_unsigned<T>::value
#define isuint(ty) std::is_integral<T>::value && std::is_unsigned<T>::value
#define isreal(ty) std::is_floating_point<T>::value

C4FOR(T, isiint)
c4_xtoa(bm::State& st)
{
    sbuf<> buf;
    T i = 0;
    for(auto _ : st)
    {
        ++i;
        c4::itoa(buf, i);
    }
    report<T>(st);
}

C4FOR(T, isuint)
c4_xtoa(bm::State& st)
{
    sbuf<> buf;
    T i = 0;
    for(auto _ : st)
    {
        ++i;
        c4::utoa(buf, i);
    }
    report<T>(st);
}

C4FOR(T, isreal)
c4_xtoa(bm::State& st)
{
    sbuf<> buf;
    ranf<T> rans;
    for(auto _ : st)
    {
        c4::ftoa(buf, rans.next());
    }
    report<T>(st);
}


//-----------------------------------------------------------------------------

template<class T> struct fmtspec;
template<> struct fmtspec< uint8_t> { constexpr static const char fmt[] = "%" PRIu8; };
template<> struct fmtspec<  int8_t> { constexpr static const char fmt[] = "%" PRId8; };
template<> struct fmtspec<uint16_t> { constexpr static const char fmt[] = "%" PRIu16; };
template<> struct fmtspec< int16_t> { constexpr static const char fmt[] = "%" PRId16; };
template<> struct fmtspec<uint32_t> { constexpr static const char fmt[] = "%" PRIu32; };
template<> struct fmtspec< int32_t> { constexpr static const char fmt[] = "%" PRId32; };
template<> struct fmtspec<uint64_t> { constexpr static const char fmt[] = "%" PRIu64; };
template<> struct fmtspec< int64_t> { constexpr static const char fmt[] = "%" PRId64; };
template<> struct fmtspec< float  > { constexpr static const char fmt[] = "%g"; };
template<> struct fmtspec< double > { constexpr static const char fmt[] = "%lg"; };

template<class T>
C4_ALWAYS_INLINE void sprintf(c4::substr buf, T val)
{
    ::snprintf(buf.str, buf.len, fmtspec<T>::fmt, val);
}

C4FOR(T, isint)
sprintf(bm::State& st)
{
    sbuf<> buf;
    T i = 0;
    for(auto _ : st)
    {
        ++i;
        sprintf(buf, i);
    }
    report<T>(st);
}

C4FOR(T, isreal)
sprintf(bm::State& st)
{
    sbuf<> buf;
    ranf<T> rans;
    for(auto _ : st)
    {
        sprintf(buf, rans.next());
    }
    report<T>(st);
}


//-----------------------------------------------------------------------------

template<class StreamType, class T>
std::string sstream_(T const& C4_RESTRICT val)
{
    StreamType ss;
    ss << val;
    return ss.str();
}

C4FOR2(T, StreamType, isint)
sstream(bm::State& st)
{
    T i = 0;
    for(auto _ : st)
    {
        ++i;
        std::string out = sstream_<StreamType>(i);
        C4_UNUSED(out);
    }
    report<T>(st);
}

C4FOR2(T, StreamType, isreal)
sstream(bm::State& st)
{
    ranf<T> rans;
    for(auto _ : st)
    {
        std::string out = sstream_<StreamType>(rans.next());
        C4_UNUSED(out);
    }
    report<T>(st);
}


//-----------------------------------------------------------------------------

template<class StreamType, class T>
std::string sstream_reuse_(StreamType &ss, T const& C4_RESTRICT val)
{
    ss.clear();
    ss.str("");
    ss << val;
    return ss.str();
}

C4FOR2(T, StreamType, isint)
sstream_reuse(bm::State& st)
{
    T i = 0;
    StreamType ss;
    for(auto _ : st)
    {
        ++i;
        std::string out = sstream_reuse_(ss, i);
        C4_UNUSED(out);
    }
    report<T>(st);
}

C4FOR2(T, StreamType, isreal)
sstream_reuse(bm::State& st)
{
    ranf<T> rans;
    StreamType ss;
    for(auto _ : st)
    {
        std::string out = sstream_reuse_(ss, rans.next());
        C4_UNUSED(out);
    }
    report<T>(st);
}


//-----------------------------------------------------------------------------

C4FOR(T, isint)
std_to_string(bm::State& st)
{
    T i = 0;
    for(auto _ : st)
    {
        ++i;
        std::string out = std::to_string(i);
        C4_UNUSED(out);
    }
    report<T>(st);
}

C4FOR(T, isreal)
std_to_string(bm::State& st)
{
    ranf<T> rans;
    for(auto _ : st)
    {
        std::string out = std::to_string(rans.next());
        C4_UNUSED(out);
    }
    report<T>(st);
}


//-----------------------------------------------------------------------------

#if (C4_CPP >= 17)
C4FOR(T, isint)
std_to_chars(bm::State& st)
{
    sbuf<> buf;
    T i = 0;
    for(auto _ : st)
    {
        ++i;
        std::to_chars(buf.begin(), buf.end(), i);
    }
    report<T>(st);
}

C4FOR(T, isreal)
std_to_chars(bm::State& st)
{
    sbuf<> buf;
    ranf<T> rans;
    for(auto _ : st)
    {
        std::to_chars(buf.begin(), buf.end(), rans.next());
    }
    report<T>(st);
}
#endif

C4FOR(T, isint)
c4_to_chars(bm::State& st)
{
    sbuf<> buf;
    T i = 0;
    for(auto _ : st)
    {
        ++i;
        c4::to_chars(buf, i);
    }
    report<T>(st);
}

C4FOR(T, isreal)
c4_to_chars(bm::State& st)
{
    sbuf<> buf;
    ranf<T> rans;
    for(auto _ : st)
    {
        c4::to_chars(buf, rans.next());
    }
    report<T>(st);
}


//-----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(c4_xtoa,  uint8_t);
BENCHMARK_TEMPLATE(c4_to_chars,  uint8_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  uint8_t);
BENCHMARK_TEMPLATE(std_to_string,  uint8_t);
BENCHMARK_TEMPLATE(sprintf,  uint8_t);
BENCHMARK_TEMPLATE(sstream_reuse,  uint8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse,  uint8_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream,  uint8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream,  uint8_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa,   int8_t);
BENCHMARK_TEMPLATE(c4_to_chars,  int8_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  int8_t);
BENCHMARK_TEMPLATE(std_to_string,  int8_t);
BENCHMARK_TEMPLATE(sprintf,  int8_t);
BENCHMARK_TEMPLATE(sstream_reuse,   int8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse,   int8_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream,   int8_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream,   int8_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa, uint16_t);
BENCHMARK_TEMPLATE(c4_to_chars,  uint16_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  uint16_t);
BENCHMARK_TEMPLATE(std_to_string,  uint16_t);
BENCHMARK_TEMPLATE(sprintf,  uint16_t);
BENCHMARK_TEMPLATE(sstream_reuse, uint16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse, uint16_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream, uint16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream, uint16_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa,  int16_t);
BENCHMARK_TEMPLATE(c4_to_chars,  int16_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  int16_t);
BENCHMARK_TEMPLATE(std_to_string,  int16_t);
BENCHMARK_TEMPLATE(sprintf,  int16_t);
BENCHMARK_TEMPLATE(sstream_reuse,  int16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse,  int16_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream,  int16_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream,  int16_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa, uint32_t);
BENCHMARK_TEMPLATE(c4_to_chars,  uint32_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  uint32_t);
BENCHMARK_TEMPLATE(std_to_string,  uint32_t);
BENCHMARK_TEMPLATE(sprintf,  uint32_t);
BENCHMARK_TEMPLATE(sstream_reuse, uint32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse, uint32_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream, uint32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream, uint32_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa,  int32_t);
BENCHMARK_TEMPLATE(c4_to_chars,  int32_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  int32_t);
BENCHMARK_TEMPLATE(std_to_string,  int32_t);
BENCHMARK_TEMPLATE(sprintf,  int32_t);
BENCHMARK_TEMPLATE(sstream_reuse,  int32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse,  int32_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream,  int32_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream,  int32_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa, uint64_t);
BENCHMARK_TEMPLATE(c4_to_chars,  uint64_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  uint64_t);
BENCHMARK_TEMPLATE(std_to_string,  uint64_t);
BENCHMARK_TEMPLATE(sprintf,  uint64_t);
BENCHMARK_TEMPLATE(sstream_reuse, uint64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse, uint64_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream, uint64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream, uint64_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa,  int64_t);
BENCHMARK_TEMPLATE(c4_to_chars,  int64_t);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  int64_t);
BENCHMARK_TEMPLATE(std_to_string,  int64_t);
BENCHMARK_TEMPLATE(sprintf,  int64_t);
BENCHMARK_TEMPLATE(sstream_reuse,  int64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse,  int64_t, std::stringstream);
BENCHMARK_TEMPLATE(sstream,  int64_t, std::ostringstream);
BENCHMARK_TEMPLATE(sstream,  int64_t, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa,  float);
BENCHMARK_TEMPLATE(c4_to_chars,  float);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  float);
BENCHMARK_TEMPLATE(std_to_string,  float);
BENCHMARK_TEMPLATE(sprintf,  float);
BENCHMARK_TEMPLATE(sstream_reuse,  float, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse,  float, std::stringstream);
BENCHMARK_TEMPLATE(sstream,  float, std::ostringstream);
BENCHMARK_TEMPLATE(sstream,  float, std::stringstream);

BENCHMARK_TEMPLATE(c4_xtoa,  double);
BENCHMARK_TEMPLATE(c4_to_chars,  double);
BENCHMARK_TEMPLATE_CPP17(std_to_chars,  double);
BENCHMARK_TEMPLATE(std_to_string,  double);
BENCHMARK_TEMPLATE(sprintf,  double);
BENCHMARK_TEMPLATE(sstream_reuse,  double, std::ostringstream);
BENCHMARK_TEMPLATE(sstream_reuse,  double, std::stringstream);
BENCHMARK_TEMPLATE(sstream,  double, std::ostringstream);
BENCHMARK_TEMPLATE(sstream,  double, std::stringstream);


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
