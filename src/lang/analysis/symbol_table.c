#include <base/macros.h>
#include <lang/analysis/symbol_table.h>

struct DataTypeSymbol *
__new__DataTypeSymbol(enum DataTypeKind kind)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = kind;
    self->scope = NULL;
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolPtr(struct DataTypeSymbol *ptr)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindPtr;
    self->scope = NULL;
    self->value.ptr = ptr;
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolRef(struct DataTypeSymbol *ref)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindRef;
    self->scope = NULL;
    self->value.ref = ref;
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolOptional(struct DataTypeSymbol *optional)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindOptional;
    self->scope = NULL;
    self->value.optional = optional;
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolException(struct DataTypeSymbol *exception)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindException;
    self->scope = NULL;
    self->value.exception = exception;
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolLambda(struct Vec *params,
                            struct DataTypeSymbol *return_type)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindLambda;
    self->scope = NULL;
    self->value.lambda = NEW(Tuple, 2, params, return_type);
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolArray(struct DataTypeSymbol *data_type,
                           struct Option *size)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindArray;
    self->scope = NULL;
    self->value.array = NEW(Tuple, 2, data_type, size);
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolCustom(struct Vec *generic_params, struct Scope *scope)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindCustom;
    self->scope = scope;
    self->value.custom = generic_params;
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolTuple(struct Vec *tuple)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindTuple;
    self->scope = NULL;
    self->value.tuple = tuple;
    return self;
}

void
__free__DataTypeSymbolAll(struct DataTypeSymbol *self)
{
    switch (self->kind) {
        case DataTypeKindPtr:
            FREE(DataTypeSymbolPtr, self);
            break;
        case DataTypeKindRef:
            FREE(DataTypeSymbolRef, self);
            break;
        case DataTypeKindOptional:
            FREE(DataTypeSymbolOptional, self);
            break;
        case DataTypeKindException:
            FREE(DataTypeSymbolException, self);
            break;
        case DataTypeKindLambda:
            FREE(DataTypeSymbolLambda, self);
            break;
        case DataTypeKindArray:
            FREE(DataTypeSymbolArray, self);
            break;
        case DataTypeKindCustom:
            FREE(DataTypeSymbolCustom, self);
            break;
        case DataTypeKindTuple:
            FREE(DataTypeSymbolTuple, self);
            break;
        default:
            FREE(DataTypeSymbol, self);
            break;
    }
}

void
__free__DataTypeSymbolLambda(struct DataTypeSymbol *self)
{
    FREE(Vec, self->value.lambda->items[0]);
    FREE(DataTypeSymbolAll, self->value.lambda->items[1]);
    FREE(Tuple, self->value.lambda);
    free(self);
}

void
__free__DataTypeSymbolArray(struct DataTypeSymbol *self)
{
    if (self->value.array->items[0] != NULL)
        FREE(DataTypeSymbolAll, self->value.array->items[0]);

    FREE(Tuple, self->value.array);
    free(self);
}

void
__free__DataTypeSymbolCustom(struct DataTypeSymbol *self)
{
    if (self->value.custom != NULL) {
        for (Usize i = len__Vec(*self->value.custom); i--;)
            FREE(DataTypeSymbolAll, get__Vec(*self->value.custom, i));

        FREE(Vec, self->value.custom);
    }

    free(self);
}

void
__free__GenericSymbolRestrictedDataType(struct Generic *self)
{
    FREE(
      DataTypeSymbolAll,
      ((struct Tuple *)self->value.restricted_data_type->items[1])->items[0]);
    free(
      ((struct Tuple *)self->value.restricted_data_type->items[1])->items[1]);
    FREE(Tuple, self->value.restricted_data_type->items[1]);
    FREE(Tuple, self->value.restricted_data_type);
    free(self);
}

void
__free__GenericSymbolAll(struct Generic *self)
{
    switch (self->kind) {
        case GenericKindDataType:
            FREE(GenericDataType, self);
            break;
        case GenericKindRestrictedDataType:
            FREE(GenericSymbolRestrictedDataType, self);
            break;
        default:
            UNREACHABLE("unknown generic kind");
    }
}

inline void
__free__FieldCallSymbol(struct FieldCallSymbol *self)
{
    FREE(ExprSymbolAll, self->value);
    free(self);
}

