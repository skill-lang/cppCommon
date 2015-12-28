cmake_minimum_required(VERSION 2.8.2)
project(skill_cpp_common)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")


################################
# Build common lib
################################

file(GLOB_RECURSE SOURCE_FILES LIST_DIRECTORIES false skill/*.cpp)

# The resulting library to be used by generated code
ADD_LIBRARY(skill.cpp.common STATIC ${SOURCE_FILES})

################################
# GTest
################################
ADD_SUBDIRECTORY(lib/gtest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################

file(GLOB_RECURSE TEST_FILES LIST_DIRECTORIES false test/*.cpp)

add_executable(runTests ${SOURCE_FILES} ${TEST_FILES})

# Link test executable against gtest & gtest_main
target_link_libraries(runTests gtest gtest_main)
add_test(runTests runTests)

set_property(TARGET runTests PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
