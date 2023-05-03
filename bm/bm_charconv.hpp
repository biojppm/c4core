#include <string>
#include <sstream>
#include <c4/c4_push.hpp>
#include <c4/substr.hpp>
#include <c4/std/std.hpp>
#include <c4/charconv.hpp>
#include <c4/format.hpp>
#include <c4/memory_util.hpp>
#include <inttypes.h>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <c4/ext/rng/rng.hpp>

inline double getmax(std::vector<double> const& v)
{
    return *(std::max_element(std::begin(v), std::end(v)));
}
inline double getmin(std::vector<double> const& v)
{
    return *(std::min_element(std::begin(v), std::end(v)));
}
inline double getrange(std::vector<double> const& v)
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

#define C4BM_TEMPLATE(fn, ...) BENCHMARK_TEMPLATE(fn, __VA_ARGS__) _c4bm_stats


// benchmarks depending on c++17 features are disabled using the
// preprocessor.  google benchmark has state.SkipWithError() but it
// makes the program return a nonzero exit code when it finishes. So
// we must resort to the preprocessor to conditionally disable these
// benchmarks
#if defined(__cpp_lib_to_chars) || (C4_CPP >= 17)
#define C4BM_TEMPLATE_TO_CHARS_INT(fn, ...) BENCHMARK_TEMPLATE(fn, __VA_ARGS__) _c4bm_stats
#define C4BM_TEMPLATE_TO_CHARS_FLOAT(fn, ...) BENCHMARK_TEMPLATE(fn, __VA_ARGS__) _c4bm_stats
#else
#define C4BM_TEMPLATE_TO_CHARS_INT(fn, ...) void shutup_extra_semicolon()
#define C4BM_TEMPLATE_TO_CHARS_FLOAT(fn, ...) void shutup_extra_semicolon()
#endif

C4_SUPPRESS_WARNING_GCC_CLANG_PUSH
C4_SUPPRESS_WARNING_GCC_CLANG("-Wdouble-promotion")
C4_SUPPRESS_WARNING_GCC_CLANG("-Wdeprecated")
C4_SUPPRESS_WARNING_GCC_CLANG("-Wsign-conversion")
C4_SUPPRESS_WARNING_GCC_CLANG("-Wconversion")
C4_SUPPRESS_WARNING_GCC_CLANG("-Wold-style-cast")
#if defined(__GNUC__) && (__GNUC__ >= 6)
C4_SUPPRESS_WARNING_GCC_WITH_PUSH("-Wunused-const-variable")
#endif
#include <benchmark/benchmark.h>
#if defined(__GNUC__) && (__GNUC__ >= 6)
C4_SUPPRESS_WARNING_GCC_POP
#endif


namespace bm = benchmark;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// utilities for use in the benchmarks below


// facilities to deuglify SFINAE
#define C4FOR(ty, condition)                            \
    template<class ty>                                  \
    typename std::enable_if<condition(ty), void>::type
#define C4FOR2(ty1, ty2, condition)                             \
    template<class ty1, class ty2>                              \
    typename std::enable_if<condition(ty1), void>::type

#define isint(ty) std::is_integral<ty>::value
#define isiint(ty) std::is_integral<ty>::value && !std::is_unsigned<ty>::value
#define isuint(ty) std::is_integral<ty>::value && std::is_unsigned<ty>::value
#define isreal(ty) std::is_floating_point<ty>::value
#define isfloat(ty) std::is_same<ty, float>::value
#define isdouble(ty) std::is_same<ty, double>::value


/** convenience wrapper to avoid boilerplate code */
template<class T>
void report(bm::State &st, size_t numvals=1)
{
    int64_t iters = (int64_t)(st.iterations() * numvals);
    int64_t bytes = (int64_t)(st.iterations() * numvals * sizeof(T));
    st.SetBytesProcessed(bytes);
    st.SetItemsProcessed(iters);
}
template<class T>
void report_threadavg(bm::State &st, size_t numvals=1)
{
    int64_t iters = (int64_t)(st.iterations() * numvals);
    int64_t bytes = (int64_t)(st.iterations() * numvals * sizeof(T));
    st.SetBytesProcessed(bytes / (int64_t)st.threads());
    st.SetItemsProcessed(iters / (int64_t)st.threads());
}

template<class T>
constexpr bool is_pot(T val)
{
    return val > 0 && (val & (val-T(1))) == 0;
}

constexpr const uint64_t kSeed = 37;
constexpr const size_t kNumValues = 1u<<20; // 1.049M
C4_STATIC_ASSERT(is_pot(kNumValues));


template<class T, class Eng, class Dist>
T gen(Dist &dist, Eng &eng)
{
    return static_cast<T>(dist(eng));
}

template<class T, class Eng, class Dist>
T gen_pos(Dist &dist, Eng &eng)
{
    T val = static_cast<T>(dist(eng));
    while(val <= T(0))
        val = static_cast<T>(dist(eng));
    return val;
}

