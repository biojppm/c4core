
# old gcc-4.8 support
if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND
  (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 4.8) AND
  (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0))

  # header injection, required to compile external projects (not necessary for c4core)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/src/c4/compat/gcc-4.8.hpp")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/src/c4/compat/gcc-4.8.hpp")

  # c++17 compiler required
  set(C4CORE_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
  # LLVM required
  set(C4CORE_SANITIZE OFF CACHE BOOL "" FORCE)
endif()