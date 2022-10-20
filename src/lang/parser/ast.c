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

#include <base/format.h>
#include <base/macros.h>
#include <base/str.h>
#include <base/util.h>
#include <lang/parser/ast.h>
#include <string.h>

// It's only used when to_String* (dump AST) function is used.
static Usize current_tab_size = 0;

struct DataType *
__new__DataType(enum DataTypeKind kind)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = kind;
    return self;
}

struct DataType *
__new__DataTypePtr(struct DataType *ptr)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindPtr;
    self->value.ptr = ptr;
    return self;
}

struct DataType *
__new__DataTypeRef(struct DataType *ref)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindRef;
    self->value.ref = ref;
    return self;
}

struct DataType *
__new__DataTypeOptional(struct DataType *optional)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindOptional;
    self->value.optional = optional;
    return self;
}

struct DataType *
__new__DataTypeException(struct DataType *exception)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindException;
    self->value.exception = exception;
    return self;
}

struct DataType *
__new__DataTypeMut(struct DataType *mut)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindMut;
    self->value.mut = mut;
    return self;
}

struct DataType *
__new__DataTypeLambda(struct Vec *params, struct DataType *return_type)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindLambda;
    self->value.lambda = NEW(Tuple, 2, params, return_type);
    return self;
}

struct DataType *
__new__DataTypeArray(struct DataType *data_type, Usize *size)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindArray;
    self->value.array = NEW(Tuple, 2, data_type, size);
    return self;
}

struct DataType *
__new__DataTypeCustom(struct Vec *names, struct Vec *generic_params)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindCustom;
    self->value.custom = NEW(Tuple, 2, names, generic_params);
    return self;
}

struct DataType *
__new__DataTypeTuple(struct Vec *tuple)
{
    struct DataType *self = malloc(sizeof(struct DataType));
    self->kind = DataTypeKindTuple;
    self->value.tuple = tuple;
    return self;
}

struct String *
to_String__DataType(struct DataType self)
{
    switch (self.kind) {
        case DataTypeKindSelf:
            return from__String("self");
        case DataTypeKindPtr:
            return format("*{Sr}", to_String__DataType(*self.value.ptr));
        case DataTypeKindRef:
            return format("&{Sr}", to_String__DataType(*self.value.ref));
        case DataTypeKindStr:
            return from__String("Str");
        case DataTypeKindChar:
            return from__String("Char");
        case DataTypeKindI8:
            return from__String("Int8");
        case DataTypeKindI16:
            return from__String("Int16");
        case DataTypeKindI32:
            return from__String("Int32");
        case DataTypeKindI64:
            return from__String("Int64");
        case DataTypeKindI128:
            return from__String("Int128");
        case DataTypeKindU8:
            return from__String("Uint8");
        case DataTypeKindU16:
            return from__String("Uint16");
        case DataTypeKindU32:
            return from__String("Uint32");
        case DataTypeKindU64:
            return from__String("Uint64");
        case DataTypeKindU128:
            return from__String("Uint128");
        case DataTypeKindF32:
            return from__String("Float32");
        case DataTypeKindF64:
            return from__String("Float64");
        case DataTypeKindBool:
            return from__String("Bool");
        case DataTypeKindIsize:
            return from__String("Isize");
        case DataTypeKindUsize:
            return from__String("Usize");
        case DataTypeKindNever:
            return from__String("Never");
        case DataTypeKindAny:
            return from__String("Any");
        case DataTypeKindOptional:
            return format("?{Sr}", to_String__DataType(*self.value.optional));
        case DataTypeKindUnit:
            return from__String("Unit");
        case DataTypeKindException:
            return format("!{Sr}", to_String__DataType(*self.value.exception));
        case DataTypeKindLambda: {
            struct Vec *params = self.value.lambda->items[0];
            struct String *lambda_string = NEW(String);

            push_str__String(lambda_string, "|");
            append__String(lambda_string,
                           to_String__DataType(
                             *CAST(get__Vec(*params, 0), struct DataType *)),
                           true);

            for (Usize i = 1; i < len__Vec(*params); i++) {
                push_str__String(lambda_string, " -> ");
                append__String(lambda_string,
                               to_String__DataType(*CAST(get__Vec(*params, i),
                                                         struct DataType *)),
                               true);
            }

            push_str__String(lambda_string, " -> ");
            append__String(lambda_string,
                           to_String__DataType(*CAST(
                             self.value.lambda->items[1], struct DataType *)),
                           true);
            push_str__String(lambda_string, "|");

            return lambda_string;
        }
        case DataTypeKindArray: {
            struct Option *data_type_op = self.value.array->items[0];
            struct Option *size_op = self.value.array->items[1];
            struct String *array_string = NEW(String);

            if (is_Some__Option(size_op)) {
                Usize *size = get__Option(size_op);

                append__String(array_string, format("[{d}]", (int)*size), true);
            } else
                push_str__String(array_string, "[_]");

            if (is_Some__Option(data_type_op)) {
                struct DataType *data_type = get__Option(data_type_op);

                append__String(
                  array_string, to_String__DataType(*data_type), true);
            }

            return array_string;
        }
        case DataTypeKindTuple: {
            struct String *tuple_string = NEW(String);

            push_str__String(tuple_string, "(");
            append__String(
              tuple_string,
              to_String__DataType(
                *CAST(get__Vec(*self.value.tuple, 0), struct DataType *)),
              true);

            for (Usize i = 1; i < len__Vec(*self.value.tuple); i++)
                append__String(
                  tuple_string,
                  format(", {Sr}",
                         to_String__DataType(*CAST(
                           get__Vec(*self.value.tuple, i), struct DataType *))),
                  true);

            push_str__String(tuple_string, ")");

            return tuple_string;
        }
        case DataTypeKindCustom: {
            struct String *custom_string = NEW(String);

            if (self.value.custom->items[0]) {
                struct Vec *names = self.value.custom->items[0];

                for (Usize i = 0; i < len__Vec(*names) - 1; i++)
                    append__String(
                      custom_string,
                      format("{S}.",
                             CAST(get__Vec(*names, i), struct String *)),
                      true);

                append__String(
                  custom_string,
                  format("{S}",
                         CAST(get__Vec(*names, len__Vec(*names) - 1),
                              struct String *)),
                  true);
            }

            if (self.value.custom->items[1]) {
                struct Vec *generic = self.value.custom->items[1];

                push_str__String(custom_string, "[");
                append__String(custom_string,
                               to_String__DataType(*CAST(get__Vec(*generic, 0),
                                                         struct DataType *)),
                               true);

                for (Usize i = 1; i < len__Vec(*generic); i++)
                    append__String(
                      custom_string,
                      format(", {Sr}",
                             to_String__DataType(*CAST(get__Vec(*generic, i),
                                                       struct DataType *))),
                      true);

                push_str__String(custom_string, "]");
            }

            return custom_string;
        }
        default:
            UNREACHABLE("unknown data type kind");
    }
}

struct DataType *
copy__DataType(struct DataType *self)
{
    struct DataType *copy = malloc(sizeof(struct DataType));

    memcpy(copy, self, sizeof(struct DataType));

    return copy;
}

void
__free__DataType(struct DataType *self)
{
    free(self);
}

void
__free__DataTypePtr(struct DataType *self)
{
    FREE(DataTypeAll, self->value.ptr);
    free(self);
}

void
__free__DataTypeRef(struct DataType *self)
{
    FREE(DataTypeAll, self->value.ref);
    free(self);
}

void
__free__DataTypeOptional(struct DataType *self)
{
    FREE(DataTypeAll, self->value.optional);
    free(self);
}

void
__free__DataTypeException(struct DataType *self)
{
    FREE(DataTypeAll, self->value.exception);
    free(self);
}

void
__free__DataTypeMut(struct DataType *self)
{
    FREE(DataTypeAll, self->value.mut);
    free(self);
}

void
__free__DataTypeLambda(struct DataType *self)
{
    if (self->value.lambda->items[0]) {
        struct Vec *temporary = self->value.lambda->items[0];

        for (Usize i = len__Vec(*temporary); i--;)
            FREE(DataTypeAll, get__Vec(*temporary, i));

        FREE(Vec, temporary);
    }

    FREE(DataTypeAll, self->value.lambda->items[1]);
    FREE(Tuple, self->value.lambda);
    free(self);
}

void
__free__DataTypeArray(struct DataType *self)
{
    if (self->value.array->items[0])
        FREE(DataTypeAll, self->value.array->items[0]);

    FREE(Tuple, self->value.array);
    free(self);
}

void
__free__DataTypeCustom(struct DataType *self)
{
    FREE(Vec, self->value.custom->items[0]);

    if (self->value.custom->items[1]) {
        for (Usize i =
               len__Vec(*CAST(self->value.custom->items[1], struct Vec *));
             i--;)
            FREE(
              DataTypeAll,
              get__Vec(*CAST(self->value.custom->items[1], struct Vec *), i));

        FREE(Vec, self->value.custom->items[1]);
    }

    FREE(Tuple, self->value.custom);
    free(self);
}

void
__free__DataTypeTuple(struct DataType *self)
{
    for (Usize i = len__Vec(*self->value.tuple); i--;)
        FREE(DataTypeAll, get__Vec(*self->value.tuple, i));

    FREE(Vec, self->value.tuple);
    free(self);
}

void
__free__DataTypeAll(struct DataType *self)
{
    switch (self->kind) {
        case DataTypeKindPtr:
            FREE(DataTypePtr, self);
            break;
        case DataTypeKindRef:
            FREE(DataTypeRef, self);
            break;
        case DataTypeKindOptional:
            FREE(DataTypeOptional, self);
            break;
        case DataTypeKindException:
            FREE(DataTypeException, self);
            break;
        case DataTypeKindMut:
            FREE(DataTypeMut, self);
            break;
        case DataTypeKindLambda:
            FREE(DataTypeLambda, self);
            break;
        case DataTypeKindArray:
            FREE(DataTypeArray, self);
            break;
        case DataTypeKindCustom:
            FREE(DataTypeCustom, self);
            break;
        case DataTypeKindTuple:
            FREE(DataTypeTuple, self);
            break;
        case DataTypeKindCompilerDefined:
            UNREACHABLE("CompilerDefined is not used in AST");
        default:
            FREE(DataType, self);
            break;
    }
}

struct Generic *
__new__GenericDataType(struct String *data_type, struct Location loc)
{
    struct Generic *self = malloc(sizeof(struct Generic));
    self->kind = GenericKindDataType;
    self->loc = loc;
    self->value.data_type = data_type;
    return self;
}

struct Generic *
__new__GenericRestrictedDataType(struct String *name,
                                 struct Location loc,
                                 struct Tuple *data_type)
{
    struct Generic *self = malloc(sizeof(struct Generic));
    self->kind = GenericKindRestrictedDataType;
    self->loc = loc;
    self->value.restricted_data_type = NEW(Tuple, 2, name, data_type);
    return self;
}

struct String *
get_name__Generic(struct Generic *self)
{
    switch (self->kind) {
        case GenericKindDataType:
            return self->value.data_type;
        case GenericKindRestrictedDataType:
            return self->value.restricted_data_type->items[0];
    }

    return NULL;
}

struct String *
to_String__Generic(struct Generic self)
{
    switch (self.kind) {
        case GenericKindDataType:
            return format("{S}", self.value.data_type);
        case GenericKindRestrictedDataType:
            return format(
              "{S}: {Sr}",
              self.value.restricted_data_type->items[0],
              to_String__DataType(*CAST(
                CAST(self.value.restricted_data_type->items[1], struct Tuple *)
                  ->items[0],
                struct DataType *)));
        default:
            UNREACHABLE("unknown generic kind");
    }
}

void
__free__GenericRestrictedDataType(struct Generic *self)
{
    FREE(DataTypeAll,
         CAST(self->value.restricted_data_type->items[1], struct Tuple *)
           ->items[0]);
    free(CAST(self->value.restricted_data_type->items[1], struct Tuple *)
           ->items[1]);
    FREE(Tuple, self->value.restricted_data_type->items[1]);
    FREE(Tuple, self->value.restricted_data_type);
    free(self);
}

void
__free__GenericAll(struct Generic *self)
{
    switch (self->kind) {
        case GenericKindDataType:
            FREE(GenericDataType, self);
            break;
        case GenericKindRestrictedDataType:
            FREE(GenericRestrictedDataType, self);
            break;
        default:
            UNREACHABLE("unknown generic kind");
    }
}

struct VariableDecl
__new__VariableDecl(struct String *name,
                    struct DataType *data_type,
                    struct Expr *expr,
                    bool is_mut)
{
    struct VariableDecl self = {
        .name = name, .data_type = data_type, .expr = expr, .is_mut = is_mut
    };

    return self;
}

struct String *
to_String__VariableDecl(struct VariableDecl self)
{
    if (self.data_type)
        return format("{S} :: {Sr} := {Sr}",
                      self.name,
                      to_String__DataType(*self.data_type),
                      to_String__Expr(*self.expr));

    return format("{S} := {Sr}", self.name, to_String__Expr(*self.expr));
}

void
__free__VariableDecl(struct VariableDecl self)
{
    if (self.data_type)
        FREE(DataTypeAll, self.data_type);

    FREE(ExprAll, self.expr);
}