void
__free__RecordCallSymbol(struct RecordCallSymbol self)
{
    if (self.fields != NULL) {
        for (Usize i = len__Vec(*self.fields); i--;) {
            FREE(FieldCallSymbol,
                 ((struct Tuple *)get__Vec(*self.fields, i))->items[0]);
            FREE(Tuple, get__Vec(*self.fields, i));
        }

        FREE(Vec, self.fields);
    }
}

void
__free__ArrayAccessSymbol(struct ArrayAccessSymbol self)
{
    for (Usize i = len__Vec(*self.access); i--;)
        FREE(ExprSymbolAll, get__Vec(*self.access, i));

    FREE(Vec, self.access);
}

void
__free__TupleAccessSymbol(struct TupleAccessSymbol self)
{
    for (Usize i = len__Vec(*self.access); i--;)
        FREE(ExprSymbolAll, get__Vec(*self.access, i));

    FREE(Vec, self.access);
}

void
__free__VariantSymbol(struct VariantSymbol self)
{
    FREE(ExprSymbolAll, self.value);
}

struct ExprSymbol *
__new__ExprSymbol(struct Expr *expr)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr->kind;
    self->loc = expr->loc;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolUnaryOp(struct Expr expr, struct UnaryOpSymbol unary_op)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.unary_op = unary_op;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolBinaryOp(struct Expr expr, struct BinaryOpSymbol binary_op)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.binary_op = binary_op;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolFunCall(struct Expr expr, struct FunCallSymbol fun_call)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.fun_call = fun_call;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolRecordCall(struct Expr expr,
                            struct RecordCallSymbol record_call)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.record_call = record_call;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolIdentifier(struct Expr expr, struct Scope *identifier)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.identifier = identifier;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolIdentifierAccess(struct Expr expr,
                                  struct Scope *identifier_access)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.identifier_access = identifier_access;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolArrayAccess(struct Expr expr,
                             struct ArrayAccessSymbol array_access)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.array_access = array_access;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolTupleAccess(struct Expr expr,
                             struct TupleAccessSymbol tuple_access)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.tuple_access = tuple_access;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolLambda(struct Expr expr, struct LambdaSymbol lambda)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.lambda = lambda;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolTuple(struct Expr expr, struct Vec *tuple)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.tuple = tuple;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolArray(struct Expr expr, struct Vec *array)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.array = array;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolVariant(struct Expr expr, struct VariantSymbol variant)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.variant = variant;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolTry(struct Expr expr, struct ExprSymbol *try)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.try = try;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolBlock(struct Expr expr, struct Vec *block)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.block = block;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolQuestionMark(struct Expr expr, struct Scope *question_mark)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.question_mark = question_mark;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolDereference(struct Expr expr, struct Scope *dereference)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.dereference = dereference;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolRef(struct Expr expr, struct Scope *ref)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.ref = ref;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolLiteral(struct Expr expr, struct LiteralSymbol literal)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.literal = literal;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolVariable(struct Expr expr, struct VariableSymbol *variable)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.variable = variable;
    return self;
}

void
__free__ExprSymbolFunCall(struct ExprSymbol *self)
{
    FREE(FunCallSymbol, self->value.fun_call);
    free(self);
}

void
__free__ExprSymbolLambda(struct ExprSymbol *self)
{
    TODO("Free lambda symbol");
    free(self);
}

void
__free__ExprSymbolTuple(struct ExprSymbol *self)
{
    for (Usize i = len__Vec(*self->value.tuple); i--;)
        FREE(ExprSymbolAll, get__Vec(*self->value.tuple, i));

    FREE(Vec, self->value.tuple);
    free(self);
}

void
__free__ExprSymbolArray(struct ExprSymbol *self)
{
    for (Usize i = len__Vec(*self->value.array); i--;)
        FREE(ExprSymbolAll, get__Vec(*self->value.array, i));

    FREE(Vec, self->value.array);
    free(self);
}

void
__free__ExprSymbolBlock(struct ExprSymbol *self)
{
    for (Usize i = len__Vec(*self->value.block); i--;)
        FREE(SymbolTableAll, get__Vec(*self->value.block, i));

    FREE(Vec, self->value.block);
    free(self);
}

void
__free__ExprSymbolVariable(struct ExprSymbol *self)
{
    FREE(VariableSymbol, self->value.variable);
    free(self);
}

