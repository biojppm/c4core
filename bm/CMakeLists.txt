c4_setup_benchmarking()

add_custom_target(c4core-bm-plot)
c4_set_folder_remote_project_targets(bm/plot c4core-bm-plot)
find_package(Python REQUIRED COMPONENTS Interpreter)

#----------------------------------------------

c4_add_executable(c4core-bm-format
    SOURCES bm_format.cpp
    LIBS c4core benchmark
    FOLDER bm)

c4_add_target_benchmark(c4core-bm-format cat FILTER "^cat_.*")
c4_add_target_benchmark(c4core-bm-format catsep FILTER "^catsep_.*")
c4_add_target_benchmark(c4core-bm-format format FILTER "^format_.*")

c4_add_target_benchmark(c4core-bm-format catfile FILTER "^catfile_.*")
c4_add_target_benchmark(c4core-bm-format catsepfile FILTER "^catsepfile_.*")
c4_add_target_benchmark(c4core-bm-format formatfile FILTER "^formatfile_.*")


#----------------------------------------------

# R&D for xtoa
option(C4CORE_BM_XTOA_RND "whether to enable R&D benchmark for xtoa" OFF)
option(C4CORE_BM_ATOX_RND "whether to enable R&D benchmark for atox" OFF)

if(C4CORE_BM_XTOA_RND)
    c4_add_executable(c4core-bm-xtoa
        SOURCES bm_xtoa.cpp
        LIBS c4core benchmark
        FOLDER bm)

    function(c4core_xtoa_benchmark pfx)
        foreach(case ${ARGN})
            c4_add_target_benchmark(c4core-bm-xtoa ${pfx}-${case} FILTER "^${pfx}_.*<${case}.*")
        endforeach()
    endfunction()

    c4core_xtoa_benchmark(msb uint8 int8 uint16 int16 uint32 int32 uint64 int64)
    c4core_xtoa_benchmark(digits_dec uint8 int8 uint16 int16 uint32 int32 uint64 int64)
    c4core_xtoa_benchmark(write_dec uint8 int8 uint16 int16 uint32 int32 uint64 int64)
endif()

if(C4CORE_BM_ATOX_RND)
    c4_add_executable(c4core-bm-atox
        SOURCES bm_atox.cpp
        LIBS c4core benchmark
        FOLDER bm)
    function(c4core_atox_benchmark)
        foreach(case ${ARGN})
            c4_add_target_benchmark(c4core-bm-atox ${case} FILTER "^.*<${case}.*")
        endforeach()
    endfunction()
    c4core_atox_benchmark(uint8 int8 uint16 int16 uint32 int32 uint64 int64)
endif()


#----------------------------------------------

c4_add_executable(c4core-bm-charconv
    SOURCES bm_charconv.cpp bm_charconv.hpp
    LIBS c4core benchmark
    INC_DIRS ${C4CORE_SRC_DIR}/c4/ext/fast_float/include
    FOLDER bm)

# libraries to compare. defaults to off.
option(C4CORE_BM_USE_RYU "enable comparison with ryu library" OFF)
option(C4CORE_BM_USE_FP "enable comparison with fp library" OFF)

# fp requires c++17 or later
get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
if(cxx_std_17 IN_LIST known_features)
    set(C4CORE_BM_USE_FP OFF CACHE BOOL "" FORCE)
endif()
if(C4CORE_BM_USE_FP)
    c4_require_subproject(fp
        REMOTE
            GIT_REPOSITORY https://github.com/jk-jeon/fp
            GIT_TAG master GIT_SHALLOW ON
        EXCLUDE_FROM_ALL)
    target_link_libraries(c4core-bm-charconv PRIVATE jkj_fp)
    target_compile_definitions(c4core-bm-charconv PUBLIC -DC4CORE_BM_USE_FP)
endif()

