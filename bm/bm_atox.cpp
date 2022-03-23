#include "./bm_charconv.hpp"


// this is an exploratory benchmark to compare the possible
// combinations for all the components of the read_dec() algorithm


template<class T>
bool range_based_restrictvar0(c4::csubstr s, T * v)
{
    *v = 0;
    for(char c : s)
    {
        if(C4_UNLIKELY(c < '0' || c > '9'))
            return false;
        *v = (*v) * T(10) + (T(c) - T('0'));
    }
    return true;
}

template<class T>
bool range_based_restrictvar1(c4::csubstr s, T *C4_RESTRICT v)
{
    *v = 0;
    for(char c : s)
    {
        if(C4_UNLIKELY(c < '0' || c > '9'))
            return false;
        *v = (*v) * T(10) + (T(c) - T('0'));
    }
    return true;
}

template<class T>
bool indexloop_restrictvar0(c4::csubstr s, T * v)
{
    *v = 0;
    for(size_t i = 0; i < s.len; ++i)
    {
        const char c = s.str[i];
        if(C4_UNLIKELY(c < '0' || c > '9'))
            return false;
        *v = (*v) * T(10) + (T(c) - T('0'));
    }
    return true;
}

template<class T>
bool indexloop_restrictvar1(c4::csubstr s, T *C4_RESTRICT v)
{
    *v = 0;
    for(size_t i = 0; i < s.len; ++i)
    {
        const char c = s.str[i];
        if(C4_UNLIKELY(c < '0' || c > '9'))
            return false;
        *v = (*v) * T(10) + (T(c) - T('0'));
    }
    return true;
}

template<class T>
bool prefer_likely(c4::csubstr s, T * v)
{
    *v = 0;
    for(char c : s)
    {
        if(C4_LIKELY(c >= '0' && c <= '9'))
            *v = (*v) * T(10) + (T(c) - T('0'));
        else
            return false;
    }
    return true;
}

template<class T>
bool no_early_return(c4::csubstr s, T *C4_RESTRICT v)
{
    *v = 0;
    bool stat = true;
    for(char c : s)
    {
        if(C4_LIKELY(c >= '0' && c <= '9'))
            *v = (*v) * T(10) + (T(c) - T('0'));
        else
        {
            stat = false;
            break;
        }
    }
    return stat;
}

template<class T>
bool no_early_return_auto_type(c4::csubstr s, T *C4_RESTRICT v)
{
    *v = 0;
    bool stat = true;
    for(char c : s)
    {
        if(C4_LIKELY(c >= '0' && c <= '9'))
            *v = (*v) * T(10) + (T)(c - '0');
        else
        {
            stat = false;
            break;
        }
    }
    return stat;
}

template<class T>
bool no_early_return_auto_type2(c4::csubstr s, T *C4_RESTRICT v)
{
    *v = 0;
    bool stat = true;
    for(char c : s)
    {
        if(C4_LIKELY(c >= '0' && c <= '9'))
        {
            *v *= 10;
            *v += (T)(c - '0');
        }
        else
        {
            stat = false;
            break;
        }
    }
    return stat;
}

#define _(i) (T)(s.str[i] - '0')
C4_SUPPRESS_WARNING_GCC_CLANG_WITH_PUSH("-Wimplicit-fallthrough")

template<class T>
C4_ALWAYS_INLINE auto unroll_switch_nocheck(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 1, bool>::type
{
    *v = 0;
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return true;
    case 2:
        *v = T(10) * _(0) + _(1);
        return true;
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return true;
    }
    return false;
}

template<class T>
C4_ALWAYS_INLINE auto unroll_switch_nocheck(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 2, bool>::type
{
    *v = 0;
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return true;
    case 2:
        *v = T(10) * _(0) + _(1);
        return true;
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return true;
    case 4:
        *v = T(1000) * _(0) + T(100) * _(1) + T(10) * _(2) + _(3);
        return true;
    case 5:
        *v = T(10000) * _(0) + T(1000) * _(1) + T(100) * _(2) + T(10) * _(3) + _(4);
        return true;
    }
    return false;
}

