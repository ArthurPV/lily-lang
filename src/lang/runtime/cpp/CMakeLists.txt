cmake_minimum_required(VERSION 3.11)
project(lily_runtime_cpp CXX)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -O -O2 -O3 -D LILY_BUILD")

set(RUNTIME_SRC
	mod.cpp)

add_library(lily_runtime_cpp SHARED ${RUNTIME_SRC})
target_include_directories(lily_runtime_cpp PRIVATE ../)
target_link_libraries(lily_runtime_cpp lily_runtime_c)
