cmake_minimum_required(VERSION 3.25)
project(collatz_tests)

file(GLOB TEST_SRC_FILES
        *.cpp
        )

add_executable(collatz_unit_tests ${TEST_SRC_FILES} )

target_include_directories(collatz_unit_tests PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
