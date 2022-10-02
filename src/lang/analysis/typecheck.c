#include <base/macros.h>
#include <lang/analysis/symbol_table.h>
#include <lang/analysis/typecheck.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <lang/parser/ast.h>

static inline struct DataType *
get_data_type_of_expr_symbol(struct ExprSymbol *symb);
static struct SymbolTable *
check_function(struct Decl decl);
static struct ExprSymbol *
check_variable(struct Expr expr);
static inline void
verify_expr_data_type(struct Expr expr,
                      struct DataType *already_defined_data_type);
static inline struct DataType *
infer_on_integer(struct Expr expr);
static inline struct DataType *
expr_to_data_type(struct Expr expr, struct DataType *already_defined_data_type);
static inline struct ExprSymbol *
expr_to_symbol(struct Expr expr, struct DataType *already_defined_data_type);

struct Typecheck
__new__Typecheck(struct Parser parser)
{
    struct Typecheck self = {
        .parser = parser,
        .pos = 0,
        .count_error = 0,
        .count_warning = 0,
        .current_fun_id = 0,
        .current_const_id = 0,
        .current_module_id = 0,
        .current_alias_id = 0,
        .current_record_id = 0,
        .current_enum_id = 0,
        .current_error_id = 0,
        .current_class_id = 0,
        .current_trait_id = 0,
        .current_record_obj_id = 0,
        .current_enum_obj_id = 0,
        .funs = NULL,
        .consts = NULL,
        .aliases = NULL,
        .records = NULL,
        .enums = NULL,
        .errors = NULL,
        .classes = NULL,
        .traits = NULL,
        .records_obj = NULL,
        .enums_obj = NULL,
        .builtins = Load_C_builtins(),
    };

    return self;
}

void
run__Typecheck(struct Typecheck *self)
{
}

void
__free__Typecheck(struct Typecheck self)
{
    for (Usize i = len__Vec(*self.builtins); i--;)
        FREE(BuiltinAll, get__Vec(*self.builtins, i));

    FREE(Vec, self.builtins);

    if (self.funs != NULL) {
        for (Usize i = len__Vec(*self.funs); i--;)
            FREE(FunSymbol, get__Vec(*self.funs, i));

        FREE(Vec, self.funs);
    }

    if (self.consts != NULL) {
        for (Usize i = len__Vec(*self.consts); i--;)
            FREE(ConstantSymbol, get__Vec(*self.consts, i));

        FREE(Vec, self.consts);
    }

    if (self.modules != NULL) {
        for (Usize i = len__Vec(*self.modules); i--;)
            FREE(ModuleSymbol, get__Vec(*self.modules, i));

        FREE(Vec, self.modules);
    }

    if (self.aliases != NULL) {
        for (Usize i = len__Vec(*self.aliases); i--;)
            FREE(AliasSymbol, get__Vec(*self.aliases, i));

        FREE(Vec, self.aliases);
    }

    if (self.records != NULL) {
        for (Usize i = len__Vec(*self.records); i--;)
            FREE(RecordSymbol, get__Vec(*self.records, i));

        FREE(Vec, self.records);
    }

    if (self.records_obj != NULL) {
        for (Usize i = len__Vec(*self.records_obj); i--;)
            FREE(RecordObjSymbol, get__Vec(*self.records_obj, i));

        FREE(Vec, self.records_obj);
    }

    if (self.enums != NULL) {
        for (Usize i = len__Vec(*self.enums); i--;)
            FREE(EnumSymbol, get__Vec(*self.enums, i));

        FREE(Vec, self.enums);
    }

    if (self.enums_obj != NULL) {
        for (Usize i = len__Vec(*self.enums_obj); i--;)
            FREE(EnumObjSymbol, get__Vec(*self.enums_obj, i));

        FREE(Vec, self.enums_obj);
    }

    if (self.errors != NULL) {
        for (Usize i = len__Vec(*self.errors); i--;)
            FREE(ErrorSymbol, get__Vec(*self.errors, i));

        FREE(Vec, self.errors);
    }

    if (self.classes != NULL) {
        for (Usize i = len__Vec(*self.classes); i--;)
            FREE(ClassSymbol, get__Vec(*self.classes, i));

        FREE(Vec, self.classes);
    }


    if (self.traits != NULL) {
        for (Usize i = len__Vec(*self.traits); i--;)
            FREE(TraitSymbol, get__Vec(*self.traits, i));

        FREE(Vec, self.traits);
    }

    FREE(Parser, self.parser);
}

static struct SymbolTable *
check_function(struct Decl decl)
{
}

static inline struct DataType *
get_data_type_of_expr_symbol(struct ExprSymbol *symb)
{
    switch (symb->kind) {
        case ExprKindUnaryOp:
            return symb->value.unary_op.data_type;
        case ExprKindBinaryOp:
            return symb->value.binary_op.data_type;
        case ExprKindFunCall:
            return symb->value.fun_call.data_type;
        default:
            TODO("");
    }
}

