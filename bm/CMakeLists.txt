c4core_setup_benchmarking()

c4core_add_executable(c4core-bm-charconv
    SOURCES charconv.cpp
    LIBS c4core benchmark
    FOLDER c4core-bm)

c4core_add_target_benchmark(c4core-bm-charconv xtoa FILTER "^xtoa_")
c4core_add_target_benchmark(c4core-bm-charconv atox FILTER "^atox_")