struct String *
to_String__Literal(struct Literal self)
{
    switch (self.kind) {
        case LiteralKindBool:
            return format("{b}", self.value.bool_);
        case LiteralKindChar:
            return format("'{c}'", self.value.char_);
        case LiteralKindBitChar:
            return format("b'{c}'", (char)self.value.bit_char);
        case LiteralKindInt32WithoutSuffix:
            return format("{d}", self.value.int32_ws);
        case LiteralKindInt64WithoutSuffix:
            return format("{L}", self.value.int64_ws);
        case LiteralKindInt128WithoutSuffix:
            return from__String("I128");
        case LiteralKindInt8:
            return format("{d}", self.value.int8);
        case LiteralKindInt16:
            return format("{d}", self.value.int16);
        case LiteralKindInt32:
            return format("{d}", self.value.int32);
        case LiteralKindInt64:
            return format("{L}", self.value.int64);
        case LiteralKindInt128:
            return from__String("I128");
        case LiteralKindUint8:
            return format("{d}", self.value.uint8);
        case LiteralKindUint16:
            return format("{d}", self.value.uint16);
        case LiteralKindUint32:
            return format("{d}", self.value.uint32);
        case LiteralKindUint64:
            return format("{Lu}", self.value.uint64);
        case LiteralKindUint128:
            return itoa_u128(self.value.uint128, 10);
        case LiteralKindFloat32:
            return format("{f}", self.value.float32);
        case LiteralKindFloat64:
            return format("{f}", self.value.float64);
        case LiteralKindFloat:
            return format("{f}", self.value.float_);
        case LiteralKindStr:
            return format("\"{s}\"", self.value.str);
        case LiteralKindBitStr: {
            struct String *s = NEW(String);

            for (Usize i = 0; self.value.bit_str[i]; i++)
                append__String(
                  s, format("{c}", (char)(UPtr)self.value.bit_str[i]), true);

            return format("b\"{s}\"", self.value.bit_str);
        }
        case LiteralKindUnit:
            return from__String("()");
    }

    return NULL;
}

void
__free__LiteralAll(struct Literal self)
{
    switch (self.kind) {
        case LiteralKindStr:
            FREE(LiteralStr, self);
            break;
        case LiteralKindBitStr:
            free(self.value.bit_str);
            break;
        default:
            break;
    }
}

const Str
to_str__UnaryOpKind(enum UnaryOpKind kind)
{
    switch (kind) {
        case UnaryOpKindNegative:
            return "-";
        case UnaryOpKindNot:
            return "not";
        case UnaryOpKindReference:
            return "&";
        case UnaryOpKindBitNot:
            return "~";
        case UnaryOpKindCustom:
            return "Custom";
    }

    return "unknown";
}

struct String *
to_String__UnaryOp(struct UnaryOp self)
{
    switch (self.kind) {
        case UnaryOpKindNegative:
            return format("-{Sr}", to_String__Expr(*self.right));
        case UnaryOpKindNot:
            return format("not {Sr}", to_String__Expr(*self.right));
        case UnaryOpKindReference:
            return format("&{Sr}", to_String__Expr(*self.right));
        case UnaryOpKindBitNot:
            return format("~{Sr}", to_String__Expr(*self.right));
        case UnaryOpKindCustom:
            return format("`{S}`{Sr}", self.op, to_String__Expr(*self.right));
    }

    return NULL;
}

void
__free__UnaryOp(struct UnaryOp self)
{
    if (self.kind == UnaryOpKindCustom)
        FREE(String, self.op);

    FREE(ExprAll, self.right);
}

Usize
get_precedence__BinaryOpKind(enum BinaryOpKind kind)
{
    switch (kind) {
        case BinaryOpKindMul:
        case BinaryOpKindDiv:
        case BinaryOpKindMod:
        case BinaryOpKindExponent:
            return 4;

        case BinaryOpKindAdd:
        case BinaryOpKindSub:
            return 5;

        case BinaryOpKindBitLShift:
        case BinaryOpKindBitRShift:
            return 6;

        case BinaryOpKindLt:
        case BinaryOpKindGt:
        case BinaryOpKindLe:
        case BinaryOpKindGe:
        case BinaryOpKindChain:
            return 7;

        case BinaryOpKindMerge:
        case BinaryOpKindUnmerge:
        case BinaryOpKindRepeat:
        case BinaryOpKindConcat:
        case BinaryOpKindCustom:
            return 8;

        case BinaryOpKindEq:
        case BinaryOpKindNe:
            return 9;

        case BinaryOpKindBitAnd:
            return 10;

        case BinaryOpKindXor:
            return 11;

        case BinaryOpKindBitOr:
            return 12;

        case BinaryOpKindAnd:
            return 13;

        case BinaryOpKindOr:
            return 14;

        case BinaryOpKindRange:
            return 15;

        case BinaryOpKindAssign:
        case BinaryOpKindAddAssign:
        case BinaryOpKindSubAssign:
        case BinaryOpKindMulAssign:
        case BinaryOpKindDivAssign:
        case BinaryOpKindModAssign:
        case BinaryOpKindConcatAssign:
        case BinaryOpKindBitLShiftAssign:
        case BinaryOpKindBitRShiftAssign:
        case BinaryOpKindBitOrAssign:
        case BinaryOpKindXorAssign:
        case BinaryOpKindBitAndAssign:
        case BinaryOpKindMergeAssign:
        case BinaryOpKindUnmergeAssign:
        case BinaryOpKindExponentAssign:
            return 16;
    }

    return -1;
}

const Str
to_str__BinaryOpKind(enum BinaryOpKind kind)
{
    switch (kind) {
        case BinaryOpKindAdd:
            return "+";
        case BinaryOpKindSub:
            return "-";
        case BinaryOpKindMul:
            return "*";
        case BinaryOpKindDiv:
            return "/";
        case BinaryOpKindMod:
            return "%";
        case BinaryOpKindRange:
            return "..";
        case BinaryOpKindLt:
            return "<";
        case BinaryOpKindGt:
            return ">";
        case BinaryOpKindLe:
            return "<=";
        case BinaryOpKindGe:
            return ">=";
        case BinaryOpKindEq:
            return "==";
        case BinaryOpKindNe:
            return "not=";
        case BinaryOpKindAnd:
            return "and";
        case BinaryOpKindOr:
            return "or";
        case BinaryOpKindXor:
            return "xor";
        case BinaryOpKindAssign:
            return "=";
        case BinaryOpKindAddAssign:
            return "+=";
        case BinaryOpKindSubAssign:
            return "-=";
        case BinaryOpKindMulAssign:
            return "*=";
        case BinaryOpKindDivAssign:
            return "/=";
        case BinaryOpKindModAssign:
            return "%=";
        case BinaryOpKindConcatAssign:
            return "^=";
        case BinaryOpKindBitLShiftAssign:
            return "<<=";
        case BinaryOpKindBitRShiftAssign:
            return ">>=";
        case BinaryOpKindBitOrAssign:
            return "|=";
        case BinaryOpKindXorAssign:
            return "xor=";
        case BinaryOpKindBitAndAssign:
            return "&=";
        case BinaryOpKindMergeAssign:
            return "++=";
        case BinaryOpKindUnmergeAssign:
            return "--=";
        case BinaryOpKindExponentAssign:
            return "**=";
        case BinaryOpKindChain:
            return "|>";
        case BinaryOpKindMerge:
            return "++";
        case BinaryOpKindUnmerge:
            return "--";
        case BinaryOpKindRepeat:
            return "$";
        case BinaryOpKindConcat:
            return "^";
        case BinaryOpKindBitLShift:
            return "<<";
        case BinaryOpKindBitRShift:
            return ">>";
        case BinaryOpKindBitOr:
            return "|";
        case BinaryOpKindBitAnd:
            return "&";
        case BinaryOpKindExponent:
            return "**";
        case BinaryOpKindCustom:
            return "Custom";
    }

    return "unknown";
}

struct BinaryOp
__new__BinaryOp(enum BinaryOpKind kind,
                struct Expr *left,
                struct Expr *right,
                struct String *op)
{
    struct BinaryOp self = {
        .kind = kind, .left = left, .right = right, .op = op
    };

    return self;
}

