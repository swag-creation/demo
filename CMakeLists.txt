cmake_minimum_required(VERSION 3.14)
project(threads2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(threads2 main.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests DISCOVERY_MODE PRE_TEST)
