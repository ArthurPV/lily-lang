#include <base/macros.h>
#include <lang/analysis/symbol_table.h>
#include <lang/analysis/typecheck.h>
#include <lang/parser/ast.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>

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
        .funs = NEW(Vec, sizeof(struct FunSymbol)),
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
    for (Usize i = 0; i < len__Vec(*self.funs); i++)
        FREE(FunSymbol, (struct FunSymbol *)get__Vec(*self.funs, i));

    FREE(Vec, self.funs);
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
        var_expr = expr_to_symbol(*expr.value.variable.expr, get__Option(expr.value.variable.data_type));
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

            return NEW(
              ExprSymbolUnary, expr, NEW(UnaryOpSymbol, expr, dt, right));
        }
        default:
            TODO("");
    }
}
