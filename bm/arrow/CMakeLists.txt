cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
include(../../cmake/c4Project.cmake)
project(c4corecsv LANGUAGES CXX)
c4_project()

c4_require_subproject(c4core
    REMOTE
        GIT_REPOSITORY https://github.com/biojppm/c4core
        GIT_TAG master GIT_SHALLOW ON
    EXCLUDE_FROM_ALL)

find_package(Arrow REQUIRED)

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

function(add_case lib name)
    set(llib ${lib})
    if(${lib} STREQUAL "arrow")
        set(llib arrow_static)
    endif()
    set(exe ${lib}csv-${name})
    c4_add_executable(${exe}
        SOURCES bm_${lib}csv_${name}.cpp bm_c4corecsv_common.hpp
        LIBS ${llib} c4core)
    target_compile_features(${exe} PUBLIC cxx_std_17)
    add_custom_target(${exe}-run
        DEPENDS ${exe} ${csv_file}
        COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_FILE:${exe}> ${csv_file}
        COMMAND $<TARGET_FILE:${exe}> ${csv_file}
        VERBATIM
    )
endfunction()

add_case(c4core 1_strings_serial)
add_case(c4core 2_values_serial)
add_case(c4core 3_values_parallel)
add_case(c4core 4_values_parallel_cols)

add_case(arrow 1_plain)
