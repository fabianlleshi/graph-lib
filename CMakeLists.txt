cmake_minimum_required(VERSION 3.16)
project(graphlib CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_library(graphlib src/graph.cpp)
target_include_directories(graphlib PUBLIC include)

enable_testing()