/** generate in place a random sequence of values: integral version*/
C4FOR(T, isint)
generate_n(T *begin, T *end)
{
    // do not use T in the distribution:
    //  N4659 29.6.1.1 [rand.req.genl]/1e requires one of short, int, long, long long, unsigned short, unsigned int, unsigned long, or unsigned long long
    std::uniform_int_distribution<int64_t> idist;
    c4::rng::pcg rng(kSeed);
    for(; begin != end; ++begin)
        *begin = gen<T>(idist, rng);
}

C4FOR(T, isint)
generate_n_positive(T *begin, T *end)
{
    // do not use T in the distribution:
    //  N4659 29.6.1.1 [rand.req.genl]/1e requires one of short, int, long, long long, unsigned short, unsigned int, unsigned long, or unsigned long long
    std::uniform_int_distribution<uint32_t> idist;
    c4::rng::pcg rng(kSeed);
    for(; begin != end; ++begin)
        *begin = gen_pos<T>(idist, rng);
}

/** generate in place a random sequence of values: real-number version*/
C4FOR(T, isreal)
generate_n(T *begin, T *end)
{
    c4::rng::pcg rng(kSeed);
    // make sure we also have some integral numbers in the real sequence
    T *rstart = begin + (std::distance(begin, end) / 20); // 5% integral numbers
    std::uniform_int_distribution<uint32_t> idist;
    std::uniform_real_distribution<T> rdist;
    for(; begin != rstart; ++begin)
        *begin = gen<T>(idist, rng);
    for(; begin != end; ++begin)
        *begin = gen<T>(rdist, rng);
}

/** generate in place a random sequence of values: real-number version*/
C4FOR(T, isreal)
generate_n_positive(T *begin, T *end)
{
    c4::rng::pcg rng(kSeed);
    // make sure we also have some integral numbers in the real sequence
    T *rstart = begin + (std::distance(begin, end) / 20); // 5% integral numbers
    std::uniform_int_distribution<int32_t> idist;
    std::uniform_real_distribution<T> rdist;
    for(; begin != rstart; ++begin)
        *begin = gen_pos<T>(idist, rng);
    for(; begin != end; ++begin)
        *begin = gen_pos<T>(rdist, rng);
}


/** a ring buffer with input values for xtoa benchmarks */
template<class T>
struct random_values
{
    std::vector<T> v;
    mutable size_t curr;
    size_t szm1;
    T next() const { T f = v[curr]; curr = (curr + 1) & szm1; return f; }
    random_values(bool positive_only, size_t sz) : v(sz), curr(0), szm1(sz)
    {
        C4_CHECK(is_pot(sz));
        if(positive_only)
            generate_n_positive<T>(&v.front(), &v.back());
        else
            generate_n<T>(&v.front(), &v.back());
    }
};
template<class T>
using random_values_cref = random_values<T> const&;

template<class T>
random_values_cref<T> mkvals()
{
    static random_values<T> vals(/*positive_only*/false, kNumValues);
    return vals;
}
template<class T>
random_values_cref<T> mkvals_positive()
{
    static random_values<T> vals(/*positive_only*/true, kNumValues);
    return vals;
}


/** a ring buffer with input strings for atox benchmarks */
struct random_strings
{
    std::vector<std::string> v_s;
    std::vector<c4::csubstr> v;
    std::vector<char> arena;
    mutable size_t curr;
    size_t szm1;

    C4_HOT C4_ALWAYS_INLINE c4::csubstr next() const noexcept { c4::csubstr f = v[curr]; curr = (curr + 1) & szm1; return f; }
    C4_HOT C4_ALWAYS_INLINE std::string const& next_s() const noexcept { std::string const& f = v_s[curr]; curr = (curr + 1) & szm1; return f; }

    random_strings() = default;

    template<class T>
    void _init(random_values<T> const& tmp)
    {
        C4_CHECK(is_pot(tmp.v.size()));
        v.resize(tmp.v.size());
        v_s.resize(tmp.v.size());
        curr = 0;
        szm1 = tmp.v.size() - 1;
    }
    void _build_arena()
    {
        size_t sum = 0;
        for(std::string const& s : v_s)
            sum += s.size();
        sum += v_s.size();
        v.resize(v_s.size());
        arena.resize(sum);
        size_t pos = 0;
        size_t i = 0;
        for(std::string const& s : v_s)
        {
            memcpy(&arena[pos], s.data(), s.size());
            v[i++] = c4::csubstr(&arena[pos], s.size());
            pos += s.size();
            arena[pos++] = '\0';
        }
    }