template<class T>
C4_ALWAYS_INLINE auto unroll_switch_nocheck(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 4, bool>::type
{
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return true;
    case 2:
        *v = T(10) * _(0) + _(1);
        return true;
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return true;
    case 4:
        *v = T(1000) * _(0) + T(100) * _(1) + T(10) * _(2) + _(3);
        return true;
    case 5:
        *v = T(10000) * _(0) + T(1000) * _(1) + T(100) * _(2) + T(10) * _(3) + _(4);
        return true;
    case 6:
        *v = T(100000) * _(0) + T(10000) * _(1) + T(1000) * _(2) + T(100) * _(3) + T(10) * _(4) + _(5);
        return true;
    case 7:
        *v = T(1000000) * _(0) + T(100000) * _(1) + T(10000) * _(2) + T(1000) * _(3) + T(100) * _(4) + T(10) * _(5) + _(6);
        return true;
    case 8:
        *v = T(10000000) * _(0) + T(1000000) * _(1) + T(100000) * _(2) + T(10000) * _(3) + T(1000) * _(4) + T(100) * _(5) + T(10) * _(6) + _(7);
        return true;
    case 9:
        *v = T(100000000) * _(0) + T(10000000) * _(1) + T(1000000) * _(2) + T(100000) * _(3) + T(10000) * _(4) + T(1000) * _(5) + T(100) * _(6) + T(10) * _(7) + _(8);
        return true;
    case 10:
        *v = T(1000000000) * _(0) + T(100000000) * _(1) + T(10000000) * _(2) + T(1000000) * _(3) + T(100000) * _(4) + T(10000) * _(5) + T(1000) * _(6) + T(100) * _(7) + T(10) * _(8) + _(9);
        return true;
    }
    return false;
}

template<class T>
C4_ALWAYS_INLINE auto unroll_switch_nocheck(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 8, bool>::type
{
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return true;
    case 2:
        *v = T(10) * _(0) + _(1);
        return true;
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return true;
    case 4:
        *v = T(1000) * _(0) + T(100) * _(1) + T(10) * _(2) + _(3);
        return true;
    case 5:
        *v = T(10000) * _(0) + T(1000) * _(1) + T(100) * _(2) + T(10) * _(3) + _(4);
        return true;
    case 6:
        *v = T(100000) * _(0) + T(10000) * _(1) + T(1000) * _(2) + T(100) * _(3) + T(10) * _(4) + _(5);
        return true;
    case 7:
        *v = T(1000000) * _(0) + T(100000) * _(1) + T(10000) * _(2) + T(1000) * _(3) + T(100) * _(4) + T(10) * _(5) + _(6);
        return true;
    case 8:
        *v = T(10000000) * _(0) + T(1000000) * _(1) + T(100000) * _(2) + T(10000) * _(3) + T(1000) * _(4) + T(100) * _(5) + T(10) * _(6) + _(7);
        return true;
    case 9:
        *v = T(100000000) * _(0) + T(10000000) * _(1) + T(1000000) * _(2) + T(100000) * _(3) + T(10000) * _(4) + T(1000) * _(5) + T(100) * _(6) + T(10) * _(7) + _(8);
        return true;
    case 10:
        *v = T(1000000000) * _(0) + T(100000000) * _(1) + T(10000000) * _(2) + T(1000000) * _(3) + T(100000) * _(4) + T(10000) * _(5) + T(1000) * _(6) + T(100) * _(7) + T(10) * _(8) + _(9);
        return true;
    default:
    {
        size_t i = 0;
        *v = 0;
        for( ; i + 10 < s.len; ++i)
            *v = *v * T(10) + _(i);
        *v = T(1000000000) * _(i) + T(100000000) * _(i+1) + T(10000000) * _(i+2) + T(1000000) * _(i+3) + T(100000) * _(i+4) + T(10000) * _(i+5) + T(1000) * _(i+6) + T(100) * _(i+7) + T(10) * _(i+8) + _(i+9);
        return true;
    }
    }
    return false;
}

#define ok(i) (s.str[i] >= '0' && s.str[i] <= '9')

