cmake_minimum_required(VERSION 3.2)
project(c4core)

include(./cmake/c4project.cmake)

if(MSVC)
    # silence MSVC pedantic error on googletest's use of tr1: https://github.com/google/googletest/issues/1111
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
endif()

c4_declare_project(C4CORE)

set(C4CORE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)

if(C4CORE_BUILD_TESTS)
    enable_testing() # this must be done here (and not inside the test dir)
                     # so that the test targets are available at the top level
    add_subdirectory(test)
endif()

if(C4CORE_BUILD_BENCHMARKS)
    add_subdirectory(bm)
endif()
