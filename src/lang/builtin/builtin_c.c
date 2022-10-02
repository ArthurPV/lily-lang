#include <base/new.h>
#include <base/string.h>
#include <lang/analysis/symbol_table.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <stdarg.h>

static struct Builtin *
Load_Int8_module();
static struct Builtin *
Load_Int16_module();
static struct Builtin *
Load_Int32_module();
static struct Builtin *
Load_Int64_module();
static struct Builtin *
Load_Int128_module();
static struct Builtin *
Load_Uint8_module();
static struct Builtin *
Load_Uint16_module();
static struct Builtin *
Load_Uint32_module();
static struct Builtin *
Load_Uint64_module();
static struct Builtin *
Load_Uint128_module();
static struct Builtin *
Load_Bool_module();
static struct Builtin *
Load_Float32_module();
static struct Builtin *
Load_Float64_module();
static struct Builtin *
Load_Char_module();
static struct Builtin *
Load_Str_module();
static struct Builtin *
Load_Isize_module();
static struct Builtin *
Load_Usize_module();
static struct Builtin *
Load_Optional_module();
static struct Builtin *
Load_Ptr_module();
static struct Builtin *
Load_Tuple_module();
static struct Builtin *
Load_Array_module();
static struct Builtin *
Load_Fun_module();
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI8)),
               NEW(DataTypeSymbol, DataTypeKindI8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI16)),
               NEW(DataTypeSymbol, DataTypeKindI16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI32)),
               NEW(DataTypeSymbol, DataTypeKindI32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI64)),
               NEW(DataTypeSymbol, DataTypeKindI64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindI128)),
               NEW(DataTypeSymbol, DataTypeKindI128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU8)),
               NEW(DataTypeSymbol, DataTypeKindU8),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU16)),
               NEW(DataTypeSymbol, DataTypeKindU16),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU32)),
               NEW(DataTypeSymbol, DataTypeKindU32),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU64)),
               NEW(DataTypeSymbol, DataTypeKindU64),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindU128)),
               NEW(DataTypeSymbol, DataTypeKindU128),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

static struct Builtin *
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

static struct Builtin *
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

static struct Builtin *
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

static struct Builtin *
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindIsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindIsize)),
               NEW(DataTypeSymbol, DataTypeKindIsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
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

        PARAMS(3,
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUsize));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolMut, NEW(DataTypeSymbol, DataTypeKindUsize)),
               NEW(DataTypeSymbol, DataTypeKindUsize),
               NEW(DataTypeSymbol, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
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

static struct Builtin *
Load_Optional_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbolOptional,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Optional", items));
}

static struct Builtin *
Load_Ptr_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbolPtr,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL)),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Ptr", items));
}

static struct Builtin *
Load_Tuple_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(
          3,
          NEW(DataTypeSymbolTuple,
              init__Vec(
                sizeof(struct DataTypeSymbol),
                1,
                NEW(DataTypeSymbolCustom,
                    init__Vec(sizeof(struct String), 1, from__String("T...")),
                    NULL,
                    NULL))),
          NEW(DataTypeSymbolTuple,
              init__Vec(
                sizeof(struct DataTypeSymbol),
                1,
                NEW(DataTypeSymbolCustom,
                    init__Vec(sizeof(struct String), 1, from__String("T...")),
                    NULL,
                    NULL))),
          NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(
          3,
          NEW(DataTypeSymbolTuple,
              init__Vec(
                sizeof(struct DataTypeSymbol),
                1,
                NEW(DataTypeSymbolCustom,
                    init__Vec(sizeof(struct String), 1, from__String("T...")),
                    NULL,
                    NULL))),
          NEW(DataTypeSymbolTuple,
              init__Vec(
                sizeof(struct DataTypeSymbol),
                1,
                NEW(DataTypeSymbolCustom,
                    init__Vec(sizeof(struct String), 1, from__String("T...")),
                    NULL,
                    NULL))),
          NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Tuple", items));
}

static struct Builtin *
Load_Array_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(3,
               NEW(DataTypeSymbolArray,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL),
                   NULL),
               NEW(DataTypeSymbolArray,
                   NEW(DataTypeSymbolCustom,
                       init__Vec(sizeof(struct String), 1, from__String("T")),
                       NULL,
                       NULL),
                   NULL),
               NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

        PARAMS(
          3,
          NEW(DataTypeSymbolArray,
              NEW(DataTypeSymbolCustom,
                  init__Vec(sizeof(struct String), 1, from__String("T...")),
                  NULL,
                  NULL),
              NULL),
          NEW(DataTypeSymbolArray,
              NEW(DataTypeSymbolCustom,
                  init__Vec(sizeof(struct String), 1, from__String("T...")),
                  NULL,
                  NULL),
              NULL),
          NEW(DataTypeSymbol, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Array", items));
}

static struct Builtin *
Load_Fun_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Fun", items));
}

struct Vec *
Load_C_builtins()
{
    return init__Vec(sizeof(struct Builtin),
                     22,
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
                     Load_Fun_module());
}
