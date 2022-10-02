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

#ifndef LILY_RUNTIME_GENERIC_H
#define LILY_RUNTIME_GENERIC_H

#include <lang/runtime/Array.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct Generic
    {
        union
        {
            void *ptr;                  // *
            void *ref;                  // &
            MutStr str;                 // Str
            MutChar char_;              // Char
            MutI8 i8;                   // Int8
            MutI16 i16;                 // Int16
            MutI32 i32;                 // Int32
            MutI64 i64;                 // Int64
                                        // MutI128 i128; // Int128
            MutU8 u8;                   // Uint8
            MutU16 u16;                 // Uint16
            MutU32 u32;                 // Uint32
            MutU64 u64;                 // Uint64
                                        // MutU128 u128; // Uint128
            MutF32 f32;                 // Float32
            MutF64 f64;                 // Float64
            MutBool bool_;              // Bool
            MutUsize usize;             // Usize
            MutIsize isize;             // Isize
            void *any;                  // Any
            struct Optional op;         // Optional[T]
            struct Exception ext;       // Exception
            const struct Generic *imut; // Imutable[T]
            struct Array *array;        // Array[T]
        };
    } Generic;

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_GENERIC_H
