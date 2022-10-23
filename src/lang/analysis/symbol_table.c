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

#include "lang/parser/ast.h"
#include <base/macros.h>
#include <lang/analysis/symbol_table.h>
#include <string.h>

struct Scope *
__new__Scope(const Str filename,
             struct String *name,
             enum ScopeItemKind item_kind,
             enum Visibility visibility,
             struct Vec *scopes,
             struct Vec *public_names,
             struct Vec *names,
             struct Location *loc)
{
    struct Scope *self = malloc(sizeof(struct Scope));
    self->filename = filename;
    self->name = name;
    self->item_kind = item_kind;
    self->visibility = visibility;
    self->scopes = scopes;
    self->public_names = public_names;
    self->names = names;
    return self;
}

struct Scope *
copy__Scope(struct Scope *self)
{
    struct Scope *copy = malloc(sizeof(struct Scope));

    memcpy(copy, self, sizeof(struct Scope));

    return copy;
}

void
__free__Scope(struct Scope *self)
{
    if (self->names) {
        for (Usize i = len__Vec(*self->names); i--;)
            FREE(Tuple, get__Vec(*self->names, i));

        FREE(Vec, self->names);
    }

    if (self->scopes)
        FREE(Vec, self->scopes);

    if (self->public_names)
        FREE(Vec, self->public_names);

    free(self);
}

void
__free__LocalDataType(struct LocalDataType *self)
{
    if (self->restricted) {
        FREE(DataTypeSymbolAll, self->restricted->items[0]);
        FREE(Tuple, self->restricted);
    }

    free(self);
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
__new__DataTypeSymbolMut(struct DataTypeSymbol *mut)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindMut;
    self->scope = NULL;
    self->value.mut = mut;
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
__new__DataTypeSymbolArray(struct DataTypeSymbol *data_type, Usize *size)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindArray;
    self->scope = NULL;
    self->value.array = NEW(Tuple, 2, data_type, size);
    return self;
}

struct DataTypeSymbol *
__new__DataTypeSymbolCustom(struct Vec *generic_params,
                            struct String *name,
                            struct Scope *scope)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindCustom;
    self->custom_name = name;
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

struct DataTypeSymbol *
__new__DataTypeSymbolCompilerDefined(
  struct CompilerDefinedDataType compiler_defined)
{
    struct DataTypeSymbol *self = malloc(sizeof(struct DataTypeSymbol));
    self->kind = DataTypeKindCompilerDefined;
    self->scope = NULL;
    self->value.compiler_defined = compiler_defined;
    return self;
}

struct DataTypeSymbol *
copy__DataTypeSymbol(struct DataTypeSymbol *self)
{
    struct DataTypeSymbol *copy = malloc(sizeof(struct DataTypeSymbol));

    memcpy(copy, self, sizeof(struct DataTypeSymbol));

    return copy;
}

