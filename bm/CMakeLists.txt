c4_setup_benchmarking()

c4_add_executable(c4core-bm-charconv
    SOURCES charconv.cpp
    LIBS c4core benchmark
    FOLDER bm)

c4_add_target_benchmark(c4core-bm-charconv xtoa FILTER "^xtoa_")
c4_add_target_benchmark(c4core-bm-charconv atox FILTER "^atox_")
