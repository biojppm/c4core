c4core_setup_benchmarking()

c4core_add_executable(c4core-bm-charconv
    SOURCES charconv.cpp
    LIBS c4core benchmark
    FOLDER c4core-benchmark)

c4core_add_target_benchmark(c4core-bm-charconv xtoa)