template<class T>
C4_ALWAYS_INLINE auto unroll_switch(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 1, bool>::type
{
    *v = 0;
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return ok(0);
    case 2:
        *v = T(10) * _(0) + _(1);
        return ok(0) && ok(1);
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return ok(0) && ok(1) && ok(2);
    }
    return false;
}

template<class T>
C4_ALWAYS_INLINE auto unroll_switch(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 2, bool>::type
{
    *v = 0;
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return true;
    case 2:
        *v = T(10) * _(0) + _(1);
        return true;
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return ok(0) && ok(1) && ok(2);
    case 4:
        *v = T(1000) * _(0) + T(100) * _(1) + T(10) * _(2) + _(3);
        return ok(0) && ok(1) && ok(2) && ok(3);
    case 5:
        *v = T(10000) * _(0) + T(1000) * _(1) + T(100) * _(2) + T(10) * _(3) + _(4);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4);
    }
    return false;
}

template<class T>
C4_ALWAYS_INLINE auto unroll_switch(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 4, bool>::type
{
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return ok(0);
    case 2:
        *v = T(10) * _(0) + _(1);
        return ok(0) && ok(1);
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return ok(0) && ok(1) && ok(2);
    case 4:
        *v = T(1000) * _(0) + T(100) * _(1) + T(10) * _(2) + _(3);
        return ok(0) && ok(1) && ok(2) && ok(3);
    case 5:
        *v = T(10000) * _(0) + T(1000) * _(1) + T(100) * _(2) + T(10) * _(3) + _(4);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4);
    case 6:
        *v = T(100000) * _(0) + T(10000) * _(1) + T(1000) * _(2) + T(100) * _(3) + T(10) * _(4) + _(5);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5);
    case 7:
        *v = T(1000000) * _(0) + T(100000) * _(1) + T(10000) * _(2) + T(1000) * _(3) + T(100) * _(4) + T(10) * _(5) + _(6);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6);
    case 8:
        *v = T(10000000) * _(0) + T(1000000) * _(1) + T(100000) * _(2) + T(10000) * _(3) + T(1000) * _(4) + T(100) * _(5) + T(10) * _(6) + _(7);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6) && ok(7);
    case 9:
        *v = T(100000000) * _(0) + T(10000000) * _(1) + T(1000000) * _(2) + T(100000) * _(3) + T(10000) * _(4) + T(1000) * _(5) + T(100) * _(6) + T(10) * _(7) + _(8);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6) && ok(7) && ok(8);
    case 10:
        *v = T(1000000000) * _(0) + T(100000000) * _(1) + T(10000000) * _(2) + T(1000000) * _(3) + T(100000) * _(4) + T(10000) * _(5) + T(1000) * _(6) + T(100) * _(7) + T(10) * _(8) + _(9);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6) && ok(7) && ok(8) && ok(9);
    }
    return false;
}

