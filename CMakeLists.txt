cmake_minimum_required(VERSION 3.14)
project(DiversCpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(Regexp regexp.cpp )

add_executable(CLParser CommandLineParser.cpp CommandLineParser.h)