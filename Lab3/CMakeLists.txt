cmake_minimum_required(VERSION 3.6)
project(Lab3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    t1.c
    t2.c
    t3.c 3.1.c)

add_executable(Lab3 ${SOURCE_FILES})