void
__free__ExprSymbolAll(struct ExprSymbol *self)
{
    switch (self->kind) {
        case ExprKindUnaryOp:
            FREE(ExprSymbolUnaryOp, self);
            break;
        case ExprKindBinaryOp:
            FREE(ExprSymbolBinaryOp, self);
            break;
        case ExprKindFunCall:
            FREE(ExprSymbolFunCall, self);
            break;
        case ExprKindRecordCall:
            FREE(ExprSymbolRecordCall, self);
            break;
        case ExprKindIdentifier:
            FREE(ExprSymbolIdentifier, self);
            break;
        case ExprKindIdentifierAccess:
            FREE(ExprSymbolIdentifierAccess, self);
            break;
        case ExprKindArrayAccess:
            FREE(ExprSymbolArrayAccess, self);
            break;
        case ExprKindTupleAccess:
            FREE(ExprSymbolTupleAccess, self);
            break;
        case ExprKindLambda:
            FREE(ExprSymbolLambda, self);
            break;
        case ExprKindTuple:
            FREE(ExprSymbolTuple, self);
            break;
        case ExprKindArray:
            FREE(ExprSymbolArray, self);
            break;
        case ExprKindVariant:
            FREE(ExprSymbolVariant, self);
            break;
        case ExprKindTry:
            FREE(ExprSymbolTry, self);
            break;
        case ExprKindIf:
            TODO("free if block");
            break;
        case ExprKindBlock:
            FREE(ExprSymbolBlock, self);
            break;
        case ExprKindQuestionMark:
            FREE(ExprSymbolQuestionMark, self);
            break;
        case ExprKindDereference:
            FREE(ExprSymbolDereference, self);
            break;
        case ExprKindRef:
            FREE(ExprSymbolRef, self);
            break;
        case ExprKindLiteral:
            FREE(ExprSymbolLiteral, self);
            break;
        case ExprKindVariable:
            FREE(ExprSymbolVariable, self);
            break;
        default:
            FREE(ExprSymbol, self);
            break;
    }
}

void
__free__MatchSymbol(struct MatchSymbol self)
{
    FREE(ExprSymbolAll, self.matching);

    for (Usize i = len__Vec(*self.pattern); i--;) {
        FREE(ExprSymbolAll,
             ((struct Tuple *)get__Vec(*self.pattern, i))->items[0]);

        struct Vec *temp =
          ((struct Tuple *)get__Vec(*self.pattern, i))->items[1];

        for (Usize j = len__Vec(*temp); j--;)
            FREE(SymbolTableAll, get__Vec(*temp, j));

        FREE(Vec, temp);
        FREE(Tuple, get__Vec(*self.pattern, i));
    }

    FREE(Vec, self.pattern);
}

struct IfBranchSymbol *
__new__IfBranchSymbol(struct ExprSymbol *cond, struct Vec *body)
{
    struct IfBranchSymbol *self = malloc(sizeof(struct IfBranchSymbol));
    self->cond = cond;
    self->body = body;
    return self;
}

void
__free__IfBranchSymbol(struct IfBranchSymbol *self)
{
    FREE(ExprSymbolAll, self->cond);

    for (Usize i = len__Vec(*self->body); i--;)
        FREE(SymbolTableAll, get__Vec(*self->body, i));

    FREE(Vec, self->body);
    free(self);
}

struct IfCondSymbol
__new__IfCondSymbol(struct IfBranchSymbol *if_,
                    struct Vec *elif,
                    struct IfBranchSymbol *else_)
{
    struct IfCondSymbol self = { if_, elif, else_ };

    return self;
}

void
__free__IfCondSymbol(struct IfCondSymbol self)
{
    FREE(IfBranchSymbol, self.if_);

    for (Usize i = len__Vec(*self.elif); i--;)
        FREE(IfBranchSymbol, get__Vec(*self.elif, i));

    FREE(Vec, self.elif);
    FREE(IfBranchSymbol, self.else_);
}

struct TrySymbol
__new__TrySymbol(struct ExprSymbol *try_expr,
                 struct Vec *try_body,
                 struct ExprSymbol *catch_expr,
                 struct Vec *catch_body)
{
    struct TrySymbol self = { .try_expr = try_expr,
                              .try_body = try_body,
                              .catch_expr = catch_expr,
                              .catch_body = catch_body };

    return self;
}

