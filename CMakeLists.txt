cmake_minimum_required(VERSION 3.2)
project(c4core)

include(./cmake/c4Project.cmake)

c4_declare_project(C4CORE
    DESC "Multiplatform low-level C++ utilities"
    AUTHOR "Joao Paulo Magalhaes <dev@jpmag.me>"
    MAJOR 0 MINOR 0 RELEASE 1)

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
