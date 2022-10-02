#include <base/new.h>
#include <base/string.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <lang/parser/ast.h>
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
static inline void
__params__(struct Vec *params, Usize count, ...);

static inline void
__params__(struct Vec *params, Usize count, ...)
{
    va_list vl;
    va_start(vl, count);

    for (Usize i = 0; i < count; i++)
        push__Vec(params, va_arg(vl, struct DataType *));

    va_end(vl);
}

#define PARAMS(count, ...) __params__(params, count, __VA_ARGS__)

static struct Builtin *
Load_Int8_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI8)),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(2, NEW(DataType, DataTypeKindI8), NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int8", items));
}

static struct Builtin *
Load_Int16_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI16)),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindI16), NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindI16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int16", items));
}

static struct Builtin *
Load_Int32_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI32)),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindI32), NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindI32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int32", items));
}

static struct Builtin *
Load_Int64_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI64)),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindI64), NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindI64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int64", items));
}

static struct Builtin *
Load_Int128_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindI128)),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindI128), NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindI128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int128", items));
}

static struct Builtin *
Load_Uint8_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU8)),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(2, NEW(DataType, DataTypeKindU8), NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindU8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint8", items));
}

static struct Builtin *
Load_Uint16_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU16)),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindU16), NEW(DataType, DataTypeKindI16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindU16),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint16", items));
}

static struct Builtin *
Load_Uint32_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU32)),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindU32), NEW(DataType, DataTypeKindI32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindU32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint32", items));
}

static struct Builtin *
Load_Uint64_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU64)),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindU64), NEW(DataType, DataTypeKindI64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindU64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint64", items));
}

static struct Builtin *
Load_Uint128_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">>=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "|=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "&=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "~=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindU128)),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "xor=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindU128), NEW(DataType, DataTypeKindI128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "..", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindU128),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Uint128", items));
}

static struct Builtin *
Load_Bool_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindBool), NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "and", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "or", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindI16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindI32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindI64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindI128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindU8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindU16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindU32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindU64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindU128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_float32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_float64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindBool),
               NEW(DataType, DataTypeKindStr));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "to_str", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Bool", items));
}

static struct Builtin *
Load_Float32_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF32)),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF32)),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF32)),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF32)),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF32)),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF32)),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(1, NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "pi", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindI8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindI16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindI32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindI64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindI128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindU8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindU16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindU32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindU64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindU128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindStr));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "to_str", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sqrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cbrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log10", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32),
               NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "hypot", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tanh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF32), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atanh", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Float32", items));
}

static struct Builtin *
Load_Float64_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "**", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF64)),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF64)),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "+=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF64)),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF64)),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "*=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF64)),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "/=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataTypeMut, NEW(DataType, DataTypeKindF64)),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindUnit));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "%=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "==", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "not=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF32));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "-", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "<=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, ">=", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(1, NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "pi", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindI8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindI16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindI32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindI64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindI128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_int128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindU8));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint8", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindU16));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint16", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindU32));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint32", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindU64));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint64", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindU128));

        push__Vec(items,
                  NEW(BuiltinFunVar, NEW(BuiltinFun, "to_uint128", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindStr));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "to_str", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sqrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cbrt", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "exp2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log10", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "log2", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acos", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asin", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atan", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64),
               NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "hypot", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "cosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "sinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "tanh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "acosh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "asinh", params)));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(
          2, NEW(DataType, DataTypeKindF64), NEW(DataType, DataTypeKindF64));

        push__Vec(items, NEW(BuiltinFunVar, NEW(BuiltinFun, "atanh", params)));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Float64", items));
}

struct Vec *
Load_C_builtins()
{
    struct Vec *builtins = NEW(Vec, sizeof(struct Builtin));

    push__Vec(builtins, Load_Int8_module());
    push__Vec(builtins, Load_Int16_module());
    push__Vec(builtins, Load_Int32_module());
    push__Vec(builtins, Load_Int64_module());
    push__Vec(builtins, Load_Int128_module());
    push__Vec(builtins, Load_Uint8_module());
    push__Vec(builtins, Load_Uint16_module());
    push__Vec(builtins, Load_Uint32_module());
    push__Vec(builtins, Load_Uint64_module());
    push__Vec(builtins, Load_Uint128_module());
    push__Vec(builtins, Load_Bool_module());
    push__Vec(builtins, Load_Float32_module());
    push__Vec(builtins, Load_Float64_module());

    return builtins;
}
