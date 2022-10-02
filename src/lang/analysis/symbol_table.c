#include <base/macros.h>
#include <lang/analysis/symbol_table.h>

struct Scope
__new__Scope(const Str filename,
             struct String *name,
             struct Vec *id,
             enum ScopeItemKind item_kind,
             enum ScopeKind kind)
{
    struct Scope self = { .filename = filename,
                          .name = name,
                          .id = id,
                          .item_kind = item_kind,
                          .kind = kind };

    return self;
}

void
__free__Scope(struct Scope scope)
{
    FREE(Vec, scope.id);
}

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
__new__DataTypeSymbolCustom(struct String *name,
                            struct Vec *generic_params,
                            struct Scope *scope)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindCustom;
    self->scope = scope;
    self->value.custom = NEW(Tuple, 2, name, generic_params);
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
__free__DataTypeSymbol(struct DataTypeSymbol *self)
{
    free(self);
}

void
__free__DataTypeSymbolPtr(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.ptr);
    free(self);
}

void
__free__DataTypeSymbolRef(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.ref);
    free(self);
}

void
__free__DataTypeSymbolOptional(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.optional);
    free(self);
}

void
__free__DataTypeSymbolException(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.exception);
    free(self);
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
    TODO("generic params");
    FREE(Vec, self->value.custom->items[1]);
    FREE(Tuple, self->value.custom);
    free(self);
}

void
__free__DataTypeSymbolTuple(struct DataTypeSymbol *self)
{
    FREE(Vec, self->value.tuple);
    free(self);
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

struct LiteralSymbol
__new__LiteralSymbolBool(bool bool_)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindBool,
                                  .value.bool_ = bool_ };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolChar(char char_)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindChar,
                                  .value.char_ = char_ };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolBitChar(UInt8 bit_char)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindBitChar,
                                  .value.bit_char = bit_char };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolInt8(Int8 int8)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt8,
                                  .value.int8 = int8 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolInt16(Int16 int16)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt16,
                                  .value.int16 = int16 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolInt32(Int32 int32)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt32,
                                  .value.int32 = int32 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolInt64(Int64 int64)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt64,
                                  .value.int64 = int64 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolInt128(Int128 int128)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt128,
                                  .value.int128 = int128 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolUint8(UInt8 uint8)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint8,
                                  .value.uint8 = uint8 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolUint16(UInt16 uint16)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint16,
                                  .value.uint16 = uint16 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolUint32(UInt32 uint32)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint32,
                                  .value.uint32 = uint32 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolUint64(UInt64 uint64)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint64,
                                  .value.uint64 = uint64 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolUint128(Int128 uint128)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint128,
                                  .value.uint128 = uint128 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolFloat32(Float32 float32)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindFloat32,
                                  .value.float32 = float32 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolFloat64(Float64 float64)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindFloat64,
                                  .value.float64 = float64 };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolStr(Str str)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindStr,
                                  .value.str = str };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolBitStr(UInt8 **bit_str)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindBitStr,
                                  .value.bit_str = bit_str };

    return self;
}

struct LiteralSymbol
__new__LiteralSymbolUnit()
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUnit };

    return self;
}

struct FieldCallSymbol *
__new__FieldCallSymbol(struct String *name, struct ExprSymbol *value)
{
    struct FieldCallSymbol *self = malloc(sizeof(struct FieldCallSymbol));
    self->name = name;
    self->value = value;
    return self;
}

void
__free__FieldCallSymbol(struct FieldCallSymbol *self)
{
    FREE(ExprSymbolAll, self->value);
    free(self);
}

