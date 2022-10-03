cmake_minimum_required(VERSION 3.11)
project(lily_runtime C)

set(CMAKE_C_STANDARD 11)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -O -O2 -O3 -D LILY_BUILD")

set(RUNTIME_SRC
        c/Allocator.c
        c/Array.c
        c/io.c
	c/generic.c
        c/runtime.c
        c/Int8.c
        c/Int16.c
        c/Int32.c
    	c/Int32.c
        c/Int64.c
        c/Int128.c
	c/Mem.c
        c/Net.c
        c/Tuple.c
    	c/Uint8.c
        c/Uint16.c
        c/Uint32.c
        c/Uint64.c
        c/Uint128.c)

add_library(lily_runtime SHARED ${RUNTIME_SRC})
