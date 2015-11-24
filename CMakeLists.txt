cmake_minimum_required(VERSION 3.2.2)
project(skill_cpp_common)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")


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
        test/streams/parseTest.cpp
        test/streams/streamTest.cpp
        src/streams/InStream.cpp
        src/streams/InStream.h
        src/streams/FileInputStream.cpp
        src/streams/FileInputStream.h
        src/streams/streams.h
        src/api/SkillException.cpp
        src/api/SkillException.h
        src/streams/MappedInStream.cpp
        src/streams/MappedInStream.h
        src/internal/FileParser.h src/internal/AbstractStoragePool.cpp src/internal/AbstractStoragePool.h src/internal/StoragePool.cpp src/internal/StoragePool.h src/api/SkillFile.cpp src/api/SkillFile.h src/internal/SkillState.cpp src/internal/SkillState.h src/api/StringAccess.cpp src/api/StringAccess.h src/internal/StringPool.cpp src/internal/StringPool.h src/api/String.cpp src/api/String.h src/api/Box.h src/api/Object.cpp src/api/Object.h src/common.h src/internal/FileStructure.h src/api/AbstractField.cpp src/api/AbstractField.h src/internal/FieldDeclaration.cpp src/internal/FieldDeclaration.h src/fieldTypes/FieldType.cpp src/fieldTypes/FieldType.h src/internal/UnknownBasePool.cpp src/internal/UnknownBasePool.h src/internal/LazyField.cpp src/internal/LazyField.h src/fieldTypes/BuiltinFieldType.cpp src/fieldTypes/BuiltinFieldType.h src/streams/BoxedStreamReader.h)
add_executable(runTests ${SOURCE_FILES})

# Link test executable against gtest & gtest_main
target_link_libraries(runTests gtest gtest_main)
add_test(runTests runTests)