struct RecordCallSymbol
__new__RecordCallSymbol(struct Scope id, struct Vec *fields)
{
    struct RecordCallSymbol self = { .id = id, .fields = fields };

    return self;
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

struct ArrayAccessSymbol
__new__ArrayAccessSymbol(struct Scope id, struct Vec *access)
{
    struct ArrayAccessSymbol self = { .id = id, .access = access };

    return self;
}

void
__free__ArrayAccessSymbol(struct ArrayAccessSymbol self)
{
    for (Usize i = len__Vec(*self.access); i--;)
        FREE(ExprSymbolAll, get__Vec(*self.access, i));

    FREE(Vec, self.access);
}

struct TupleAccessSymbol
__new__TupleAccessSymbol(struct Scope id, struct Vec *access)
{
    struct TupleAccessSymbol self = { .id = id, .access = access };

    return self;
}

void
__free__TupleAccessSymbol(struct TupleAccessSymbol self)
{
    for (Usize i = len__Vec(*self.access); i--;)
        FREE(ExprSymbolAll, get__Vec(*self.access, i));

    FREE(Vec, self.access);
}

struct VariantSymbol
__new__VariantSymbol(struct Scope id, struct ExprSymbol *value)
{
    struct VariantSymbol self = { .id = id, .value = value };

    return self;
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
__new__ExprSymbolIdentifier(struct Expr expr, struct Scope identifier)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.identifier = identifier;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolIdentifierAccess(struct Expr expr,
                                  struct Scope identifier_access)
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
__new__ExprSymbolQuestionMark(struct Expr expr, struct Scope question_mark)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.question_mark = question_mark;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolDereference(struct Expr expr, struct Scope dereference)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.dereference = dereference;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolRef(struct Expr expr, struct Scope ref)
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
__new__ExprSymbolVariable(struct Expr expr, struct VariableSymbol variable)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->value.variable = variable;
    return self;
}

void
__free__ExprSymbol(struct ExprSymbol *self)
{
    free(self);
}

void
__free__ExprSymbolUnaryOp(struct ExprSymbol *self)
{
    FREE(UnaryOpSymbol, self->value.unary_op);
    free(self);
}

void
__free__ExprSymbolBinaryOp(struct ExprSymbol *self)
{
    FREE(BinaryOpSymbol, self->value.binary_op);
    free(self);
}

void
__free__ExprSymbolFunCall(struct ExprSymbol *self)
{
    FREE(FunCallSymbol, self->value.fun_call);
    free(self);
}

void
__free__ExprSymbolRecordCall(struct ExprSymbol *self)
{
    FREE(RecordCallSymbol, self->value.record_call);
    free(self);
}

void
__free__ExprSymbolIdentifier(struct ExprSymbol *self)
{
    FREE(Scope, self->value.identifier);
    free(self);
}

void
__free__ExprSymbolIdentifierAccess(struct ExprSymbol *self)
{
    FREE(Scope, self->value.identifier_access);
    free(self);
}

void
__free__ExprSymbolArrayAccess(struct ExprSymbol *self)
{
    FREE(ArrayAccessSymbol, self->value.array_access);
    free(self);
}

void
__free__ExprSymbolTupleAccess(struct ExprSymbol *self)
{
    FREE(TupleAccessSymbol, self->value.tuple_access);
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
__free__ExprSymbolVariant(struct ExprSymbol *self)
{
    FREE(VariantSymbol, self->value.variant);
    free(self);
}

void
__free__ExprSymbolTry(struct ExprSymbol *self)
{
    FREE(ExprSymbolAll, self->value.try);
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
__free__ExprSymbolQuestionMark(struct ExprSymbol *self)
{
    FREE(Scope, self->value.question_mark);
    free(self);
}

void
__free__ExprSymbolDereference(struct ExprSymbol *self)
{
    FREE(Scope, self->value.dereference);
    free(self);
}

void
__free__ExprSymbolRef(struct ExprSymbol *self)
{
    FREE(Scope, self->value.ref);
    free(self);
}

void
__free__ExprSymbolLiteral(struct ExprSymbol *self)
{
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

struct VariableSymbol
__new__VariableSymbol(struct VariableDecl decl, struct Location decl_loc)
{
    struct VariableSymbol self = { .name = decl.name,
                                   .data_type = NULL,
                                   .expr = NULL,
                                   .scope = NULL,
                                   .loc = decl_loc,
                                   .is_mut = decl.is_mut };

    return self;
}

void
__free__VariableSymbol(struct VariableSymbol self)
{
    FREE(DataTypeSymbol, self.data_type);
    FREE(ExprSymbolAll, self.expr);
}

struct FunSymbol *
__new__FunSymbol(struct Decl *fun_decl)
{
    struct FunSymbol *self = malloc(sizeof(struct FunSymbol));
    self->name = &*fun_decl->value.fun->name;
    self->taged_type = NEW(Vec, sizeof(struct SymbolTable));
    self->generic_params = &*fun_decl->value.fun->generic_params;
    self->params = &*fun_decl->value.fun->params;
    self->visibility = VISIBILITY(fun_decl->value.fun);
    self->is_async = fun_decl->value.fun->is_async;
    self->return_type = fun_decl->value.fun->return_type;
    self->body = NEW(Vec, sizeof(struct SymbolTable));
    self->scope = NULL;
    self->fun_decl = &*fun_decl;

    return self;
}

void
__free__FunSymbol(struct FunSymbol *self)
{
    FREE(Vec, self->taged_type);
    FREE(Vec, self->body);
    FREE(Scope, *self->scope);
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
    FREE(Scope, *self->scope);
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
    FREE(Scope, *self->scope);
    free(self);
}

struct AliasSymbol *
__new__AliasSymbol(struct Decl *alias_decl)
{
    struct AliasSymbol *self = malloc(sizeof(struct AliasSymbol));
    self->name = alias_decl->value.alias->name;
    self->generic_params = alias_decl->value.alias->generic_params;
    self->data_type = alias_decl->value.alias->data_type;
    self->scope = NULL;
    self->alias_decl = alias_decl;
    self->visibility = VISIBILITY(alias_decl->value.alias);
    return self;
}

void
__free__AliasSymbol(struct AliasSymbol *self)
{
    FREE(Scope, *self->scope);
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
