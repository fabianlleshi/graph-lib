cmake_minimum_required(VERSION 3.16)
project(graphlib CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_library(graphlib
    src/graph.cpp
    src/bfs.cpp
    src/components.cpp
    src/io.cpp
    src/dfs.cpp
    src/dijkstra.cpp
)
target_include_directories(graphlib PUBLIC include)

add_executable(graphlib_cli src/main.cpp)
target_link_libraries(graphlib_cli PRIVATE graphlib)

enable_testing()
add_subdirectory(tests)