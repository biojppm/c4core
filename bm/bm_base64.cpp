#include "./bm_charconv.hpp"

#include <map>
#include <libbase64.h>
#include <c4/format_base64.hpp>

struct payload { c4::csubstr input; c4::substr output; };
template<class T>
struct encode_case
{
    random_values<T> rvals;
    std::vector<T> wvals;
    std::string rbase64;
    std::string wbase64;
    payload enc;
    payload dec;
};
template<class T>
encode_case<T>* makedata(size_t sz, bool positive=true)
{
    static std::map<size_t, encode_case<T>> vals;
    auto it = vals.find(sz);
    if(it != vals.end())
        return &it->second;
    auto &slot = vals[sz];
    slot.rvals = std::move(random_values<T>(positive, sz));
    c4::catrs<std::string>(&slot.rbase64, c4::fmt::base64(slot.rvals.v));
    slot.wvals.resize(slot.rvals.v.size());
    slot.wbase64.resize(slot.rbase64.size());
    slot.enc = {
        c4::csubstr{(const char*)slot.rvals.v.data(), sizeof(T) * slot.rvals.v.size()},
        c4::substr{&slot.wbase64[0], slot.wbase64.size()}
    };
    slot.dec = {
        c4::csubstr{&slot.rbase64[0], slot.rbase64.size()},
        c4::substr{(char*)slot.wvals.data(), sizeof(T) * slot.wvals.size()}
    };
    return &slot;
}


//-----------------------------------------------------------------------------

template<class T>
void bm_encode_c4_base64(bm::State &st)
{
    payload enc = makedata<T>(st.range(0))->enc;
    size_t sz = 0;
    for(auto _ : st)
    {
        sz = c4::base64_encode(enc.output.str, enc.output.len,
                               enc.input.str, enc.input.len);
    }
    bm::DoNotOptimize(sz);
    C4_ASSERT(sz == enc.output.len);
    report<char>(st, sz);
}

template<class T>
void bm_encode_aklomp_base64_plain(bm::State &st)
{
    payload enc = makedata<T>(st.range(0))->enc;
    size_t sz = 0;
    for(auto _ : st)
    {
        ::base64_encode(enc.input.str, enc.input.len,
                        enc.output.str, &sz, BASE64_FORCE_PLAIN);
    }
    bm::DoNotOptimize(sz);
    C4_ASSERT(sz == enc.output.len);
    report<char>(st, sz);
}

template<class T>
void bm_encode_aklomp_base64(bm::State &st)
{
    payload enc = makedata<T>(st.range(0))->enc;
    size_t sz = 0;
    for(auto _ : st)
    {
        ::base64_encode(enc.input.str, enc.input.len,
                        enc.output.str, &sz, 0);
    }
    bm::DoNotOptimize(sz);
    C4_ASSERT(sz == enc.output.len);
    report<char>(st, sz);
}


//-----------------------------------------------------------------------------

template<class T>
void bm_decode_c4_base64(bm::State &st)
{
    payload dec = makedata<T>(st.range(0))->dec;
    size_t sz = 0;
    bool ok = false;
    for(auto _ : st)
    {
        ok = c4::base64_decode(dec.input.str, dec.input.len,
                               dec.output.str, dec.output.len, &sz);
    }
    bm::DoNotOptimize(sz);
    bm::DoNotOptimize(ok);
    C4_ASSERT(sz == dec.output.len);
    C4_ASSERT(ok);
    report<char>(st, sz);
}

template<class T>
void bm_decode_aklomp_base64_plain(bm::State &st)
{
    payload dec = makedata<T>(st.range(0))->dec;
    size_t sz = 0;
    for(auto _ : st)
    {
        ::base64_decode(dec.input.str, dec.input.len,
                        dec.output.str, &sz, BASE64_FORCE_PLAIN);
    }
    bm::DoNotOptimize(sz);
    C4_ASSERT(sz == dec.output.len);
    report<char>(st, sz);
}

template<class T>
void bm_decode_aklomp_base64(bm::State &st)
{
    payload dec = makedata<T>(st.range(0))->dec;
    size_t sz = 0;
    for(auto _ : st)
    {
        ::base64_decode(dec.input.str, dec.input.len,
                        dec.output.str, &sz, 0);
    }
    bm::DoNotOptimize(sz);
    C4_ASSERT(sz == dec.output.len);
    report<char>(st, sz);
}


//-----------------------------------------------------------------------------

#define SIZES Arg(8)->Arg(64)->Arg(512)->Arg(4096)->Arg(16384)

BENCHMARK_TEMPLATE(bm_encode_c4_base64          , uint8_t)->SIZES;
BENCHMARK_TEMPLATE(bm_encode_aklomp_base64_plain, uint8_t)->SIZES;
BENCHMARK_TEMPLATE(bm_encode_aklomp_base64      , uint8_t)->SIZES;

BENCHMARK_TEMPLATE(bm_decode_c4_base64          , uint8_t)->SIZES;
BENCHMARK_TEMPLATE(bm_decode_aklomp_base64_plain, uint8_t)->SIZES;
BENCHMARK_TEMPLATE(bm_decode_aklomp_base64      , uint8_t)->SIZES;

BENCHMARK_TEMPLATE(bm_encode_c4_base64          , int)->SIZES;
BENCHMARK_TEMPLATE(bm_encode_aklomp_base64_plain, int)->SIZES;
BENCHMARK_TEMPLATE(bm_encode_aklomp_base64      , int)->SIZES;

BENCHMARK_TEMPLATE(bm_decode_c4_base64          , int)->SIZES;
BENCHMARK_TEMPLATE(bm_decode_aklomp_base64_plain, int)->SIZES;
BENCHMARK_TEMPLATE(bm_decode_aklomp_base64      , int)->SIZES;

int main(int argc, char *argv[])
{
    bm::MaybeReenterWithoutASLR(argc, argv);
    bm::Initialize(&argc, argv);
    bm::RunSpecifiedBenchmarks();
    return 0;
}

#include <c4/c4_pop.hpp>
