cmake_minimum_required(VERSION 3.14)
project(collatz)

set(CMAKE_CXX_STANDARD 17)

# Default to Release build with maximum optimizations
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# source all .cpp files in dir
file(GLOB SOURCE_FILES
        "src/*.cpp"
        )

add_executable(collatz ${SOURCE_FILES})

option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
