c4_setup_benchmarks(c4core)

# results dir
set(rdir ${CMAKE_CURRENT_BINARY_DIR}/results)
set(rfile ${rdir}/to_chars.json)

c4core_add_executable(c4core-bm-to_chars
    SOURCES to_chars.cpp
    LIBS c4core benchmark
    FOLDER c4core-benchmark)

c4core_add_benchmark(c4core-bm-to_chars
    c4core-bm-to_chars-run "${rdir}"
    "results are in ${rfile}"
    --benchmark_out_format=json --benchmark_out=${rfile}
    )