static struct ExprSymbol *
check_variable(struct Expr expr)
{
    struct ExprSymbol *var_expr = NULL;
    struct DataType *var_dt = NULL;

    if (is_Some__Option(expr.value.variable.data_type))
        var_expr = expr_to_symbol(*expr.value.variable.expr,
                                  get__Option(expr.value.variable.data_type));
    else
        var_expr = expr_to_data_type(*expr.value.variable.expr, NULL);

    if (var_dt == NULL)
        var_dt = copy__DataType(get_data_type_of_expr_symbol(var_expr));
}

static inline void
verify_expr_data_type(struct Expr expr,
                      struct DataType *already_defined_data_type)
{
    switch (expr.kind) {
        case ExprKindLiteral: {
            switch (expr.kind) {
                case LiteralKindBool:
                    if (already_defined_data_type->kind != DataTypeKindBool)
                        assert(0 && "error");
                    break;
                default:
                    TODO("");
            }
        }
        default:
            TODO("");
    }
}

static Int128 MinUInt8 = 0x0;
static Int128 MaxUInt8 = 0xFF;
static Int128 MinUInt16 = 0x100;
static Int128 MaxUInt16 = 0xFFFF;
static Int128 MinUInt32 = 0x10000;
static Int128 MaxUInt32 = 0xFFFFFFFF;
static Int128 MinUInt64 = 0x100000000;
static Int128 MaxUInt64 = 0xFFFFFFFFFFFFFFFF;

static Int128 MinInt8 = -0x80;
static Int128 MaxInt8 = 0x7F;
static Int128 MinInt16 = -0x8000;
static Int128 MaxInt16 = 0x7FFF;
static Int128 MinInt32 = -0x80000000;
static Int128 MaxInt32 = 0x7FFFFFFF;
static Int128 MinInt64 = -0x8000000000000000;
static Int128 MaxInt64 = 0x7FFFFFFFFFFFFFFF;

static inline struct DataType *
infer_on_integer(struct Expr expr)
{
    switch (expr.value.literal.kind) {
        case LiteralKindInt32:
            if (expr.value.literal.value.int32 >= MinInt8 &&
                expr.value.literal.value.int32 <= MaxInt8)
                return NEW(DataType, DataTypeKindI8);
            else if (expr.value.literal.value.int32 >= MinInt16 &&
                     expr.value.literal.value.int32 <= MaxInt16)
                return NEW(DataType, DataTypeKindI16);
            else
                return NEW(DataType, DataTypeKindI32);
        case LiteralKindInt64:
            return NEW(DataType, DataTypeKindI64);
        case LiteralKindInt128:
            return NEW(DataType, DataTypeKindI128);
    }
}

static inline struct DataType *
expr_to_data_type(struct Expr expr, struct DataType *already_defined_data_type)
{
    switch (expr.kind) {
        case ExprKindUnaryOp: {
            switch (expr.value.unary_op.kind) {
                case UnaryOpKindNegative: {
                    struct DataType *already = NEW(DataType, DataTypeKindBool);

                    verify_expr_data_type(*expr.value.unary_op.right, already);

                    return already;
                }
                case UnaryOpKindNot: {
                    struct DataType *already = NEW(DataType, DataTypeKindBool);

                    verify_expr_data_type(*expr.value.unary_op.right, already);

                    return already;
                }
                case UnaryOpKindReference: {
                    break;
                }
            }
            break;
        }
        case ExprKindLiteral: {
            switch (expr.value.literal.kind) {
                case LiteralKindBool:
                    return NEW(DataType, DataTypeKindBool);
                case LiteralKindChar:
                    return NEW(DataType, DataTypeKindChar);
                case LiteralKindBitChar:
                    assert(0 && "todo");
                case LiteralKindInt32: {
                    if (already_defined_data_type != NULL) {
                        switch (already_defined_data_type->kind) {
                            case DataTypeKindI32:
                            case DataTypeKindI16:
                            case DataTypeKindI8:
                                return already_defined_data_type;
                            default:
                                assert(0 && "error");
                        }
                    } else
                        return infer_on_integer(expr);
                }
                default:
                    TODO("")
            }
        }
        default:
            break;
    }
}

static inline struct ExprSymbol *
expr_to_symbol(struct Expr expr, struct DataType *already_defined_data_type)
{
    switch (expr.kind) {
        case ExprKindUnaryOp: {
            struct ExprSymbol *right = expr_to_symbol(
              *expr.value.unary_op.right, already_defined_data_type);
            struct DataType *dt =
              expr_to_data_type(expr, already_defined_data_type);

            /* return NEW(
              ExprSymbolUnary, expr, NEW(UnaryOpSymbol, expr, dt, right)); */
        }
        default:
            TODO("");
    }
}
