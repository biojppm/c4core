cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(c4core
    DESCRIPTION "Single header version of c4core library for testing"
    HOMEPAGE_URL "https://github.com/biojppm/c4core"
    LANGUAGES CXX)
include(../../cmake/c4Project.cmake)
c4_project(VERSION 0.1.7
    AUTHOR "Joao Paulo Magalhaes <dev@jpmag.me>")

set(singleheaderdir ${CMAKE_CURRENT_LIST_DIR}/../../src_singleheader)
set(singleheader ${singleheaderdir}/c4/c4core_all.hpp)
if(NOT (EXISTS ${singleheader}))
    message(FATAL_ERROR "cannot find single header for c4core: ${singleheader}")
endif()

c4_add_library(c4core
    INC_DIRS
        $<BUILD_INTERFACE:${singleheaderdir}> $<INSTALL_INTERFACE:include>
    SOURCE_ROOT ${singleheaderdir}
    SOURCES
        ${singleheader}
        ${CMAKE_CURRENT_LIST_DIR}/libc4core_singleheader.cpp)
target_compile_definitions(c4core PUBLIC -DC4CORE_SINGLE_HEADER)

enable_testing()
set(C4CORE_DEFINED_FROM_SINGLEHEADER ON)
add_subdirectory(../../test test)
