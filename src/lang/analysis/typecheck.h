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

#ifndef LILY_TYPECHECK_H
#define LILY_TYPECHECK_H

#include <lang/parser/parser.h>

typedef struct Typecheck
{
    struct Parser parser;
    struct Scope *global;
    struct Vec *funs;        // struct Vec<struct SymbolTable*>*
    struct Vec *consts;      // struct Vec<struct SymbolTable*>*
    struct Vec *modules;     // struct Vec<struct SymbolTable*>*
    struct Vec *aliases;     // struct Vec<struct SymbolTable*>*
    struct Vec *records;     // struct Vec<struct SymbolTable*>*
    struct Vec *enums;       // struct Vec<struct SymbolTable*>*
    struct Vec *errors;      // struct Vec<struct SymbolTable*>*
    struct Vec *classes;     // struct Vec<struct SymbolTable*>*
    struct Vec *traits;      // struct Vec<struct SymbolTable*>*
    struct Vec *records_obj; // struct Vec<struct SymbolTable*>*
    struct Vec *enums_obj;   // struct Vec<struct SymbolTable*>
} Typecheck;

typedef struct Compiler
{
    struct Typecheck *tc;
    struct Vec *buffers;       // struct Vec<struct Compiler*>*
    struct Vec *builtins;      // struct Vec<struct Builtin*>*
    struct Vec *import_values; // struct Vec<struct Tuple<struct SymbolTable*>,
                               // int*, Str>* the int* value represents if the
                               // SymbolTable* is free
} Compiler;

/**
 *
 * @brief Construct the Typecheck type.
 */
struct Typecheck
__new__Typecheck(struct Parser parser);

/**
 *
 * @brief Run the typecheck phase.
 */
void
run__Typecheck(struct Compiler *self, struct Vec *primary_buffer);

/**
 *
 * @brief Free the Typecheck type.
 */
void
__free__Typecheck(struct Typecheck self);

/**
 *
 * @brief Construct the Compiler type.
 */
struct Compiler
__new__Compiler(struct Typecheck *tc);

/**
 *
 * @brief Free the Compiler type.
 */
void
__free__Compiler(struct Compiler self);

#endif // LILY_TYPECHECK_H
