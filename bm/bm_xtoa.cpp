#include "./bm_charconv.hpp"
#include <c4/error.hpp>
#include <iostream>


// this is an exploratory benchmark to compare the possible
// combinations for all the components of the write_dec() algorithm


template<class T> using msb_func = unsigned (*)(T val);

#if defined(__GNUC__)
#   pragma GCC diagnostic push
#   if __GNUC__ >= 8
#       pragma GCC diagnostic ignored "-Wstringop-truncation"
#       pragma GCC diagnostic ignored "-Wstringop-overflow"
#   endif
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// WIP.
// TODO: _BitscanReverse() in MSVC
C4_SUPPRESS_WARNING_GCC_WITH_PUSH("-Wuseless-cast")
C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto msb_intrinsic(unsigned v) noexcept
 -> typename std::enable_if<__has_builtin(__builtin_clz), unsigned>::type
{
    using I = unsigned;
    enum : I { total = (I)(I(8) * sizeof(I) - 1) };
    return (total - (I) __builtin_clz(v));
}

C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto msb_intrinsic_64bit(unsigned long v) noexcept
-> typename std::enable_if<__has_builtin(__builtin_clzl), unsigned>::type
{
    using I = unsigned long;
    enum : I { total = (I)(I(8) * sizeof(I) - 1) };
    return (unsigned)(total - (I) __builtin_clzl(v));
}

C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto msb_intrinsic_64bit(unsigned long long v) noexcept
    -> typename std::enable_if<__has_builtin(__builtin_clzll), unsigned>::type
{
    using I = unsigned long long;
    enum : I { total = (I)(I(8) * sizeof(I) - 1) };
    return (unsigned)(total - (I) __builtin_clzll(v));
}
C4_SUPPRESS_WARNING_GCC_POP

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_loop(I v) noexcept
{
    // https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10Obvious
    unsigned r = 0;
    while (v >>= 1) // unroll for more speed...
        r++;
    return r;
}

// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup
constexpr static const int8_t LogTable256[256] = {
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
    LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
#undef LT
};
template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
unsigned msb_de_bruijn_32bit(I v) noexcept
{
    I t, tt; // temporaries
    tt = v >> 16;
    if (tt)
        return (unsigned)((t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt]);
    return (unsigned)((t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v]);
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
unsigned msb_intrinsic_8bit(I v) noexcept
{
    return msb_intrinsic((unsigned)v);
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
unsigned msb_divconq_8bit(I v) noexcept
{
    C4_STATIC_ASSERT(sizeof(I) == 1);
    unsigned n = 0;
    if(v & I(0xf0)) v >>= 4, n |= I(4);
    if(v & I(0x0c)) v >>= 2, n |= I(2);
    if(v & I(0x02)) v >>= 1, n |= I(1);
    return n;
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_intrinsic_16bit(I v) noexcept
{
    return msb_intrinsic((unsigned)v);
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_divconq_16bit(I v) noexcept
{
    C4_STATIC_ASSERT(sizeof(I) == 2);
    unsigned n = 0;
    if(v & I(0xff00)) v >>= 8, n |= I(8);
    if(v & I(0x00f0)) v >>= 4, n |= I(4);
    if(v & I(0x000c)) v >>= 2, n |= I(2);
    if(v & I(0x0002)) v >>= 1, n |= I(1);
    return n;
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_intrinsic_32bit(I v) noexcept
{
    return msb_intrinsic((unsigned)v);
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_divconq_32bit(I v) noexcept
{
    C4_STATIC_ASSERT(sizeof(I) == 4);
    unsigned n = 0;
    if(v & I(0xffff0000)) v >>= 16, n |= I(16);
    if(v & I(0x0000ff00)) v >>= 8, n |= I(8);
    if(v & I(0x000000f0)) v >>= 4, n |= I(4);
    if(v & I(0x0000000c)) v >>= 2, n |= I(2);
    if(v & I(0x00000002)) v >>= 1, n |= I(1);
    return n;
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_divconq_branchless_32bit(I v) noexcept
{
    C4_STATIC_ASSERT(sizeof(I) == 4);
    unsigned r =     (unsigned)(v > 0xFFFF) << 4; v >>= r;
    unsigned shift = (unsigned)(v > 0xFF  ) << 3; v >>= shift; r |= shift;
             shift = (unsigned)(v > 0xF   ) << 2; v >>= shift; r |= shift;
             shift = (unsigned)(v > 0x3   ) << 1; v >>= shift; r |= shift;
                                                               r |= (unsigned)(v >> 1);
    return r;
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_intrinsic_64bit(I v) noexcept
{
    return msb_intrinsic_64bit((uint64_t)v);
}

template<class I>
C4_CONSTEXPR14 C4_ALWAYS_INLINE unsigned msb_divconq_64bit(I v) noexcept
{
    C4_STATIC_ASSERT(sizeof(I) == 8);
    unsigned n = 0;
    if(v & I(0xffffffff00000000)) v >>= 32, n |= I(32);
    if(v & I(0x00000000ffff0000)) v >>= 16, n |= I(16);
    if(v & I(0x000000000000ff00)) v >>= 8, n |= I(8);
    if(v & I(0x00000000000000f0)) v >>= 4, n |= I(4);
    if(v & I(0x000000000000000c)) v >>= 2, n |= I(2);
    if(v & I(0x0000000000000002)) v >>= 1, n |= I(1);
    return n;
}


template<class T, msb_func<T> msbfunc>
void msb(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += msbfunc(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


C4BM_TEMPLATE(msb, uint8_t, msb_intrinsic_8bit<uint8_t>);
C4BM_TEMPLATE(msb, uint8_t, msb_divconq_8bit<uint8_t>);
C4BM_TEMPLATE(msb, uint8_t, msb_loop<uint8_t>);

C4BM_TEMPLATE(msb, int8_t, msb_intrinsic_8bit<int8_t>);
C4BM_TEMPLATE(msb, int8_t, msb_divconq_8bit<int8_t>);
C4BM_TEMPLATE(msb, int8_t, msb_loop<int8_t>);

C4BM_TEMPLATE(msb, uint16_t, msb_intrinsic_16bit<uint16_t>);
C4BM_TEMPLATE(msb, uint16_t, msb_divconq_16bit<uint16_t>);
C4BM_TEMPLATE(msb, uint16_t, msb_loop<uint16_t>);

C4BM_TEMPLATE(msb, int16_t, msb_intrinsic_16bit<int16_t>);
C4BM_TEMPLATE(msb, int16_t, msb_divconq_16bit<int16_t>);
C4BM_TEMPLATE(msb, int16_t, msb_loop<int16_t>);

C4BM_TEMPLATE(msb, uint32_t, msb_intrinsic_32bit<uint32_t>);
C4BM_TEMPLATE(msb, uint32_t, msb_de_bruijn_32bit<uint32_t>);
C4BM_TEMPLATE(msb, uint32_t, msb_divconq_32bit<uint32_t>);
C4BM_TEMPLATE(msb, uint32_t, msb_divconq_branchless_32bit<uint32_t>);
C4BM_TEMPLATE(msb, uint32_t, msb_loop<uint32_t>);

C4BM_TEMPLATE(msb, int32_t, msb_intrinsic_32bit<int32_t>);
C4BM_TEMPLATE(msb, int32_t, msb_de_bruijn_32bit<int32_t>);
C4BM_TEMPLATE(msb, int32_t, msb_divconq_32bit<int32_t>);
C4BM_TEMPLATE(msb, int32_t, msb_divconq_branchless_32bit<int32_t>);
C4BM_TEMPLATE(msb, int32_t, msb_loop<int32_t>);

C4BM_TEMPLATE(msb, uint64_t, msb_intrinsic_64bit<uint64_t>);
C4BM_TEMPLATE(msb, uint64_t, msb_divconq_64bit<uint64_t>);
C4BM_TEMPLATE(msb, uint64_t, msb_loop<uint64_t>);

C4BM_TEMPLATE(msb, int64_t, msb_intrinsic_64bit<int64_t>);
C4BM_TEMPLATE(msb, int64_t, msb_divconq_64bit<int64_t>);
C4BM_TEMPLATE(msb, int64_t, msb_loop<int64_t>);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace impl {

template<typename _Tp, unsigned __base>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto digits_glibc(_Tp __value) noexcept
    -> typename std::enable_if<std::is_unsigned<_Tp>::value, unsigned>::type
{
    static_assert(std::is_integral<_Tp>::value, "implementation bug");
    static_assert(std::is_unsigned<_Tp>::value, "implementation bug");
    unsigned __n = 1;
    const unsigned __b2 = __base  * __base;
    const unsigned __b3 = __b2 * __base;
    const unsigned long __b4 = __b3 * __base;
    for (;;)
	{
        if (__value < (unsigned)__base) return __n;
        if (__value < __b2) return __n + 1;
        if (__value < __b3) return __n + 2;
        if (__value < __b4) return __n + 3;
        __value /= (_Tp) __b4;
        __n += 4;
	}
}

template<typename _Tp>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto digits_glibc(_Tp __value, unsigned __base) noexcept
    -> typename std::enable_if<std::is_unsigned<_Tp>::value, unsigned>::type
{
    static_assert(std::is_integral<_Tp>::value, "implementation bug");
    static_assert(std::is_unsigned<_Tp>::value, "implementation bug");
    unsigned __n = 1;
    const unsigned __b2 = __base  * __base;
    const unsigned __b3 = __b2 * __base;
    const unsigned long __b4 = __b3 * __base;
    for (;;)
	{
        if (__value < (unsigned)__base) return __n;
        if (__value < __b2) return __n + 1;
        if (__value < __b3) return __n + 2;
        if (__value < __b4) return __n + 3;
        __value /= (_Tp)__b4;
        __n += 4;
	}
}

template<typename _Tp, unsigned __base>
constexpr C4_ALWAYS_INLINE
auto digits_glibc(_Tp __value) noexcept
    -> typename std::enable_if<std::is_signed<_Tp>::value, unsigned>::type
{
    using U = typename std::make_unsigned<_Tp>::type;
    return digits_glibc<U, __base>((U)__value);
}

template<typename _Tp>
constexpr C4_ALWAYS_INLINE
auto digits_glibc(_Tp __value, unsigned __base) noexcept
    -> typename std::enable_if<std::is_signed<_Tp>::value, unsigned>::type
{
    using U = typename std::make_unsigned<_Tp>::type;
    return digits_glibc<U>((U)__value, __base);
}


//-------------------------------------------
// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10Obvious

// these functions assume the numbers are positive even when the type
// is signed

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_hifirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==1u,unsigned>::type
{
    // best when the numbers are uniformly distributed over the whole range
    return ((v >= 100) ? 3u : ((v >= 10) ? 2u : 1u));
}

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_lofirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==1u,unsigned>::type
{
    // best when lower numbers are more likely
    return ((v < 10) ? 1u : ((v < 100) ? 2u : 3u));
}


// 16 bit

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_hifirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==2u,unsigned>::type
{
    // best when the numbers are uniformly distributed over the whole range
    return ((v >= 10000) ? 5u : (v >= 1000) ? 4u : (v >= 100) ? 3u : (v >= 10) ? 2u : 1u);
}

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_lofirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==2u,unsigned>::type
{
    // best when lower numbers are more likely
    return ((v < 100) ? ((v >= 10) ? 2u : 1u) : ((v < 1000) ? 3u : ((v < 10000) ? 4u : 5u)));
}


// 32 bit

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_hifirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==4u,unsigned>::type
{
    return ((v >= 1000000000) ? 10u : (v >= 100000000) ? 9u : (v >= 10000000) ? 8u :
            (v >= 1000000) ? 7u : (v >= 100000) ? 6u : (v >= 10000) ? 5u :
            (v >= 1000) ? 4u : (v >= 100) ? 3u : (v >= 10) ? 2u : 1u);
}

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_lofirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==4u,unsigned>::type
{
    return ((v < 10) ? 1u : (v < 100) ? 2u : (v < 1000) ? 3u : (v < 10000) ? 4u :
            (v < 100000) ? 5u : (v < 1000000) ? 6u : (v < 10000000) ? 7u :
            (v < 100000000) ? 8u : (v < 1000000000) ? 9u : 10u);
}


// 64 bit

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_hifirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==8u,unsigned>::type
{
    return ((std::is_unsigned<T>::value && v >= T(10000000000000000000u)) ? 20u :
            (v >= 1000000000000000000) ? 19u : (v >= 100000000000000000) ? 18u : (v >= 10000000000000000) ? 17u :
            (v >= 1000000000000000) ? 16u : (v >= 100000000000000) ? 15u : (v >= 10000000000000) ? 14u :
            (v >= 1000000000000) ? 13u : (v >= 100000000000) ? 12u : (v >= 10000000000) ? 11u :
            (v >= 1000000000) ? 10u : (v >= 100000000) ? 9u : (v >= 10000000) ? 8u :
            (v >= 1000000) ? 7u : (v >= 100000) ? 6u : (v >= 10000) ? 5u :
            (v >= 1000) ? 4u : (v >= 100) ? 3u : (v >= 10) ? 2u : 1u);
}

template<class T>
constexpr C4_ALWAYS_INLINE
auto digits_dec_naive_lofirst(T v) noexcept
    -> typename std::enable_if<sizeof(T)==8u,unsigned>::type
{
    return ((v < 10) ? 1u : (v < 100) ? 2u : (v < 1000) ? 3u :
            (v < 10000) ? 4u : (v < 100000) ? 5u : (v < 1000000) ? 6u :
            (v < 10000000) ? 7u : (v < 100000000) ? 8u : (v < 1000000000) ? 9u :
            (v < 10000000000) ? 10u : (v < 100000000000) ? 11u : (v < 1000000000000) ? 12u :
            (v < 10000000000000) ? 13u : (v < 100000000000000) ? 14u : (v < 1000000000000000) ? 15u :
            (v < 10000000000000000) ? 16u : (v < 100000000000000000) ? 17u : (v < 1000000000000000000) ? 18u :
            ((typename std::make_unsigned<T>::type)v < 10000000000000000000u) ? 19u : 20u);
}

template<class T>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto digits_dec_naive_hifirst64fallback32(T v) noexcept
    -> typename std::enable_if<sizeof(T)==8u,unsigned>::type
{
    if(v >= std::numeric_limits<uint32_t>::max())
        return digits_glibc<T, 10u>(v);
    else
        return ((v >= 1000000000) ? 10u : (v >= 100000000) ? 9u : (v >= 10000000) ? 8u :
                (v >= 1000000) ? 7u : (v >= 100000) ? 6u : (v >= 10000) ? 5u :
                (v >= 1000) ? 4u : (v >= 100) ? 3u : (v >= 10) ? 2u : 1u);
}

template<class T>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto digits_dec_naive_lofirst64fallback32(T v) noexcept
    -> typename std::enable_if<sizeof(T)==8u,unsigned>::type
{
    if(v < std::numeric_limits<uint32_t>::max())
        return ((v < 10) ? 1u : (v < 100) ? 2u : (v < 1000) ? 3u : (v < 10000) ? 4u :
                (v < 100000) ? 5u : (v < 1000000) ? 6u : (v < 10000000) ? 7u :
                (v < 100000000) ? 8u : (v < 1000000000) ? 9u : 10u);
    else
        return digits_glibc<T, 10u>(v);
}

template<class T>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
auto digits_dec_naive_fargies(T v) noexcept
    -> typename std::enable_if<sizeof(T)==8u,unsigned>::type
{
    // https://github.com/biojppm/c4core/pull/77#issuecomment-1063753568
    if(v >= 1000000000) // 10
    {
        if(v >= 100000000000000) // 15 [15-20] range
        {
            if(v >= 100000000000000000) // 18 (15 + (20 - 15) / 2)
            {
                if((typename std::make_unsigned<T>::type)v >= 10000000000000000000u) // 20
                    return 20u;
                else
                    return (v >= 1000000000000000000) ? 19u : 18u;
            }
            else if(v >= 10000000000000000) // 17
                return 17u;
            else
                return(v >= 1000000000000000) ? 16u : 15u;
        }
        else if(v >= 1000000000000) // 13
            return (v >= 10000000000000) ? 14u : 13u;
        else if(v >= 100000000000) // 12
            return 12;
        else
            return(v >= 10000000000) ? 11u : 10u;
    }
    else if(v >= 10000) // 5 [5-9] range
    {
        if(v >= 10000000) // 8
            return (v >= 100000000) ? 9u : 8u;
        else if(v >= 1000000) // 7
            return 7;
        else
            return (v >= 100000) ? 6u : 5u;
    }
    else if(v >= 100)
        return (v >= 1000) ? 4u : 3u;
    else
        return (v >= 10) ? 2u : 1u;
}


//-------------------------------------------
namespace c4 {
namespace detail {
template<class T, typename=void>
struct powers_of_10;

#define _C4_POWERS_OF_10_FOR(cond, ...)                                   \
template<class T>                                                       \
struct powers_of_10<T, typename std::enable_if<cond, void>::type> \
{                                                                       \
    static C4_INLINE_CONSTEXPR const T values[] = {__VA_ARGS__};        \
    static C4_INLINE_CONSTEXPR const T values_size = C4_COUNTOF(values); \
};                                                                      \
template<class T>                                                       \
C4_INLINE_CONSTEXPR const T powers_of_10<T, typename std::enable_if<cond, void>::type>::values[]

_C4_POWERS_OF_10_FOR(sizeof(T)==1u, 1, 10, 100 );
_C4_POWERS_OF_10_FOR(sizeof(T)==2u, 1, 10, 100, 1000, 10000 );
_C4_POWERS_OF_10_FOR(sizeof(T)==4u, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 );
_C4_POWERS_OF_10_FOR(std::is_signed<T>::value &&
                     sizeof(T)==8u, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000 );
_C4_POWERS_OF_10_FOR(std::is_unsigned<T>::value &&
                     sizeof(T)==8u, 1u, 10u, 100u, 1000u, 10000u, 100000u, 1000000u, 10000000u, 100000000u, 1000000000u, 10000000000u, 100000000000u, 1000000000000u, 10000000000000u, 100000000000000u, 1000000000000000u, 10000000000000000u, 100000000000000000u, 1000000000000000000u, 10000000000000000000u );
} // namespace detail
} // namespace c4


template<class T, msb_func<T> msbfunc>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
unsigned digits_dec_log10_nocheck(T v) noexcept
{
    // https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10
    const unsigned mag = ((unsigned)msbfunc(v) + 1u) * 1233u >> 12;
    C4_ASSERT(mag < c4::detail::powers_of_10<T>::values_size);
    return 1u + mag - (v < c4::detail::powers_of_10<T>::values[mag]);
}

template<class T, msb_func<T> msbfunc>
C4_CONSTEXPR14 C4_ALWAYS_INLINE
unsigned digits_dec_log10(T v) noexcept
{
    if(v)
    {
        // https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10
        const unsigned mag = ((unsigned)msbfunc(v) + 1u) * 1233u >> 12;
        C4_ASSERT(mag < c4::detail::powers_of_10<T>::values_size);
        return 1u + mag - (v < c4::detail::powers_of_10<T>::values[mag]);
    }
    return 1u;
}

} // namespace impl


template<class T>
void digits_dec_naive_fargies(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_dec_naive_fargies(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

template<class T>
void digits_dec_naive_hifirst(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_dec_naive_hifirst(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

template<class T>
void digits_dec_naive_lofirst(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_dec_naive_lofirst(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

template<class T>
void digits_dec_naive_hifirst64fallback32(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_dec_naive_hifirst64fallback32(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

template<class T>
void digits_dec_naive_lofirst64fallback32(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_dec_naive_lofirst64fallback32(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}


template<class T>
void digits_dec_glibc_tpl(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_glibc<T, 10>(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

template<class T>
void digits_dec_glibc(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_glibc(values.next(), 10);
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

template<class T, msb_func<T> msbfunc>
void digits_dec_log10_nocheck(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_dec_log10_nocheck<T, msbfunc>(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

template<class T, msb_func<T> msbfunc>
void digits_dec_log10(bm::State &st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    unsigned sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += impl::digits_dec_log10<T, msbfunc>(values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

#define C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(ty, num)                   \
C4BM_TEMPLATE(digits_dec_naive_hifirst,  ty);                           \
C4BM_TEMPLATE(digits_dec_naive_lofirst,  ty);                           \
C4BM_TEMPLATE(digits_dec_log10_nocheck, ty, msb_intrinsic_##num##bit<ty>); \
C4BM_TEMPLATE(digits_dec_log10_nocheck, ty, msb_divconq_##num##bit<ty>); \
C4BM_TEMPLATE(digits_dec_log10_nocheck, ty, msb_loop<ty>);              \
C4BM_TEMPLATE(digits_dec_log10, ty, msb_intrinsic_##num##bit<ty>);      \
C4BM_TEMPLATE(digits_dec_log10, ty, msb_divconq_##num##bit<ty>);        \
C4BM_TEMPLATE(digits_dec_log10, ty, msb_loop<ty>);                      \
C4BM_TEMPLATE(digits_dec_glibc_tpl,  ty);                               \
C4BM_TEMPLATE(digits_dec_glibc,  ty)


C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(uint8_t, 8);
C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(int8_t, 8);
C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(uint16_t, 16);
C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(int16_t, 16);
C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(uint32_t, 32);
C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(int32_t, 32);
C4BM_TEMPLATE(digits_dec_naive_fargies,  uint64_t);
C4BM_TEMPLATE(digits_dec_naive_hifirst64fallback32,  uint64_t);
C4BM_TEMPLATE(digits_dec_naive_lofirst64fallback32,  uint64_t);
C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(uint64_t, 64);
C4BM_TEMPLATE(digits_dec_naive_fargies,  int64_t);
C4BM_TEMPLATE(digits_dec_naive_hifirst64fallback32,  int64_t);
C4BM_TEMPLATE(digits_dec_naive_lofirst64fallback32,  int64_t);
C4_INSTANTIATE_DIGITS_DEC_BENCHMARKS(int64_t, 64);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace impl {

// LEGEND:

// checkall: check buffer length on every insertion
// checkonce: check buffer length only once when entering the function
// checkoncemax: as above, and compare against a compile-time maxdigits for the type
// checkoncelog: as above, and compare against the exact digits each from the actual number

// divrem: compute div with operator/ and rem with operator%
// singlediv: compute div with operator/ but rem without using operator% (explicitly compute the remainder)

// write1: write 1 digit per division (divide by 10 on each step)
// write2: write 2 digits per division (divide by 100 on each step)


static constexpr const char digits0099[201] =
    "0001020304050607080910111213141516171819"
    "2021222324252627282930313233343536373839"
    "4041424344454647484950515253545556575859"
    "6061626364656667686970717273747576777879"
    "8081828384858687888990919293949596979899";


//-------------------------------------------------------------------

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkall_divrem_write1(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    size_t pos = 0;
    do {
        if(C4_LIKELY(pos < buf.len))
            buf.str[pos] = (char)('0' + (v % T(10)));
        ++pos;
        v /= T(10);
    } while(v);
    buf.reverse_range(0, pos);
    return pos;
}

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkall_divrem_write2(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    size_t pos = 0;
    while(v >= T(100))
	{
        const auto num = (v % T(100)) << 1u;
        v /= T(100);
        if(C4_LIKELY(pos + 2 < buf.len))
        {
            buf.str[pos++] = digits0099[num + 1];
            buf.str[pos++] = digits0099[num];
        }
	}
    if(v >= T(10))
	{
        const auto num = v << 1u;
        if(C4_LIKELY(pos + 2 < buf.len))
        {
            buf.str[pos++] = digits0099[num + 1];
            buf.str[pos++] = digits0099[num];
        }
	}
    else
    {
        if(C4_LIKELY(pos < buf.len))
            buf.str[pos++] = (char)('0' + v);
    }
    buf.reverse_range(0, pos);
    return pos;
}


//-------------------------------------------------------------------

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkall_singlediv_write1(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    size_t pos = 0;
    do {
        const T quo = v / T(10);
        const auto rem = v - quo * T(10);
        v = quo;
        if(C4_LIKELY(pos < buf.len))
            buf.str[pos] = (char)('0' + rem);
        ++pos;
    } while(v);
    buf.reverse_range(0, pos);
    return pos;
}

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkall_singlediv_write2(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    size_t pos = 0;
    while(v >= T(100))
	{
        const T quo = v / T(100);
        const auto num = (v - quo * T(100)) << 1u;
        v = quo;
        if(C4_LIKELY(pos+2 < buf.len))
        {
            buf.str[pos++] = digits0099[num + 1];
            buf.str[pos++] = digits0099[num];
        }
	}
    if(v >= T(10))
	{
        const auto num = v << 1u;
        if(C4_LIKELY(pos+2 < buf.len))
        {
            buf.str[pos++] = digits0099[num + 1];
            buf.str[pos++] = digits0099[num    ];
        }
	}
    else
    {
        if(C4_LIKELY(pos < buf.len))
            buf.str[pos++] = (char)('0' + v);
    }
    buf.reverse_range(0, pos);
    return pos;
}


//-------------------------------------------------------------------

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkoncemax_divrem_write1(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    if(C4_UNLIKELY(buf.len < c4::detail::charconv_digits<T>::maxdigits_dec))
        return c4::detail::charconv_digits<T>::maxdigits_dec;
    size_t pos = 0;
    do {
        buf.str[pos++] = (char)('0' + (v % T(10)));
        v /= T(10);
    } while(v);
    buf.reverse_range(0, pos);
    return pos;
}

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkoncemax_divrem_write2(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    if(C4_UNLIKELY(buf.len < c4::detail::charconv_digits<T>::maxdigits_dec))
        return c4::detail::charconv_digits<T>::maxdigits_dec;
    size_t pos = 0;
    while(v >= T(100))
	{
        const auto num = (v % T(100)) << 1u;
        v /= T(100);
        buf.str[pos++] = digits0099[num + 1];
        buf.str[pos++] = digits0099[num];
	}
    if(v >= T(10))
	{
        const auto num = v << 1u;
        buf.str[pos++] = digits0099[num + 1];
        buf.str[pos++] = digits0099[num    ];
	}
    else
    {
        buf.str[pos++] = (char)('0' + v);
    }
    buf.reverse_range(0, pos);
    return pos;
}


//-------------------------------------------------------------------

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkoncemax_singlediv_write1(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    if(C4_UNLIKELY(buf.len < c4::detail::charconv_digits<T>::maxdigits_dec))
        return c4::detail::charconv_digits<T>::maxdigits_dec;
    size_t pos = 0;
    do {
        const T quo = v / T(10);
        const auto rem = (v - quo * T(10));
        v = quo;
        buf.str[pos++] = (char)('0' + rem);
    } while(v);
    buf.reverse_range(0, pos);
    return pos;
}

template<class T>
C4_ALWAYS_INLINE size_t write_dec_checkoncemax_singlediv_write2(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    size_t pos = 0;
    if(C4_UNLIKELY(buf.len < c4::detail::charconv_digits<T>::maxdigits_dec))
        return c4::detail::charconv_digits<T>::maxdigits_dec;
    while(v >= T(100))
	{
        const T quo = v / T(100);
        const auto num = (v - quo * T(100)) << 1u;
        v = quo;
        buf.str[pos++] = digits0099[num + 1];
        buf.str[pos++] = digits0099[num];
	}
    if(v >= T(10))
	{
        const auto num = v << 1u;
        buf.str[pos++] = digits0099[num + 1];
        buf.str[pos++] = digits0099[num    ];
	}
    else
    {
        buf.str[pos++] = (char)('0' + v);
    }
    buf.reverse_range(0, pos);
    return pos;
}


//-------------------------------------------------------------------

template<class T, msb_func<T> digitsfunc>
C4_ALWAYS_INLINE size_t write_dec_checkoncelog_divrem_write1(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    unsigned digits = digitsfunc(v);
    if(C4_UNLIKELY(buf.len < digits))
        return digits;
    size_t pos = digits;
    do {
        buf.str[--pos] = (char)('0' + (v % T(10)));
        v /= T(10);
    } while(v);
    return digits;
}

template<class T, msb_func<T> digitsfunc>
C4_ALWAYS_INLINE size_t write_dec_checkoncelog_divrem_write2(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    unsigned digits = digitsfunc(v);
    if(C4_UNLIKELY(buf.len < digits))
        return digits;
    size_t pos = digits;
    while(v >= T(100))
	{
        const auto num = (v % T(100)) << 1u;
        v /= T(100);
        buf.str[--pos] = digits0099[num + 1];
        buf.str[--pos] = digits0099[num];
	}
    if(v >= T(10))
	{
        const auto num = v << 1u;
        buf.str[1] = digits0099[num + 1];
        buf.str[0] = digits0099[num];
	}
    else
    {
        buf.str[0] = (char)('0' + v);
    }
    return digits;
}


//-------------------------------------------------------------------

template<class T, msb_func<T> digitsfunc>
C4_ALWAYS_INLINE size_t write_dec_checkoncelog_singlediv_write1(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    unsigned digits = digitsfunc(v);
    if(C4_UNLIKELY(buf.len < digits))
        return digits;
    size_t pos = digits;
    do {
        const T quo = v / T(10);
        const auto rem = (v - quo * T(10));
        v = quo;
        buf.str[--pos] = (char)('0' + rem);
    } while(v);
    return digits;
}

template<class T, msb_func<T> digitsfunc>
C4_ALWAYS_INLINE size_t write_dec_checkoncelog_singlediv_write2(c4::substr buf, T v) noexcept
{
    C4_STATIC_ASSERT(std::is_integral<T>::value);
    C4_ASSERT(v >= 0);
    unsigned digits = digitsfunc(v);
    if(C4_UNLIKELY(buf.len < digits))
        return digits;
    size_t pos = digits;
    while(v >= T(100))
	{
        const T quo = v / T(100);
        const auto num = (v - quo * T(100)) << 1u;
        v = quo;
        buf.str[--pos] = digits0099[num + 1];
        buf.str[--pos] = digits0099[num];
	}
    if(v >= T(10))
	{
        const auto num = v << 1u;
        buf.str[1] = digits0099[num + 1];
        buf.str[0] = digits0099[num    ];
	}
    else
    {
        buf.str[0] = (char)('0' + v);
    }
    return digits;
}
} // namespace impl



#define C4_DEFINE_WRITE_DEC_BM(name)                        \
template<class T>                                           \
void write_dec_##name(bm::State &st)                        \
{                                                           \
    random_values_cref<T> values = mkvals_positive<T>();    \
    string_buffer buf_ = {};                                \
    c4::substr buf = buf_;                                  \
    C4_ASSERT(buf.len > 11);                                \
    size_t sum = {};                                        \
    for(auto _ : st)                                        \
    {                                                       \
        C4DOALL(kNumValues)                                 \
            sum += impl::write_dec_##name<T>(buf, values.next());   \
    }                                                       \
    bm::DoNotOptimize(sum);                                 \
    report<T>(st, kNumValues);                              \
}

#define C4_DEFINE_WRITE_DEC_BM_FUNC(name)                   \
template<class T, msb_func<T> msbfunc>                      \
void write_dec_##name(bm::State &st)                        \
{                                                           \
    random_values_cref<T> values = mkvals_positive<T>();    \
    string_buffer buf_ = {};                                \
    c4::substr buf = buf_;                                  \
    C4_ASSERT(buf.len > 11);                                \
    size_t sum = {};                                        \
    for(auto _ : st)                                        \
    {                                                       \
        C4DOALL(kNumValues)                                 \
            sum += impl::write_dec_##name<T, msbfunc>(buf, values.next());    \
    }                                                       \
    bm::DoNotOptimize(sum);                                 \
    report<T>(st, kNumValues);                              \
}


C4FOR(T, isint)
write_dec_c4_write_dec(bm::State& st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    string_buffer buf_ = {};
    c4::substr buf = buf_;
    size_t sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
            sum += c4::write_dec(buf, values.next());
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}

#if defined(__cpp_lib_to_chars) || (C4_CPP >= 17)
#define C4_TO_CHARS_BM(ty) C4BM_TEMPLATE(write_dec_std_to_chars, ty)
C4FOR(T, isint)
write_dec_std_to_chars(bm::State& st)
{
    random_values_cref<T> values = mkvals_positive<T>();
    string_buffer buf_ = {};
    c4::substr buf = buf_;
    size_t sum = {};
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            auto result = std::to_chars(buf.begin(), buf.end(), values.next());
            sum += (size_t)(result.ptr - buf.str);
        }
    }
    bm::DoNotOptimize(sum);
    report<T>(st, kNumValues);
}
#else
#define C4_TO_CHARS_BM(ty)
#endif


C4_DEFINE_WRITE_DEC_BM_FUNC(checkoncelog_singlediv_write2)
C4_DEFINE_WRITE_DEC_BM_FUNC(checkoncelog_singlediv_write1)
C4_DEFINE_WRITE_DEC_BM_FUNC(checkoncelog_divrem_write2)
C4_DEFINE_WRITE_DEC_BM_FUNC(checkoncelog_divrem_write1)

C4_DEFINE_WRITE_DEC_BM(checkoncemax_singlediv_write2)
C4_DEFINE_WRITE_DEC_BM(checkoncemax_singlediv_write1)
C4_DEFINE_WRITE_DEC_BM(checkoncemax_divrem_write2)
C4_DEFINE_WRITE_DEC_BM(checkoncemax_divrem_write1)

C4_DEFINE_WRITE_DEC_BM(checkall_singlediv_write2)
C4_DEFINE_WRITE_DEC_BM(checkall_singlediv_write1)
C4_DEFINE_WRITE_DEC_BM(checkall_divrem_write2)
C4_DEFINE_WRITE_DEC_BM(checkall_divrem_write1)



#define C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(ty, num)                    \
                                                                        \
/*compare against std::to_chars()*/                                     \
C4_TO_CHARS_BM(ty);                                                     \
                                                                        \
/*our versions*/                                                        \
C4BM_TEMPLATE(write_dec_c4_write_dec, ty);                              \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_naive_hifirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_naive_lofirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_glibc<ty, 10u>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>); \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_naive_hifirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_naive_lofirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_glibc<ty C4_COMMA 10u>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>); \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_naive_hifirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_naive_lofirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_glibc<ty C4_COMMA 10u>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>); \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_naive_hifirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_naive_lofirst<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_glibc<ty C4_COMMA 10u>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>); \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncemax_singlediv_write2, ty);             \
C4BM_TEMPLATE(write_dec_checkoncemax_singlediv_write1, ty);             \
C4BM_TEMPLATE(write_dec_checkoncemax_divrem_write2, ty);                \
C4BM_TEMPLATE(write_dec_checkoncemax_divrem_write1, ty);                \
                                                                        \
C4BM_TEMPLATE(write_dec_checkall_singlediv_write2, ty);                 \
C4BM_TEMPLATE(write_dec_checkall_singlediv_write1, ty);                 \
C4BM_TEMPLATE(write_dec_checkall_divrem_write2, ty);                    \
C4BM_TEMPLATE(write_dec_checkall_divrem_write1, ty)



#define C4_INSTANTIATE_WRITE_DEC_BENCHMARKS64(ty)                       \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_naive_fargies<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_naive_hifirst64fallback32<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write2, ty, impl::digits_dec_naive_lofirst64fallback32<ty>); \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_naive_fargies<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_naive_hifirst64fallback32<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_singlediv_write1, ty, impl::digits_dec_naive_lofirst64fallback32<ty>); \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_naive_fargies<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_naive_hifirst64fallback32<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write2, ty, impl::digits_dec_naive_lofirst64fallback32<ty>); \
                                                                        \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_naive_fargies<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_naive_hifirst64fallback32<ty>); \
C4BM_TEMPLATE(write_dec_checkoncelog_divrem_write1, ty, impl::digits_dec_naive_lofirst64fallback32<ty>)


C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(uint8_t, 8);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(int8_t, 8);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(uint16_t, 16);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(int16_t, 16);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(uint32_t, 32);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(int32_t, 32);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS64(uint64_t);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(uint64_t, 64);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS64(int64_t);
C4_INSTANTIATE_WRITE_DEC_BENCHMARKS(int64_t, 64);


#if defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<class T> struct xtoacase { c4::csubstr str; T val; };
#define _(num) {c4::csubstr(#num), num##u}
xtoacase<uint8_t> cases_8bit[] = {
    _(0),
    _(1),
    _(9),
    _(10),
    _(11),
    _(98),
    _(99),
    _(100),
    _(101),
};
xtoacase<uint16_t> cases_16bit[] = {
    _(999),
    _(1000),
    _(1001),
    _(9999),
    _(10000),
    _(10001),
};
xtoacase<uint32_t> cases_32bit[] = {
    _(99999),
    _(100000),
    _(100001),
    _(999999),
    _(1000000),
    _(1000001),
    _(9999999),
    _(10000000),
    _(10000001),
    _(99999999),
    _(100000000),
    _(100000001),
    _(999999999),
    _(1000000000),
    _(1000000001),
};
xtoacase<uint64_t> cases_64bit[] = {
    _(9999999999),
    _(10000000000),
    _(10000000001),
    _(99999999999),
    _(100000000000),
    _(100000000001),
    _(999999999999),
    _(1000000000000),
    _(1000000000001),
    _(9999999999999),
    _(10000000000000),
    _(10000000000001),
    _(99999999999999),
    _(100000000000000),
    _(100000000000001),
    _(999999999999999),
    _(1000000000000000),
    _(1000000000000001),
    _(9999999999999999),
    _(10000000000000000),
    _(10000000000000001),
    _(99999999999999999),
    _(100000000000000000),
    _(100000000000000001),
    _(999999999999999999),
    _(1000000000000000000),
    _(1000000000000000001),
    _(9223372036854775807),
};
xtoacase<uint64_t> cases_64bitu[] = {
    _(9999999999999999999),
    _(10000000000000000000),
    _(18446744073709551615),
};
#undef _

bool logtest = true;
bool printok = false;
bool testfail = false;
#define C4_CHECK_(lhs, op, rhs, ...)                            \
    {                                                           \
    if(!((lhs) op (rhs)))                                       \
    {                                                           \
        std::cout << __FILE__ << ":" << __LINE__                \
                  << ": failed! " << #lhs " " #op " " #rhs "\n" \
                  << "   " #lhs "=" << (lhs) << "\n"            \
                  << "   " #rhs "=" << (rhs) << "\n"            \
                  << "   " << __VA_ARGS__ << "\n";              \
        testfail = true;                                        \
    }                                                           \
    else if(printok)                                            \
    {                                                           \
        std::cout << __FILE__ << ":" << __LINE__                \
                  << ": ok!     " << #lhs " " #op " " #rhs      \
                  << " " << __VA_ARGS__ << "\n";                \
    }                                                           \
    }

C4_SUPPRESS_WARNING_CLANG("-Wgnu-zero-variadic-macro-arguments")
#define C4_CHECK_LT(lhs, rhs, ...) C4_CHECK_(lhs, <, rhs, ## __VA_ARGS__)
#define C4_CHECK_LE(lhs, rhs, ...) C4_CHECK_(lhs, <=, rhs, ## __VA_ARGS__)
#define C4_CHECK_GT(lhs, rhs, ...) C4_CHECK_(lhs, >, rhs, ## __VA_ARGS__)
#define C4_CHECK_GE(lhs, rhs, ...) C4_CHECK_(lhs, >=, rhs, ## __VA_ARGS__)
#define C4_CHECK_EQ(lhs, rhs, ...) C4_CHECK_(lhs, ==, rhs, ## __VA_ARGS__)
#define C4_CHECK_NE(lhs, rhs, ...) C4_CHECK_(lhs, !=, rhs, ## __VA_ARGS__)

#define DO_TEST_DIGITS_(ty, fn, num) \
    if(logtest) std::cout << "\ntesting: " #fn "\n";   \
    test_digits##num<ty, fn>(); \
    if(logtest) std::cout << "success: " #fn "\n"

#define DO_TEST_WRITE_(ty, fn, num) \
    if(logtest) std::cout << "\ntesting: " #fn "\n";  \
    test_write##num<ty>(&fn); \
    if(logtest) std::cout << "success: " #fn "\n"


template<class T, class U, class Func>
void test_write(xtoacase<U> c, Func fn)
{
    if(c.val == 0)
        return;
    C4_STATIC_ASSERT(sizeof(T) >= sizeof(U));
    char buf_[32] = {};
    c4::substr buf = buf_;
    C4_CHECK_GT(c.val, 0, c.str << "/" << (uint64_t)c.val);
    C4_CHECK_LE((U)c.val, (U)std::numeric_limits<T>::max(), c.str << "/" << (uint64_t)c.val);
    T val = (T)c.val;
    size_t ret = fn(buf, val);
    C4_CHECK_EQ(ret, c.str.len, c.str << "/" << (uint64_t)c.val << ": " << buf.first(ret));
    C4_CHECK_EQ(buf.first(ret), c.str, c.str << "/" << (uint64_t)c.val);
}

template<class T, msb_func<T> digitsfunc>
void test_digits8()
{
    for(auto c : cases_8bit)
        C4_CHECK_EQ(digitsfunc((T)c.val), c.str.len, (uint64_t)c.val);
}
template<class T, class Func>
void test_write8(Func func)
{
    for(auto c : cases_8bit)
        test_write<T>(c, func);
}

template<class T, msb_func<T> digitsfunc>
void test_digits16()
{
    test_digits8<T, digitsfunc>();
    for(auto c : cases_16bit)
        C4_CHECK_EQ(digitsfunc((T)c.val), c.str.len, (uint64_t)c.val);
}
template<class T, class Func>
void test_write16(Func func)
{
    test_write8<T>(func);
    for(auto c : cases_16bit)
        test_write<T>(c, func);
}

template<class T, msb_func<T> digitsfunc>
void test_digits32()
{
    test_digits8<T, digitsfunc>();
    test_digits16<T, digitsfunc>();
    for(auto c : cases_32bit)
        C4_CHECK_EQ(digitsfunc((T)c.val), c.str.len, (uint64_t)c.val);
}
template<class T, class Func>
void test_write32(Func func)
{
    test_write8<T>(func);
    test_write16<T>(func);
    for(auto c : cases_32bit)
        test_write<T>(c, func);
}

template<class T, msb_func<T> digitsfunc>
void test_digits64()
{
    test_digits8<T, digitsfunc>();
    test_digits16<T, digitsfunc>();
    test_digits32<T, digitsfunc>();
    for(auto c : cases_64bit)
        C4_CHECK_EQ(digitsfunc((T)c.val), c.str.len, (uint64_t)c.val);
    if(std::is_unsigned<T>::value)
        for(auto c : cases_64bitu)
            C4_CHECK_EQ(digitsfunc((T)c.val), c.str.len, (uint64_t)c.val << "/" << c.str);
}
template<class T, class Func>
auto test_write64(Func func)
    -> typename std::enable_if<std::is_unsigned<T>::value, void>::type
{
    test_write8<T>(func);
    test_write16<T>(func);
    test_write32<T>(func);
    for(auto c : cases_64bit)
        test_write<T>(c, func);
    for(auto c : cases_64bitu)
        test_write<T>(c, func);
}
template<class T, class Func>
auto test_write64(Func func)
    -> typename std::enable_if<std::is_signed<T>::value, void>::type
{
    test_write8<T>(func);
    test_write16<T>(func);
    test_write32<T>(func);
    for(auto c : cases_64bit)
        test_write<T>(c, func);
}

#define DO_TEST_DIGITS(ty, num)                        \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_hifirst<ty>, num); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_lofirst<ty>, num); \
DO_TEST_DIGITS_(ty, impl::digits_glibc<ty C4_COMMA 10u>, num); \
DO_TEST_DIGITS_(ty, impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>, num); \
DO_TEST_DIGITS_(ty, impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>, num); \
DO_TEST_DIGITS_(ty, impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>, num)

#define DO_TEST_DIGITS_64(ty) \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_fargies<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_hifirst64fallback32<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_lofirst64fallback32<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_fargies<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_hifirst64fallback32<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_lofirst64fallback32<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_fargies<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_hifirst64fallback32<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_lofirst64fallback32<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_fargies<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_hifirst64fallback32<ty>, 64); \
DO_TEST_DIGITS_(ty, impl::digits_dec_naive_lofirst64fallback32<ty>, 64)


#define DO_TEST_WRITE(ty, num)                                          \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_naive_hifirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_naive_lofirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_glibc<ty C4_COMMA 10u>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_naive_hifirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_naive_lofirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_glibc<ty C4_COMMA 10u>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_naive_hifirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_naive_lofirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_glibc<ty C4_COMMA 10u>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_naive_hifirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_naive_lofirst<ty>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_glibc<ty C4_COMMA 10u>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_log10_nocheck<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_intrinsic_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_divconq_##num##bit<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_log10<ty C4_COMMA msb_loop<ty>>>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncemax_singlediv_write2<ty>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncemax_singlediv_write1<ty>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncemax_divrem_write2<ty>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkoncemax_divrem_write1<ty>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkall_singlediv_write2<ty>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkall_singlediv_write1<ty>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkall_divrem_write2<ty>, num);\
DO_TEST_WRITE_(ty, impl::write_dec_checkall_divrem_write1<ty>, num)


#define DO_TEST_WRITE_64(ty)                                            \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_naive_fargies<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_naive_hifirst64fallback32<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write2<ty C4_COMMA impl::digits_dec_naive_lofirst64fallback32<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_naive_fargies<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_naive_hifirst64fallback32<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_singlediv_write1<ty C4_COMMA impl::digits_dec_naive_lofirst64fallback32<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_naive_fargies<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_naive_hifirst64fallback32<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write2<ty C4_COMMA impl::digits_dec_naive_lofirst64fallback32<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_naive_fargies<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_naive_hifirst64fallback32<ty>>, 64); \
DO_TEST_WRITE_(ty, impl::write_dec_checkoncelog_divrem_write1<ty C4_COMMA impl::digits_dec_naive_lofirst64fallback32<ty>>, 64)


void do_test()
{
    DO_TEST_DIGITS(uint8_t, 8);
    DO_TEST_DIGITS(int8_t, 8);
    DO_TEST_DIGITS(uint16_t, 16);
    DO_TEST_DIGITS(int16_t, 16);
    DO_TEST_DIGITS(uint32_t, 32);
    DO_TEST_DIGITS(int32_t, 32);
    DO_TEST_DIGITS(uint64_t, 64);
    DO_TEST_DIGITS(int64_t, 64);
    DO_TEST_DIGITS_64(uint64_t);
    DO_TEST_DIGITS_64(int64_t);
    printf("\n");
    DO_TEST_WRITE(uint8_t, 8);
    DO_TEST_WRITE(int8_t, 8);
    DO_TEST_WRITE(uint16_t, 16);
    DO_TEST_WRITE(int16_t, 16);
    DO_TEST_WRITE(uint32_t, 32);
    DO_TEST_WRITE(int32_t, 32);
    DO_TEST_WRITE(uint64_t, 64);
    DO_TEST_WRITE(int64_t, 64);
    DO_TEST_WRITE_64(uint64_t);
    DO_TEST_WRITE_64(int64_t);
    printf("\n");
    C4_CHECK(!testfail);
}


int main(int argc, char *argv[])
{
    //do_test();
    bm::Initialize(&argc, argv);
    bm::RunSpecifiedBenchmarks();
    return 0;
}
