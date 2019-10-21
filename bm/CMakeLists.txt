c4_setup_benchmarks(c4core)

# results dir
set(rdir ${CMAKE_CURRENT_BINARY_DIR}/results)
set(rfile ${rdir}/itoa.json)

c4core_add_executable(c4core-bm-itoa
    SOURCES itoa.cpp
    LIBS c4core benchmark
    FOLDER c4core-benchmark)

c4core_add_benchmark(c4core-bm-itoa
    c4core-bm-itoa-run "${rdir}"
    "results are in ${rfile}"
    --benchmark_out_format=json --benchmark_out=${rfile}
    )
