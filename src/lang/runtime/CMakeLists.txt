cmake_minimum_required(VERSION 3.11)
project(lily_runtime C)

set(CMAKE_C_STANDARD 23)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -O -O2 -O3 -D LILY_BUILD")

set(RUNTIME_SRC
        Allocator.c
        Array.c
        io.c
	    generic.c
        runtime.c
        Int8.c
        Int16.c
        Int32.c
    	Int32.c
        Int64.c
        Int128.c
		Mem.c
        Net.c
        Tuple.c
    	Uint8.c
        Uint16.c
        Uint32.c
        Uint64.c
        Uint128.c)

add_library(lily_runtime SHARED ${RUNTIME_SRC})