bool
eq__DataTypeSymbol(struct DataTypeSymbol *self, struct DataTypeSymbol *y)
{
    switch (self->kind) {
        case DataTypeKindSelf:
            return y->kind == DataTypeKindSelf ? true : false;
        case DataTypeKindPtr:
            return y->kind == DataTypeKindPtr
                     ? eq__DataTypeSymbol(self->value.ptr, y->value.ptr)
                     : false;
        case DataTypeKindRef:
            return y->kind == DataTypeKindRef
                     ? eq__DataTypeSymbol(self->value.ref, y->value.ref)
                     : false;
        case DataTypeKindStr:
            return y->kind == DataTypeKindStr ? true : false;
        case DataTypeKindBitStr:
            return y->kind == DataTypeKindBitStr ? true : false;
        case DataTypeKindChar:
            return y->kind == DataTypeKindChar ? true : false;
        case DataTypeKindBitChar:
            return y->kind == DataTypeKindBitChar ? true : false;
        case DataTypeKindI8:
            return y->kind == DataTypeKindI8 ? true : false;
        case DataTypeKindI16:
            return y->kind == DataTypeKindI16 ? true : false;
        case DataTypeKindI32:
            return y->kind == DataTypeKindI32 ? true : false;
        case DataTypeKindI64:
            return y->kind == DataTypeKindI64 ? true : false;
        case DataTypeKindI128:
            return y->kind == DataTypeKindI128 ? true : false;
        case DataTypeKindU8:
            return y->kind == DataTypeKindU8 ? true : false;
        case DataTypeKindU16:
            return y->kind == DataTypeKindU16 ? true : false;
        case DataTypeKindU32:
            return y->kind == DataTypeKindU32 ? true : false;
        case DataTypeKindU64:
            return y->kind == DataTypeKindU64 ? true : false;
        case DataTypeKindU128:
            return y->kind == DataTypeKindU128 ? true : false;
        case DataTypeKindF32:
            return y->kind == DataTypeKindF32 ? true : false;
        case DataTypeKindF64:
            return y->kind == DataTypeKindF64 ? true : false;
        case DataTypeKindBool:
            return y->kind == DataTypeKindBool ? true : false;
        case DataTypeKindIsize:
            return y->kind == DataTypeKindIsize ? true : false;
        case DataTypeKindUsize:
            return y->kind == DataTypeKindUsize ? true : false;
        case DataTypeKindNever:
            return y->kind == DataTypeKindNever ? true : false;
        case DataTypeKindAny:
            return y->kind == DataTypeKindAny ? true : false;
        case DataTypeKindOptional:
            return y->kind == DataTypeKindOptional
                     ? eq__DataTypeSymbol(self->value.optional,
                                          y->value.optional)
                     : false;
        case DataTypeKindUnit:
            return y->kind == DataTypeKindUnit ? true : false;
        case DataTypeKindException:
            return y->kind == DataTypeKindException
                     ? eq__DataTypeSymbol(self->value.exception,
                                          y->value.exception)
                     : false;
        case DataTypeKindMut:
            return y->kind == DataTypeKindMut
                     ? eq__DataTypeSymbol(self->value.mut, y->value.mut)
                     : false;
        case DataTypeKindLambda: {
            struct Vec *v_self = self->value.lambda->items[0];
            struct Vec *v_y = y->value.lambda->items[0];

            if (len__Vec(*v_self) == len__Vec(*v_y)) {
                for (Usize i = len__Vec(*v_self); i--;) {
                    if (!eq__DataTypeSymbol(get__Vec(*v_self, i),
                                            get__Vec(*v_y, i)))
                        return false;
                }

                return eq__DataTypeSymbol(self->value.lambda->items[1],
                                          y->value.lambda->items[1]);
            } else
                return false;
        }
        case DataTypeKindArray:
            return (Usize *)self->value.array->items[1] ==
                         (Usize *)y->value.array->items[1] &&
                       eq__DataTypeSymbol(self->value.array->items[0],
                                          y->value.array->items[0])
                     ? true
                     : false;
        case DataTypeKindTuple: {
            struct Vec *v_self = self->value.tuple->items[0];
            struct Vec *v_y = y->value.tuple->items[0];

            for (Usize i = len__Vec(*v_self); i--;) {
                if (!eq__DataTypeSymbol(get__Vec(*v_self, i),
                                        get__Vec(*v_y, i)))
                    return false;
            }

            return true;
        }
        case DataTypeKindCustom:
            if (y->kind == DataTypeKindCustom) {
                if (self->custom_name && y->custom_name)
                    return eq__String(self->custom_name, y->custom_name, false);
                else if (!self->custom_name && !y->custom_name)
                    return eq__String(self->scope->name, y->scope->name, false);
                else
                    return false;
            } else
                return false;
        case DataTypeKindCompilerDefined:
            return true;
    }
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
        case DataTypeKindMut:
            FREE(DataTypeSymbolMut, self);
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
        case DataTypeKindCompilerDefined:
            FREE(DataTypeSymbolCompilerDefined, self);
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
    if (self->value.array->items[0])
        FREE(DataTypeSymbolAll, self->value.array->items[0]);

    FREE(Tuple, self->value.array);
    free(self);
}

