#include <base/new.h>
#include <base/string.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <lang/parser/ast.h>
#include <stdarg.h>

static inline struct Builtin *
Load_Int8_module();
static inline void
__params__(struct Vec *params, Usize count, ...);

static inline void
__params__(struct Vec *params, Usize count, ...)
{
    va_list vl;
    va_start(vl, count);

    for (count--;;)
        push__Vec(params, va_arg(vl, struct DataType *));

    va_end(vl);
}

#define PARAMS(count, ...) __params__(params, count, __VA_ARGS__)

static inline struct Builtin *
Load_Int8_module()
{
    struct Vec *items = NEW(Vec, sizeof(struct Builtin));

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "+", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "-", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "*", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "/", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "%", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "**", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "<<", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, ">>", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "<<=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, ">>=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "|", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "&", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "|=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "&=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "~", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "~=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "xor", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "xor=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFun, "==", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFun, "not=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(2, NEW(DataType, DataTypeKindI8), NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "-", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8));

        push__Vec(items, NEW(BuiltinFun, "..", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFun, "<", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFun, ">", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFun, "<=", params));
    }

    {
        struct Vec *params = NEW(Vec, sizeof(struct DataType));

        PARAMS(3,
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindI8),
               NEW(DataType, DataTypeKindBool));

        push__Vec(items, NEW(BuiltinFun, ">=", params));
    }

    return NEW(BuiltinModuleVar, NEW(BuiltinModule, "Int8", items));
}

struct Vec *
Load_C_builtins()
{
    struct Vec *builtins = NEW(Vec, sizeof(struct Builtin));

    push__Vec(builtins, Load_Int8_module());

    return builtins;
}