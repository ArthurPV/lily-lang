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

#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>
#include <lang/analysis/symbol_table.h>
#include <lang/builtin/builtin.h>
#include <stdlib.h>

struct BuiltinFun *
__new__BuiltinFun(const Str name, struct Vec *params)
{
    struct BuiltinFun *self = malloc(sizeof(struct BuiltinFun));
    self->name = name;
    self->params = params;
    return self;
}

void
__free__BuiltinFun(struct BuiltinFun *self)
{
    for (Usize i = len__Vec(*self->params); i--;)
        FREE(DataTypeSymbolAll, get__Vec(*self->params, i));

    FREE(Vec, self->params);
    free(self);
}

struct BuiltinModule *
__new__BuiltinModule(const Str name, struct Vec *items)
{
    struct BuiltinModule *self = malloc(sizeof(struct BuiltinModule));
    self->name = name;
    self->items = items;
    return self;
}

void
__free__BuiltinModule(struct BuiltinModule *self)
{
    for (Usize i = len__Vec(*self->items); i--;)
        FREE(BuiltinAll, get__Vec(*self->items, i));

    FREE(Vec, self->items);
    free(self);
}

struct Builtin *
__new__BuiltinFunVar(struct BuiltinFun *fun)
{
    struct Builtin *self = malloc(sizeof(struct Builtin));
    self->kind = BuiltinKindFun;
    self->fun = fun;
    return self;
}

struct Builtin *
__new__BuiltinModuleVar(struct BuiltinModule *module)
{
    struct Builtin *self = malloc(sizeof(struct Builtin));
    self->kind = BuiltinKindModule;
    self->module = module;
    return self;
}

void
__free__BuiltinFunVar(struct Builtin *self)
{
    FREE(BuiltinFun, self->fun);
    free(self);
}

void
__free__BuiltinModuleVar(struct Builtin *self)
{
    FREE(BuiltinModule, self->module);
    free(self);
}

void
__free__BuiltinAll(struct Builtin *self)
{
    switch (self->kind) {
        case BuiltinKindFun:
            FREE(BuiltinFunVar, self);
            break;
        case BuiltinKindModule:
            FREE(BuiltinModuleVar, self);
            break;
        default:
            UNREACHABLE("unknown builtin kind");
    }
}