struct String *
to_String__BinaryOp(struct BinaryOp self)
{
    switch (self.kind) {
        case BinaryOpKindAdd:
            return format("{Sr} + {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindSub:
            return format("{Sr} - {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindMul:
            return format("{Sr} * {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindDiv:
            return format("{Sr} / {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindMod:
            return format("{Sr} % {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindRange:
            return format("{Sr} .. {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindLt:
            return format("{Sr} < {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindGt:
            return format("{Sr} > {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindLe:
            return format("{Sr} <= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindGe:
            return format("{Sr} >= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindEq:
            return format("{Sr} == {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindNe:
            return format("{Sr} not= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindAnd:
            return format("{Sr} and {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindOr:
            return format("{Sr} or {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindXor:
            return format("{Sr} xor {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindAssign:
            return format("{Sr} = {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindAddAssign:
            return format("{Sr} += {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindSubAssign:
            return format("{Sr} -= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindMulAssign:
            return format("{Sr} *= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindDivAssign:
            return format("{Sr} /= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindModAssign:
            return format("{Sr} %= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindConcatAssign:
            return format("{Sr} ^= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitLShiftAssign:
            return format("{Sr} <<= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitRShiftAssign:
            return format("{Sr} >>= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitOrAssign:
            return format("{Sr} |= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindXorAssign:
            return format("{Sr} xor= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitAndAssign:
            return format("{Sr} &= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindMergeAssign:
            return format("{Sr} ++= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindUnmergeAssign:
            return format("{Sr} --= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindExponentAssign:
            return format("{Sr} **= {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindChain:
            return format("{Sr} |> {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindMerge:
            return format("{Sr} ++ {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindUnmerge:
            return format("{Sr} -- {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindRepeat:
            return format("{Sr} $ {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindConcat:
            return format("{Sr} ^ {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitLShift:
            return format("{Sr} << {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitRShift:
            return format("{Sr} >> {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitOr:
            return format("{Sr} | {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindBitAnd:
            return format("{Sr} & {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindExponent:
            return format("{Sr} ** {Sr}",
                          to_String__Expr(*self.left),
                          to_String__Expr(*self.right));
        case BinaryOpKindCustom:
            return format("{Sr} `{S}` {Sr}",
                          to_String__Expr(*self.left),
                          self.op,
                          to_String__Expr(*self.right));
        default:
            UNREACHABLE("unknown binop kind");
    }
}

void
__free__BinaryOp(struct BinaryOp self)
{
    if (self.kind == BinaryOpKindCustom)
        FREE(String, self.op);

    FREE(ExprAll, self.left);
    FREE(ExprAll, self.right);
}

struct FunCall
__new__FunCall(struct Expr *id, struct Vec *params)
{
    struct FunCall self = { .id = id, .params = params };

    return self;
}

struct String *
to_String__FunCall(struct FunCall self)
{
    struct String *s = NEW(String);

    append__String(s, format("{Sr}(", to_String__Expr(*self.id)), true);

    if (len__Vec(*self.params) > 0) {
        for (Usize i = 0; i < len__Vec(*self.params) - 1; i++)
            append__String(
              s,
              format(
                "{Sr}, ",
                to_String__FunParamCall(*CAST(
                  CAST(get__Vec(*self.params, i), struct Tuple *)->items[0],
                  struct FunParamCall *))),
              true);

        append__String(
          s,
          format("{Sr})",
                 to_String__FunParamCall(*CAST(
                   CAST(get__Vec(*self.params, len__Vec(*self.params) - 1),
                        struct Tuple *)
                     ->items[0],
                   struct FunParamCall *))),
          true);
    } else
        push_str__String(s, ")");

    return s;
}

void
__free__FunCall(struct FunCall self)
{
    FREE(ExprAll, self.id);

    for (Usize i = len__Vec(*self.params); i--;) {
        FREE(FunParamCallAll,
             CAST(get__Vec(*self.params, i), struct Tuple *)->items[0]);
        free(CAST(get__Vec(*self.params, i), struct Tuple *)->items[1]);
        FREE(Tuple, CAST(get__Vec(*self.params, i), struct Tuple *));
    }

    FREE(Vec, self.params);
}

struct FieldCall *
__new__FieldCall(struct String *name, struct Option *value)
{
    struct FieldCall *self = malloc(sizeof(struct FieldCall));
    self->name = name;
    self->value = value;
    return self;
}

struct String *
to_String__FieldCall(struct FieldCall self)
{
    if (is_Some__Option(self.value))
        return format(
          "{S} := {Sr}",
          self.name,
          to_String__Expr(*CAST(get__Option(self.value), struct Expr *)));
    else
        return format("{S}", self.name);
}

void
__free__FieldCall(struct FieldCall *self)
{
    if (is_Some__Option(self->value))
        FREE(ExprAll, get__Option(self->value));

    FREE(Option, self->value);
    free(self);
}

struct RecordCall
__new__RecordCall(struct Expr *id, struct Vec *fields)
{
    struct RecordCall self = { .id = id, .fields = fields };

    return self;
}

struct String *
to_String__RecordCall(struct RecordCall self)
{
    struct String *s = NEW(String);

    append__String(s, format("{Sr} {{ ", to_String__Expr(*self.id)), true);

    for (Usize i = 0; i < len__Vec(*self.fields) - 1; i++)
        append__String(
          s,
          format("{Sr}, ",
                 to_String__FieldCall(*CAST(
                   CAST(get__Vec(*self.fields, i), struct Tuple *)->items[0],
                   struct FieldCall *))),
          true);

    append__String(
      s,
      format("{Sr}}",
             to_String__FieldCall(
               *CAST(CAST(get__Vec(*self.fields, len__Vec(*self.fields) - 1),
                          struct Tuple *)
                       ->items[0],
                     struct FieldCall *))),
      true);

    return s;
}

void
__free__RecordCall(struct RecordCall self)
{
    FREE(ExprAll, self.id);

    for (Usize i = len__Vec(*self.fields); i--;) {
        FREE(FieldCall,
             CAST(get__Vec(*self.fields, i), struct Tuple *)->items[0]);
        free(CAST(get__Vec(*self.fields, i), struct Tuple *)->items[1]);
        FREE(Tuple, CAST(get__Vec(*self.fields, i), struct Tuple *));
    }

    FREE(Vec, self.fields);
}

struct String *
to_String__ArrayAccess(struct ArrayAccess self)
{
    struct String *s = NEW(String);

    append__String(s, format("{Sr}", to_String__Expr(*self.id)), true);

    for (Usize i = 0; i < len__Vec(*self.access); i++)
        append__String(s,
                       format("[{Sr}]",
                              to_String__Expr(*CAST(get__Vec(*self.access, i),
                                                    struct Expr *))),
                       true);

    return s;
}

void
__free__ArrayAccess(struct ArrayAccess self)
{
    FREE(ExprAll, self.id);

    for (Usize i = len__Vec(*self.access); i--;)
        FREE(ExprAll, get__Vec(*self.access, i));

    FREE(Vec, self.access);
}

struct String *
to_String__TupleAccess(struct TupleAccess self)
{
    struct String *s = NEW(String);

    append__String(s, format("{Sr}", to_String__Expr(*self.id)), true);

    for (Usize i = 0; i < len__Vec(*self.access); i++)
        append__String(s,
                       format("#{Sr}",
                              to_String__Expr(*CAST(get__Vec(*self.access, i),
                                                    struct Expr *))),
                       true);

    return s;
}

void
__free__TupleAccess(struct TupleAccess self)
{
    FREE(ExprAll, self.id);

    for (Usize i = len__Vec(*self.access); i--;)
        FREE(ExprAll, get__Vec(*self.access, i));

    FREE(Vec, self.access);
}

struct Lambda
__new__Lambda(struct Vec *params,
              struct DataType *return_type,
              struct Vec *body,
              bool instantly_call)
{
    struct Lambda self = { .params = params,
                           .return_type = return_type,
                           .body = body,
                           .instantly_call = instantly_call };

    return self;
}

struct String *
to_String__Lambda(struct Lambda self)
{
    struct String *s = NEW(String);

    append__String(s, from__String("fun ("), true);

    // 1. Dump params
    for (Usize i = 0; i < len__Vec(*self.params) - 1; i++)
        append__String(s,
                       format("{Sr}, ",
                              to_String__FunParam(*CAST(
                                get__Vec(*self.params, i), struct FunParam *))),
                       true);

    append__String(s,
                   format("{Sr})",
                          to_String__FunParam(*CAST(
                            get__Vec(*self.params, len__Vec(*self.params) - 1),
                            struct FunParam *))),
                   true);

    // 2. Dump return data type
    if (self.return_type)
        append__String(
          s,
          format(" {Sr} -> (", to_String__DataType(*self.return_type)),
          true);
    else
        push_str__String(s, " -> (");

    // 3. Dump body
    for (Usize i = 0; i < len__Vec(*self.body); i++)
        append__String(
          s,
          format("{Sr}",
                 to_String__FunBodyItem(
                   *CAST(get__Vec(*self.body, i), struct FunBodyItem *))),
          true);

    if (len__Vec(*self.body) == 1)
        pop__String(s);

    push_str__String(s, ")");

    if (self.instantly_call)
        push_str__String(s, "()");

    return s;
}

void
__free__Lambda(struct Lambda self)
{
    for (Usize i = len__Vec(*self.params); i--;)
        FREE(FunParamAll, get__Vec(*self.params, i));

    FREE(Vec, self.params);

    if (self.return_type)
        FREE(DataTypeAll, self.return_type);

    for (Usize i = len__Vec(*self.body); i--;)
        FREE(FunBodyItemAll, get__Vec(*self.body, i));

    FREE(Vec, self.body);
}

struct String *
to_String__Variant(struct Variant self)
{
    if (self.value)
        return format(
          "{Sr}:{Sr}", to_String__Expr(*self.id), to_String__Expr(*self.value));

    return format("{Sr}:$", to_String__Expr(*self.id));
}

void
__free__Variant(struct Variant self)
{
    FREE(ExprAll, self.id);

    if (self.value)
        FREE(ExprAll, self.value);
}

struct Expr *
__new__Expr(enum ExprKind kind, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = kind;
    self->loc = loc;
    return self;
}

struct Expr *
__new__ExprUnaryOp(struct UnaryOp unary_op, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindUnaryOp;
    self->loc = loc;
    self->value.unary_op = unary_op;
    return self;
}

struct Expr *
__new__ExprBinaryOp(struct BinaryOp binary_op, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindBinaryOp;
    self->loc = loc;
    self->value.binary_op = binary_op;
    return self;
}

struct Expr *
__new__ExprFunCall(struct FunCall fun_call, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindFunCall;
    self->loc = loc;
    self->value.fun_call = fun_call;
    return self;
}

struct Expr *
__new__ExprRecordCall(struct RecordCall record_call, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindRecordCall;
    self->loc = loc;
    self->value.record_call = record_call;
    return self;
}

struct Expr *
__new__ExprIdentifier(struct String *identifier, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindIdentifier;
    self->loc = loc;
    self->value.identifier = identifier;
    return self;
}

struct Expr *
__new__ExprIdentifierAccess(struct Vec *identifier_access, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindIdentifierAccess;
    self->loc = loc;
    self->value.identifier_access = identifier_access;
    return self;
}

struct Expr *
__new__ExprGlobalAccess(struct Vec *global_access, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindGlobalAccess;
    self->loc = loc;
    self->value.global_access = global_access;
    return self;
}

struct Expr *
__new__ExprPropertyAccessInit(struct Vec *property_access_init,
                              struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindPropertyAccessInit;
    self->loc = loc;
    self->value.property_access_init = property_access_init;
    return self;
}

struct Expr *
__new__ExprArrayAccess(struct ArrayAccess array_access, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindArrayAccess;
    self->loc = loc;
    self->value.array_access = array_access;
    return self;
}

struct Expr *
__new__ExprTupleAccess(struct TupleAccess tuple_access, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindTupleAccess;
    self->loc = loc;
    self->value.tuple_access = tuple_access;
    return self;
}

struct Expr *
__new__ExprLambda(struct Lambda lambda, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindLambda;
    self->loc = loc;
    self->value.lambda = lambda;
    return self;
}

struct Expr *
__new__ExprArray(struct Vec *array, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindArray;
    self->loc = loc;
    self->value.array = array;
    return self;
}

struct Expr *
__new__ExprTuple(struct Vec *tuple, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindTuple;
    self->loc = loc;
    self->value.tuple = tuple;
    return self;
}

struct Expr *
__new__ExprVariant(struct Variant variant, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindVariant;
    self->loc = loc;
    self->value.variant = variant;
    return self;
}

struct Expr *
__new__ExprTry(struct Expr *try, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindTry;
    self->loc = loc;
    self->value.try = try;
    return self;
}

struct Expr *
__new__ExprIf(struct IfCond *if_, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindIf;
    self->loc = loc;
    self->value.if_ = if_;
    return self;
}

struct Expr *
__new__ExprBlock(struct Vec *block, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindBlock;
    self->loc = loc;
    self->value.block = block;
    return self;
}

struct Expr *
__new__ExprQuestionMark(struct Expr *question_mark, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindQuestionMark;
    self->loc = loc;
    self->value.question_mark = question_mark;
    return self;
}

struct Expr *
__new__ExprDereference(struct Expr *dereference, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindDereference;
    self->loc = loc;
    self->value.dereference = dereference;
    return self;
}

struct Expr *
__new__ExprRef(struct Expr *ref, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindRef;
    self->loc = loc;
    self->value.ref = ref;
    return self;
}

struct Expr *
__new__ExprLiteral(struct Literal literal, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindLiteral;
    self->loc = loc;
    self->value.literal = literal;
    return self;
}

struct Expr *
__new__ExprVariable(struct VariableDecl variable, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindVariable;
    self->loc = loc;
    self->value.variable = variable;
    return self;
}

struct Expr *
__new__ExprGrouping(struct Expr *grouping, struct Location loc)
{
    struct Expr *self = malloc(sizeof(struct Expr));
    self->kind = ExprKindGrouping;
    self->loc = loc;
    self->value.grouping = grouping;
    return self;
}

UInt32
get_precedence__Expr(struct Expr *expr)
{
    assert(expr && "expr == NULL");

    switch (expr->kind) {
        case ExprKindGrouping:
            return 1;

        case ExprKindFunCall:
        case ExprKindArrayAccess:
        case ExprKindIdentifierAccess:
        case ExprKindTupleAccess:
            return 2;

        case ExprKindUnaryOp:
        case ExprKindDereference:
        case ExprKindQuestionMark:
        case ExprKindRef:
            return 3;

        case ExprKindBinaryOp:
            return get_precedence__BinaryOpKind(expr->value.binary_op.kind);

        case ExprKindLiteral:
        case ExprKindRecordCall:
        case ExprKindTuple:
        case ExprKindArray:
        case ExprKindSelf:
        case ExprKindUndef:
        case ExprKindNil:
        case ExprKindWildcard:
        case ExprKindIdentifier:
        case ExprKindVariant:
            return 17;

        case ExprKindTry:
        case ExprKindIf:
        case ExprKindBlock:
        case ExprKindVariable:
        case ExprKindLambda:
            return 18;

        default:
            return -1;
    }
}

struct String *
to_String__Expr(struct Expr self)
{
    switch (self.kind) {
        case ExprKindUnaryOp:
            return format("{Sr}", to_String__UnaryOp(self.value.unary_op));
        case ExprKindBinaryOp:
            return format("{Sr}", to_String__BinaryOp(self.value.binary_op));
        case ExprKindFunCall:
            return to_String__FunCall(self.value.fun_call);
        case ExprKindRecordCall:
            return to_String__RecordCall(self.value.record_call);
        case ExprKindIdentifier:
            return format("{S}", self.value.identifier);
        case ExprKindIdentifierAccess: {
            struct String *s = NEW(String);

            for (Usize i = 0; i < len__Vec(*self.value.identifier_access) - 1;
                 i++)
                append__String(
                  s,
                  format("{Sr}.",
                         to_String__Expr(
                           *CAST(get__Vec(*self.value.identifier_access, i),
                                 struct Expr *))),
                  true);

            append__String(
              s,
              format("{Sr}",
                     to_String__Expr(*CAST(
                       get__Vec(*self.value.identifier_access,
                                len__Vec(*self.value.identifier_access) - 1),
                       struct Expr *))),
              true);

            return s;
        }
        case ExprKindGlobalAccess: {
            struct String *s = NEW(String);

            append__String(s, from__String("global."), true);

            for (Usize i = 0; i < len__Vec(*self.value.global_access) - 1; i++)
                append__String(s,
                               format("{Sr}.",
                                      to_String__Expr(*CAST(
                                        get__Vec(*self.value.global_access, i),
                                        struct Expr *))),
                               true);

            append__String(
              s,
              format("{Sr}",
                     to_String__Expr(
                       *CAST(get__Vec(*self.value.global_access,
                                      len__Vec(*self.value.global_access) - 1),
                             struct Expr *))),
              true);

            return s;
        }
        case ExprKindPropertyAccessInit: {
            struct String *s = NEW(String);

            append__String(s, from__String("@."), true);

            for (Usize i = 0;
                 i < len__Vec(*self.value.property_access_init) - 1;
                 i++)
                append__String(
                  s,
                  format("{Sr}.",
                         to_String__Expr(
                           *CAST(get__Vec(*self.value.property_access_init, i),
                                 struct Expr *))),
                  true);

            append__String(
              s,
              format("{Sr}",
                     to_String__Expr(*CAST(
                       get__Vec(*self.value.property_access_init,
                                len__Vec(*self.value.property_access_init) - 1),
                       struct Expr *))),
              true);

            return s;
        }
        case ExprKindArrayAccess:
            return to_String__ArrayAccess(self.value.array_access);
        case ExprKindTupleAccess:
            return to_String__TupleAccess(self.value.tuple_access);
        case ExprKindLambda:
            return to_String__Lambda(self.value.lambda);
        case ExprKindArray: {
            struct String *s = NEW(String);

            push_str__String(s, "[");

            if (len__Vec(*self.value.array) > 0) {
                for (Usize i = 0; i < len__Vec(*self.value.array) - 1; i++)
                    append__String(
                      s,
                      format("{Sr}, ",
                             to_String__Expr(*CAST(
                               get__Vec(*self.value.array, i), struct Expr *))),
                      true);

                append__String(
                  s,
                  format("{Sr}]",
                         to_String__Expr(
                           *CAST(get__Vec(*self.value.array,
                                          len__Vec(*self.value.array) - 1),
                                 struct Expr *))),
                  true);
            } else
                push_str__String(s, "]");

            return s;
        }
        case ExprKindTuple: {
            struct String *s = NEW(String);

            push_str__String(s, "(");

            if (len__Vec(*self.value.tuple) > 0) {
                for (Usize i = 0; i < len__Vec(*self.value.tuple) - 1; i++)
                    append__String(
                      s,
                      format("{Sr}, ",
                             to_String__Expr(*CAST(
                               get__Vec(*self.value.tuple, i), struct Expr *))),
                      true);

                append__String(
                  s,
                  format("{Sr})",
                         to_String__Expr(
                           *CAST(get__Vec(*self.value.tuple,
                                          len__Vec(*self.value.tuple) - 1),
                                 struct Expr *))),
                  true);
            } else
                push_str__String(s, ")");

            return s;
        }
        case ExprKindVariant:
            return to_String__Variant(self.value.variant);
        case ExprKindTry:
            return format("try {Sr}", to_String__Expr(*self.value.try));
        case ExprKindIf:
            return to_String__IfCond(*self.value.if_);
        case ExprKindBlock: {
            struct String *s = NEW(String);

            push_str__String(s, "begin =\n");

            ++current_tab_size;

            for (Usize i = 0; i < len__Vec(*self.value.block); i++)
                append__String(
                  s,
                  to_String__FunBodyItem(*CAST(get__Vec(*self.value.block, i),
                                               struct FunBodyItem *)),
                  true);

            --current_tab_size;

            append__String(
              s,
              format("{Sr}end", repeat__String("\t", current_tab_size)),
              true);

            return s;
        }
        case ExprKindQuestionMark:
            return format("{Sr}.?", to_String__Expr(*self.value.question_mark));
        case ExprKindDereference:
            return format("{Sr}.*", to_String__Expr(*self.value.dereference));
        case ExprKindRef:
            return format("&{Sr}", to_String__Expr(*self.value.ref));
        case ExprKindSelf:
            return from__String("self");
        case ExprKindUndef:
            return from__String("undef");
        case ExprKindNil:
            return from__String("nil");
        case ExprKindNone:
            return from__String("None");
        case ExprKindWildcard:
            return from__String("_");
        case ExprKindLiteral:
            return to_String__Literal(self.value.literal);
        case ExprKindVariable:
            return to_String__VariableDecl(self.value.variable);
        case ExprKindGrouping:
            return format("({Sr})", to_String__Expr(*self.value.grouping));
        default:
            UNREACHABLE("unknown expr kind");
    }
}

void
__free__ExprIdentifierAccess(struct Expr *self)
{
    for (Usize i = len__Vec(*self->value.identifier_access); i--;)
        FREE(ExprAll, get__Vec(*self->value.identifier_access, i));

    FREE(Vec, self->value.identifier_access);
    free(self);
}

void
__free__ExprGlobalAccess(struct Expr *self)
{
    for (Usize i = len__Vec(*self->value.global_access); i--;)
        FREE(ExprAll, get__Vec(*self->value.global_access, i));

    FREE(Vec, self->value.global_access);
    free(self);
}

void
__free__ExprPropertyAccessInit(struct Expr *self)
{
    for (Usize i = len__Vec(*self->value.property_access_init); i--;)
        FREE(ExprAll, get__Vec(*self->value.property_access_init, i));

    FREE(Vec, self->value.property_access_init);
    free(self);
}

void
__free__ExprArray(struct Expr *self)
{
    for (Usize i = len__Vec(*self->value.array); i--;)
        FREE(ExprAll, get__Vec(*self->value.array, i));

    FREE(Vec, self->value.array);
    free(self);
}

void
__free__ExprTuple(struct Expr *self)
{
    for (Usize i = len__Vec(*self->value.tuple); i--;)
        FREE(ExprAll, get__Vec(*self->value.tuple, i));

    FREE(Vec, self->value.tuple);
    free(self);
}

void
__free__ExprTry(struct Expr *self)
{
    FREE(ExprAll, self->value.try);
    free(self);
}

void
__free__ExprIf(struct Expr *self)
{
    FREE(IfCond, self->value.if_);
    free(self);
}

void
__free__ExprBlock(struct Expr *self)
{
    for (Usize i = len__Vec(*self->value.block); i--;)
        FREE(FunBodyItemAll, get__Vec(*self->value.block, i));

    FREE(Vec, self->value.block);
    free(self);
}

void
__free__ExprQuestionMark(struct Expr *self)
{
    FREE(ExprAll, self->value.question_mark);
    free(self);
}

void
__free__ExprDereference(struct Expr *self)
{
    FREE(ExprAll, self->value.dereference);
    free(self);
}

void
__free__ExprRef(struct Expr *self)
{
    FREE(ExprAll, self->value.ref);
    free(self);
}

void
__free__ExprGrouping(struct Expr *self)
{
    FREE(ExprAll, self->value.grouping);
    free(self);
}

void
__free__ExprAll(struct Expr *self)
{
    switch (self->kind) {
        case ExprKindUnaryOp:
            FREE(ExprUnaryOp, self);
            break;
        case ExprKindBinaryOp:
            FREE(ExprBinaryOp, self);
            break;
        case ExprKindFunCall:
            FREE(ExprFunCall, self);
            break;
        case ExprKindRecordCall:
            FREE(ExprRecordCall, self);
            break;
        case ExprKindIdentifier:
            FREE(ExprIdentifier, self);
            break;
        case ExprKindIdentifierAccess:
            FREE(ExprIdentifierAccess, self);
            break;
        case ExprKindGlobalAccess:
            FREE(ExprGlobalAccess, self);
            break;
        case ExprKindPropertyAccessInit:
            FREE(ExprPropertyAccessInit, self);
            break;
        case ExprKindArrayAccess:
            FREE(ExprArrayAccess, self);
            break;
        case ExprKindTupleAccess:
            FREE(ExprTupleAccess, self);
            break;
        case ExprKindLambda:
            FREE(ExprLambda, self);
            break;
        case ExprKindArray:
            FREE(ExprArray, self);
            break;
        case ExprKindTuple:
            FREE(ExprTuple, self);
            break;
        case ExprKindVariant:
            FREE(ExprVariant, self);
            break;
        case ExprKindTry:
            FREE(ExprTry, self);
            break;
        case ExprKindIf:
            FREE(ExprIf, self);
            break;
        case ExprKindBlock:
            FREE(ExprBlock, self);
            break;
        case ExprKindQuestionMark:
            FREE(ExprQuestionMark, self);
            break;
        case ExprKindDereference:
            FREE(ExprDereference, self);
            break;
        case ExprKindRef:
            FREE(ExprRef, self);
            break;
        case ExprKindLiteral:
            FREE(ExprLiteral, self);
            break;
        case ExprKindVariable:
            FREE(ExprVariable, self);
            break;
        case ExprKindGrouping:
            FREE(ExprGrouping, self);
            break;
        default:
            FREE(Expr, self);
            break;
    }
}

struct MatchStmt *
__new__MatchStmt(struct Expr *matching, struct Vec *pattern)
{
    struct MatchStmt *self = malloc(sizeof(struct MatchStmt));
    self->matching = matching;
    self->pattern = pattern;
    return self;
}

struct String *
to_String__MatchStmt(struct MatchStmt self)
{
    struct String *s = NEW(String);

    append__String(
      s, format("match {Sr}\n", to_String__Expr(*self.matching)), true);

    ++current_tab_size;

    for (Usize i = 0; i < len__Vec(*self.pattern); i++) {
        struct Expr *temp_matched =
          ((struct Tuple *)get__Vec(*self.pattern, i))->items[0];
        struct Expr *temp_cond =
          ((struct Tuple *)get__Vec(*self.pattern, i))->items[1];
        struct Expr *temp_expr =
          ((struct Tuple *)get__Vec(*self.pattern, i))->items[2];

        if (temp_cond)
            append__String(s,
                           format("{Sr}{Sr} ? {Sr} => {Sr},\n",
                                  repeat__String("\t", current_tab_size),
                                  to_String__Expr(*temp_matched),
                                  to_String__Expr(*temp_cond),
                                  to_String__Expr(*temp_expr)),
                           true);
        else
            append__String(s,
                           format("{Sr}{Sr} => {Sr},\n",
                                  repeat__String("\t", current_tab_size),
                                  to_String__Expr(*temp_matched),
                                  to_String__Expr(*temp_expr)),
                           true);
    }

    --current_tab_size;

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__MatchStmt(struct MatchStmt *self)
{
    FREE(ExprAll, self->matching);

    for (Usize i = len__Vec(*self->pattern); i--;) {
        FREE(ExprAll, ((struct Tuple *)get__Vec(*self->pattern, i))->items[0]);
        FREE(ExprAll, ((struct Tuple *)get__Vec(*self->pattern, i))->items[2]);

        if (((struct Tuple *)get__Vec(*self->pattern, i))->items[1])
            FREE(ExprAll,
                 ((struct Tuple *)get__Vec(*self->pattern, i))->items[1]);

        if ((struct Tuple *)get__Vec(*self->pattern, i))
            FREE(Tuple, ((struct Tuple *)get__Vec(*self->pattern, i)));
    }

    FREE(Vec, self->pattern);
    free(self);
}

struct IfBranch *
__new__IfBranch(struct Expr *cond, struct Vec *body)
{
    struct IfBranch *self = malloc(sizeof(struct IfBranch));
    self->cond = cond;
    self->body = body;
    return self;
}

struct String *
to_String__IfBranch(struct IfBranch self)
{
    TODO("IfBranch");
}

void
__free__IfBranch(struct IfBranch *self)
{
    FREE(ExprAll, self->cond);

    for (Usize i = len__Vec(*self->body); i--;)
        FREE(FunBodyItemAll, get__Vec(*self->body, i));

    FREE(Vec, self->body);
    free(self);
}

struct IfCond *
__new__IfCond(struct IfBranch *if_, struct Vec *elif, struct Vec *else_)
{
    struct IfCond *self = malloc(sizeof(struct IfCond));
    self->if_ = if_;
    self->elif = elif;
    self->else_ = else_;
    return self;
}

struct String *
to_String__IfCond(struct IfCond self)
{
    struct String *s = NEW(String);

    append__String(
      s, format("if {Sr} do\n", to_String__Expr(*self.if_->cond)), true);

    ++current_tab_size;

    for (Usize i = 0; i < len__Vec(*self.if_->body); i++)
        append__String(
          s,
          format("{Sr}",
                 to_String__FunBodyItem(
                   *CAST(get__Vec(*self.if_->body, i), struct FunBodyItem *))),
          true);

    --current_tab_size;

    if (self.elif) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.elif); i++) {
            append__String(
              s,
              format(
                "{Sr}elif {Sr} do\n",
                repeat__String("\t", current_tab_size - 1),
                to_String__Expr(
                  *(CAST(get__Vec(*self.elif, i), struct IfBranch *))->cond)),
              true);

            struct Vec *temp_body =
              CAST(get__Vec(*self.elif, i), struct IfBranch *)->body;

            for (Usize j = 0; j < len__Vec(*temp_body); j++)
                append__String(
                  s,
                  format("{Sr}",
                         to_String__FunBodyItem(*CAST(get__Vec(*temp_body, j),
                                                      struct FunBodyItem *))),
                  true);
        }

        --current_tab_size;
    }

    if (self.else_) {
        append__String(
          s,
          format("{Sr}else\n", repeat__String("\t", current_tab_size)),
          true);

        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.else_); i++)
            append__String(
              s,
              format("{Sr}",
                     to_String__FunBodyItem(
                       *CAST(get__Vec(*self.else_, i), struct FunBodyItem *))),
              true);

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__IfCond(struct IfCond *self)
{
    FREE(IfBranch, self->if_);

    if (self->elif) {
        for (Usize i = len__Vec(*self->elif); i--;)
            FREE(IfBranch, get__Vec(*self->elif, i));

        FREE(Vec, self->elif);
    }

    if (self->else_) {
        for (Usize i = len__Vec(*self->else_); i--;)
            FREE(FunBodyItemAll, get__Vec(*self->else_, i));

        FREE(Vec, self->else_);
    }

    free(self);
}

struct TryStmt *
__new__TryStmt(struct Vec *try_body,
               struct Expr *catch_expr,
               struct Vec *catch_body)
{
    struct TryStmt *self = malloc(sizeof(struct TryStmt));
    self->try_body = try_body;
    self->catch_expr = catch_expr;
    self->catch_body = catch_body;
    return self;
}

struct String *
to_String__TryStmt(struct TryStmt self)
{
    struct String *s = NEW(String);

    push_str__String(s, "try do\n");

    ++current_tab_size;

    for (Usize i = 0; i < len__Vec(*self.try_body); i++)
        append__String(
          s,
          format("{Sr}\n",
                 to_String__FunBodyItem(
                   *CAST(get__Vec(*self.try_body, i), struct FunBodyItem *))),
          true);

    --current_tab_size;

    if (self.catch_expr && self.catch_body) {
        append__String(s,
                       format("{Sr}catch {Sr} do\n",
                              repeat__String("\t", current_tab_size),
                              to_String__Expr(*self.catch_expr)),
                       true);

        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.catch_body); i++)
            append__String(
              s,
              format("{Sr}\n",
                     to_String__FunBodyItem(*CAST(get__Vec(*self.catch_body, i),
                                                  struct FunBodyItem *))),
              true);

        --current_tab_size;
    } else if ((!self.catch_expr || !self.catch_body) &&
               (self.catch_expr || self.catch_body))
        UNREACHABLE("catch expr and catch body must equal to Some");

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__TryStmt(struct TryStmt *self)
{
    for (Usize i = len__Vec(*self->try_body); i--;)
        FREE(FunBodyItemAll, get__Vec(*self->try_body, i));

    FREE(Vec, self->try_body);

    if (self->catch_expr)
        FREE(ExprAll, self->catch_expr);

    if (self->catch_body) {
        for (Usize i = len__Vec(*self->catch_body); i--;)
            FREE(FunBodyItemAll, get__Vec(*self->catch_body, i));

        FREE(Vec, self->catch_body);
    }

    free(self);
}

struct WhileStmt *
__new__WhileStmt(struct Expr *cond, struct Vec *body)
{
    struct WhileStmt *self = malloc(sizeof(struct WhileStmt));
    self->cond = cond;
    self->body = body;
    return self;
}

struct String *
to_String__WhileStmt(struct WhileStmt self)
{
    struct String *s = NEW(String);

    append__String(
      s, format("while {Sr} do\n", to_String__Expr(*self.cond)), true);

    for (Usize i = 0; i < len__Vec(*self.body); i++)
        append__String(
          s,
          format("\t{Sr}\n",
                 to_String__FunBodyItem(
                   *CAST(get__Vec(*self.body, i), struct FunBodyItem *))),
          true);

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__WhileStmt(struct WhileStmt *self)
{
    FREE(ExprAll, self->cond);

    for (Usize i = len__Vec(*self->body); i--;)
        FREE(FunBodyItemAll, get__Vec(*self->body, i));

    FREE(Vec, self->body);
    free(self);
}

struct ForStmtExprTraditional *
__new__ForStmtExprTraditional(struct Expr *var,
                              struct Expr *cond,
                              struct Expr *action)
{
    struct ForStmtExprTraditional *self =
      malloc(sizeof(struct ForStmtExprTraditional));
    self->var = var;
    self->cond = cond;
    self->action = action;
    return self;
}

void
__free__ForStmtExprTraditional(struct ForStmtExprTraditional *self)
{
    if (self->var)
        FREE(ExprAll, self->var);

    if (self->cond)
        FREE(ExprAll, self->cond);

    if (self->action)
        FREE(ExprAll, self->action);

    free(self);
}

struct ForStmtExpr *
__new__ForStmtExprRange(struct Tuple *range, struct Location loc)
{
    struct ForStmtExpr *self = malloc(sizeof(struct ForStmtExpr));
    self->kind = ForStmtExprKindRange;
    self->loc = loc;
    self->value.range = range;
    return self;
}

struct ForStmtExpr *
__new__ForStmtExprTraditionalVar(struct ForStmtExprTraditional *traditional,
                                 struct Location loc)
{
    struct ForStmtExpr *self = malloc(sizeof(struct ForStmtExpr));
    self->kind = ForStmtExprKindTraditional;
    self->loc = loc;
    self->value.traditional = traditional;
    return self;
}

struct String *
to_String__ForStmtExpr(struct ForStmtExpr self)
{
    switch (self.kind) {
        case ForStmtExprKindRange:
            return format(
              "for {Sr} in {Sr} do\n",
              to_String__Expr(*CAST(self.value.range->items[0], struct Expr *)),
              to_String__Expr(
                *CAST(self.value.range->items[1], struct Expr *)));
        case ForStmtExprKindTraditional: {
            struct String *s = NEW(String);

            push_str__String(s, "for ");

            if (self.value.traditional->var) {
                append__String(
                  s,
                  format("{Sr}, ",
                         to_String__Expr(*self.value.traditional->var)),
                  true);
            } else
                push_str__String(s, ",");

            if (self.value.traditional->cond) {
                append__String(
                  s,
                  format("{Sr}, ",
                         to_String__Expr(*self.value.traditional->cond)),
                  true);
            } else
                push_str__String(s, ",");

            if (self.value.traditional->action) {
                append__String(
                  s,
                  format("{Sr}",
                         to_String__Expr(*self.value.traditional->action)),
                  true);
            }

            push_str__String(s, " do\n");

            return s;
        }
        default:
            UNREACHABLE("unknown for expr kind");
    }
}

void
__free__ForStmtExprRange(struct ForStmtExpr *self)
{
    FREE(ExprAll, self->value.range->items[0]);
    FREE(ExprAll, self->value.range->items[1]);
    FREE(Tuple, self->value.range);
    free(self);
}

void
__free__ForStmtExprTraditionalVar(struct ForStmtExpr *self)
{
    FREE(ForStmtExprTraditional, self->value.traditional);
    free(self);
}

void
__free__ForStmtExprAll(struct ForStmtExpr *self)
{
    switch (self->kind) {
        case ForStmtExprKindRange:
            FREE(ForStmtExprRange, self);
            break;
        case ForStmtExprKindTraditional:
            FREE(ForStmtExprTraditionalVar, self);
            break;
        default:
            UNREACHABLE("unknown for expr");
    }
}

struct ForStmt *
__new__ForStmt(struct ForStmtExpr *expr, struct Vec *body)
{
    struct ForStmt *self = malloc(sizeof(struct ForStmt));
    self->expr = expr;
    self->body = body;
    return self;
}

struct String *
to_String__ForStmt(struct ForStmt self)
{
    struct String *s = NEW(String);

    append__String(s, to_String__ForStmtExpr(*self.expr), true);

    for (Usize i = 0; i < len__Vec(*self.body); i++)
        append__String(
          s,
          format("\t{Sr}",
                 to_String__FunBodyItem(
                   *CAST(get__Vec(*self.body, i), struct FunBodyItem *))),
          true);

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__ForStmt(struct ForStmt *self)
{
    FREE(ForStmtExprAll, self->expr);

    for (Usize i = len__Vec(*self->body); i--;)
        FREE(FunBodyItemAll, get__Vec(*self->body, i));

    FREE(Vec, self->body);
    free(self);
}

struct ImportStmtValue *
__new__ImportStmtValueAccess(struct String *access)
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindAccess;
    self->value.access = access;
    return self;
}

struct ImportStmtValue *
__new__ImportStmtValueSelector(struct Vec *selector)
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindSelector;
    self->value.selector = selector;
    return self;
}

struct String *
to_String__ImportStmtValue(struct ImportStmtValue self)
{
    switch (self.kind) {
        case ImportStmtValueKindStd:
            return from__String("@std");
        case ImportStmtValueKindCore:
            return from__String("@core");
        case ImportStmtValueKindBuiltin:
            return from__String("@builtin");
        case ImportStmtValueKindFile:
            return format("@file({S})", self.value.file);
        case ImportStmtValueKindUrl:
            return format("@url({S})", self.value.url);
        case ImportStmtValueKindAccess:
            return format("{S}", self.value.access);
        case ImportStmtValueKindSelector: {
            struct String *s = NEW(String);

            push_str__String(s, "{");

            bool is_last = false;

            for (Usize i = 0; i < len__Vec(*self.value.selector) - 1; i++) {
                struct Vec *temp;

            print_stmt_value : {
                temp = !is_last ? get__Vec(*self.value.selector, i)
                                : get__Vec(*self.value.selector,
                                           len__Vec(*self.value.selector) - 1);

                for (Usize j = 0; j < len__Vec(*temp) - 1; j++)
                    append__String(
                      s,
                      format("{Sr}.",
                             to_String__ImportStmtValue(*CAST(
                               get__Vec(*temp, j), struct ImportStmtValue *))),
                      true);
            }

                append__String(s,
                               format("{Sr}",
                                      to_String__ImportStmtValue(*CAST(
                                        get__Vec(*temp, len__Vec(*temp) - 1),
                                        struct ImportStmtValue *))),
                               true);

                if (!is_last)
                    push_str__String(s, ", ");
                else
                    goto exit;
            }

            is_last = true;

            if (is_last)
                goto print_stmt_value;

        exit : {
            push_str__String(s, "}");
        }

            return s;
        }
        case ImportStmtValueKindWildcard:
            return from__String("*");
    }

    return NULL;
}

void
__free__ImportStmtValueAccess(struct ImportStmtValue *self)
{
    FREE(String, self->value.access);
    free(self);
}

void
__free__ImportStmtValueSelector(struct ImportStmtValue *self)
{
    for (Usize i = len__Vec(*self->value.selector); i--;) {
        struct Vec *temp = get__Vec(*self->value.selector, i);

        for (Usize j = len__Vec(*temp); j--;)
            FREE(ImportStmtValueAll, get__Vec(*temp, j));

        FREE(Vec, temp);
    }

    FREE(Vec, self->value.selector);
    free(self);
}

void
__free__ImportStmtValueFile(struct ImportStmtValue *self)
{
    FREE(String, self->value.file);
    free(self);
}

void
__free__ImportStmtValueUrl(struct ImportStmtValue *self)
{
    FREE(String, self->value.url);
    free(self);
}

void
__free__ImportStmtValueAll(struct ImportStmtValue *self)
{
    switch (self->kind) {
        case ImportStmtValueKindAccess:
            FREE(ImportStmtValueAccess, self);
            break;
        case ImportStmtValueKindSelector:
            FREE(ImportStmtValueSelector, self);
            break;
        case ImportStmtValueKindCore:
        case ImportStmtValueKindStd:
        case ImportStmtValueKindBuiltin:
        case ImportStmtValueKindWildcard:
            free(self);
            break;
        case ImportStmtValueKindFile:
            FREE(ImportStmtValueFile, self);
            break;
        case ImportStmtValueKindUrl:
            FREE(ImportStmtValueUrl, self);
            break;
        default:
            UNREACHABLE("unknown import stmt value");
    }
}

struct ImportStmt *
__new__ImportStmt(struct Vec *import_value, bool is_pub, struct String *as)
{
    struct ImportStmt *self = malloc(sizeof(struct ImportStmt));
    self->import_value = import_value;
    self->is_pub = is_pub;
    self->as = as;
    return self;
}

struct String *
to_String__ImportStmt(struct ImportStmt self)
{
    struct String *s = NEW(String);

    if (self.is_pub)
        push_str__String(s, "pub import \"");
    else
        push_str__String(s, "import \"");

    for (Usize i = 0; i < len__Vec(*self.import_value) - 1; i++)
        append__String(
          s,
          format("{Sr}.",
                 to_String__ImportStmtValue(*CAST(
                   get__Vec(*self.import_value, i), struct ImportStmtValue *))),
          true);

    append__String(
      s,
      format("{Sr}\"",
             to_String__ImportStmtValue(*CAST(
               get__Vec(*self.import_value, len__Vec(*self.import_value) - 1),
               struct ImportStmtValue *))),
      true);

    if (self.as)
        append__String(s, format(" as {S}", self.as), true);

    return s;
}

void
__free__ImportStmt(struct ImportStmt *self)
{
    for (Usize i = len__Vec(*self->import_value); i--;)
        FREE(ImportStmtValueAll, get__Vec(*self->import_value, i));

    FREE(Vec, self->import_value);

    free(self);
}

struct Stmt *
__new__Stmt(enum StmtKind kind, struct Location loc)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = kind;
    self->loc = loc;
    return self;
}

struct Stmt *
__new__StmtReturn(struct Location loc, struct Expr *return_)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindReturn;
    self->loc = loc;
    self->value.return_ = return_;
    return self;
}

struct Stmt *
__new__StmtIf(struct Location loc, struct IfCond *if_)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindIf;
    self->loc = loc;
    self->value.if_ = if_;
    return self;
}

struct Stmt *
__new__StmtAwait(struct Location loc, struct Expr *await)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindAwait;
    self->loc = loc;
    self->value.await = await;
    return self;
}

struct Stmt *
__new__StmtTry(struct Location loc, struct TryStmt *try)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindTry;
    self->loc = loc;
    self->value.try = try;
    return self;
}

struct Stmt *
__new__StmtMatch(struct Location loc, struct MatchStmt *match)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindMatch;
    self->loc = loc;
    self->value.match = match;
    return self;
}

struct Stmt *
__new__StmtWhile(struct Location loc, struct WhileStmt *while_)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindWhile;
    self->loc = loc;
    self->value.while_ = while_;
    return self;
}

struct Stmt *
__new__StmtFor(struct Location loc, struct ForStmt *for_)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindFor;
    self->loc = loc;
    self->value.for_ = for_;
    return self;
}

struct Stmt *
__new__StmtImport(struct Location loc, struct ImportStmt *import)
{
    struct Stmt *self = malloc(sizeof(struct Stmt));
    self->kind = StmtKindImport;
    self->loc = loc;
    self->value.import = import;
    return self;
}

struct String *
to_String__Stmt(struct Stmt self)
{
    switch (self.kind) {
        case StmtKindReturn:
            return format("return {Sr}", to_String__Expr(*self.value.return_));
        case StmtKindIf:
            return to_String__IfCond(*self.value.if_);
        case StmtKindAwait:
            return format("await {Sr}", to_String__Expr(*self.value.await));
        case StmtKindTry:
            return to_String__TryStmt(*self.value.try);
        case StmtKindMatch:
            return to_String__MatchStmt(*self.value.match);
        case StmtKindWhile:
            return to_String__WhileStmt(*self.value.while_);
        case StmtKindFor:
            return to_String__ForStmt(*self.value.for_);
        case StmtKindNext:
            return from__String("next");
        case StmtKindBreak:
            return from__String("break");
        case StmtKindImport:
            return to_String__ImportStmt(*self.value.import);
        default:
            UNREACHABLE("unknown stmt kind");
    }
}

void
__free__StmtAll(struct Stmt *self)
{
    switch (self->kind) {
        case StmtKindReturn:
            FREE(StmtReturn, self);
            break;
        case StmtKindIf:
            FREE(StmtIf, self);
            break;
        case StmtKindAwait:
            FREE(StmtAwait, self);
            break;
        case StmtKindTry:
            FREE(StmtTry, self);
            break;
        case StmtKindMatch:
            FREE(StmtMatch, self);
            break;
        case StmtKindWhile:
            FREE(StmtWhile, self);
            break;
        case StmtKindFor:
            FREE(StmtFor, self);
            break;
        case StmtKindImport:
            FREE(StmtImport, self);
            break;
        default:
            FREE(Stmt, self);
            break;
    }
}

struct FunBodyItem *
__new__FunBodyItemExpr(struct Expr *expr)
{
    struct FunBodyItem *self = malloc(sizeof(struct FunBodyItem));
    self->kind = FunBodyItemKindExpr;
    self->expr = expr;
    return self;
}

struct FunBodyItem *
__new__FunBodyItemStmt(struct Stmt *stmt)
{
    struct FunBodyItem *self = malloc(sizeof(struct FunBodyItem));
    self->kind = FunBodyItemKindStmt;
    self->stmt = stmt;
    return self;
}

struct String *
to_String__FunBodyItem(struct FunBodyItem self)
{
    switch (self.kind) {
        case FunBodyItemKindExpr:
            return format("{Sr}{Sr}\n",
                          repeat__String("\t", current_tab_size),
                          to_String__Expr(*self.expr));
        case FunBodyItemKindStmt:
            return format("{Sr}{Sr}\n",
                          repeat__String("\t", current_tab_size),
                          to_String__Stmt(*self.stmt));
        default:
            UNREACHABLE("unknown fun body item kind");
    }

    return from__String("unknown fun body item kind");
}

void
__free__FunBodyItemAll(struct FunBodyItem *self)
{
    switch (self->kind) {
        case FunBodyItemKindExpr:
            FREE(FunBodyItemExpr, self);
            break;
        case FunBodyItemKindStmt:
            FREE(FunBodyItemStmt, self);
            break;
    }
}

struct FunParamCall *
__new__FunParamCall(struct Expr *value)
{
    struct FunParamCall *self = malloc(sizeof(struct FunParamCall));
    self->kind = FunParamKindNormal;
    self->value = value;
    return self;
}

struct FunParamCall *
__new__FunParamCallDefault(struct Expr *value, struct String *name)
{
    struct FunParamCall *self = malloc(sizeof(struct FunParamCall));
    self->kind = FunParamKindDefault;
    self->value = value;
    self->name = name;
    return self;
}

struct String *
to_String__FunParamCall(struct FunParamCall self)
{
    switch (self.kind) {
        case FunParamKindNormal:
            return format("{Sr}", to_String__Expr(*self.value));
        case FunParamKindDefault:
            return format(
              "{S} = {Sr}", self.name, to_String__Expr(*self.value));
        case FunParamKindSelf:
            return from__String("self");
        default:
            UNREACHABLE("unknown fun param kind");
    }
}

void
__free__FunParamCallAll(struct FunParamCall *self)
{
    switch (self->kind) {
        case FunParamKindDefault:
            FREE(FunParamCallDefault, self);
            break;
        case FunParamKindNormal:
            FREE(FunParamCall, self);
            break;
        default:
            UNREACHABLE("unknown fun param kind");
    }
}

struct FunParam *
__new__FunParamDefault(struct String *name,
                       struct String *super_tag_name,
                       struct Tuple *param_data_type,
                       struct Location loc,
                       struct Expr *default_)
{
    struct FunParam *self = malloc(sizeof(struct FunParam));
    self->kind = FunParamKindDefault;
    self->name = name;
    self->super_tag.name = super_tag_name;
    self->param_data_type = param_data_type;
    self->loc = loc;
    self->value.default_ = default_;
    return self;
}

struct FunParam *
__new__FunParamNormal(struct String *name,
                      struct String *super_tag_name,
                      struct Tuple *param_data_type,
                      struct Location loc)
{
    struct FunParam *self = malloc(sizeof(struct FunParam));
    self->kind = FunParamKindNormal;
    self->name = name;
    self->super_tag.name = super_tag_name;
    self->param_data_type = param_data_type;
    self->loc = loc;
    return self;
}

struct FunParam *
__new__FunParamSelf(struct Location loc)
{
    struct FunParam *self = malloc(sizeof(struct FunParam));
    self->kind = FunParamKindSelf;
    self->param_data_type = NULL;
    self->loc = loc;
    return self;
}

struct String *
to_String__FunParam(struct FunParam self)
{
    switch (self.kind) {
        case FunParamKindDefault: {
            struct String *s = NEW(String);

            if (self.param_data_type)
                if (self.super_tag.name)
                    append__String(
                      s,
                      format(
                        "{S}#{S} {Sr} := {Sr}",
                        self.name,
                        self.super_tag.name,
                        to_String__DataType(*CAST(
                          self.param_data_type->items[0], struct DataType *)),
                        to_String__Expr(*self.value.default_)),
                      true);
                else
                    append__String(
                      s,
                      format(
                        "{S} {Sr} := {Sr}",
                        self.name,
                        to_String__DataType(*CAST(
                          self.param_data_type->items[0], struct DataType *)),
                        to_String__Expr(*self.value.default_)),
                      true);
            else if (self.super_tag.name)
                append__String(s,
                               format("{S}#{S} := {Sr}",
                                      self.name,
                                      self.super_tag.name,
                                      to_String__Expr(*self.value.default_)),
                               true);
            else
                append__String(s,
                               format("{S} := {Sr}",
                                      self.name,
                                      to_String__Expr(*self.value.default_)),
                               true);

            return s;
        }
        case FunParamKindNormal: {
            struct String *s = NEW(String);

            if (self.param_data_type)
                if (self.super_tag.name)
                    append__String(s,
                                   format("{S}#{S} {Sr}",
                                          self.name,
                                          self.super_tag.name,
                                          to_String__DataType(*CAST(
                                            self.param_data_type->items[0],
                                            struct DataType *))),
                                   true);
                else
                    append__String(s,
                                   format("{S} {Sr}",
                                          self.name,
                                          to_String__DataType(*CAST(
                                            self.param_data_type->items[0],
                                            struct DataType *))),
                                   true);
            else if (self.super_tag.name)
                append__String(
                  s, format("{S}#{S}", self.name, self.super_tag.name), true);
            else
                append__String(s, self.name, false);

            return s;
        }
        case FunParamKindSelf:
            return from__String("self");
    }

    return NULL;
}

void
__free__FunParamDefault(struct FunParam *self)
{
    if (self->param_data_type) {
        FREE(DataTypeAll, self->param_data_type->items[0]);
        free(self->param_data_type->items[1]);
        FREE(Tuple, self->param_data_type);
    }

    FREE(ExprAll, self->value.default_);
    free(self);
}

void
__free__FunParamNormal(struct FunParam *self)
{
    if (self->param_data_type) {
        FREE(DataTypeAll, self->param_data_type->items[0]);
        free(self->param_data_type->items[1]);
        FREE(Tuple, self->param_data_type);
    }

    free(self);
}

void
__free__FunParamSelf(struct FunParam *self)
{
    free(self);
}

void
__free__FunParamAll(struct FunParam *self)
{
    switch (self->kind) {
        case FunParamKindNormal:
            FREE(FunParamNormal, self);
            break;
        case FunParamKindDefault:
            FREE(FunParamDefault, self);
            break;
        case FunParamKindSelf:
            FREE(FunParamSelf, self);
            break;
        default:
            UNREACHABLE("unknown fun param kind");
    }
}

struct FunDecl *
__new__FunDecl(struct String *name,
               struct Vec *tags,
               struct Vec *generic_params,
               struct Vec *params,
               struct Tuple *return_type,
               struct Vec *body,
               bool is_pub,
               bool is_async)
{
    struct FunDecl *self = malloc(sizeof(struct FunDecl));
    self->name = name;
    self->tags = tags;
    self->generic_params = generic_params;
    self->params = params;
    self->return_type = return_type;
    self->body = body;
    self->is_pub = is_pub;
    self->is_async = is_async;
    return self;
}

struct String *
to_String__FunDecl(struct FunDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub && self.is_async)
        push_str__String(s, "pub async fun");
    else if (self.is_pub)
        push_str__String(s, "pub fun");
    else if (self.is_async)
        push_str__String(s, "async fun");
    else
        push_str__String(s, "fun");

    if (self.tags) {
        push_str__String(s, "#(");

        for (Usize i = 0; i < len__Vec(*self.tags) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__DataType(*CAST(
                       CAST(get__Vec(*self.tags, i), struct Tuple *)->items[0],
                       struct DataType *))),
              true);

        append__String(
          s,
          format("{Sr})",
                 to_String__DataType(
                   *CAST(CAST(get__Vec(*self.tags, len__Vec(*self.tags) - 1),
                              struct Tuple *)
                           ->items[0],
                         struct DataType *))),
          true);
    }

    append__String(s, format(" {S}", self.name), true);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    if (self.params) {
        push_str__String(s, "(");

        if (len__Vec(*self.params) > 0) {
            for (Usize i = 0; i < len__Vec(*self.params) - 1; i++)
                append__String(
                  s,
                  format("{Sr}, ",
                         to_String__FunParam(*CAST(get__Vec(*self.params, i),
                                                   struct FunParam *))),
                  true);

            append__String(
              s,
              format("{Sr})",
                     to_String__FunParam(
                       *CAST(get__Vec(*self.params, len__Vec(*self.params) - 1),
                             struct FunParam *))),
              true);
        } else
            push_str__String(s, ")");
    }

    if (self.return_type)
        append__String(
          s,
          format(" {Sr} =\n",
                 to_String__DataType(
                   *CAST(self.return_type->items[0], struct DataType *))),
          true);
    else
        push_str__String(s, " =\n");

    if (self.body) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.body) - 1; i++)
            append__String(
              s,
              format("{Sr}",
                     to_String__FunBodyItem(
                       *CAST(get__Vec(*self.body, i), struct FunBodyItem *))),
              true);

        append__String(s,
                       format("{Sr}",
                              to_String__FunBodyItem(*CAST(
                                get__Vec(*self.body, len__Vec(*self.body) - 1),
                                struct FunBodyItem *))),
                       true);

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__FunDecl(struct FunDecl *self)
{
    if (self->tags) {
        for (Usize i = len__Vec(*self->tags); i--;) {
            struct Tuple *temp = get__Vec(*self->tags, i);
            FREE(DataTypeAll, temp->items[0]);
            free(temp->items[1]);
            FREE(Tuple, temp);
        }

        FREE(Vec, self->tags);
    }

    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        for (Usize i = len__Vec(*self->params); i--;)
            FREE(FunParamAll, get__Vec(*self->params, i));

        FREE(Vec, self->params);
    }

    if (self->return_type) {
        FREE(DataTypeAll, self->return_type->items[0]);
        free(self->return_type->items[1]);
        FREE(Tuple, self->return_type);
    }

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(FunBodyItemAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    free(self);
}

struct ConstantDecl *
__new__ConstantDecl(struct String *name,
                    struct DataType *data_type,
                    struct Expr *expr,
                    bool is_pub)
{
    struct ConstantDecl *self = malloc(sizeof(struct ConstantDecl));
    self->name = name;
    self->data_type = data_type;
    self->expr = expr;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__ConstantDecl(struct ConstantDecl self)
{
    struct String *s = NEW(String);

    if (self.is_pub)
        append__String(s,
                       format("{Sr}pub {S}",
                              repeat__String("\t", current_tab_size),
                              self.name),
                       true);
    else
        append__String(
          s,
          format("{Sr}{S}", repeat__String("\t", current_tab_size), self.name),
          true);

    if (self.data_type)
        append__String(
          s, format(" :: {Sr}", to_String__DataType(*self.data_type)), true);

    append__String(s, format(" := {Sr};", to_String__Expr(*self.expr)), true);

    return s;
}

void
__free__ConstantDecl(struct ConstantDecl *self)
{
    if (self->data_type)
        FREE(DataTypeAll, self->data_type);

    FREE(ExprAll, self->expr);
    free(self);
}

struct ModuleBodyItem *
__new__ModuleBodyItemDecl(struct Decl *decl)
{
    struct ModuleBodyItem *self = malloc(sizeof(struct ModuleBodyItem));
    self->kind = ModuleBodyItemKindDecl;
    self->value.decl = decl;
    return self;
}

struct ModuleBodyItem *
__new__ModuleBodyItemImport(struct Tuple *import)
{
    struct ModuleBodyItem *self = malloc(sizeof(struct ModuleBodyItem));
    self->kind = ModuleBodyItemKindImport;
    self->value.import = import;
    return self;
}

struct String *
to_String__ModuleBodyItem(struct ModuleBodyItem self)
{
    switch (self.kind) {
        case ModuleBodyItemKindDecl:
            return format("{Sr}\n", to_String__Decl(*self.value.decl));
        case ModuleBodyItemKindImport:
            return format("{Sr}\n",
                          to_String__ImportStmt(*CAST(
                            self.value.import->items[0], struct ImportStmt *)));
        case ModuleBodyItemKindInclude:
            TODO("include");
    }

    return NULL;
}

void
__free__ModuleBodyItemDecl(struct ModuleBodyItem *self)
{
    FREE(DeclAll, self->value.decl);
    free(self);
}

void
__free__ModuleBodyItemImport(struct ModuleBodyItem *self)
{
    FREE(ImportStmt, self->value.import->items[0]);
    FREE(Tuple, self->value.import);
    free(self);
}

void
__free__ModuleBodyItemAll(struct ModuleBodyItem *self)
{
    switch (self->kind) {
        case ModuleBodyItemKindDecl:
            FREE(ModuleBodyItemDecl, self);
            break;
        case ModuleBodyItemKindImport:
            FREE(ModuleBodyItemImport, self);
            break;
        default:
            UNREACHABLE("unknown module body item kind");
    }
}

struct ModuleDecl *
__new__ModuleDecl(struct String *name, struct Vec *body, bool is_pub)
{
    struct ModuleDecl *self = malloc(sizeof(struct ModuleDecl));
    self->name = name;
    self->body = body;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__ModuleDecl(struct ModuleDecl self)
{
    struct String *s = NEW(String);

    if (self.is_pub)
        append__String(s,
                       format("{Sr}pub module {S} =\n",
                              repeat__String("\t", current_tab_size),
                              self.name),
                       true);
    else
        append__String(s,
                       format("{Sr}module {S} =\n",
                              repeat__String("\t", current_tab_size),
                              self.name),
                       true);

    if (self.body) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.body); i++) {
            append__String(s,
                           to_String__ModuleBodyItem(*CAST(
                             get__Vec(*self.body, i), struct ModuleBodyItem *)),
                           true);
        }

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__ModuleDecl(struct ModuleDecl *self)
{
    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(ModuleBodyItemAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    free(self);
}

struct AliasDecl *
__new__AliasDecl(struct String *name,
                 struct Vec *generic_params,
                 struct DataType *data_type,
                 bool is_pub)
{
    struct AliasDecl *self = malloc(sizeof(struct AliasDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->data_type = data_type;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__AliasDecl(struct AliasDecl self)
{
    struct String *s = NEW(String);

    if (self.is_pub)
        append__String(s,
                       format("{Sr}pub type {S}",
                              repeat__String("\t", current_tab_size),
                              self.name),
                       true);
    else
        append__String(s,
                       format("{Sr}type {S}",
                              repeat__String("\t", current_tab_size),
                              self.name),
                       true);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    append__String(
      s, format(": alias = {Sr};", to_String__DataType(*self.data_type)), true);

    return s;
}

void
__free__AliasDecl(struct AliasDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    FREE(DataTypeAll, self->data_type);
    free(self);
}

struct FieldRecord *
__new__FieldRecord(struct String *name,
                   struct DataType *data_type,
                   struct Expr *value,
                   bool is_pub,
                   struct Location loc)
{
    struct FieldRecord *self = malloc(sizeof(struct FieldRecord));
    self->name = name;
    self->data_type = data_type;
    self->value = value;
    self->is_pub = is_pub;
    self->loc = loc;
    return self;
}

struct String *
to_String__FieldRecord(struct FieldRecord self)
{
    struct String *s = NEW(String);

    if (self.is_pub)
        append__String(s,
                       format("{Sr}pub {S} {Sr}",
                              repeat__String("\t", current_tab_size),
                              self.name,
                              to_String__DataType(*self.data_type)),
                       true);
    else
        append__String(s,
                       format("{Sr}{S} {Sr}",
                              repeat__String("\t", current_tab_size),
                              self.name,
                              to_String__DataType(*self.data_type)),
                       true);

    if (self.value)
        append__String(
          s, format(" := {Sr}", to_String__Expr(*self.value)), true);

    return s;
}

void
__free__FieldRecord(struct FieldRecord *self)
{
    FREE(DataTypeAll, self->data_type);

    if (self->value)
        FREE(ExprAll, self->value);

    free(self);
}

struct RecordDecl *
__new__RecordDecl(struct String *name,
                  struct Vec *generic_params,
                  struct Vec *fields,
                  bool is_pub,
                  bool is_object)
{
    struct RecordDecl *self = malloc(sizeof(struct RecordDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->fields = fields;
    self->is_pub = is_pub;
    self->is_object = is_object;
    return self;
}

struct String *
to_String__RecordDecl(struct RecordDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub && self.is_object)
        push_str__String(s, "pub object ");
    else if (!self.is_pub && self.is_object)
        push_str__String(s, "object ");
    else if (self.is_pub && !self.is_object)
        push_str__String(s, "pub type ");
    else
        push_str__String(s, "type ");

    append__String(s, self.name, false);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    push_str__String(s, ": record =\n");

    if (self.fields) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.fields) - 1; i++)
            append__String(
              s,
              format("{Sr},\n",
                     to_String__FieldRecord(
                       *CAST(get__Vec(*self.fields, i), struct FieldRecord *))),
              true);

        append__String(
          s,
          format("{Sr}\n",
                 to_String__FieldRecord(
                   *CAST(get__Vec(*self.fields, len__Vec(*self.fields) - 1),
                         struct FieldRecord *))),
          true);

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__RecordDecl(struct RecordDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->fields) {
        for (Usize i = len__Vec(*self->fields); i--;)
            FREE(FieldRecord, get__Vec(*self->fields, i));

        FREE(Vec, self->fields);
    }

    free(self);
}

struct VariantEnum *
__new__VariantEnum(struct String *name,
                   struct DataType *data_type,
                   struct Location loc)
{
    struct VariantEnum *self = malloc(sizeof(struct VariantEnum));
    self->name = name;
    self->data_type = data_type;
    self->loc = loc;
    return self;
}

struct String *
to_String__VariantEnum(struct VariantEnum self)
{
    return format("{Sr}{S} {Sr}",
                  repeat__String("\t", current_tab_size),
                  self.name,
                  to_String__DataType(*self.data_type));
}

void
__free__VariantEnum(struct VariantEnum *self)
{
    if (self->data_type)
        FREE(DataTypeAll, self->data_type);

    free(self);
}

struct EnumDecl *
__new__EnumDecl(struct String *name,
                struct Vec *generic_params,
                struct Vec *variants,
                struct DataType *type_value,
                bool is_pub,
                bool is_object,
                bool is_error)
{
    struct EnumDecl *self = malloc(sizeof(struct EnumDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->variants = variants;
    self->type_value = type_value;
    self->is_pub = is_pub;
    self->is_object = is_object;
    self->is_error = is_error;
    return self;
}

struct String *
to_String__EnumDecl(struct EnumDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub && self.is_object)
        push_str__String(s, "pub object ");
    else if (!self.is_pub && self.is_object)
        push_str__String(s, "object ");
    else if (self.is_pub && !self.is_object)
        push_str__String(s, "pub type ");
    else
        push_str__String(s, "type ");

    append__String(s, self.name, false);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    if (self.type_value) {
        append__String(
          s,
          format(": enum({Sr}) =\n", to_String__DataType(*self.type_value)),
          true);
    } else if (self.is_error)
        push_str__String(s, ": enum(error) =\n");
    else
        push_str__String(s, ": enum =\n");

    if (self.variants) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.variants) - 1; i++)
            append__String(
              s,
              format("{Sr},\n",
                     to_String__VariantEnum(*CAST(get__Vec(*self.variants, i),
                                                  struct VariantEnum *))),
              true);

        append__String(
          s,
          format("{Sr}\n",
                 to_String__VariantEnum(
                   *CAST(get__Vec(*self.variants, len__Vec(*self.variants) - 1),
                         struct VariantEnum *))),
          true);

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__EnumDecl(struct EnumDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->variants) {
        for (Usize i = len__Vec(*self->variants); i--;)
            FREE(VariantEnum, get__Vec(*self->variants, i));

        FREE(Vec, self->variants);
    }

    if (self->type_value)
        FREE(DataTypeAll, self->type_value);

    free(self);
}

struct ErrorDecl *
__new__ErrorDecl(struct String *name,
                 struct Vec *generic_params,
                 struct DataType *data_type,
                 bool is_pub)
{
    struct ErrorDecl *self = malloc(sizeof(struct ErrorDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->data_type = data_type;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__ErrorDecl(struct ErrorDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub)
        push_str__String(s, "pub error ");
    else
        push_str__String(s, "error ");

    append__String(s, self.name, false);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    if (self.data_type)
        append__String(
          s, format(" {Sr};", to_String__DataType(*self.data_type)), true);
    else
        push_str__String(s, ";");

    return s;
}

void
__free__ErrorDecl(struct ErrorDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->data_type)
        FREE(DataTypeAll, self->data_type);

    free(self);
}

struct PropertyDecl *
__new__PropertyDecl(struct String *name,
                    struct DataType *data_type,
                    bool is_pub)
{
    struct PropertyDecl *self = malloc(sizeof(struct PropertyDecl));
    self->name = name;
    self->data_type = data_type;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__PropertyDecl(struct PropertyDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub)
        push_str__String(s, "pub ");

    append__String(
      s,
      format("@{S} {Sr};", self.name, to_String__DataType(*self.data_type)),
      true);

    return s;
}

void
__free__PropertyDecl(struct PropertyDecl *self)
{
    FREE(DataTypeAll, self->data_type);
    free(self);
}

struct MethodDecl *
__new__MethodDecl(struct String *name,
                  struct Vec *generic_params,
                  struct Vec *params,
                  struct DataType *return_type,
                  struct Vec *body,
                  bool has_first_self_param,
                  bool is_async,
                  bool is_pub)
{
    struct MethodDecl *self = malloc(sizeof(struct MethodDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->params = params;
    self->return_type = return_type;
    self->body = body;
    self->has_first_self_param = has_first_self_param;
    self->is_async = is_async;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__MethodDecl(struct MethodDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub && self.is_async)
        push_str__String(s, "pub async");
    else if (self.is_pub)
        push_str__String(s, "pub");
    else if (self.is_async)
        push_str__String(s, "async");

    if (!self.is_pub && !self.is_async)
        append__String(s, format("@{S}", self.name), true);
    else
        append__String(s, format(" @{S}", self.name), true);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    if (self.params) {
        push_str__String(s, "(");

        if (len__Vec(*self.params) > 0) {
            for (Usize i = 0; i < len__Vec(*self.params) - 1; i++)
                append__String(
                  s,
                  format("{Sr}, ",
                         to_String__FunParam(*CAST(get__Vec(*self.params, i),
                                                   struct FunParam *))),
                  true);

            append__String(
              s,
              format("{Sr})",
                     to_String__FunParam(
                       *CAST(get__Vec(*self.params, len__Vec(*self.params) - 1),
                             struct FunParam *))),
              true);
        }
    }

    if (self.return_type)
        append__String(
          s, format(" {Sr} =\n", to_String__DataType(*self.return_type)), true);
    else
        push_str__String(s, " =\n");

    if (self.body) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.body); i++)
            append__String(
              s,
              format("{Sr}",
                     to_String__FunBodyItem(
                       *CAST(get__Vec(*self.body, i), struct FunBodyItem *))),
              true);

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__MethodDecl(struct MethodDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        for (Usize i = len__Vec(*self->params); i--;)
            FREE(FunParamAll, get__Vec(*self->params, i));

        FREE(Vec, self->params);
    }

    if (self->return_type)
        FREE(DataTypeAll, self->return_type);

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(FunBodyItemAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    free(self);
}

struct ClassBodyItem *
__new__ClassBodyItemProperty(struct PropertyDecl *property, struct Location loc)
{
    struct ClassBodyItem *self = malloc(sizeof(struct ClassBodyItem));
    self->kind = ClassBodyItemKindProperty;
    self->loc = loc;
    self->value.property = property;
    return self;
}

struct ClassBodyItem *
__new__ClassBodyItemMethod(struct MethodDecl *method, struct Location loc)
{
    struct ClassBodyItem *self = malloc(sizeof(struct ClassBodyItem));
    self->kind = ClassBodyItemKindMethod;
    self->loc = loc;
    self->value.method = method;
    return self;
}

struct ClassBodyItem *
__new__ClassBodyItemImport(struct ImportStmt *import, struct Location loc)
{
    struct ClassBodyItem *self = malloc(sizeof(struct ClassBodyItem));
    self->kind = ClassBodyItemKindImport;
    self->loc = loc;
    self->value.import = import;
    return self;
}

struct String *
get_name__ClassBodyItem(struct ClassBodyItem *self)
{
    switch (self->kind) {
        case ClassBodyItemKindProperty:
            return self->value.property->name;
        case ClassBodyItemKindMethod:
            return self->value.method->name;
        default:
            return NULL;
    }
}

struct String *
to_String__ClassBodyItem(struct ClassBodyItem self)
{
    switch (self.kind) {
        case ClassBodyItemKindMethod:
            return to_String__MethodDecl(*self.value.method);
        case ClassBodyItemKindProperty:
            return to_String__PropertyDecl(*self.value.property);
        case ClassBodyItemKindImport:
            return to_String__ImportStmt(*self.value.import);
    }

    return NULL;
}

void
__free__ClassBodyItemAll(struct ClassBodyItem *self)
{
    switch (self->kind) {
        case ClassBodyItemKindProperty:
            FREE(ClassBodyItemProperty, self);
            break;
        case ClassBodyItemKindMethod:
            FREE(ClassBodyItemMethod, self);
            break;
        case ClassBodyItemKindImport:
            FREE(ClassBodyItemImport, self);
            break;
        default:
            UNREACHABLE("unknown class body item");
    }
}

struct ClassDecl *
__new__ClassDecl(struct String *name,
                 struct Vec *generic_params,
                 struct Vec *inheritance,
                 struct Vec *impl,
                 struct Vec *body,
                 bool is_pub)
{
    struct ClassDecl *self = malloc(sizeof(struct ClassDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->inheritance = inheritance;
    self->impl = impl;
    self->body = body;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__ClassDecl(struct ClassDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub)
        push_str__String(s, "pub object");
    else
        push_str__String(s, "object");

    append__String(s, format(" {S}", self.name), true);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    if (self.inheritance) {
        push_str__String(s, " => [");

        for (Usize i = 0; i < len__Vec(*self.inheritance) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__DataType(*CAST(
                       CAST(get__Vec(*self.inheritance, i), struct Tuple *)
                         ->items[0],
                       struct DataType *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__DataType(*CAST(
                                CAST(get__Vec(*self.inheritance,
                                              len__Vec(*self.inheritance) - 1),
                                     struct Tuple *)
                                  ->items[0],
                                struct DataType *))),
                       true);
    }

    if (self.impl) {
        push_str__String(s, "impl ");

        for (Usize i = 0; i < len__Vec(*self.inheritance) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__DataType(*CAST(
                       CAST(get__Vec(*self.inheritance, i), struct Tuple *)
                         ->items[0],
                       struct DataType *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__DataType(*CAST(
                                CAST(get__Vec(*self.inheritance,
                                              len__Vec(*self.inheritance) - 1),
                                     struct Tuple *)
                                  ->items[0],
                                struct DataType *))),
                       true);
    }

    push_str__String(s, ": class =\n");

    if (self.body) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.body); i++)
            append__String(
              s,
              format("{Sr}\n",
                     to_String__ClassBodyItem(
                       *CAST(get__Vec(*self.body, i), struct ClassBodyItem *))),
              true);

        --current_tab_size;
    }

    push_str__String(s, "end");

    return s;
}

void
__free__ClassDecl(struct ClassDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->inheritance) {
        for (Usize i = len__Vec(*self->inheritance); i--;) {
            FREE(
              DataTypeAll,
              CAST(get__Vec(*self->inheritance, i), struct Tuple *)->items[0]);
            free(
              CAST(get__Vec(*self->inheritance, i), struct Tuple *)->items[1]);
            FREE(Tuple, get__Vec(*self->inheritance, i));
        }

        FREE(Vec, self->inheritance);
    }

    if (self->impl) {
        for (Usize i = len__Vec(*self->impl); i--;) {
            FREE(DataTypeAll,
                 CAST(get__Vec(*self->impl, i), struct Tuple *)->items[0]);
            free(CAST(get__Vec(*self->impl, i), struct Tuple *)->items[1]);
            FREE(Tuple, get__Vec(*self->impl, i));
        }

        FREE(Vec, self->impl);
    }

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(ClassBodyItemAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    free(self);
}

struct Prototype *
__new__Prototype(struct String *name,
                 struct Vec *params_type,
                 struct DataType *return_type,
                 bool is_async,
                 bool has_first_self_param)
{
    struct Prototype *self = malloc(sizeof(struct Prototype));
    self->name = name;
    self->params_type = params_type;
    self->return_type = return_type;
    self->is_async = is_async;
    self->has_first_self_param = has_first_self_param;
    return self;
}

struct String *
to_String__Prototype(struct Prototype self)
{
    assert((self.return_type && self.params_type) && "UNREACHABLE");

    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_async)
        push_str__String(s, "async ");

    append__String(s, format("@{S} :: ", self.name), true);

    for (Usize i = 0; i < len__Vec(*self.params_type); i++)
        append__String(
          s,
          format("{Sr} -> ",
                 to_String__DataType(
                   *CAST(get__Vec(*self.params_type, i), struct DataType *))),
          true);

    append__String(
      s, format("{Sr}", to_String__DataType(*self.return_type)), true);

    return s;
}

void
__free__Prototype(struct Prototype *self)
{
    for (Usize i = len__Vec(*self->params_type); i--;)
        FREE(DataTypeAll, get__Vec(*self->params_type, i));

    FREE(Vec, self->params_type);
    FREE(DataTypeAll, self->return_type);
    free(self);
}

struct TraitBodyItem *
__new__TraitBodyItemPrototype(struct Location loc, struct Prototype *prototype)
{
    struct TraitBodyItem *self = malloc(sizeof(struct TraitBodyItem));
    self->kind = TraitBodyItemKindPrototype;
    self->loc = loc;
    self->value.prototype = prototype;
    return self;
}

struct TraitBodyItem *
__new__TraitBodyItemImport(struct Location loc, struct ImportStmt *import)
{
    struct TraitBodyItem *self = malloc(sizeof(struct TraitBodyItem));
    self->kind = TraitBodyItemKindImport;
    self->loc = loc;
    self->value.import = import;
    return self;
}

struct String *
to_String__TraitBodyItem(struct TraitBodyItem self)
{
    switch (self.kind) {
        case TraitBodyItemKindImport:
            return format("{Sr}\n", to_String__ImportStmt(*self.value.import));
        case TraitBodyItemKindPrototype:
            return format("{Sr}\n",
                          to_String__Prototype(*self.value.prototype));
    }

    return NULL;
}

void
__free__TraitBodyItemAll(struct TraitBodyItem *self)
{
    switch (self->kind) {
        case TraitBodyItemKindPrototype:
            FREE(TraitBodyItemPrototype, self);
            break;
        case TraitBodyItemKindImport:
            FREE(TraitBodyItemImport, self);
            break;
        default:
            UNREACHABLE("unknown trait body item");
    }
}

struct TraitDecl *
__new__TraitDecl(struct String *name,
                 struct Vec *generic_params,
                 struct Vec *inh,
                 struct Vec *body,
                 bool is_pub)
{
    struct TraitDecl *self = malloc(sizeof(struct TraitDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->inh = inh;
    self->body = body;
    self->is_pub = is_pub;
    return self;
}

struct String *
to_String__TraitDecl(struct TraitDecl self)
{
    struct String *s = NEW(String);

    append__String(s, repeat__String("\t", current_tab_size), true);

    if (self.is_pub)
        append__String(s, format("pub object {S}", self.name), true);
    else
        append__String(s, format("object {S}", self.name), true);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    push_str__String(s, ": trait =\n");

    if (self.body) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.body); i++)
            append__String(s,
                           to_String__TraitBodyItem(*CAST(
                             get__Vec(*self.body, i), struct TraitBodyItem *)),
                           true);

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__TraitDecl(struct TraitDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->inh) {
        for (Usize i = len__Vec(*self->inh); i--;) {
            FREE(DataTypeAll,
                 CAST(get__Vec(*self->inh, i), struct Tuple *)->items[0]);
            free(CAST(get__Vec(*self->inh, i), struct Tuple *)->items[1]);
            FREE(Tuple, get__Vec(*self->inh, i));
        }

        FREE(Vec, self->inh);
    }

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(TraitBodyItemAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    free(self);
}

struct TagDecl *
__new__TagDecl(struct String *name,
               struct Vec *generic_params,
               struct Vec *body)
{
    struct TagDecl *self = malloc(sizeof(struct TagDecl));
    self->name = name;
    self->generic_params = generic_params;
    self->body = body;
    return self;
}

struct String *
to_String__TagDecl(struct TagDecl self)
{
    struct String *s = NEW(String);

    append__String(
      s,
      format("{Sr}tag {S}", repeat__String("\t", current_tab_size), self.name),
      true);

    if (self.generic_params) {
        push_str__String(s, "[");

        for (Usize i = 0; i < len__Vec(*self.generic_params) - 1; i++)
            append__String(
              s,
              format("{Sr}, ",
                     to_String__Generic(*CAST(get__Vec(*self.generic_params, i),
                                              struct Generic *))),
              true);

        append__String(s,
                       format("{Sr}]",
                              to_String__Generic(*CAST(
                                get__Vec(*self.generic_params,
                                         len__Vec(*self.generic_params) - 1),
                                struct Generic *))),
                       true);
    }

    push_str__String(s, " =\n");

    if (self.body) {
        ++current_tab_size;

        for (Usize i = 0; i < len__Vec(*self.body); i++)
            append__String(s,
                           to_String__ModuleBodyItem(*CAST(
                             get__Vec(*self.body, i), struct ModuleBodyItem *)),
                           true);

        --current_tab_size;
    }

    append__String(
      s, format("{Sr}end", repeat__String("\t", current_tab_size)), true);

    return s;
}

void
__free__TagDecl(struct TagDecl *self)
{
    if (self->generic_params) {
        for (Usize i = len__Vec(*self->generic_params); i--;)
            FREE(GenericAll, get__Vec(*self->generic_params, i));

        FREE(Vec, self->generic_params);
    }

    if (self->body) {
        for (Usize i = len__Vec(*self->body); i--;)
            FREE(ModuleBodyItemAll, get__Vec(*self->body, i));

        FREE(Vec, self->body);
    }

    free(self);
}

struct Decl *
__new__DeclFun(struct Location loc, struct FunDecl *fun)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindFun;
    self->loc = loc;
    self->value.fun = fun;
    return self;
}

struct Decl *
__new__DeclConstant(struct Location loc, struct ConstantDecl *constant)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindConstant;
    self->loc = loc;
    self->value.constant = constant;
    return self;
}

struct Decl *
__new__DeclModule(struct Location loc, struct ModuleDecl *module)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindModule;
    self->loc = loc;
    self->value.module = module;
    return self;
}

struct Decl *
__new__DeclAlias(struct Location loc, struct AliasDecl *alias)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindAlias;
    self->loc = loc;
    self->value.alias = alias;
    return self;
}

struct Decl *
__new__DeclRecord(struct Location loc, struct RecordDecl *record)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindRecord;
    self->loc = loc;
    self->value.record = record;
    return self;
}

struct Decl *
__new__DeclEnum(struct Location loc, struct EnumDecl *enum_)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindEnum;
    self->loc = loc;
    self->value.enum_ = enum_;
    return self;
}

struct Decl *
__new__DeclError(struct Location loc, struct ErrorDecl *error)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindError;
    self->loc = loc;
    self->value.error = error;
    return self;
}

struct Decl *
__new__DeclClass(struct Location loc, struct ClassDecl *class)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindClass;
    self->loc = loc;
    self->value.class = class;
    return self;
}

struct Decl *
__new__DeclTrait(struct Location loc, struct TraitDecl *trait)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindTrait;
    self->loc = loc;
    self->value.trait = trait;
    return self;
}

struct Decl *
__new__DeclTag(struct Location loc, struct TagDecl *tag)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindTag;
    self->loc = loc;
    self->value.tag = tag;
    return self;
}

struct Decl *
__new__DeclImport(struct Location loc, struct ImportStmt *import)
{
    struct Decl *self = malloc(sizeof(struct Decl));
    self->kind = DeclKindImport;
    self->loc = loc;
    self->value.import = import;
    return self;
}

struct String *
get_name__Decl(struct Decl *decl)
{
    switch (decl->kind) {
        case DeclKindFun:
            return decl->value.fun->name;
        case DeclKindConstant:
            return decl->value.constant->name;
        case DeclKindModule:
            return decl->value.module->name;
        case DeclKindAlias:
            return decl->value.alias->name;
        case DeclKindRecord:
            return decl->value.record->name;
        case DeclKindEnum:
            return decl->value.enum_->name;
        case DeclKindError:
            return decl->value.error->name;
        case DeclKindClass:
            return decl->value.class->name;
        case DeclKindTrait:
            return decl->value.trait->name;
        case DeclKindTag:
            return decl->value.tag->name;
        case DeclKindImport:
            return NULL;
    }

    return NULL;
}

struct String *
to_String__Decl(struct Decl self)
{
    switch (self.kind) {
        case DeclKindFun:
            return to_String__FunDecl(*self.value.fun);
        case DeclKindConstant:
            return to_String__ConstantDecl(*self.value.constant);
        case DeclKindModule:
            return to_String__ModuleDecl(*self.value.module);
        case DeclKindAlias:
            return to_String__AliasDecl(*self.value.alias);
        case DeclKindRecord:
            return to_String__RecordDecl(*self.value.record);
        case DeclKindEnum:
            return to_String__EnumDecl(*self.value.enum_);
        case DeclKindError:
            return to_String__ErrorDecl(*self.value.error);
        case DeclKindClass:
            return to_String__ClassDecl(*self.value.class);
        case DeclKindTrait:
            return to_String__TraitDecl(*self.value.trait);
        case DeclKindTag:
            return to_String__TagDecl(*self.value.tag);
        case DeclKindImport:
            return to_String__ImportStmt(*self.value.import);
    }

    return NULL;
}

void
__free__DeclAll(struct Decl *self)
{
    switch (self->kind) {
        case DeclKindFun:
            FREE(DeclFun, self);
            break;
        case DeclKindConstant:
            FREE(DeclConstant, self);
            break;
        case DeclKindModule:
            FREE(DeclModule, self);
            break;
        case DeclKindAlias:
            FREE(DeclAlias, self);
            break;
        case DeclKindRecord:
            FREE(DeclRecord, self);
            break;
        case DeclKindEnum:
            FREE(DeclEnum, self);
            break;
        case DeclKindError:
            FREE(DeclError, self);
            break;
        case DeclKindClass:
            FREE(DeclClass, self);
            break;
        case DeclKindTrait:
            FREE(DeclTrait, self);
            break;
        case DeclKindTag:
            FREE(DeclTag, self);
            break;
        case DeclKindImport:
            FREE(DeclImport, self);
            break;
        default:
            UNREACHABLE("unknown decl kind");
    }
}

struct Contract *
__new__ContractValue(struct Expr *value)
{
    struct Contract *self = malloc(sizeof(struct Contract));
    self->kind = ContractKindByValue;
    self->contract.value = value;
    return self;
}

struct Contract *
__new__ContractDataType(struct Expr *data_type)
{
    struct Contract *self = malloc(sizeof(struct Contract));
    self->kind = ContractKindByDataType;
    self->contract.data_type = data_type;
    return self;
}

struct Contract *
__new__ContractGeneric(struct Expr *generic)
{
    struct Contract *self = malloc(sizeof(struct Contract));
    self->kind = ContractKindByGeneric;
    self->contract.generic = generic;
    return self;
}

void
__free__ContractAll(struct Contract *self)
{
    switch (self->kind) {
        case ContractKindByValue:
            FREE(ContractValue, self);
            break;
        case ContractKindByDataType:
            FREE(ContractDataType, self);
            break;
        case ContractKindByGeneric:
            FREE(ContractGeneric, self);
            break;
        default:
            UNREACHABLE("unknown ContractKind");
    }
}

struct CommentDoc *
__new__CommentDoc(enum DocKind kind, struct String *s)
{
    struct CommentDoc *self = malloc(sizeof(struct CommentDoc));
    self->kind = kind;
    self->s = s;
    return self;
}