    template<class T>
    void init_as(random_values<T> const& tmp)
    {
        _init(tmp);
        for(size_t i = 0; i < v.size(); ++i)
            c4::catrs(&v_s[i], tmp.v[i]);
        _build_arena();
    }
    template<class T>
    void init_as_hex(random_values<T> const& tmp, bool with_prefix)
    {
        _init(tmp);
        for(size_t i = 0; i < v.size(); ++i)
        {
            c4::catrs(&v_s[i], c4::fmt::hex(tmp.v[i]));
            if(!with_prefix)
                _erase_radix_prefix(&v_s[i]);
        }
        _build_arena();
    }
    template<class T>
    void init_as_oct(random_values<T> const& tmp, bool with_prefix)
    {
        _init(tmp);
        for(size_t i = 0; i < v.size(); ++i)
        {
            c4::catrs(&v_s[i], c4::fmt::oct(tmp.v[i]));
            if(!with_prefix)
                _erase_radix_prefix(&v_s[i]);
        }
        _build_arena();
    }
    template<class T>
    void init_as_bin(random_values<T> const& tmp, bool with_prefix)
    {
        _init(tmp);
        for(size_t i = 0; i < v.size(); ++i)
        {
            c4::catrs(&v_s[i], c4::fmt::bin(tmp.v[i]));
            if(!with_prefix)
                _erase_radix_prefix(&v_s[i]);
        }
        _build_arena();
    }

    static void _erase_radix_prefix(std::string *s)
    {
        C4_ASSERT(s->front() != '-');
        s->erase(0, 2);
    }
};
using random_strings_cref = random_strings const&;

template<class T>
random_strings_cref mkstrings()
{
    static random_strings rs;
    if(rs.v.empty())
        rs.init_as<T>(mkvals<T>());
    return rs;
}
template<class T>
random_strings_cref mkstrings_positive()
{
    static random_strings rs;
    if(rs.v.empty())
        rs.init_as<T>(mkvals_positive<T>());
    return rs;
}
template<class T>
random_strings_cref mkstrings_hex(bool with_prefix=true)
{
    static random_strings rs;
    static random_strings rs_wo_prefix;
    if(with_prefix)
    {
        if(rs.v.empty())
            rs.init_as_hex<T>(mkvals<T>());
        return rs;
    }
    else
    {
        if(rs_wo_prefix.v.empty())
            rs_wo_prefix.init_as_hex<T>(mkvals<T>(), false);
        return rs_wo_prefix;
    }
}
template<class T>
random_strings_cref mkstrings_hex_positive(bool with_prefix=true)
{
    static random_strings rs;
    static random_strings rs_wo_prefix;
    if(with_prefix)
    {
        if(rs.v.empty())
            rs.init_as_hex<T>(mkvals_positive<T>(), true);
        return rs;
    }
    else
    {
        if(rs_wo_prefix.v.empty())
            rs_wo_prefix.init_as_hex<T>(mkvals_positive<T>(), false);
        return rs_wo_prefix;
    }
}
template<class T>
random_strings_cref mkstrings_oct(bool with_prefix=true)
{
    static random_strings rs;
    static random_strings rs_wo_prefix;
    if(with_prefix)
    {
        if(rs.v.empty())
            rs.init_as_oct<T>(mkvals<T>(), true);
        return rs;
    }
    else
    {
        if(rs_wo_prefix.v.empty())
            rs_wo_prefix.init_as_oct<T>(mkvals<T>(), false);
        return rs_wo_prefix;
    }
}
template<class T>
random_strings_cref mkstrings_oct_positive(bool with_prefix=true)
{
    static random_strings rs;
    static random_strings rs_wo_prefix;
    if(with_prefix)
    {
        if(rs.v.empty())
            rs.init_as_oct<T>(mkvals_positive<T>(), true);
        return rs;
    }
    else
    {
        if(rs_wo_prefix.v.empty())
            rs_wo_prefix.init_as_oct<T>(mkvals_positive<T>(), false);
        return rs_wo_prefix;
    }
}
template<class T>
random_strings_cref mkstrings_bin(bool with_prefix=true)
{
    static random_strings rs;
    static random_strings rs_wo_prefix;
    if(with_prefix)
    {
        if(rs.v.empty())
            rs.init_as_bin<T>(mkvals<T>(), true);
        return rs;
    }
    else
    {
        if(rs_wo_prefix.v.empty())
            rs_wo_prefix.init_as_bin<T>(mkvals<T>(), false);
        return rs_wo_prefix;
    }
}
template<class T>
random_strings_cref mkstrings_bin_positive(bool with_prefix=true)
{
    static random_strings rs;
    static random_strings rs_wo_prefix;
    if(with_prefix)
    {
        if(rs.v.empty())
            rs.init_as_bin<T>(mkvals_positive<T>(), true);
        return rs;
    }
    else
    {
        if(rs_wo_prefix.v.empty())
            rs_wo_prefix.init_as_bin<T>(mkvals_positive<T>(), false);
        return rs_wo_prefix;
    }
}


/** a character buffer, easily convertible to c4::substr */
template<size_t Dim=128>
struct sbuf
{
    char buf_[Dim];
    c4::substr buf;
    sbuf() : buf_(), buf(buf_) {}
    C4_HOT C4_ALWAYS_INLINE operator c4::substr& () { return buf; }
    char* begin() { return buf.begin(); }
    char* end() { return buf.end(); }
};

using string_buffer = sbuf<>;

#define C4DOALL(n) for(size_t elm##__LINE__ = 0; elm##__LINE__ < n; ++elm##__LINE__)
