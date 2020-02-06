cmake_minimum_required(VERSION 2.6)
#project(Command_line_parser_C_)
project(eclipse_clp)

set(CMAKE_CXX_STANDARD 14)

# Locate GTest
set(GTEST_INCLUDE_DIR ../googletest/include/)
find_package(GTest REQUIRED)

include_directories(. ${GTEST_INCLUDE_DIRS})

add_executable(parser 
        CommandLineParser.cpp
        CommandLineParser.h
        CLUtils.h CLParser.h)

add_executable(runTests UnitTests.h UnitTests.cpp)

# Link test executable against gtest & gtest_main
target_link_libraries(runTests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

