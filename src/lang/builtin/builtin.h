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

#ifndef LILY_BUILTIN_H
#define LILY_BUILTIN_H

#include <base/types.h>

enum BuiltinKind
{
    BuiltinKindFun,
    BuiltinKindModule
};

typedef struct BuiltinFun
{
    Str name;
    struct Vec *params; // struct Vec<struct DataTypeSymbol*>*
    // Last params is the return type
} BuiltinFun;

/**
 *
 * @brief Construct the BuiltinFun type.
 */
struct BuiltinFun *
__new__BuiltinFun(const Str name, struct Vec *params);

/**
 *
 * @brief Free the BuiltinFun type.
 */
void
__free__BuiltinFun(struct BuiltinFun *self);

typedef struct BuiltinModule
{
    Str name;
    struct Vec *items; // struct Vec<struct Builtin*>*
} BuiltinModule;

/**
 *
 * @brief Construct the BuiltinModule type.
 */
struct BuiltinModule *
__new__BuiltinModule(const Str name, struct Vec *items);

/**
 *
 * @brief Free the BuiltinFun type.
 */
void
__free__BuiltinModule(struct BuiltinModule *self);

typedef struct Builtin
{
    enum BuiltinKind kind;

    union
    {
        struct BuiltinFun *fun;
        struct BuiltinModule *module;
    };
} Builtin;

/**
 *
 * @brief Construct the Builtin type (Fun variant).
 */
struct Builtin *
__new__BuiltinFunVar(struct BuiltinFun *fun);

/**
 *
 * @brief Construct the Builtin type (Module variant).
 */
struct Builtin *
__new__BuiltinModuleVar(struct BuiltinModule *module);

/**
 *
 * @brief Free the Builtin type (Fun variant).
 */
void
__free__BuiltinFunVar(struct Builtin *self);

/**
 *
 * @brief Free the Builtin type (Module variant).
 */
void
__free__BuiltinModuleVar(struct Builtin *self);

/**
 *
 * @brief Free the Builtin type (all variants).
 */
void
__free__BuiltinAll(struct Builtin *self);

#endif // LILY_BUILTIN_H
