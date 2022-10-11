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

#include <base/new.h>
#include <base/string.h>
#include <lang/analysis/symbol_table.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <stdarg.h>

struct Builtin *
Load_Int8_module();
struct Builtin *
Load_Int16_module();
struct Builtin *
Load_Int32_module();
struct Builtin *
Load_Int64_module();
struct Builtin *
Load_Int128_module();
struct Builtin *
Load_Uint8_module();
struct Builtin *
Load_Uint16_module();
struct Builtin *
Load_Uint32_module();
struct Builtin *
Load_Uint64_module();
struct Builtin *
Load_Uint128_module();
struct Builtin *
Load_Bool_module();
struct Builtin *
Load_Float32_module();
struct Builtin *
Load_Float64_module();
struct Builtin *
Load_Char_module();
struct Builtin *
Load_Str_module();
struct Builtin *
Load_Isize_module();
struct Builtin *
Load_Usize_module();
struct Builtin *
Load_Optional_module();
struct Builtin *
Load_Ptr_module();
struct Builtin *
Load_Tuple_module();
struct Builtin *
Load_Array_module();
struct Builtin *
Load_Fun_module();
struct Builtin *
Load_Ref_module();
struct Builtin *
Load_Custom_module();
struct Builtin *
Load_Mem_module();
struct Builtin *
Load_Io_module();
struct Builtin *
Load_Never_module();
static inline void
__params__(struct Vec *params, Usize count, ...);

static inline void
__params__(struct Vec *params, Usize count, ...)
{
    va_list vl;
    va_start(vl, count);

    for (Usize i = 0; i < count; i++)
        push__Vec(params, va_arg(vl, struct DataTypeSymbol *));

    va_end(vl);
}

#define PARAMS(count, ...) __params__(params, count, __VA_ARGS__)

struct Builtin *
Load_Int8_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int8", items));
}

struct Builtin *
Load_Int16_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int16", items));
}

struct Builtin *
Load_Int32_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int32", items));
}

struct Builtin *
Load_Int64_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int64", items));
}

struct Builtin *
Load_Int128_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int128", items));
}

struct Builtin *
Load_Uint8_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint8", items));
}

struct Builtin *
Load_Uint16_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint16", items));
}

struct Builtin *
Load_Uint32_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint32", items));
}

struct Builtin *
Load_Uint64_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint64", items));
}

struct Builtin *
Load_Uint128_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint128", items));
}

struct Builtin *
Load_Bool_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "and", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "or", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_float32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_float64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindBool),
               NEW(DataTypeSymbol, DataTypeKindStr));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "to_str", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Bool", items));
}

struct Builtin *
Load_Float32_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF32)),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF32)),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF32)),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF32)),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF32)),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF32)),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(1, NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "pi", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindStr));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "to_str", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sqrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cbrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log10", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "hypot", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tanh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF32),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atanh", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Float32", items));
}

struct Builtin *
Load_Float64_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF64)),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF64)),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF64)),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF64)),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF64)),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindF64)),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(1, NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "pi", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindStr));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "to_str", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sqrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cbrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log10", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "hypot", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tanh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindF64),
               NEW(DataTypeSymbol, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atanh", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Float64", items));
}

struct Builtin *
Load_Char_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "code", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindChar));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "chr", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "lowercase", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "uppercase", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindChar)),
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindChar),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Char", items));
}

struct Builtin *
Load_Str_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindStr)),
               NEW(DataTypeSymbol, DataTypeKindStr),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindStr),
               NEW(DataTypeSymbol, DataTypeKindStr),
               NEW(DataTypeSymbol, DataTypeKindStr));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "$", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindStr),
               NEW(DataTypeSymbol, DataTypeKindStr),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindStr),
               NEW(DataTypeSymbol, DataTypeKindStr),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Str", items));
}

struct Builtin *
Load_Isize_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Isize", items));
}

struct Builtin *
Load_Usize_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Usize", items));
}

struct Builtin *
Load_Optional_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Optional", items));
}

struct Builtin *
Load_Ptr_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Ptr", items));
}

struct Builtin *
Load_Tuple_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolTuple,
                   init__Vec(sizeof(struct DataTypeSymbol),
                             1,
                             NEW(DataTypeSymbolCompilerDefined,
                                 NEW(CompilerDefinedDataType, "A", true)))),
               NEW(DataTypeSymbolTuple,
                   init__Vec(sizeof(struct DataTypeSymbol),
                             1,
                             NEW(DataTypeSymbolCompilerDefined,
                                 NEW(CompilerDefinedDataType, "A", true)))),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolTuple,
                   init__Vec(sizeof(struct DataTypeSymbol),
                             1,
                             NEW(DataTypeSymbolCompilerDefined,
                                 NEW(CompilerDefinedDataType, "A", true)))),
               NEW(DataTypeSymbolTuple,
                   init__Vec(sizeof(struct DataTypeSymbol),
                             1,
                             NEW(DataTypeSymbolCompilerDefined,
                                 NEW(CompilerDefinedDataType, "A", true)))),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Tuple", items));
}

struct Builtin *
Load_Array_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolArray,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false)),
                   NULL),
               NEW(DataTypeSymbolArray,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false)),
                   NULL),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolArray,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false)),
                   NULL),
               NEW(DataTypeSymbolArray,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false)),
                   NULL),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Array", items));
}

struct Builtin *
Load_Fun_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Fun", items));
}

struct Builtin *
Load_Ref_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbolCompilerDefined,
                   NEW(CompilerDefinedDataType, "A", false)),
               NEW(DataTypeSymbolRef,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Ref", items));
}

struct Builtin *
Load_Custom_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))),
               NEW(DataTypeSymbolCompilerDefined,
                   NEW(CompilerDefinedDataType, "A", false)),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Custom", items));
}

struct Builtin *
Load_Mem_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(2,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "A", false))));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "malloc", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Mem", items));
}

struct Builtin *
Load_Io_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(
          3,
          NEW(DataTypeSymbol, DataTypeKindStr),
          NEW(DataTypeSymbolArray, NEW(DataTypeSymbol, DataTypeKindAny), NULL),
          NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "print", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(
          3,
          NEW(DataTypeSymbol, DataTypeKindStr),
          NEW(DataTypeSymbolArray, NEW(DataTypeSymbol, DataTypeKindAny), NULL),
          NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "println", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Io", items));
}

struct Builtin *
Load_Never_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Never", items));
}

struct Vec *
Load_C_builtins()
{
    return init__Vec(sizeof(struct Builtin),
                     27,
                     Load_Int8_module(),
                     Load_Int16_module(),
                     Load_Int32_module(),
                     Load_Int64_module(),
                     Load_Int128_module(),
                     Load_Uint8_module(),
                     Load_Uint16_module(),
                     Load_Uint32_module(),
                     Load_Uint64_module(),
                     Load_Uint128_module(),
                     Load_Bool_module(),
                     Load_Float32_module(),
                     Load_Float64_module(),
                     Load_Char_module(),
                     Load_Str_module(),
                     Load_Isize_module(),
                     Load_Usize_module(),
                     Load_Optional_module(),
                     Load_Ptr_module(),
                     Load_Tuple_module(),
                     Load_Array_module(),
                     Load_Fun_module(),
                     Load_Ref_module(),
                     Load_Custom_module(),
                     Load_Mem_module(),
                     Load_Io_module(),
                     Load_Never_module());
}
