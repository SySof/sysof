cmake_minimum_required(VERSION 3.2)
project(blatt11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu11" CMAKE)
set(CMAKE_BINARY_DIR "libowfat")

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)

set(SOURCE_FILES
    hash.c
    hash.h
    main.c
	)

add_executable(blatt11 ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(blatt11 owfat)
