cmake_minimum_required(VERSION 3.25.0)
project(
  permutation
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
find_package(TBB CONFIG REQUIRED)

add_executable(benchmark main.cpp permutation.hpp)
target_link_libraries(benchmark PRIVATE TBB::tbb TBB::tbbmalloc)
