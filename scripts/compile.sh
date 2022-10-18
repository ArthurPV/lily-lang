#!/bin/bash

# Arg1 = File to compile
# Arg2 = Path which contains include file
# Arg3 = Path of shared library
# Arg4 = Binary name

rm -rf lily_cache

mkdir -p lily_cache
mkdir -p lily_cache/lib
mkdir -p lily_cache/bin

cp $3 ./lily_cache/lib

# LD_LIBRARY_PATH=./lily_cache/lib
# export LD_LIBRARY_PATH

gcc -Wall -O0 -O -O2 -O3 $1 -llily_runtime_c -I $2 -L ./lily_cache/lib -o ./lily_cache/bin/$4
