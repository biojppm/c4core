#include "./bm_charconv.hpp"

#include <c4/ext/fast_float.hpp>
#if C4_CPP >= 17
#include <charconv>
#if defined(__cpp_lib_to_chars)
#define C4CORE_BM_HAVE_TOCHARS 1
#endif
#include <utility>
#endif

#ifdef C4CORE_BM_USE_RYU
#include <ryu/ryu.h>
#include <ryu/ryu_parse.h>
#endif

#ifdef C4CORE_BM_USE_FP
#include <jkj/fp/from_chars/from_chars.h>
#endif


// some of the benchmarks do not need to be templates,
// but it helps in the naming scheme.

// xtoa means <X> to string
// atox means string to <X>



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

C4FOR(T, isint)
c4_digits_dec(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += c4::digits_dec(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
c4_digits_hex(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += c4::digits_hex(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
c4_digits_oct(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += c4::digits_oct(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
c4_digits_bin(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += c4::digits_bin(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

C4FOR(T, isint)
atox_c4_read_dec(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::read_dec(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atoxhex_c4_read_hex(bm::State& st)
{
    random_strings_cref strings = mkstrings_hex_positive<T>(/*with_prefix*/false);
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::read_hex(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atoxoct_c4_read_oct(bm::State& st)
{
    random_strings_cref strings = mkstrings_oct_positive<T>(/*with_prefix*/false);
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::read_oct(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atoxbin_c4_read_bin(bm::State& st)
{
    random_strings_cref strings = mkstrings_bin_positive<T>(/*with_prefix*/false);
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::read_bin(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


C4FOR(T, isint)
xtoa_c4_write_dec(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::write_dec(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
xtoahex_c4_write_hex(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::write_hex(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
xtoaoct_c4_write_oct(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::write_oct(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
xtoabin_c4_write_bin(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::write_bin(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

C4FOR(T, isiint)
xtoa_c4_itoa(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::itoa(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isuint)
xtoa_c4_utoa(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::utoa(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isfloat)
xtoa_c4_ftoa(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::ftoa(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isdouble)
xtoa_c4_dtoa(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::dtoa(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
xtoa_c4_xtoa(bm::State& st)
{
    string_buffer buf;
    random_values<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::xtoa(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
xtoa_c4_xtoa(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::xtoa(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

C4FOR(T, isiint)
atox_c4_atoi(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::atoi(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isuint)
atox_c4_atou(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::atou(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isfloat)
atox_c4_atof(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::atof(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isdouble)
atox_c4_atod(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::atod(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_c4_atox(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::atox(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_c4_atox(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::atox(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

C4FOR(T, isint)
atox_std_atoi(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            val = (T) std::atoi(strings.next_s().c_str());
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_std_atol(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            val = (T) std::atol(strings.next_s().c_str());
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_std_atof(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            val = (T) std::atof(strings.next_s().c_str());
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

C4FOR(T, isint)
atox_std_strtol(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::string const& s = strings.next_s();
            val = (T) std::strtol(s.data(), nullptr, 10);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_std_strtoll(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::string const& s = strings.next_s();
            val = (T) std::strtoll(s.data(), nullptr, 10);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_std_strtoul(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::string const& s = strings.next_s();
            val = (T) std::strtoul(s.data(), nullptr, 10);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_std_strtoull(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::string const& s = strings.next_s();
            val = (T) std::strtoull(s.data(), nullptr, 10);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_std_strtof(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::string const& s = strings.next_s();
            val = (T) std::strtof(s.data(), nullptr);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_std_strtod(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::string const& s = strings.next_s();
            val = (T) std::strtod(s.data(), nullptr);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_std_stof(bm::State &st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            val = std::stof(strings.next_s());
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_std_stod(bm::State &st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            val = std::stod(strings.next_s());
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

#ifdef C4CORE_BM_USE_RYU
C4FOR(T, isfloat)
atox_ryu_s2f(bm::State &st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::csubstr s = strings.next();
            s2f_n(s.data(), (int) s.size(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isdouble)
atox_ryu_s2d(bm::State &st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::csubstr s = strings.next();
            s2d_n(s.data(), (int) s.size(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isfloat)
xtoa_ryu_f2s(bm::State &st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            f2s_buffered_n(values.next(), buf.buf.str);
    }
    bm::DoNotOptimize(buf.buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isdouble)
xtoa_ryu_d2s(bm::State &st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            d2s_buffered_n(values.next(), buf.buf.str);
    }
    bm::DoNotOptimize(buf.buf);
    report<T>(st, kNumValues);
}
#endif // C4CORE_BM_USE_RYU


//-----------------------------------------------------------------------------

// fp is still experimental and undocumented;
// some assertions are firing in debug builds
// so we make these benchmarks available only with NDEBUG
#if (defined(C4CORE_BM_USE_FP)) && (!defined(NDEBUG))
#undef C4CORE_BM_USE_FP
#endif

#ifdef C4CORE_BM_USE_FP
C4FOR(T, isreal)
atox_fp_from_chars_limited(bm::State &st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::csubstr s = strings.next();
            val = jkj::fp::from_chars_limited<T>(s.begin(), s.end()).to_float();
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}
#endif // C4CORE_BM_USE_FP

#ifdef C4CORE_BM_USE_FP
C4FOR(T, isreal)
atox_fp_from_chars_unlimited(bm::State &st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::csubstr s = strings.next();
            val = jkj::fp::from_chars_unlimited<T>(s.begin(), s.end()).to_float();
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}
#endif // C4CORE_BM_USE_FP


//-----------------------------------------------------------------------------

C4FOR(T, isreal)
atox_fast_float(bm::State &st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::csubstr s = strings.next();
            fast_float::from_chars(s.begin(), s.end(), val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

template<class T> struct fmtspec;
template<> struct fmtspec< uint8_t> { static const char w[], r[]; };
template<> struct fmtspec<  int8_t> { static const char w[], r[]; };
template<> struct fmtspec<uint16_t> { static const char w[], r[]; };
template<> struct fmtspec< int16_t> { static const char w[], r[]; };
template<> struct fmtspec<uint32_t> { static const char w[], r[]; };
template<> struct fmtspec< int32_t> { static const char w[], r[]; };
template<> struct fmtspec<uint64_t> { static const char w[], r[]; };
template<> struct fmtspec< int64_t> { static const char w[], r[]; };
template<> struct fmtspec< float  > { static const char w[], r[]; };
template<> struct fmtspec< double > { static const char w[], r[]; };

constexpr const char fmtspec< uint8_t>::w[] = "%" PRIu8 ;
constexpr const char fmtspec<  int8_t>::w[] = "%" PRIi8 ;
constexpr const char fmtspec<uint16_t>::w[] = "%" PRIu16;
constexpr const char fmtspec< int16_t>::w[] = "%" PRIi16;
constexpr const char fmtspec<uint32_t>::w[] = "%" PRIu32;
constexpr const char fmtspec< int32_t>::w[] = "%" PRIi32;
constexpr const char fmtspec<uint64_t>::w[] = "%" PRIu64;
constexpr const char fmtspec< int64_t>::w[] = "%" PRIi64;
constexpr const char fmtspec< float  >::w[] = "%g"      ;
constexpr const char fmtspec< double >::w[] = "%lg"     ;

constexpr const char fmtspec< uint8_t>::r[] = "%" SCNu8 ;
constexpr const char fmtspec<  int8_t>::r[] = "%" SCNi8 ;
constexpr const char fmtspec<uint16_t>::r[] = "%" SCNu16;
constexpr const char fmtspec< int16_t>::r[] = "%" SCNi16;
constexpr const char fmtspec<uint32_t>::r[] = "%" SCNu32;
constexpr const char fmtspec< int32_t>::r[] = "%" SCNi32;
constexpr const char fmtspec<uint64_t>::r[] = "%" SCNu64;
constexpr const char fmtspec< int64_t>::r[] = "%" SCNi64;
constexpr const char fmtspec< float  >::r[] = "%g"      ;
constexpr const char fmtspec< double >::r[] = "%lg"     ;

C4FOR(T, isint)
xtoa_sprintf(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals_positive<T>();
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            ::snprintf(buf.buf.str, buf.buf.len, fmtspec<T>::w, values.next());
    }
    bm::DoNotOptimize(buf.buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
xtoa_sprintf(bm::State& st)
{
    string_buffer buf;
    random_values_cref<T> values = mkvals<T>();
    C4_SUPPRESS_WARNING_GCC_CLANG_WITH_PUSH("-Wdouble-promotion")
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            ::snprintf(buf.buf.str, buf.buf.len, fmtspec<T>::w, values.next());
    }
    C4_SUPPRESS_WARNING_GCC_CLANG_POP
    bm::DoNotOptimize(buf.buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_scanf(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            ::sscanf(strings.next_s().c_str(), fmtspec<T>::r, &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_scanf(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            ::sscanf(strings.next_s().c_str(), fmtspec<T>::r, &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

C4FOR(T, isint)
xtoa_sstream(bm::State& st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    std::string out; C4_UNUSED(out);
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::stringstream ss;
            ss << values.next();
            out = ss.str();
        }
    }
    bm::DoNotOptimize(out);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
xtoa_sstream(bm::State& st)
{
    random_values_cref<T> values = mkvals<T>();
    std::string out; C4_UNUSED(out);
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::stringstream ss;
            ss << values.next();
            out = ss.str();
        }
    }
    bm::DoNotOptimize(out);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
xtoa_sstream_reuse(bm::State& st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    std::stringstream ss;
    std::string out; C4_UNUSED(out);
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            ss.clear();
            ss.str("");
            ss << values.next();
            out = ss.str();
        }
    }
    bm::DoNotOptimize(out);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
xtoa_sstream_reuse(bm::State& st)
{
    random_values_cref<T> values = mkvals<T>();
    std::stringstream ss;
    std::string out; C4_UNUSED(out);
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            ss.clear();
            ss.str("");
            ss << values.next();
            out = ss.str();
        }
    }
    bm::DoNotOptimize(out);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_sstream(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::stringstream ss(strings.next_s());
            ss >> val;
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_sstream(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::stringstream ss(strings.next_s());
            ss >> val;
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_sstream_reuse(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    std::stringstream ss;
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            ss.clear();
            ss.str(strings.next_s());
            ss >> val;
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_sstream_reuse(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    std::stringstream ss;
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            ss.clear();
            ss.str(strings.next_s());
            ss >> val;
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

C4FOR(T, isint)
xtoa_std_to_string(bm::State& st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    std::string out;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            out = std::to_string(values.next());
    }
    bm::DoNotOptimize(out);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
xtoa_std_to_string(bm::State& st)
{
    random_values_cref<T> values = mkvals<T>();
    std::string out;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            out = std::to_string(values.next());
    }
    bm::DoNotOptimize(out);
    report<T>(st, kNumValues);
}


//-----------------------------------------------------------------------------

C4FOR(T, isint)
xtoa_c4_to_chars(bm::State& st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    string_buffer buf;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::to_chars(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
xtoa_c4_to_chars(bm::State& st)
{
    random_values_cref<T> values = mkvals<T>();
    string_buffer buf;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            c4::to_chars(buf, values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_c4_from_chars(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::from_chars(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isint)
atox_c4_from_chars_checked(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::from_chars(strings.next(), c4::fmt::overflow_checked(val));
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

C4FOR(T, isreal)
atox_c4_from_chars(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::from_chars(strings.next(), &val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

#if (C4_CPP >= 17)
C4FOR(T, isint)
xtoa_std_to_chars(bm::State& st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    string_buffer buf;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            std::to_chars(buf.begin(), buf.end(), values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}
#endif

#if C4CORE_BM_HAVE_TOCHARS
C4FOR(T, isreal)
xtoa_std_to_chars(bm::State& st)
{
    random_values_cref<T> values = mkvals<T>();
    string_buffer buf;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            std::to_chars(buf.begin(), buf.end(), values.next());
    }
    bm::DoNotOptimize(buf);
    report<T>(st, kNumValues);
}
#endif

#if (C4_CPP >= 17)
C4FOR(T, isint)
atox_std_from_chars(bm::State& st)
{
    random_strings_cref strings = mkstrings_positive<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::csubstr s = strings.next();
            std::from_chars(s.begin(), s.end(), val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}
#endif

#if C4CORE_BM_HAVE_TOCHARS
C4FOR(T, isreal)
atox_std_from_chars(bm::State& st)
{
    random_strings_cref strings = mkstrings<T>();
    T val = {}, sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            c4::csubstr s = strings.next();
            std::from_chars(s.begin(), s.end(), val);
            sum += val;
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}
#endif


//-----------------------------------------------------------------------------

C4BM_TEMPLATE(c4_digits_dec,  uint8_t);
C4BM_TEMPLATE(c4_digits_hex,  uint8_t);
C4BM_TEMPLATE(c4_digits_oct,  uint8_t);
C4BM_TEMPLATE(c4_digits_bin,  uint8_t);

C4BM_TEMPLATE(c4_digits_dec,  int8_t);
C4BM_TEMPLATE(c4_digits_hex,  int8_t);
C4BM_TEMPLATE(c4_digits_oct,  int8_t);
C4BM_TEMPLATE(c4_digits_bin,  int8_t);

C4BM_TEMPLATE(c4_digits_dec,  uint16_t);
C4BM_TEMPLATE(c4_digits_hex,  uint16_t);
C4BM_TEMPLATE(c4_digits_oct,  uint16_t);
C4BM_TEMPLATE(c4_digits_bin,  uint16_t);

C4BM_TEMPLATE(c4_digits_dec,  int16_t);
C4BM_TEMPLATE(c4_digits_hex,  int16_t);
C4BM_TEMPLATE(c4_digits_oct,  int16_t);
C4BM_TEMPLATE(c4_digits_bin,  int16_t);

C4BM_TEMPLATE(c4_digits_dec,  uint32_t);
C4BM_TEMPLATE(c4_digits_hex,  uint32_t);
C4BM_TEMPLATE(c4_digits_oct,  uint32_t);
C4BM_TEMPLATE(c4_digits_bin,  uint32_t);

C4BM_TEMPLATE(c4_digits_dec,  int32_t);
C4BM_TEMPLATE(c4_digits_hex,  int32_t);
C4BM_TEMPLATE(c4_digits_oct,  int32_t);
C4BM_TEMPLATE(c4_digits_bin,  int32_t);

C4BM_TEMPLATE(c4_digits_dec,  uint64_t);
C4BM_TEMPLATE(c4_digits_hex,  uint64_t);
C4BM_TEMPLATE(c4_digits_oct,  uint64_t);
C4BM_TEMPLATE(c4_digits_bin,  uint64_t);

C4BM_TEMPLATE(c4_digits_dec,  int64_t);
C4BM_TEMPLATE(c4_digits_hex,  int64_t);
C4BM_TEMPLATE(c4_digits_oct,  int64_t);
C4BM_TEMPLATE(c4_digits_bin,  int64_t);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

C4BM_TEMPLATE(xtoahex_c4_write_hex,  uint8_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  uint8_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  uint8_t);

C4BM_TEMPLATE(xtoahex_c4_write_hex,  int8_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  int8_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  int8_t);

C4BM_TEMPLATE(xtoahex_c4_write_hex,  uint16_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  uint16_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  uint16_t);

C4BM_TEMPLATE(xtoahex_c4_write_hex,  int16_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  int16_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  int16_t);

C4BM_TEMPLATE(xtoahex_c4_write_hex,  uint32_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  uint32_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  uint32_t);

C4BM_TEMPLATE(xtoahex_c4_write_hex,  int32_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  int32_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  int32_t);

C4BM_TEMPLATE(xtoahex_c4_write_hex,  uint64_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  uint64_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  uint64_t);

C4BM_TEMPLATE(xtoahex_c4_write_hex,  int64_t);
C4BM_TEMPLATE(xtoaoct_c4_write_oct,  int64_t);
C4BM_TEMPLATE(xtoabin_c4_write_bin,  int64_t);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
C4BM_TEMPLATE(xtoa_c4_write_dec,  uint8_t);
C4BM_TEMPLATE(xtoa_c4_utoa,  uint8_t);
C4BM_TEMPLATE(xtoa_c4_xtoa,  uint8_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  uint8_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  uint8_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  uint8_t);
C4BM_TEMPLATE(xtoa_sprintf,  uint8_t);
C4BM_TEMPLATE(xtoa_sstream_reuse,  uint8_t);
C4BM_TEMPLATE(xtoa_sstream,  uint8_t);

C4BM_TEMPLATE(xtoa_c4_write_dec,  int8_t);
C4BM_TEMPLATE(xtoa_c4_itoa,   int8_t);
C4BM_TEMPLATE(xtoa_c4_xtoa,   int8_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  int8_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  int8_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  int8_t);
C4BM_TEMPLATE(xtoa_sprintf,  int8_t);
C4BM_TEMPLATE(xtoa_sstream_reuse,   int8_t);
C4BM_TEMPLATE(xtoa_sstream,   int8_t);

C4BM_TEMPLATE(xtoa_c4_write_dec,  uint16_t);
C4BM_TEMPLATE(xtoa_c4_utoa, uint16_t);
C4BM_TEMPLATE(xtoa_c4_xtoa, uint16_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  uint16_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  uint16_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  uint16_t);
C4BM_TEMPLATE(xtoa_sprintf,  uint16_t);
C4BM_TEMPLATE(xtoa_sstream_reuse, uint16_t);
C4BM_TEMPLATE(xtoa_sstream, uint16_t);

C4BM_TEMPLATE(xtoa_c4_write_dec,  int16_t);
C4BM_TEMPLATE(xtoa_c4_itoa,  int16_t);
C4BM_TEMPLATE(xtoa_c4_xtoa,  int16_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  int16_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  int16_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  int16_t);
C4BM_TEMPLATE(xtoa_sprintf,  int16_t);
C4BM_TEMPLATE(xtoa_sstream_reuse,  int16_t);
C4BM_TEMPLATE(xtoa_sstream,  int16_t);

C4BM_TEMPLATE(xtoa_c4_write_dec,  uint32_t);
C4BM_TEMPLATE(xtoa_c4_utoa, uint32_t);
C4BM_TEMPLATE(xtoa_c4_xtoa, uint32_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  uint32_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  uint32_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  uint32_t);
C4BM_TEMPLATE(xtoa_sprintf,  uint32_t);
C4BM_TEMPLATE(xtoa_sstream_reuse, uint32_t);
C4BM_TEMPLATE(xtoa_sstream, uint32_t);

C4BM_TEMPLATE(xtoa_c4_write_dec,  int32_t);
C4BM_TEMPLATE(xtoa_c4_itoa,  int32_t);
C4BM_TEMPLATE(xtoa_c4_xtoa,  int32_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  int32_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  int32_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  int32_t);
C4BM_TEMPLATE(xtoa_sprintf,  int32_t);
C4BM_TEMPLATE(xtoa_sstream_reuse,  int32_t);
C4BM_TEMPLATE(xtoa_sstream,  int32_t);

C4BM_TEMPLATE(xtoa_c4_write_dec,  uint64_t);
C4BM_TEMPLATE(xtoa_c4_utoa, uint64_t);
C4BM_TEMPLATE(xtoa_c4_xtoa, uint64_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  uint64_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  uint64_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  uint64_t);
C4BM_TEMPLATE(xtoa_sprintf,  uint64_t);
C4BM_TEMPLATE(xtoa_sstream_reuse, uint64_t);
C4BM_TEMPLATE(xtoa_sstream, uint64_t);

C4BM_TEMPLATE(xtoa_c4_write_dec,  int64_t);
C4BM_TEMPLATE(xtoa_c4_itoa,  int64_t);
C4BM_TEMPLATE(xtoa_c4_xtoa,  int64_t);
C4BM_TEMPLATE(xtoa_c4_to_chars,  int64_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(xtoa_std_to_chars,  int64_t);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  int64_t);
C4BM_TEMPLATE(xtoa_sprintf,  int64_t);
C4BM_TEMPLATE(xtoa_sstream_reuse,  int64_t);
C4BM_TEMPLATE(xtoa_sstream,  int64_t);

C4BM_TEMPLATE(xtoa_c4_ftoa,  float);
C4BM_TEMPLATE(xtoa_c4_xtoa,  float);
C4BM_TEMPLATE(xtoa_c4_to_chars,  float);
#ifdef C4CORE_BM_USE_RYU
C4BM_TEMPLATE(xtoa_ryu_f2s,  float);
#endif
#ifdef C4CORE_BM_HAVE_TOCHARS
C4BM_TEMPLATE_TO_CHARS_FLOAT(xtoa_std_to_chars,  float);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  float);
C4BM_TEMPLATE(xtoa_sprintf,  float);
C4BM_TEMPLATE(xtoa_sstream_reuse,  float);
C4BM_TEMPLATE(xtoa_sstream,  float);

C4BM_TEMPLATE(xtoa_c4_dtoa,  double);
C4BM_TEMPLATE(xtoa_c4_xtoa,  double);
C4BM_TEMPLATE(xtoa_c4_to_chars,  double);
#ifdef C4CORE_BM_USE_RYU
C4BM_TEMPLATE(xtoa_ryu_d2s,  double);
#endif
#ifdef C4CORE_BM_HAVE_TOCHARS
C4BM_TEMPLATE_TO_CHARS_FLOAT(xtoa_std_to_chars,  double);
#endif
C4BM_TEMPLATE(xtoa_std_to_string,  double);
C4BM_TEMPLATE(xtoa_sprintf,  double);
C4BM_TEMPLATE(xtoa_sstream_reuse,  double);
C4BM_TEMPLATE(xtoa_sstream,  double);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

C4BM_TEMPLATE(atoxhex_c4_read_hex, uint8_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, uint8_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, uint8_t);

C4BM_TEMPLATE(atoxhex_c4_read_hex, int8_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, int8_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, int8_t);

C4BM_TEMPLATE(atoxhex_c4_read_hex, uint16_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, uint16_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, uint16_t);

C4BM_TEMPLATE(atoxhex_c4_read_hex, int16_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, int16_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, int16_t);

C4BM_TEMPLATE(atoxhex_c4_read_hex, uint32_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, uint32_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, uint32_t);

C4BM_TEMPLATE(atoxhex_c4_read_hex, int32_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, int32_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, int32_t);

C4BM_TEMPLATE(atoxhex_c4_read_hex, uint64_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, uint64_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, uint64_t);

C4BM_TEMPLATE(atoxhex_c4_read_hex, int64_t);
C4BM_TEMPLATE(atoxoct_c4_read_oct, int64_t);
C4BM_TEMPLATE(atoxbin_c4_read_bin, int64_t);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


C4BM_TEMPLATE(atox_c4_read_dec, uint8_t);
C4BM_TEMPLATE(atox_c4_atou,  uint8_t);
C4BM_TEMPLATE(atox_c4_atox,  uint8_t);
C4BM_TEMPLATE(atox_c4_from_chars, uint8_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, uint8_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, uint8_t);
#endif
C4BM_TEMPLATE(atox_std_atoi,   uint8_t);
C4BM_TEMPLATE(atox_std_strtoul,   uint8_t);
C4BM_TEMPLATE(atox_scanf,   uint8_t);
C4BM_TEMPLATE(atox_sstream_reuse,   uint8_t);
C4BM_TEMPLATE(atox_sstream,   uint8_t);

C4BM_TEMPLATE(atox_c4_read_dec, int8_t);
C4BM_TEMPLATE(atox_c4_atoi,   int8_t);
C4BM_TEMPLATE(atox_c4_atox,   int8_t);
C4BM_TEMPLATE(atox_c4_from_chars, int8_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, int8_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, int8_t);
#endif
C4BM_TEMPLATE(atox_std_atoi,   int8_t);
C4BM_TEMPLATE(atox_std_strtol,   int8_t);
C4BM_TEMPLATE(atox_scanf,   int8_t);
C4BM_TEMPLATE(atox_sstream_reuse,   int8_t);
C4BM_TEMPLATE(atox_sstream,   int8_t);

C4BM_TEMPLATE(atox_c4_read_dec, uint16_t);
C4BM_TEMPLATE(atox_c4_atou, uint16_t);
C4BM_TEMPLATE(atox_c4_atox, uint16_t);
C4BM_TEMPLATE(atox_c4_from_chars, uint16_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, uint16_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, uint16_t);
#endif
C4BM_TEMPLATE(atox_std_atoi,   uint16_t);
C4BM_TEMPLATE(atox_std_strtoul,   uint16_t);
C4BM_TEMPLATE(atox_scanf,   uint16_t);
C4BM_TEMPLATE(atox_sstream_reuse,   uint16_t);
C4BM_TEMPLATE(atox_sstream,   uint16_t);

C4BM_TEMPLATE(atox_c4_read_dec, int16_t);
C4BM_TEMPLATE(atox_c4_atoi,  int16_t);
C4BM_TEMPLATE(atox_c4_atox,  int16_t);
C4BM_TEMPLATE(atox_c4_from_chars, int16_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, int16_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, int16_t);
#endif
C4BM_TEMPLATE(atox_std_atoi,   int16_t);
C4BM_TEMPLATE(atox_std_strtol,   int16_t);
C4BM_TEMPLATE(atox_scanf,   int16_t);
C4BM_TEMPLATE(atox_sstream_reuse,   int16_t);
C4BM_TEMPLATE(atox_sstream,   int16_t);

C4BM_TEMPLATE(atox_c4_read_dec, uint32_t);
C4BM_TEMPLATE(atox_c4_atou, uint32_t);
C4BM_TEMPLATE(atox_c4_atox, uint32_t);
C4BM_TEMPLATE(atox_c4_from_chars, uint32_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, uint32_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, uint32_t);
#endif
C4BM_TEMPLATE(atox_std_atoi,   uint32_t);
C4BM_TEMPLATE(atox_std_strtoul,   uint32_t);
C4BM_TEMPLATE(atox_scanf,   uint32_t);
C4BM_TEMPLATE(atox_sstream_reuse,   uint32_t);
C4BM_TEMPLATE(atox_sstream,   uint32_t);

C4BM_TEMPLATE(atox_c4_read_dec, int32_t);
C4BM_TEMPLATE(atox_c4_atoi,  int32_t);
C4BM_TEMPLATE(atox_c4_atox,  int32_t);
C4BM_TEMPLATE(atox_c4_from_chars, int32_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, int32_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, int32_t);
#endif
C4BM_TEMPLATE(atox_std_atoi,   int32_t);
C4BM_TEMPLATE(atox_std_strtol,   int32_t);
C4BM_TEMPLATE(atox_scanf,   int32_t);
C4BM_TEMPLATE(atox_sstream_reuse,   int32_t);
C4BM_TEMPLATE(atox_sstream,   int32_t);

C4BM_TEMPLATE(atox_c4_read_dec, uint64_t);
C4BM_TEMPLATE(atox_c4_atou, uint64_t);
C4BM_TEMPLATE(atox_c4_atox, uint64_t);
C4BM_TEMPLATE(atox_c4_from_chars, uint64_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, uint64_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, uint64_t);
#endif
C4BM_TEMPLATE(atox_std_atol,   uint64_t);
C4BM_TEMPLATE(atox_std_strtoull,   uint64_t);
C4BM_TEMPLATE(atox_scanf,   uint64_t);
C4BM_TEMPLATE(atox_sstream_reuse,   uint64_t);
C4BM_TEMPLATE(atox_sstream,   uint64_t);

C4BM_TEMPLATE(atox_c4_read_dec, int64_t);
C4BM_TEMPLATE(atox_c4_atoi,  int64_t);
C4BM_TEMPLATE(atox_c4_atox,  int64_t);
C4BM_TEMPLATE(atox_c4_from_chars, int64_t);
C4BM_TEMPLATE(atox_c4_from_chars_checked, int64_t);
#if (C4_CPP >= 17)
C4BM_TEMPLATE_TO_CHARS_INT(atox_std_from_chars, int64_t);
#endif
C4BM_TEMPLATE(atox_std_atol,   int64_t);
C4BM_TEMPLATE(atox_std_strtoll,   int64_t);
C4BM_TEMPLATE(atox_scanf,   int64_t);
C4BM_TEMPLATE(atox_sstream_reuse,   int64_t);
C4BM_TEMPLATE(atox_sstream,   int64_t);

C4BM_TEMPLATE(atox_c4_atof,  float);
C4BM_TEMPLATE(atox_c4_atox,  float);
C4BM_TEMPLATE(atox_c4_from_chars, float);
C4BM_TEMPLATE(atox_fast_float,  float);
#ifdef C4CORE_BM_HAVE_TOCHARS
C4BM_TEMPLATE_TO_CHARS_FLOAT(atox_std_from_chars, float);
#endif
#ifdef C4CORE_BM_USE_RYU
C4BM_TEMPLATE(atox_ryu_s2f,  float);
#endif
#ifdef C4CORE_BM_USE_FP
C4BM_FP_BENCHMARK(atox_fp_from_chars_limited,  float);
C4BM_FP_BENCHMARK(atox_fp_from_chars_unlimited,  float);
#endif
C4BM_TEMPLATE(atox_std_atof,   float);
C4BM_TEMPLATE(atox_std_strtof,   float);
C4BM_TEMPLATE(atox_std_stof,   float);
C4BM_TEMPLATE(atox_scanf,   float);
C4BM_TEMPLATE(atox_sstream_reuse,   float);
C4BM_TEMPLATE(atox_sstream,   float);

C4BM_TEMPLATE(atox_c4_atod,  double);
C4BM_TEMPLATE(atox_c4_atox,  double);
C4BM_TEMPLATE(atox_c4_from_chars, double);
C4BM_TEMPLATE(atox_fast_float,  double);
#ifdef C4CORE_BM_HAVE_TOCHARS
C4BM_TEMPLATE_TO_CHARS_FLOAT(atox_std_from_chars, double);
#endif
#ifdef C4CORE_BM_USE_RYU
C4BM_TEMPLATE(atox_ryu_s2d,  double);
#endif
#ifdef C4CORE_BM_USE_FP
C4BM_FP_BENCHMARK(atox_fp_from_chars_limited,  double);
C4BM_FP_BENCHMARK(atox_fp_from_chars_unlimited,  double);
#endif
C4BM_TEMPLATE(atox_std_atof,   double);
C4BM_TEMPLATE(atox_std_strtod,   double);
C4BM_TEMPLATE(atox_std_stod,   double);
C4BM_TEMPLATE(atox_scanf,   double);
C4BM_TEMPLATE(atox_sstream_reuse,   double);
C4BM_TEMPLATE(atox_sstream,   double);


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
