#include <base/macros.h>
#include <lang/analysis/symbol_table.h>

struct Scope
__new__Scope(const Str filename, Usize id)
{
    struct Scope self = { .filename = filename, .id = id, .sub_id = 0 };
    return self;
}

struct FunSymbol *
__new__FunSymbol(struct Decl *fun_decl, const Str filename, Usize id)
{
    enum Visibility v;

    if (fun_decl->value.fun->is_pub)
        v = VisibilityPublic;
    else
        v = VisibilityPrivate;

    struct FunSymbol *self = malloc(sizeof(struct FunSymbol));
    self->name = &*fun_decl->value.fun->name;
    self->taged_type = NEW(Vec, sizeof(struct SymbolTable));
    self->generic_params = &*fun_decl->value.fun->generic_params;
    self->params = &*fun_decl->value.fun->params;
    self->visibility = v;
    self->is_async = fun_decl->value.fun->is_async;
    self->return_type = fun_decl->value.fun->return_type;
    self->body = NEW(Vec, sizeof(struct SymbolTable));
    self->scope = NEW(Scope, filename, id);
    self->fun_decl = &*fun_decl;

    return self;
}

void
__free__FunSymbol(struct FunSymbol *self)
{
    FREE(Vec, self->taged_type);
    FREE(Vec, self->body);
    free(self);
}

struct UnaryOpSymbol
__new__UnaryOpSymbol(struct Expr unary_op,
                     struct DataType *data_type,
                     struct ExprSymbol *right)
{
    struct UnaryOpSymbol self = { .kind = unary_op.kind,
                                  .data_type = data_type,
                                  .right = right };

    return self;
}

void
__free__UnaryOpSymbol(struct UnaryOpSymbol self)
{
    FREE(DataTypeAll, self.data_type);
}

struct BinaryOpSymbol
__new__BinaryOpSymbol(struct Expr binary_op,
                      struct DataType *data_type,
                      struct ExprSymbol *left,
                      struct ExprSymbol *right)
{
    struct BinaryOpSymbol self = { .kind = binary_op.kind,
                                   .data_type = data_type,
                                   .left = left,
                                   .right = right };

    return self;
}

void
__free__BinaryOpSymbol(struct BinaryOpSymbol self)
{
    FREE(DataTypeAll, self.data_type);
}

struct FunCallSymbol
__new__FunCallSymbol(bool is_builtin, struct Scope loc, struct DataType *data_type, struct Vec *params)
{
    struct FunCallSymbol self = {
        .is_builtin = is_builtin,
        .loc = loc,
        .data_type = data_type,
        .params = params
    };

    return self;
}

void
__free__FunCallSymbol(struct FunCallSymbol self)
{
    FREE(DataTypeAll, self.data_type);
    TODO("FREE all");
    FREE(Vec, self.params);
}

struct ExprSymbol *
__new__ExprSymbolUnary(struct Expr expr, struct UnaryOpSymbol unary_op)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.unary_op = unary_op;
    return self;
}

void
__free__ExprSymbolUnary(struct ExprSymbol *self)
{
    FREE(UnaryOpSymbol, self->value.unary_op);
    free(self);
}

void
__free__ExprSymbolAll(struct ExprSymbol *self)
{
    switch (self->kind) {
        case ExprKindUnaryOp:
            FREE(ExprSymbolUnary, self);
            break;
        default:
            TODO("");
    }
}

struct SymbolTable *
__new__SymbolTableFun(struct FunSymbol *fun, struct Decl *fun_decl)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindFun, self->loc = fun_decl->loc;
    self->value.fun = fun;
    return self;
}

struct SymbolTable *
__new__SymbolTableExpr(struct ExprSymbol *expr, struct Expr *expr_ast)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindExpr, self->loc = expr_ast->loc;
    self->value.expr = expr;
    return self;
}

void
__free__SymbolTableFun(struct SymbolTable *self)
{
    FREE(FunSymbol, self->value.fun);
    free(self);
}

void
__free__SymbolTableExpr(struct SymbolTable *self)
{
    FREE(ExprSymbolAll, self->value.expr);
    free(self);
}

void
__free__SymbolTableAll(struct SymbolTable *self)
{
    switch (self->kind) {
        case SymbolTableKindFun:
            FREE(SymbolTableFun, self);
            break;
        case SymbolTableKindExpr:
            FREE(SymbolTableExpr, self);
            break;
        default:
            TODO("");
    }
}