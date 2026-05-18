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

include(GoogleTest)
gtest_discover_tests(test_graph)