include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# Needed on Windows so gtest doesn't fight our CRT linkage.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(test_graph test_graph.cpp)
target_link_libraries(test_graph PRIVATE graphlib GTest::gtest_main)

add_executable(test_bfs test_bfs.cpp)
target_link_libraries(test_bfs PRIVATE graphlib GTest::gtest_main)

add_executable(test_components test_components.cpp)
target_link_libraries(test_components PRIVATE graphlib GTest::gtest_main)

add_executable(test_io test_io.cpp)
target_link_libraries(test_io PRIVATE graphlib GTest::gtest_main)

add_executable(test_dfs test_dfs.cpp)
target_link_libraries(test_dfs PRIVATE graphlib GTest::gtest_main)

add_executable(test_dijkstra test_dijkstra.cpp)
target_link_libraries(test_dijkstra PRIVATE graphlib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_graph)
gtest_discover_tests(test_bfs)
gtest_discover_tests(test_components)
gtest_discover_tests(test_io)
gtest_discover_tests(test_dfs)
gtest_discover_tests(test_dijkstra)