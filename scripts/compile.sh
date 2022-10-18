#!/bin/sh

# MIT License
#
# Copyright (c) 2022 ArthurPV
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
