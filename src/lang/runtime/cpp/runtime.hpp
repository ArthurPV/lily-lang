/*
 * MIT License
 *
 * Copyright (c) 2022 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_RUNTIME_HPP
#define LILY_RUNTIME_HPP

#include <c/api.h>
#include <cstdint>
#include <sys/types.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    //     typedef char *Str;
    //
    //     typedef uint8_t U8;
    //     typedef uint16_t U16;
    //     typedef uint32_t U32;
    //     typedef uint64_t U64;
    //
    //     typedef int8_t I8;
    //     typedef int16_t I16;
    //     typedef int32_t I32;
    //     typedef int64_t I64;
    //
    //     typedef size_t Usize;
    //     typedef ssize_t Isize;
    //
    //     typedef void Unit;

#define RUNTIME_ASSERT(expr) \
    if (!(expr))             \
        assert(0 && "runtime error");
#define RUNTIME_ERROR(msg) throw("Runtime Error: " + msg)

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_HPP