void
__free__TrySymbol(struct TrySymbol self)
{
    FREE(ExprSymbolAll, self.try_expr);

    for (Usize i = len__Vec(*self.try_body); i--;)
        FREE(SymbolTableAll, get__Vec(*self.try_body, i));

    FREE(Vec, self.try_body);

    if (self.catch_expr != NULL)
        FREE(ExprSymbolAll, self.catch_expr);

    if (self.catch_body != NULL) {
        for (Usize i = len__Vec(*self.catch_body); i--;)
            FREE(SymbolTableAll, get__Vec(*self.catch_body, i));

        FREE(Vec, self.catch_body);
    }
}

void
__free__WhileSymbol(struct WhileSymbol self)
{
    FREE(ExprSymbolAll, self.cond);

    for (Usize i = len__Vec(*self.body); i--;)
        FREE(SymbolTableAll, get__Vec(*self.body, i));

    FREE(Vec, self.body);
}

void
__free__StmtSymbolAll(struct StmtSymbol self)
{
    switch (self.kind) {
        case StmtKindIf:
            FREE(StmtSymbolIf, self);
            break;
        case StmtKindReturn:
            FREE(StmtSymbolReturn, self);
            break;
        case StmtKindAwait:
            FREE(StmtSymbolAwait, self);
            break;
        case StmtKindTry:
            FREE(StmtSymbolTry, self);
            break;
        case StmtKindMatch:
            FREE(StmtSymbolMatch, self);
            break;
        case StmtKindFor:
            UNREACHABLE("For loop is not used in symbol table");
        case StmtKindWhile:
            FREE(StmtSymbolWhile, self);
            break;
        default:
            break;
    }
}

struct FunParamSymbol *
__new__FunParamSymbol(struct FunParam *param)
{
    struct FunParamSymbol *self = malloc(sizeof(struct FunParamSymbol));
    self->kind = param->kind;
    self->param_data_type = NULL;
    self->loc = param->loc;
    self->name = param->name;
    self->default_ = NULL;
    return self;
}

void
__free__FunParamSymbol(struct FunParamSymbol *self)
{
    FREE(DataTypeSymbolAll, self->param_data_type->items[0]);
    FREE(Tuple, self->param_data_type);

    if (self->kind == FunParamKindDefault)
        FREE(ExprSymbolAll, self->default_);

    free(self);
}

struct FunSymbol *
__new__FunSymbol(struct Decl *fun_decl)
{
    struct FunSymbol *self = malloc(sizeof(struct FunSymbol));
    self->name = &*fun_decl->value.fun->name;
    self->tagged_type = NULL;
    self->generic_params = &*fun_decl->value.fun->generic_params;
    self->params = NULL;
    self->visibility = VISIBILITY(fun_decl->value.fun);
    self->is_async = fun_decl->value.fun->is_async;
    self->return_type = NULL;
    self->body = NULL;
    self->scope = NULL;
    self->fun_decl = &*fun_decl;
    return self;
}

