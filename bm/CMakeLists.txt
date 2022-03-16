c4_setup_benchmarking()


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

if(C4CORE_BM_XTOA_RND)
    c4_add_executable(c4core-bm-xtoa
        SOURCES bm_xtoa.cpp
        LIBS c4core benchmark
        FOLDER bm)

    function(c4core_xtoa_benchmark pfx)
        foreach(case ${ARGN})
            c4_add_target_benchmark(c4core-bm-xtoa ${pfx}-${case} FILTER "^${pfx}.*<${case}.*")
        endforeach()
    endfunction()

    c4core_xtoa_benchmark(msb uint8 int8 uint16 int16 uint32 int32 uint64 int64)
    c4core_xtoa_benchmark(digits_dec uint8 int8 uint16 int16 uint32 int32 uint64 int64)
    c4core_xtoa_benchmark(write_dec uint8 int8 uint16 int16 uint32 int32 uint64 int64)
endif()


#----------------------------------------------

# libraries to compare

c4_require_subproject(fp
  REMOTE
    GIT_REPOSITORY https://github.com/jk-jeon/fp
    GIT_TAG master
  EXCLUDE_FROM_ALL)

# ryu requires too much code; use an include for clarity
include(./ryu.cmake)

c4_add_executable(c4core-bm-charconv
    SOURCES bm_charconv.cpp
    LIBS c4core benchmark
    INC_DIRS ${C4CORE_SRC_DIR}/c4/ext/fast_float/include
    FOLDER bm)

if(C4CORE_BM_USE_RYU)
    target_link_libraries(c4core-bm-charconv PUBLIC ryu_c4)
    target_compile_definitions(c4core-bm-charconv PUBLIC -DC4CORE_BM_USE_RYU)
endif()

get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
if(cxx_std_17 IN_LIST known_features)
    target_link_libraries(c4core-bm-charconv PRIVATE jkj_fp)
endif()


function(c4core_charconv_benchmark name rx)
    foreach(datatype ${ARGN})
        c4_add_target_benchmark(c4core-bm-charconv ${name}-${datatype} FILTER "${rx}<${datatype}.*")
    endforeach()
endfunction()

c4core_charconv_benchmark(digits "^c4_digits.*" uint8 int8 uint16 int16 uint32 int32 uint64 int64)
c4core_charconv_benchmark(xtoa   "^xtoa.*"      uint8 int8 uint16 int16 uint32 int32 uint64 int64 float double)
c4core_charconv_benchmark(atox   "^atox.*"      uint8 int8 uint16 int16 uint32 int32 uint64 int64 float double)


#----------------------------------------------

option(C4CORE_BM_FLOAT_CMP "compare between alternative implementations for reading/writing floats" ON)
if(C4CORE_BM_FLOAT_CMP)
    add_subdirectory(float)
endif()
