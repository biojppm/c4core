cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
include(../../cmake/c4Project.cmake)
project(c4corecsv LANGUAGES CXX)
c4_project()

c4_require_subproject(c4core
    REMOTE
        GIT_REPOSITORY https://github.com/biojppm/c4core
        GIT_TAG master GIT_SHALLOW ON
    EXCLUDE_FROM_ALL)
c4_require_subproject(arrow
    REMOTE
        GIT_REPOSITORY https://github.com/apache/arrow
        GIT_TAG apache-arrow-9.0.0 GIT_SHALLOW ON
    SUBDIR cpp
    OVERRIDE
        ARROW_DEFINE_OPTIONS_DEFAULT ON
        ARROW_PYTHON ON
        ARROW_PARQUET ON
        ARROW_FLIGHT OFF
        ARROW_PLASMA ON
        ARROW_GANDIVA ON
        ARROW_THRIFT OFF
        ARROW_BUILD_BENCHMARKS ON
        ARROW_BUILD_BENCHMARKS_REFERENCE ON
        ARROW_BUILD_TESTS OFF
        ARROW_BUILD_UTILITIES ON
        ARROW_S3 OFF
        ARROW_WITH_BROTLI ON
        ARROW_WITH_BZ2 ON
        ARROW_WITH_LZ4 ON
        ARROW_WITH_SNAPPY ON
        ARROW_WITH_ZLIB ON
        ARROW_WITH_ZSTD ON
    EXCLUDE_FROM_ALL)

c4_add_executable(c4corecsv-c4core
    SOURCES bm_csv.cpp
    LIBS c4core)

set(csv_url https://www.gstatic.com/covid19/mobility/Global_Mobility_Report.csv)
set(csv_file ${CMAKE_CURRENT_SOURCE_DIR}/Global_Mobility_Report.csv)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/download_bm_csv.cmake
    "file(DOWNLOAD \"${csv_url}\" \"${csv_file}\" SHOW_PROGRESS)"
)
add_custom_command(OUTPUT ${csv_file}
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/download_bm_csv.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
)
add_custom_target(c4corecsv-c4core-run
    DEPENDS c4corecsv-c4core ${csv_file}
    COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_FILE:c4corecsv-c4core> ${csv_file}
    COMMAND $<TARGET_FILE:c4corecsv-c4core> ${csv_file}
    VERBATIM
)