void
__free__FunSymbol(struct FunSymbol *self)
{
    if (self->tagged_type != NULL) {
        for (Usize i = len__Vec(*self->tagged_type); i--;) {
            FREE(DataTypeSymbolAll,
                 ((struct Tuple *)get__Vec(*self->tagged_type, i))->items[0]);
            FREE(Tuple, get__Vec(*self->tagged_type, i));
        }

        FREE(Vec, self->tagged_type);
    }

    if (self->params != NULL) {
        for (Usize i = len__Vec(*self->params); i--;)
            FREE(FunParamSymbol, get__Vec(*self->params, i));

        FREE(Vec, self->params);
    }

    if (self->return_type != NULL)
        FREE(DataTypeSymbolAll, self->return_type);

    if (self->body != NULL) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(SymbolTableAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    FREE(Scope, self->scope);
    free(self);
}

struct ConstantSymbol *
__new__ConstantSymbol(struct Decl *constant_decl)
{
    struct ConstantSymbol *self = malloc(sizeof(struct ConstantSymbol));
    self->name = constant_decl->value.constant->name;
    self->data_type = constant_decl->value.constant->data_type;
    self->expr_symbol = NULL;
    self->scope = NULL;
    self->constant_decl = constant_decl;
    self->visibility = VISIBILITY(constant_decl->value.constant);
    return self;
}

void
__free__ConstantSymbol(struct ConstantSymbol *self)
{
    FREE(ExprSymbolAll, self->expr_symbol);
    FREE(Scope, self->scope);
    free(self);
}

struct ModuleSymbol *
__new__ModuleSymbol(struct Decl *module_decl)
{
    struct ModuleSymbol *self = malloc(sizeof(struct ModuleSymbol));
    self->name = module_decl->value.module->name;
    self->body = NEW(Vec, sizeof(struct SymbolTable));
    self->scope = NULL;
    self->module_decl = module_decl;
    self->visibility = VISIBILITY(module_decl->value.module);
    return self;
}

void
__free__ModuleSymbol(struct ModuleSymbol *self)
{
    for (Usize i = len__Vec(*self->body); i--;)
        FREE(SymbolTableAll, get__Vec(*self->body, i));

    FREE(Vec, self->body);
    FREE(Scope, self->scope);
    free(self);
}

struct AliasSymbol *
__new__AliasSymbol(struct Decl *alias_decl)
{
    struct AliasSymbol *self = malloc(sizeof(struct AliasSymbol));
    self->name = alias_decl->value.alias->name;
    self->generic_params = alias_decl->value.alias->generic_params;
    self->data_type = NULL;
    self->scope = NULL;
    self->alias_decl = alias_decl;
    self->visibility = VISIBILITY(alias_decl->value.alias);
    return self;
}

void
__free__AliasSymbol(struct AliasSymbol *self)
{
    FREE(DataTypeSymbolAll, self->data_type);
    FREE(Scope, self->scope);
    free(self);
}

struct FieldRecordSymbol *
__new__FieldRecordSymbol(struct FieldRecord *field_record)
{
    struct FieldRecordSymbol *self = malloc(sizeof(struct FieldRecordSymbol));
    self->name = &*field_record->name;
    self->data_type = NULL;
    self->value = NULL;
    self->visibility = VISIBILITY(field_record);
    self->loc = field_record->loc;
    return self;
}

void
__free__FieldRecordSymbol(struct FieldRecordSymbol *self)
{
    FREE(DataTypeSymbolAll, self->data_type);
    FREE(ExprSymbolAll, self->value);
    free(self);
}

struct RecordSymbol *
__new__RecordSymbol(struct Decl *record_decl)
{
    struct RecordSymbol *self = malloc(sizeof(struct RecordSymbol));
    self->name = record_decl->value.record->name;
    self->generic_params = record_decl->value.record->generic_params;
    self->fields = NULL;
    self->scope = NULL;
    self->visibility = VISIBILITY(record_decl->value.record);
    self->record_decl = record_decl;
    return self;
}

void
__free__RecordSymbol(struct RecordSymbol *self)
{
    if (self->fields != NULL) {
        for (Usize i = len__Vec(*self->fields); i--;)
            FREE(FieldRecordSymbol, get__Vec(*self->fields, i));

        FREE(Vec, self->fields);
    }

    FREE(Scope, self->scope);

    free(self);
}

struct RecordObjSymbol *
__new__RecordObjSymbol(struct Decl *record_decl)
{
    struct RecordObjSymbol *self = malloc(sizeof(struct RecordObjSymbol));
    self->name = record_decl->value.record->name;
    self->generic_params = record_decl->value.record->generic_params;
    self->fields = NULL;
    self->attached = NULL;
    self->scope = NULL;
    self->visibility = VISIBILITY(record_decl->value.record);
    self->record_decl = record_decl;
    return self;
}

void
__free__RecordObjSymbol(struct RecordObjSymbol *self)
{
    if (self->fields != NULL) {
        for (Usize i = len__Vec(*self->fields); i--;)
            FREE(FieldRecordSymbol, get__Vec(*self->fields, i));

        FREE(Vec, self->fields);
    }

    if (self->attached != NULL) {
        for (Usize i = len__Vec(*self->attached); i--;)
            FREE(SymbolTableAll, get__Vec(*self->attached, i));

        FREE(Vec, self->attached);
    }

    FREE(Scope, self->scope);
    free(self);
}

struct VariantEnumSymbol *
__new__VariantEnumSymbol(struct VariantEnum *variant_enum)
{
    struct VariantEnumSymbol *self = malloc(sizeof(struct VariantEnumSymbol));
    self->name = variant_enum->name;
    self->data_type = NULL;
    self->loc = variant_enum->loc;
    return self;
}

struct EnumSymbol *
__new__EnumSymbol(struct Decl *enum_decl)
{
    struct EnumSymbol *self = malloc(sizeof(struct EnumSymbol));
    self->name = enum_decl->value.enum_->name;
    self->generic_params = enum_decl->value.enum_->generic_params;
    self->variants = NULL;
    self->type_value = NULL;
    self->scope = NULL;
    self->enum_decl = enum_decl;
    self->visibility = VISIBILITY(enum_decl->value.enum_);
    self->is_error = enum_decl->value.enum_->is_error;
    return self;
}

void
__free__EnumSymbol(struct EnumSymbol *self)
{
    if (self->variants != NULL) {
        for (Usize i = len__Vec(*self->variants); i--;)
            FREE(VariantEnumSymbol, get__Vec(*self->variants, i));

        FREE(Vec, self->variants);
    }

    if (self->type_value != NULL)
        FREE(DataTypeSymbolAll, self->type_value);

    FREE(Scope, self->scope);
    free(self);
}

struct EnumObjSymbol *
__new__EnumObjSymbol(struct Decl *enum_decl)
{
    struct EnumObjSymbol *self = malloc(sizeof(struct EnumObjSymbol));
    self->name = &*enum_decl->value.enum_->name;
    self->generic_params = enum_decl->value.enum_->generic_params;
    self->variants = NULL;
    self->attached = NULL;
    self->type_value = NULL;
    self->scope = NULL;
    self->enum_decl = enum_decl;
    self->visibility = VISIBILITY(enum_decl->value.enum_);
    self->is_error = enum_decl->value.enum_->is_error;
    return self;
}

void
__free__EnumObjSymbol(struct EnumObjSymbol *self)
{
    if (self->variants != NULL) {
        for (Usize i = len__Vec(*self->variants); i--;)
            FREE(VariantEnumSymbol, get__Vec(*self->variants, i));

        FREE(Vec, self->variants);
    }

    if (self->attached != NULL) {
        for (Usize i = len__Vec(*self->attached); i--;)
            FREE(SymbolTableAll, get__Vec(*self->attached, i));

        FREE(Vec, self->attached);
    }

    FREE(DataTypeSymbol, self->type_value);
    FREE(Scope, self->scope);
    free(self);
}

struct ErrorSymbol *
__new__ErrorSymbol(struct Decl *error_decl)
{
    struct ErrorSymbol *self = malloc(sizeof(struct ErrorSymbol));
    self->name = error_decl->value.error->name;
    self->generic_params = error_decl->value.error->generic_params;
    self->data_type = NULL;
    self->scope = NULL;
    self->error_decl = error_decl;
    self->visibility = VISIBILITY(error_decl->value.error);
    return self;
}

struct MethodSymbol *
__new__MethodSymbol(struct ClassBodyItem *method_decl)
{
    struct MethodSymbol *self = malloc(sizeof(struct MethodSymbol));
    self->name = method_decl->value.method->name;
    self->generic_params = method_decl->value.method->generic_params;
    self->params = NULL;
    self->return_type = NULL;
    self->body = NULL;
    self->scope = NULL;
    self->method_decl = method_decl;
    self->visibility = VISIBILITY(method_decl->value.method);
    self->has_first_self_param =
      method_decl->value.method->has_first_self_param;
    self->is_async = method_decl->value.method->is_async;
    return self;
}

void
__free__MethodSymbol(struct MethodSymbol *self)
{
    if (self->params != NULL) {
        for (Usize i = len__Vec(*self->params); i--;)
            FREE(FunParamSymbol, get__Vec(*self->params, i));

        FREE(Vec, self->params);
    }

    if (self->return_type != NULL)
        FREE(DataTypeSymbolAll, self->return_type);

    if (self->body != NULL) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(SymbolTableAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    FREE(Scope, self->scope);
    free(self);
}

struct PropertySymbol *
__new__PropertySymbol(struct ClassBodyItem *property_decl)
{
    struct PropertySymbol *self = malloc(sizeof(struct PropertySymbol));
    self->name = property_decl->value.property->name;
    self->data_type = NULL;
    self->scope = NULL;
    self->property_decl = property_decl;
    self->visibility = VISIBILITY(property_decl->value.property);
    return self;
}

void
__free__PropertySymbol(struct PropertySymbol *self)
{
    if (self->data_type != NULL)
        FREE(DataTypeSymbolAll, self->data_type);

    FREE(Scope, self->scope);
    free(self);
}

struct ClassSymbol *
__new__ClassSymbol(struct Decl *class_decl)
{
    struct ClassSymbol *self = malloc(sizeof(struct ClassSymbol));
    self->name = class_decl->value.class->name;
    self->generic_params = class_decl->value.class->generic_params;
    self->inheritance = NULL;
    self->impl = NULL;
    self->body = NULL;
    self->scope = NULL;
    self->class_decl = class_decl;
    self->visibility = VISIBILITY(class_decl->value.class);
    return self;
}

void
__free__ClassSymbol(struct ClassSymbol *self)
{
    if (self->inheritance != NULL) {
        for (Usize i = len__Vec(*self->inheritance); i--;)
            FREE(DataTypeSymbolAll, get__Vec(*self->inheritance, i));

        FREE(Vec, self->inheritance);
    }

    if (self->impl != NULL) {
        for (Usize i = len__Vec(*self->impl); i--;)
            FREE(DataTypeSymbolAll, get__Vec(*self->impl, i));

        FREE(Vec, self->impl);
    }

    if (self->body != NULL) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(SymbolTableAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    FREE(Scope, self->scope);
    free(self);
}

struct PrototypeSymbol *
__new__PrototypeSymbol(struct TraitBodyItem *prototype_decl)
{
    struct PrototypeSymbol *self = malloc(sizeof(struct PrototypeSymbol));
    self->name = prototype_decl->value.prototype->name;
    self->params_type = NULL;
    self->return_type = NULL;
    self->scope = NULL;
    self->prototype_decl = prototype_decl;
    self->is_async = prototype_decl->value.prototype->is_async;
    self->has_first_self_param =
      prototype_decl->value.prototype->has_first_self_param;
    return self;
}

void
__free__PrototypeSymbol(struct PrototypeSymbol *self)
{
    for (Usize i = len__Vec(*self->params_type); i--;)
        FREE(DataTypeSymbolAll, get__Vec(*self->params_type, i));

    FREE(Vec, self->params_type);
    FREE(DataTypeSymbolAll, self->return_type);
    FREE(Scope, self->scope);
    free(self);
}

struct TraitSymbol *
__new__TraitSymbol(struct Decl *trait_decl)
{
    struct TraitSymbol *self = malloc(sizeof(struct TraitSymbol));
    self->name = trait_decl->value.trait->name;
    self->generic_params = trait_decl->value.trait->generic_params;
    self->inh = NULL;
    self->body = NULL;
    self->scope = NULL;
    self->trait_decl = trait_decl;
    self->visibility = VISIBILITY(trait_decl->value.trait);
    return self;
}

void
__free__TraitSymbol(struct TraitSymbol *self)
{
    if (self->inh != NULL) {
        for (Usize i = len__Vec(*self->inh); i--;)
            FREE(DataTypeSymbolAll, get__Vec(*self->inh, i));

        FREE(Vec, self->inh);
    }

    if (self->body != NULL) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(SymbolTableAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    FREE(Scope, self->scope);
    free(self);
}

struct UnaryOpSymbol
__new__UnaryOpSymbol(struct Expr unary_op,
                     struct DataType *data_type,
                     struct ExprSymbol *right)
{
    struct UnaryOpSymbol self = { .kind = unary_op.value.unary_op.kind,
                                  .data_type = data_type,
                                  .right = right };

    return self;
}

const Str
to_Str__UnaryOpSymbol(unsigned int kind)
{
    if (kind > UnaryOpKindReference)
        return 0;

    return "-\0not\0&\0\0" + 3 * kind;
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
    struct BinaryOpSymbol self = { .kind = binary_op.value.binary_op.kind,
                                   .data_type = data_type,
                                   .left = left,
                                   .right = right };

    return self;
}

const Str
to_Str__BinaryOpSymbol(unsigned int kind)
{
    if (kind > BinaryOpKindExponent)
        return 0;

    return "+\0-\0*\0/"
           "\0%\0..\0<\0>\0<=\0>=\0==\0not=\0and\0or\0xor\0=\0+=\0-=\0*=\0/"
           "=\0%=\0^=\0<<=\0>>=\0|=\0xor=\0&=\0~=\0++=\0--=\0|>\0++\0--\0$\0^"
           "\0<<\0>>\0|\0&\0not\0**\0\0" +
           3 * kind;
}

void
__free__BinaryOpSymbol(struct BinaryOpSymbol self)
{
    FREE(DataTypeAll, self.data_type);
}

struct FunCallSymbol
__new__FunCallSymbol(bool is_builtin,
                     struct Scope *id,
                     struct DataType *data_type,
                     struct Vec *params)
{
    struct FunCallSymbol self = { .is_builtin = is_builtin,
                                  .id = id,
                                  .data_type = data_type,
                                  .params = params };

    return self;
}

void
__free__FunCallSymbol(struct FunCallSymbol self)
{
    FREE(DataTypeAll, self.data_type);
    TODO("FREE all");
    FREE(Vec, self.params);
}

struct SymbolTable *
__new__SymbolTableFun(struct FunSymbol *fun)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindFun;
    self->value.fun = fun;
    return self;
}

struct SymbolTable *
__new__SymbolTableConstant(struct ConstantSymbol *constant)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindConstant;
    self->value.constant = constant;
    return self;
}

struct SymbolTable *
__new__SymbolTableModule(struct ModuleSymbol *module)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindModule;
    self->value.module = module;
    return self;
}

struct SymbolTable *
__new__SymbolTableAlias(struct AliasSymbol *alias)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindAlias;
    self->value.alias = alias;
    return self;
}

