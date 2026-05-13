#include "./bm_charconv.hpp"

#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <vector>
#include <sstream>
#if defined(__cpp_lib_to_chars) || (C4_CPP >= 17)
#define C4_HAS_STDTOCHARS 1
#else
#define C4_HAS_STDTOCHARS 0
#endif
#if C4_HAS_STDTOCHARS
#include <charconv>
#endif

C4_SUPPRESS_WARNING_GCC_CLANG_PUSH
C4_SUPPRESS_WARNING_GCC_CLANG("-Wcast-align")
C4_SUPPRESS_WARNING_GCC("-Wuseless-cast")

#ifdef C4CORE_BM_FMTLIB
C4_SUPPRESS_WARNING_GCC_CLANG_PUSH
C4_SUPPRESS_WARNING_GCC_CLANG("-Wfloat-equal")
#define FMT_HEADER_ONLY
#include <fmt/format.h>
C4_SUPPRESS_WARNING_GCC_CLANG_POP
#endif

#define STB_SPRINTF_IMPLEMENTATION
C4_SUPPRESS_WARNING_GCC_CLANG("-Wcast-qual")
#include <stb_sprintf.h>
C4_SUPPRESS_WARNING_GCC_POP

#if C4_CPP >= 20
#include <version>
#define C4_HAS_STD_FORMAT (__cpp_lib_format)
#else
#define C4_HAS_STD_FORMAT (0)
#endif
#if C4_HAS_STD_FORMAT
#include <format>
#endif


#define BMTHREADS(func)  \
    BENCHMARK(func)      \
    ->ThreadRange(1, 10) \
    ->UseRealTime()


void std_snprintf(bm::State &st)
{
    size_t sum = {};
    char buf[100];
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            int ret = std::snprintf(buf, sizeof(buf), "%i", i++);
            sum += (size_t)ret + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(std_snprintf);


#ifndef __linux__
void std_snprintf_l(bm::State &st)
{
    size_t sum = {};
    char buf[100];
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            #if defined(_MSC_VER)
            int ret = _snprintf_l(buf, 100, "%i", NULL, i++);
            #else
            int ret = snprintf_l(buf, 100, NULL, "%i", i++);
            #endif
            sum += (size_t)ret + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(std_snprintf_l);
#endif


void stb_snprintf(bm::State &st)
{
    size_t sum = {};
    char buf[100];
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            stbsp_snprintf(buf, 100, "%i", i++);
            sum += strlen(buf) + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(stb_snprintf);



void sstream(bm::State &st)
{
    size_t sum = {};
    std::stringstream buf;
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            buf.seekp(0);
            buf << i++;
            size_t len = (size_t)buf.tellp();
            buf.seekg(0);
            int firstchar = buf.get();
            sum += len + firstchar;
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(sstream);


void sstream_naive(bm::State &st)
{
    size_t sum = {};
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::stringstream buf;
            buf << i++;
            std::string ret = buf.str();
            sum += ret.size() + ret[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(sstream_naive);


void sstream_naive_reuse(bm::State &st)
{
    size_t sum = {};
    int i = 0;
    std::stringstream buf;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            buf.clear();
            buf.str("");
            buf << i++;
            std::string ret = buf.str();
            sum += ret.size() + ret[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(sstream_naive_reuse);


#ifdef _MSC_VER
void std_itoa(bm::State &st)
{
 	char buf[100];
    size_t sum = {};
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            ::_itoa(i++, buf, 10);
            sum += strlen(buf) + buf[0];
        }
	}
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(std_itoa);
#endif


#if C4_HAS_STD_FORMAT
static void std_format_to(bm::State &st)
{
    size_t sum = {};
 	char buf[100];
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            const auto res = std::format_to_n(buf, sizeof(buf), /*loc,*/ "{}", i++);
            sum += (res.out - buf) + buf[0];
        }
	}
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(std_format_to);
#endif



#ifdef C4CORE_BM_FMTLIB
void fmtlib_format_to(bm::State &st)
{
    size_t sum = {};
	fmt::memory_buffer buf;
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            buf.clear();
            fmt::format_to(fmt::appender(buf), "{}", i++);
            sum += buf.size() + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(fmtlib_format_to);
#endif


#if C4_HAS_STDTOCHARS
void std_to_chars(bm::State &st)
{
    size_t sum = {};
 	char buf[100];
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            std::to_chars_result res = std::to_chars(buf, buf+sizeof(buf), i++);
            sum += (res.ptr - buf) + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(std_to_chars);
#endif


void c4_write_dec(bm::State &st)
{
    size_t sum = {};
 	char buf_[100];
    c4::substr buf = buf_;
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            size_t len = c4::write_dec(buf, i++);
            sum += len + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(c4_write_dec);


void c4_itoa(bm::State &st)
{
    size_t sum = {};
 	char buf_[100];
    c4::substr buf = buf_;
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            size_t len = c4::itoa(buf, i++);
            sum += len + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(c4_itoa);


void c4_xtoa(bm::State &st)
{
    size_t sum = {};
 	char buf_[100];
    c4::substr buf = buf_;
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            size_t len = c4::xtoa(buf, i++);
            sum += len + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(c4_xtoa);


void c4_to_chars(bm::State &st)
{
    size_t sum = {};
 	char buf_[100];
    c4::substr buf = buf_;
    int i = 0;
    for(auto _ : st)
    {
        C4DOALL(kNumValues)
        {
            size_t len = c4::to_chars(buf, i++);
            sum += len + buf[0];
        }
    }
    bm::DoNotOptimize(sum);
    report<int>(st, kNumValues);
}
BMTHREADS(c4_to_chars);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    bm::Initialize(&argc, argv);
    bm::RunSpecifiedBenchmarks();
    return 0;
}
