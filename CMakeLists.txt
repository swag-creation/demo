cmake_minimum_required(VERSION 4.3)
project(Threads)

set(CMAKE_CXX_STANDARD 20)

add_library(test SHARED test.cpp)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Threads main.cpp)
add_dependencies(Threads test)

set_target_properties(test PROPERTIES PREFIX "")
set_target_properties(Threads test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