struct SymbolTable *
__new__SymbolTableRecord(struct RecordSymbol *record)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindRecord;
    self->value.record = record;
    return self;
}

struct SymbolTable *
__new__SymbolTableRecordObj(struct RecordObjSymbol *record_obj)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindRecordObj;
    self->value.record_obj = record_obj;
    return self;
}

struct SymbolTable *
__new__SymbolTableEnum(struct EnumSymbol *enum_)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindEnum;
    self->value.enum_ = enum_;
    return self;
}

struct SymbolTable *
__new__SymbolTableEnumObj(struct EnumObjSymbol *enum_obj)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindEnumObj;
    self->value.enum_obj = enum_obj;
    return self;
}

struct SymbolTable *
__new__SymbolTableError(struct ErrorSymbol *error)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindError;
    self->value.error = error;
    return self;
}

struct SymbolTable *
__new__SymbolTableClass(struct ClassSymbol *class)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindClass;
    self->value.class = class;
    return self;
}

struct SymbolTable *
__new__SymbolTableTrait(struct TraitSymbol *trait)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindTrait;
    self->value.trait = trait;
    return self;
}

struct SymbolTable *
__new__SymbolTableExpr(struct ExprSymbol *expr)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindExpr;
    self->value.expr = expr;
    return self;
}

