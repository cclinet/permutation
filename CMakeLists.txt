cmake_minimum_required(VERSION 3.25.0)
project(permutation VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
find_package(TBB CONFIG REQUIRED)

add_executable(permutation permutation.cpp)

target_link_libraries(permutation PRIVATE TBB::tbb TBB::tbbmalloc)