template<class T>
C4_ALWAYS_INLINE auto unroll_switch(c4::csubstr s, T *C4_RESTRICT v)
    -> typename std::enable_if<sizeof(T) == 8, bool>::type
{
    switch(s.len)
    {
    case 1:
        *v = _(0);
        return ok(0);
    case 2:
        *v = T(10) * _(0) + _(1);
        return ok(0) && ok(1);
    case 3:
        *v = T(100) * _(0) + T(10) * _(1) + _(2);
        return ok(0) && ok(1) && ok(2);
    case 4:
        *v = T(1000) * _(0) + T(100) * _(1) + T(10) * _(2) + _(3);
        return ok(0) && ok(1) && ok(2) && ok(3);
    case 5:
        *v = T(10000) * _(0) + T(1000) * _(1) + T(100) * _(2) + T(10) * _(3) + _(4);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4);
    case 6:
        *v = T(100000) * _(0) + T(10000) * _(1) + T(1000) * _(2) + T(100) * _(3) + T(10) * _(4) + _(5);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5);
    case 7:
        *v = T(1000000) * _(0) + T(100000) * _(1) + T(10000) * _(2) + T(1000) * _(3) + T(100) * _(4) + T(10) * _(5) + _(6);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6);
    case 8:
        *v = T(10000000) * _(0) + T(1000000) * _(1) + T(100000) * _(2) + T(10000) * _(3) + T(1000) * _(4) + T(100) * _(5) + T(10) * _(6) + _(7);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6) && ok(7);
    case 9:
        *v = T(100000000) * _(0) + T(10000000) * _(1) + T(1000000) * _(2) + T(100000) * _(3) + T(10000) * _(4) + T(1000) * _(5) + T(100) * _(6) + T(10) * _(7) + _(8);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6) && ok(7) && ok(8);
    case 10:
        *v = T(1000000000) * _(0) + T(100000000) * _(1) + T(10000000) * _(2) + T(1000000) * _(3) + T(100000) * _(4) + T(10000) * _(5) + T(1000) * _(6) + T(100) * _(7) + T(10) * _(8) + _(9);
        return ok(0) && ok(1) && ok(2) && ok(3) && ok(4) && ok(5) && ok(6) && ok(7) && ok(8) && ok(9);
    default:
    {
        size_t i = 0;
        *v = 0;
        for( ; i + 10 < s.len; ++i)
        {
            *v = *v * T(10) + _(i);
            if(C4_UNLIKELY(!ok(i)))
                return false;
        }
        *v = T(1000000000) * _(i) + T(100000000) * _(i+1) + T(10000000) * _(i+2) + T(1000000) * _(i+3) + T(100000) * _(i+4) + T(10000) * _(i+5) + T(1000) * _(i+6) + T(100) * _(i+7) + T(10) * _(i+8) + _(i+9);
        return ok(i) && ok(i+1) && ok(i+2) && ok(i+3) && ok(i+4) && ok(i+5) && ok(i+6) && ok(i+7) && ok(i+8) && ok(i+9);
    }
    }
    return false;
}

C4_SUPPRESS_WARNING_GCC_CLANG_POP
#undef _




#define DECLARE_BM(func)                                    \
template<class T>                                           \
void func(bm::State &st)                                    \
{                                                           \
    random_strings_cref values = mkstrings_positive<T>();   \
    T val = {};                                             \
    T sum = {};                                             \
    for(auto _ : st)                                        \
    {                                                       \
        C4DOALL(kNumValues)                                 \
        {                                                   \
            const bool ok = func(values.next(), &val);      \
            sum += (T)(T(ok) + val);                        \
        }                                                   \
    }                                                       \
    bm::DoNotOptimize(sum);                                 \
    report<T>(st, kNumValues);                              \
}

#define DEFINE_BM(ty) \
    C4BM_TEMPLATE(unroll_switch_nocheck, ty);    \
    C4BM_TEMPLATE(unroll_switch, ty);    \
    C4BM_TEMPLATE(indexloop_restrictvar0, ty);      \
    C4BM_TEMPLATE(indexloop_restrictvar1, ty);      \
    C4BM_TEMPLATE(range_based_restrictvar0, ty);    \
    C4BM_TEMPLATE(range_based_restrictvar1, ty);    \
    C4BM_TEMPLATE(prefer_likely, ty);    \
    C4BM_TEMPLATE(no_early_return, ty);    \
    C4BM_TEMPLATE(no_early_return_auto_type, ty);    \
    C4BM_TEMPLATE(no_early_return_auto_type2, ty);    \


DECLARE_BM(unroll_switch_nocheck)
DECLARE_BM(unroll_switch)
DECLARE_BM(indexloop_restrictvar0)
DECLARE_BM(indexloop_restrictvar1)
DECLARE_BM(range_based_restrictvar0)
DECLARE_BM(range_based_restrictvar1)
DECLARE_BM(prefer_likely)
DECLARE_BM(no_early_return)
DECLARE_BM(no_early_return_auto_type)
DECLARE_BM(no_early_return_auto_type2)


DEFINE_BM(uint8_t)
DEFINE_BM(int8_t)
DEFINE_BM(uint16_t)
DEFINE_BM(int16_t)
DEFINE_BM(uint32_t)
DEFINE_BM(int32_t)
DEFINE_BM(uint64_t)
DEFINE_BM(int64_t)


int main(int argc, char *argv[])
{
    //do_test();
    bm::Initialize(&argc, argv);
    bm::RunSpecifiedBenchmarks();
    return 0;
}