struct SymbolTable *
__new__SymbolTableStmt(struct StmtSymbol stmt)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindStmt;
    self->value.stmt = stmt;
    return self;
}

void
__free__SymbolTableAll(struct SymbolTable *self)
{
    switch (self->kind) {
        case SymbolTableKindFun:
            FREE(SymbolTableFun, self);
            break;
        case SymbolTableKindConstant:
            FREE(SymbolTableConstant, self);
            break;
        case SymbolTableKindModule:
            FREE(SymbolTableModule, self);
            break;
        case SymbolTableKindAlias:
            FREE(SymbolTableAlias, self);
            break;
        case SymbolTableKindRecord:
            FREE(SymbolTableRecord, self);
            break;
        case SymbolTableKindRecordObj:
            FREE(SymbolTableRecordObj, self);
            break;
        case SymbolTableKindEnum:
            FREE(SymbolTableEnum, self);
            break;
        case SymbolTableKindEnumObj:
            FREE(SymbolTableEnumObj, self);
            break;
        case SymbolTableKindError:
            FREE(SymbolTableError, self);
            break;
        case SymbolTableKindClass:
            FREE(SymbolTableClass, self);
            break;
        case SymbolTableKindTrait:
            FREE(SymbolTableTrait, self);
            break;
        case SymbolTableKindExpr:
            FREE(SymbolTableExpr, self);
            break;
        case SymbolTableKindStmt:
            FREE(SymbolTableStmt, self);
            break;
        default:
            UNREACHABLE("unknown symbol table kind");
    }
}
