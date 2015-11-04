cmake_minimum_required(VERSION 3.2.2)
project(skill_cpp_common)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


################################
# GTest
################################
ADD_SUBDIRECTORY(lib/gtest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################

set(SOURCE_FILES
        src/utils.h
        test/streams/streamTest.cpp
        src/streams/InStream.cpp
        src/streams/InStream.h
        src/streams/FileInputStream.cpp
        src/streams/FileInputStream.h
        src/streams/streams.h src/api/SkillException.cpp src/api/SkillException.h)
add_executable(runTests ${SOURCE_FILES})

# Link test executable against gtest & gtest_main
target_link_libraries(runTests gtest gtest_main)
add_test(runTests runTests)