void
__free__DataTypeSymbolCustom(struct DataTypeSymbol *self)
{
    if (self->custom_name)
        FREE(String, self->custom_name);

    if (self->value.custom) {
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
    if (self.fields) {
        for (Usize i = len__Vec(*self.fields); i--;) {
            FREE(FieldCallSymbol,
                 ((struct Tuple *)get__Vec(*self.fields, i))->items[0]);
            FREE(Tuple, get__Vec(*self.fields, i));
        }

        FREE(Vec, self.fields);
    }
}

void
__free__VariantSymbol(struct VariantSymbol self)
{
    FREE(ExprSymbolAll, self.value);
}

struct ExprSymbol *
__new__ExprSymbol(struct Expr *expr, struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr->kind;
    self->loc = expr->loc;
    self->data_type = data_type;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolUnaryOp(struct Expr expr,
                         struct UnaryOpSymbol unary_op,
                         struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.unary_op = unary_op;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolBinaryOp(struct Expr expr,
                          struct BinaryOpSymbol binary_op,
                          struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.binary_op = binary_op;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolFunCall(struct Expr expr,
                         struct FunCallSymbol fun_call,
                         struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.fun_call = fun_call;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolRecordCall(struct Expr expr,
                            struct RecordCallSymbol record_call,
                            struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.record_call = record_call;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolIdentifier(struct Expr expr,
                            struct Scope *identifier,
                            struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.identifier = identifier;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolIdentifierAccess(struct Expr expr,
                                  struct Scope *identifier_access,
                                  struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.identifier_access = identifier_access;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolGlobalAccess(struct Expr expr,
                              struct Scope *global_access,
                              struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.global_access = global_access;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolArrayAccess(struct Expr expr,
                             struct Scope *array_access,
                             struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.array_access = array_access;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolTupleAccess(struct Expr expr,
                             struct Scope *tuple_access,
                             struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.tuple_access = tuple_access;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolLambda(struct Expr expr,
                        struct LambdaSymbol lambda,
                        struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.lambda = lambda;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolTuple(struct Expr expr,
                       struct Vec *tuple,
                       struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.tuple = tuple;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolArray(struct Expr expr,
                       struct Vec *array,
                       struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.array = array;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolVariant(struct Expr expr,
                         struct VariantSymbol variant,
                         struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.variant = variant;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolTry(struct Expr expr,
                     struct ExprSymbol *try,
                     struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.try = try;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolBlock(struct Expr expr,
                       struct Vec *block,
                       struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.block = block;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolQuestionMark(struct Expr expr,
                              struct Scope *question_mark,
                              struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.question_mark = question_mark;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolDereference(struct Expr expr,
                             struct Scope *dereference,
                             struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.dereference = dereference;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolRef(struct Expr expr,
                     struct Scope *ref,
                     struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.ref = ref;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolLiteral(struct Expr expr, struct LiteralSymbol literal)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = literal.data_type;
    self->value.literal = literal;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolVariable(struct Expr expr, struct VariableSymbol *variable)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = NULL;
    self->value.variable = variable;
    return self;
}

struct ExprSymbol *
__new__ExprSymbolGrouping(struct Expr expr,
                          struct Tuple *grouping,
                          struct DataTypeSymbol *data_type)
{
    struct ExprSymbol *self = malloc(sizeof(struct ExprSymbol));
    self->kind = expr.kind;
    self->loc = expr.loc;
    self->data_type = data_type;
    self->value.grouping = grouping;
    return self;
}

void
__free__ExprSymbolFunCall(struct ExprSymbol *self)
{
    FREE(FunCallSymbol, self->value.fun_call);
    FREE(DataTypeSymbolAll, self->data_type);
    free(self);
}

void
__free__ExprSymbolRecordCall(struct ExprSymbol *self)
{
    FREE(RecordCallSymbol, self->value.record_call);
    FREE(DataTypeSymbolAll, self->data_type);
    free(self);
}

void
__free__ExprSymbolLambda(struct ExprSymbol *self)
{
    TODO("Free lambda symbol");
    FREE(DataTypeSymbolAll, self->data_type);
    free(self);
}

void
__free__ExprSymbolTuple(struct ExprSymbol *self)
{
    for (Usize i = len__Vec(*(struct Vec *)self->value.tuple->items[0]); i--;)
        FREE(ExprSymbolAll, get__Vec(*self->value.tuple, i));

    FREE(Vec, self->value.tuple);
    FREE(DataTypeSymbolAll, self->value.tuple->items[1]);
    FREE(DataTypeSymbolAll, self->data_type);
    free(self);
}

void
__free__ExprSymbolArray(struct ExprSymbol *self)
{
    for (Usize i = len__Vec(*self->value.array); i--;)
        FREE(ExprSymbolAll, get__Vec(*self->value.array, i));

    FREE(Vec, self->value.array);
    FREE(DataTypeSymbolAll, self->data_type);
    free(self);
}

void
__free__ExprSymbolBlock(struct ExprSymbol *self)
{
    for (Usize i = len__Vec(*self->value.block); i--;)
        FREE(SymbolTableAll, get__Vec(*self->value.block, i));

    FREE(Vec, self->value.block);
    FREE(DataTypeSymbolAll, self->data_type);
    free(self);
}

void
__free__ExprSymbolVariable(struct ExprSymbol *self)
{
    FREE(VariableSymbol, self->value.variable);
    free(self);
}

void
__free__ExprSymbolGrouping(struct ExprSymbol *self)
{
    FREE(ExprSymbolAll, self->value.grouping->items[0]);
    FREE(DataTypeSymbolAll, self->value.grouping->items[1]);
    FREE(DataTypeSymbolAll, self->data_type);
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

    for (Usize i = len__Vec(*self.patterns); i--;) {
        FREE(ExprSymbolAll,
             CAST(get__Vec(*self.patterns, i), struct Tuple *)->items[0]);

        if (CAST(get__Vec(*self.patterns, i), struct Tuple *)->items[1])
            FREE(ExprSymbolAll,
                 CAST(get__Vec(*self.patterns, i), struct Tuple *)->items[1]);

        FREE(ExprSymbolAll,
             CAST(get__Vec(*self.patterns, i), struct Tuple *)->items[2]);
        FREE(Tuple, get__Vec(*self.patterns, i));
    }

    FREE(Vec, self.patterns);
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
__new__TrySymbol(struct Vec *try_body,
                 struct ExprSymbol *catch_expr,
                 struct Vec *catch_body)
{
    struct TrySymbol self = { .try_body = try_body,
                              .catch_expr = catch_expr,
                              .catch_body = catch_body };

    return self;
}

void
__free__TrySymbol(struct TrySymbol self)
{
    for (Usize i = len__Vec(*self.try_body); i--;)
        FREE(SymbolTableAll, get__Vec(*self.try_body, i));

    FREE(Vec, self.try_body);

    if (self.catch_expr)
        FREE(ExprSymbolAll, self.catch_expr);

    if (self.catch_body) {
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
    self->default_defined_data_type = param->param_data_type ? true : false;
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
    self->generic_params = NULL;
    self->params = NULL;
    self->visibility = VISIBILITY(fun_decl->value.fun);
    self->is_async = fun_decl->value.fun->is_async;
    self->return_type = NULL;
    self->body = NULL;
    self->scope = NULL;
    self->local_values = NULL;
    self->fun_decl = &*fun_decl;
    return self;
}

void
__free__FunSymbol(struct FunSymbol *self)
{
    if (self->tagged_type) {
        for (Usize i = len__Vec(*self->tagged_type); i--;) {
            FREE(DataTypeSymbolAll,
                 ((struct Tuple *)get__Vec(*self->tagged_type, i))->items[0]);
            FREE(Tuple, get__Vec(*self->tagged_type, i));
        }

        FREE(Vec, self->tagged_type);
    }

    if (self->params) {
        for (Usize i = len__Vec(*self->params); i--;)
            FREE(FunParamSymbol, get__Vec(*self->params, i));

        FREE(Vec, self->params);
    }

    if (self->return_type)
        FREE(DataTypeSymbolAll, self->return_type);

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(SymbolTableAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    FREE(Scope, self->scope);

    if (self->local_values) {
        for (Usize i = 0; i < len__Vec(*self->local_values); i++) {
            struct Vec *v2 = get__Vec(*self->local_values, i);

            for (Usize j = 0; j < len__Vec(*v2); j++) {
                struct Tuple *t = get__Vec(*v2, j);

                for (Usize k = 0;
                     k < len__Vec(*CAST(t->items[1], struct Vec *));
                     k++)
                    FREE(VariableAddr,
                         CAST(get__Vec(*CAST(t->items[1], struct Vec *), k),
                              struct VariableAddr *));

                FREE(Vec, t->items[1]);
                FREE(Tuple, t);
            }

            FREE(Vec, v2);
        }

        FREE(Vec, self->local_values);
    }

    free(self);
}

struct ConstantSymbol *
__new__ConstantSymbol(struct Decl *constant_decl)
{
    struct ConstantSymbol *self = malloc(sizeof(struct ConstantSymbol));
    self->name = constant_decl->value.constant->name;
    self->data_type = NULL;
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
    FREE(DataTypeSymbolAll, self->data_type);
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
    self->import_loc = NULL;
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
    self->generic_params = NULL;
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
    self->scope = NULL;
    self->loc = field_record->loc;
    return self;
}

void
__free__FieldRecordSymbol(struct FieldRecordSymbol *self)
{
    FREE(DataTypeSymbolAll, self->data_type);
    FREE(ExprSymbolAll, self->value);
    FREE(Scope, self->scope);
    free(self);
}

struct RecordSymbol *
__new__RecordSymbol(struct Decl *record_decl)
{
    struct RecordSymbol *self = malloc(sizeof(struct RecordSymbol));
    self->name = record_decl->value.record->name;
    self->generic_params = NULL;
    self->fields = NULL;
    self->scope = NULL;
    self->visibility = VISIBILITY(record_decl->value.record);
    self->record_decl = record_decl;
    return self;
}

void
__free__RecordSymbol(struct RecordSymbol *self)
{
    if (self->fields) {
        for (Usize i = len__Vec(*self->fields); i--;)
            FREE(SymbolTableAll, get__Vec(*self->fields, i));

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
    self->generic_params = NULL;
    self->fields = NULL;
    self->attached = NULL;
    self->scope = NULL;
    self->record_decl = record_decl;
    self->visibility = VISIBILITY(record_decl->value.record);
    return self;
}

void
__free__RecordObjSymbol(struct RecordObjSymbol *self)
{
    if (self->fields) {
        for (Usize i = len__Vec(*self->fields); i--;)
            FREE(SymbolTableAll, get__Vec(*self->fields, i));

        FREE(Vec, self->fields);
    }

    if (self->attached) {
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
    self->scope = NULL;
    self->loc = variant_enum->loc;
    return self;
}

struct EnumSymbol *
__new__EnumSymbol(struct Decl *enum_decl)
{
    struct EnumSymbol *self = malloc(sizeof(struct EnumSymbol));
    self->name = enum_decl->value.enum_->name;
    self->generic_params = NULL;
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
    if (self->variants) {
        for (Usize i = len__Vec(*self->variants); i--;)
            FREE(SymbolTableAll, get__Vec(*self->variants, i));

        FREE(Vec, self->variants);
    }

    if (self->type_value)
        FREE(DataTypeSymbolAll, self->type_value);

    FREE(Scope, self->scope);
    free(self);
}

struct EnumObjSymbol *
__new__EnumObjSymbol(struct Decl *enum_decl)
{
    struct EnumObjSymbol *self = malloc(sizeof(struct EnumObjSymbol));
    self->name = &*enum_decl->value.enum_->name;
    self->generic_params = NULL;
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
    if (self->variants) {
        for (Usize i = len__Vec(*self->variants); i--;)
            FREE(SymbolTableAll, get__Vec(*self->variants, i));

        FREE(Vec, self->variants);
    }

    if (self->attached) {
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
    self->generic_params = NULL;
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
    self->generic_params = NULL;
    self->params = NULL;
    self->return_type = NULL;
    self->body = NULL;
    self->scope = NULL;
    self->local_values = NULL;
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
    if (self->params) {
        for (Usize i = len__Vec(*self->params); i--;)
            FREE(FunParamSymbol, get__Vec(*self->params, i));

        FREE(Vec, self->params);
    }

    if (self->return_type)
        FREE(DataTypeSymbolAll, self->return_type);

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(SymbolTableAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    FREE(Scope, self->scope);

    if (self->local_values) {
        for (Usize i = 0; i < len__Vec(*self->local_values); i++) {
            struct Vec *v2 = get__Vec(*self->local_values, i);

            for (Usize j = 0; j < len__Vec(*v2); j++) {
                struct Tuple *t = get__Vec(*v2, j);

                for (Usize k = 0;
                     k < len__Vec(*CAST(t->items[1], struct Vec *));
                     k++)
                    FREE(VariableAddr,
                         CAST(get__Vec(*CAST(t->items[1], struct Vec *), k),
                              struct VariableAddr *));

                FREE(Vec, t->items[1]);
                FREE(Tuple, t);
            }

            FREE(Vec, v2);
        }

        FREE(Vec, self->local_values);
    }

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
    if (self->data_type)
        FREE(DataTypeSymbolAll, self->data_type);

    FREE(Scope, self->scope);
    free(self);
}

struct ClassSymbol *
__new__ClassSymbol(struct Decl *class_decl)
{
    struct ClassSymbol *self = malloc(sizeof(struct ClassSymbol));
    self->name = class_decl->value.class->name;
    self->generic_params = NULL;
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
    if (self->inheritance) {
        for (Usize i = len__Vec(*self->inheritance); i--;)
            FREE(DataTypeSymbolAll, get__Vec(*self->inheritance, i));

        FREE(Vec, self->inheritance);
    }

    if (self->impl) {
        for (Usize i = len__Vec(*self->impl); i--;)
            FREE(DataTypeSymbolAll, get__Vec(*self->impl, i));

        FREE(Vec, self->impl);
    }

    if (self->body) {
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
    self->generic_params = NULL;
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
    if (self->inh) {
        for (Usize i = len__Vec(*self->inh); i--;)
            FREE(DataTypeSymbolAll, get__Vec(*self->inh, i));

        FREE(Vec, self->inh);
    }

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(SymbolTableAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    FREE(Scope, self->scope);
    free(self);
}

struct UnaryOpSymbol
__new__UnaryOpSymbol(struct Expr unary_op, struct ExprSymbol *right)
{
    struct UnaryOpSymbol self = { .kind = unary_op.value.unary_op.kind,
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
    FREE(ExprSymbolAll, self.right);
}

struct BinaryOpSymbol
__new__BinaryOpSymbol(struct Expr binary_op,
                      struct ExprSymbol *left,
                      struct ExprSymbol *right)
{
    struct BinaryOpSymbol self = { .kind = binary_op.value.binary_op.kind,
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
    FREE(ExprSymbolAll, self.left);
    FREE(ExprSymbolAll, self.right);
}

struct FunCallSymbol
__new__FunCallSymbol(bool is_builtin, struct Scope *id, struct Vec *params)
{
    struct FunCallSymbol self = { .is_builtin = is_builtin,
                                  .id = id,
                                  .params = params };

    return self;
}

void
__free__FunCallSymbol(struct FunCallSymbol self)
{
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

struct SymbolTable *
__new__SymbolTableVariant(struct VariantEnumSymbol *variant)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindVariant;
    self->value.variant = variant;
    return self;
}

struct SymbolTable *
__new__SymbolTableField(struct FieldRecordSymbol *field)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindField;
    self->value.field = field;
    return self;
}

struct SymbolTable *
__new__SymbolTableProperty(struct PropertySymbol *property)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindProperty;
    self->value.property = property;
    return self;
}

struct SymbolTable *
__new__SymbolTableMethod(struct MethodSymbol *method)
{
    struct SymbolTable *self = malloc(sizeof(struct SymbolTable));
    self->kind = SymbolTableKindMethod;
    self->value.method = method;
    return self;
}

struct String *
get_name__SymbolTable(struct SymbolTable *self)
{
    switch (self->kind) {
        case SymbolTableKindFun:
            return self->value.fun->name;
        case SymbolTableKindConstant:
            return self->value.constant->name;
        case SymbolTableKindModule:
            return self->value.module->name;
        case SymbolTableKindAlias:
            return self->value.alias->name;
        case SymbolTableKindRecord:
            return self->value.record->name;
        case SymbolTableKindEnum:
            return self->value.enum_->name;
        case SymbolTableKindClass:
            return self->value.class->name;
        case SymbolTableKindTrait:
            return self->value.trait->name;
        case SymbolTableKindRecordObj:
            return self->value.record_obj->name;
        case SymbolTableKindEnumObj:
            return self->value.enum_obj->name;
        case SymbolTableKindProperty:
            return self->value.property->name;
        case SymbolTableKindMethod:
            return self->value.method->name;
        case SymbolTableKindField:
            return self->value.field->name;
        case SymbolTableKindVariant:
            return self->value.variant->name;
        default:
            return NULL;
    }
}

struct Scope *
get_scope__SymbolTable(struct SymbolTable *self)
{
    switch (self->kind) {
        case SymbolTableKindFun:
            return self->value.fun->scope;
        case SymbolTableKindConstant:
            return self->value.constant->scope;
        case SymbolTableKindModule:
            return self->value.module->scope;
        case SymbolTableKindAlias:
            return self->value.alias->scope;
        case SymbolTableKindRecord:
            return self->value.record->scope;
        case SymbolTableKindEnum:
            return self->value.enum_->scope;
        case SymbolTableKindError:
            return self->value.error->scope;
        case SymbolTableKindClass:
            return self->value.class->scope;
        case SymbolTableKindTrait:
            return self->value.trait->scope;
        case SymbolTableKindRecordObj:
            return self->value.record_obj->scope;
        case SymbolTableKindEnumObj:
            return self->value.enum_obj->scope;
        case SymbolTableKindProperty:
            return self->value.property->scope;
        case SymbolTableKindMethod:
            return self->value.method->scope;
        case SymbolTableKindField:
            return self->value.field->scope;
        case SymbolTableKindVariant:
            return self->value.variant->scope;
        default:
            UNREACHABLE("can't get scope");
    }
}

enum Visibility
get_visibility__SymbolTable(struct SymbolTable *self)
{
    switch (self->kind) {
        case SymbolTableKindFun:
            return self->value.fun->visibility;
        case SymbolTableKindConstant:
            return self->value.constant->visibility;
        case SymbolTableKindModule:
            return self->value.module->visibility;
        case SymbolTableKindAlias:
            return self->value.alias->visibility;
        case SymbolTableKindRecord:
            return self->value.record->visibility;
        case SymbolTableKindEnum:
            return self->value.enum_->visibility;
        case SymbolTableKindError:
            return self->value.error->visibility;
        case SymbolTableKindClass:
            return self->value.class->visibility;
        case SymbolTableKindTrait:
            return self->value.trait->visibility;
        case SymbolTableKindRecordObj:
            return self->value.record_obj->visibility;
        case SymbolTableKindEnumObj:
            return self->value.enum_obj->visibility;
        case SymbolTableKindExpr:
        case SymbolTableKindStmt:
            return VisibilityPrivate;
        case SymbolTableKindVariant:
            return VisibilityPublic;
        case SymbolTableKindField:
            return self->value.field->visibility;
        case SymbolTableKindProperty:
            return self->value.property->visibility;
        case SymbolTableKindMethod:
            return self->value.method->visibility;
    }
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
        case SymbolTableKindVariant:
            FREE(SymbolTableVariant, self);
            break;
        case SymbolTableKindField:
            FREE(SymbolTableField, self);
            break;
        case SymbolTableKindProperty:
            FREE(SymbolTableProperty, self);
            break;
        case SymbolTableKindMethod:
            FREE(SymbolTableMethod, self);
            break;
        default:
            UNREACHABLE("unknown symbol table kind");
    }
}

struct VariableAddr *
__new__VariableAddr(Usize id, Usize id_sec)
{
    struct VariableAddr *self = malloc(sizeof(struct VariableAddr));
    self->id = id;
    self->id_sec = id_sec;
    return self;
}

void
__free__VariableAddr(struct VariableAddr *self)
{
    free(self);
}
