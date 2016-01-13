cmake_minimum_required(VERSION 3.3)
project(Blatt10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    heap.c
    heap.h
    main.c
    traverse.c)

add_executable(Blatt10 ${SOURCE_FILES})