# ryu not supported in windows with 32bit
if(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
    set(C4CORE_BM_USE_RYU OFF CACHE BOOL "" FORCE)
endif()
if(C4CORE_BM_USE_RYU)
    # ryu requires too much code; use an include for clarity
    include(./ryu.cmake)
    target_link_libraries(c4core-bm-charconv PRIVATE ryu_c4)
    target_compile_definitions(c4core-bm-charconv PUBLIC -DC4CORE_BM_USE_RYU)
endif()


function(c4core_charconv_benchmark name rx)
    unset(result_files)
    foreach(datatype ${ARGN})
        c4_add_target_benchmark(c4core-bm-charconv ${name}-${datatype} FILTER "${rx}<${datatype}.*"
            RESULTS_FILE results_file)
        list(APPEND result_files ${results_file})
    endforeach()
    add_custom_target(c4core-bm-charconv-${name}-plot
        #DEPENDS ${result_files}
        COMMAND cmake -E echo "${Python_EXECUTABLE}"
        COMMAND ${Python_EXECUTABLE} --version
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bm_plot_c4core.py ${name} ${result_files}
        )
    add_dependencies(c4core-bm-plot c4core-bm-charconv-${name}-plot)
    c4_set_folder_remote_project_targets(bm/plot c4core-bm-charconv-${name}-plot)
endfunction()

c4core_charconv_benchmark(digits "^c4_digits.*"  uint8 int8 uint16 int16 uint32 int32 uint64 int64)
c4core_charconv_benchmark(xtoa   "^xtoa_.*"      uint8 int8 uint16 int16 uint32 int32 uint64 int64)
c4core_charconv_benchmark(atox   "^atox_.*"      uint8 int8 uint16 int16 uint32 int32 uint64 int64)
c4core_charconv_benchmark(ftoa   "^xtoa_.*"      float double)
c4core_charconv_benchmark(atof   "^atox_.*"      float double)


#----------------------------------------------

option(C4CORE_BM_FLOAT_CMP "compare between alternative implementations for reading/writing floats" ON)
if(C4CORE_BM_FLOAT_CMP)
    add_subdirectory(float)
endif()


#----------------------------------------------


option(C4CORE_BM_ITOA_THREADS "compare itoa thread degradation across alternative implementations" OFF)
if(C4CORE_BM_ITOA_THREADS)
    c4_download_remote_proj(stb stbdir REMOTE
        GIT_REPOSITORY https://github.com/nothings/stb GIT_SHALLOW ON)

    c4_require_subproject(fmtlib REMOTE
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 8.1.1
        GIT_SHALLOW ON)
    c4_set_folder_remote_project_targets(ext fmt)

    c4_add_executable(c4core-bm-itoa_threads
        SOURCES bm_itoa_threads.cpp bm_charconv.hpp
        LIBS fmt c4core benchmark
        INC_DIRS ${stbdir}
        FOLDER bm)

    unset(_c4core_itoa_thread_result_files)
    add_custom_target(c4core-bm-itoa_threads-all)
    function(c4core_itoa_threads_benchmark name rx)
        set(li ${_c4core_itoa_thread_result_files})
        c4_add_target_benchmark(c4core-bm-itoa_threads ${name} FILTER "${rx}"
            RESULTS_FILE results_file)
        add_dependencies(c4core-bm-itoa_threads-all c4core-bm-itoa_threads-${name})
        c4_set_folder_remote_project_targets(bm/run c4core-bm-itoa_threads-all)
        list(APPEND li ${results_file})
        set(_c4core_itoa_thread_result_files ${li} PARENT_SCOPE)
    endfunction()

    c4core_itoa_threads_benchmark(c4_write_dec "^c4_write_dec/.*")
    c4core_itoa_threads_benchmark(c4_itoa "^c4_itoa/.*")
    c4core_itoa_threads_benchmark(c4_xtoa "^c4_xtoa/.*")
    c4core_itoa_threads_benchmark(c4_to_chars "^c4_to_chars/.*")
    c4core_itoa_threads_benchmark(std_to_chars "^std_to_chars/.*")
    c4core_itoa_threads_benchmark(fmtlib_format_to "^fmtlib_format_to/.*")
    c4core_itoa_threads_benchmark(snprintf "^snprintf/.*")
    c4core_itoa_threads_benchmark(stb_snprintf "^stb_snprintf/.*")
    if(WIN32)
    c4core_itoa_threads_benchmark(snprintf_l "^snprintf_l/.*")
    c4core_itoa_threads_benchmark(itoa "^itoa/.*")
    #get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
    #if(cxx_std_20 IN_LIST known_features)
    #c4core_itoa_threads_benchmark(std_format_to "^std_format_to/.*")
    #endif()
    endif()
    c4core_itoa_threads_benchmark(sstream "^sstream/.*")
    c4core_itoa_threads_benchmark(sstream_naive_reuse "^sstream_naive_reuse/.*")
    c4core_itoa_threads_benchmark(sstream_naive "^sstream_naive/.*")
    add_custom_target(c4core-bm-itoa_threads-plot
        #DEPENDS ${_c4core_itoa_thread_result_files}
        COMMAND cmake -E echo "${Python_EXECUTABLE}"
        COMMAND ${Python_EXECUTABLE} --version
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bm_plot_c4core.py itoa_threads ${_c4core_itoa_thread_result_files}
        )
    c4_set_folder_remote_project_targets(bm/plot c4core-bm-itoa_threads-plot)
    add_dependencies(c4core-bm-plot c4core-bm-itoa_threads-plot)
endif()
