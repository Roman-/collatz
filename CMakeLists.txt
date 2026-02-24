cmake_minimum_required(VERSION 3.25)
project(collatz)

set(CMAKE_CXX_STANDARD 17)

# source all .cpp files in dir
file(GLOB SOURCE_FILES
        "src/*.cpp"
        )

add_executable(collatz ${SOURCE_FILES})

option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
