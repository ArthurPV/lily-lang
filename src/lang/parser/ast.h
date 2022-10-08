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

#ifndef LILY_AST_H
#define LILY_AST_H

#include <base/new.h>
#include <base/option.h>
#include <base/string.h>
#include <base/tuple.h>
#include <base/types.h>
#include <base/vec.h>
#include <lang/scanner/token.h>

enum DataTypeKind
{
    DataTypeKindSelf,
    DataTypeKindPtr,
    DataTypeKindRef,
    DataTypeKindStr,
    DataTypeKindBitStr,
    DataTypeKindChar,
    DataTypeKindBitChar,
    DataTypeKindI8,
    DataTypeKindI16,
    DataTypeKindI32,
    DataTypeKindI64,
    DataTypeKindI128,
    DataTypeKindU8,
    DataTypeKindU16,
    DataTypeKindU32,
    DataTypeKindU64,
    DataTypeKindU128,
    DataTypeKindF32,
    DataTypeKindF64,
    DataTypeKindBool,
    DataTypeKindIsize,
    DataTypeKindUsize,
    DataTypeKindNever,
    DataTypeKindAny,
    DataTypeKindOptional,
    DataTypeKindUnit,
    DataTypeKindException,
    DataTypeKindMut,
    DataTypeKindLambda,
    DataTypeKindArray,
    DataTypeKindTuple,
    DataTypeKindCustom,
    DataTypeKindCompilerDefined // CompilerDefined is not used in AST
};

typedef struct DataType
{
    enum DataTypeKind kind;
    union
    {
        struct DataType *ptr;
        struct DataType *ref;
        struct DataType *optional;
        struct DataType *exception;
        struct DataType *mut;
        struct Tuple *lambda; // struct Tuple<struct Vec<struct DataType*>,
                              // struct DataType*>*
        struct Tuple *array;  // struct Tuple<struct DataType*, Usize*>*
        struct Tuple *custom; // struct Tuple<struct Vec<struct String&>*,
                              // struct Vec<struct DataType*>*>*
        struct Vec *tuple;    // struct Vec<struct DataType*>*
    } value;
} DataType;

/**
 *
 * @brief Construct DataType type.
 */
struct DataType *
__new__DataType(enum DataTypeKind kind);

/**
 *
 * @brief Construct DataType type (Ptr variant).
 */
struct DataType *
__new__DataTypePtr(struct DataType *ptr);

/**
 *
 * @brief Construct DataType type (Ref variant).
 */
struct DataType *
__new__DataTypeRef(struct DataType *ref);

/**
 *
 * @brief Construct DataType type (Optional variant).
 */
struct DataType *
__new__DataTypeOptional(struct DataType *optional);

/**
 *
 * @brief Construct DataType type (Exception variant).
 */
struct DataType *
__new__DataTypeException(struct DataType *exception);

/**
 *
 * @brief Construct DataType type (Mut variant).
 */
struct DataType *
__new__DataTypeMut(struct DataType *mut);

/**
 *
 * @brief Construct DataType type (Lambda variant).
 */
struct DataType *
__new__DataTypeLambda(struct Vec *params, struct DataType *return_type);

/**
 *
 * @brief Construct DataType type (Array variant).
 */
struct DataType *
__new__DataTypeArray(struct DataType *data_type, Usize *size);

/**
 *
 * @brief Construct DataType type (Custom variant).
 */
struct DataType *
__new__DataTypeCustom(struct Vec *names, struct Vec *generic_params);

/**
 *
 * @brief Construct DataType type (Tuple variant).
 */
;
struct DataType *
__new__DataTypeTuple(struct Vec *tuple);

/**
 *
 * @brief Convert DataType to string.
 */
struct String *
to_string__DataType(struct DataType self);

/**
 *
 * @brief Copy the DataType type.
 */
struct DataType *
copy__DataType(struct DataType *self);

/**
 *
 * @brief Free DataType type.
 */
void
__free__DataType(struct DataType *self);

/**
 *
 * @brief Free DataType type (Ptr variant).
 */
void
__free__DataTypePtr(struct DataType *self);

/**
 *
 * @brief Free DataType type (Ref variant).
 */
void
__free__DataTypeRef(struct DataType *self);

/**
 *
 * @brief Free DataType type (Optional variant).
 */
void
__free__DataTypeOptional(struct DataType *self);

/**
 *
 * @brief Free DataType type (Exception variant).
 */
void
__free__DataTypeException(struct DataType *self);

/**
 *
 * @brief Free DataType type (Mut variant).
 */
void
__free__DataTypeMut(struct DataType *self);

/**
 *
 * @brief Free DataType type (Lambda variant).
 */
void
__free__DataTypeLambda(struct DataType *self);

/**
 *
 * @brief Free DataType type (Array variant).
 */
void
__free__DataTypeArray(struct DataType *self);

/**
 *
 * @brief Free DataType type (Custom variant).
 */
void
__free__DataTypeCustom(struct DataType *self);

/**
 *
 * @brief Free DataType type (Tuple variant).
 */
void
__free__DataTypeTuple(struct DataType *self);

/**
 *
 * @brief Free DataType type (all variants).
 */
void
__free__DataTypeAll(struct DataType *self);

enum GenericKind
{
    GenericKindDataType,
    GenericKindRestrictedDataType
};

typedef struct Generic
{
    enum GenericKind kind;
    struct Location loc;

    union
    {
        struct String *data_type; // struct String&
        struct Tuple
          *restricted_data_type; // struct Tuple<struct String&, struct
                                 // Tuple<struct DataType*, struct Location*>*>*
    } value;
} Generic;

/**
 *
 * @brief Construct the Generic type (DataType variant).
 */
struct Generic *
__new__GenericDataType(struct String *data_type, struct Location loc);

/**
 *
 * @brief Construct the Generic type (RestrictedDataType variant).
 */
struct Generic *
__new__GenericRestrictedDataType(struct String *name,
                                 struct Location loc,
                                 struct Tuple *data_type);

struct String *
get_name__Generic(struct Generic *self);

/**
 *
 * @brief Convert Generic in String.
 */
struct String *
to_string__Generic(struct Generic self);

/**
 *
 * @brief Free the Generic type (DataType variant).
 */
inline void
__free__GenericDataType(struct Generic *self)
{
    free(self);
}

/**
 *
 * @brief Free the Generic type (RestrictedDataType variant).
 */
void
__free__GenericRestrictedDataType(struct Generic *self);

/**
 *
 * @brief Free the Generic type (all variants).
 */
void
__free__GenericAll(struct Generic *self);

typedef struct VariableDecl
{
    struct String *name; // struct String&
    struct DataType *data_type;
    struct Expr *expr;
    bool is_mut;
} VariableDecl;

/**
 *
 * @brief Construct the VariableDecl type.
 */
struct VariableDecl
__new__VariableDecl(struct String *name,
                    struct DataType *data_type,
                    struct Expr *expr,
                    bool is_mut);

struct String *
to_string__VariableDecl(struct VariableDecl self);

/**
 *
 * @brief Free the VariableDecl type.
 */
void
__free__VariableDecl(struct VariableDecl self);

enum LiteralKind
{
    LiteralKindBool,
    LiteralKindChar,
    LiteralKindBitChar,
    LiteralKindInt32,
    LiteralKindInt64,
    LiteralKindInt128,
    LiteralKindFloat,
    LiteralKindBitStr,
    LiteralKindStr,
    LiteralKindUnit
};

typedef struct Literal
{
    enum LiteralKind kind;
    union
    {
        bool bool_;
        char char_;
        UInt8 bit_char;
        Int32 int32;
        Int64 int64;
        Int128 int128;
        Float64 float_;
        Str str;
        UInt8 **bit_str;
    } value;
} Literal;

/**
 *
 * @brief Construct Literal (Bool variant).
 */
inline struct Literal
__new__LiteralBool(bool bool_)
{
    struct Literal self = { .kind = LiteralKindBool, .value.bool_ = bool_ };

    return self;
}

/**
 *
 * @brief Construct Literal (Char variant).
 */
inline struct Literal
__new__LiteralChar(char char_)
{
    struct Literal self = { .kind = LiteralKindChar, .value.char_ = char_ };

    return self;
}

/**
 *
 * @brief Construct Literal (BitChar variant).
 */
inline struct Literal
__new__LiteralBitChar(UInt8 bit_char)
{
    struct Literal self = { .kind = LiteralKindBitChar,
                            .value.bit_char = bit_char };

    return self;
}

/**
 *
 * @brief Construct Literal (Int32 variant).
 */
inline struct Literal
__new__LiteralInt32(Int32 int32)
{
    struct Literal self = { .kind = LiteralKindInt32, .value.int32 = int32 };

    return self;
}

/**
 *
 * @brief Construct Literal (Int64 variant).
 */
inline struct Literal
__new__LiteralInt64(Int64 int64)
{
    struct Literal self = { .kind = LiteralKindInt64, .value.int64 = int64 };

    return self;
}

/**
 *
 * @brief Construct Literal (Int128 variant).
 */
inline struct Literal
__new__LiteralInt128(Int128 int128)
{
    struct Literal self = { .kind = LiteralKindInt128, .value.int128 = int128 };

    return self;
}

/**
 *
 * @brief Construct Literal (Float64 variant).
 */
inline struct Literal
__new__LiteralFloat(Float64 float_)
{
    struct Literal self = { .kind = LiteralKindFloat, .value.float_ = float_ };

    return self;
}

/**
 *
 * @brief Construct Literal (Str variant).
 */
inline struct Literal
__new__LiteralStr(Str str)
{
    struct Literal self = { .kind = LiteralKindStr, .value.str = str };

    return self;
}

/**
 *
 * @brief Construct Literal (BitStr variant).
 */
inline struct Literal
__new__LiteralBitStr(UInt8 **bit_str)
{
    struct Literal self = { .kind = LiteralKindBitStr,
                            .value.bit_str = bit_str };

    return self;
}

/**
 *
 * @brief Construct Literal (Unit variant).
 */
inline struct Literal
__new__LiteralUnit()
{
    struct Literal self = { .kind = LiteralKindUnit };

    return self;
}

/**
 *
 * @brief Convert Literal to string.
 */
struct String *
to_string__Literal(struct Literal self);

/**
 *
 * @brief Free the Literal type (Str variant).
 */
inline void
__free__LiteralStr(struct Literal self)
{
    free(self.value.str);
}

/**
 *
 * @brief Free the Literal type (all variants).
 */
void
__free__LiteralAll(struct Literal self);

enum UnaryOpKind
{
    UnaryOpKindNegative = 1,
    UnaryOpKindNot,
    UnaryOpKindReference,
    UnaryOpKindBitNot,
    UnaryOpKindCustom
};

/**
 *
 * @brief Convert the UnaryOpKind to Str.
 */
const Str
to_str__UnaryOpKind(enum UnaryOpKind kind);

typedef struct UnaryOp
{
    enum UnaryOpKind kind;
    struct Expr *right;

    union
    {
        struct String *op; // struct String&
    };
} UnaryOp;

/**
 *
 * @brief Construct the UnaryOp type.
 */
inline struct UnaryOp
__new__UnaryOp(enum UnaryOpKind kind, struct Expr *right, struct String *op)
{
    struct UnaryOp self = { .kind = kind, .right = right, .op = op };

    return self;
}

/**
 *
 * @brief Convert UnaryOp to String.
 */
struct String *
to_string__UnaryOp(struct UnaryOp self);

/**
 *
 * @brief Free the UnaryOp type.
 */
void
__free__UnaryOp(struct UnaryOp self);

enum BinaryOpKind
{
    BinaryOpKindAdd = 1,
    BinaryOpKindSub,
    BinaryOpKindMul,
    BinaryOpKindDiv,
    BinaryOpKindMod,
    BinaryOpKindRange,
    BinaryOpKindLt,
    BinaryOpKindGt,
    BinaryOpKindLe,
    BinaryOpKindGe,
    BinaryOpKindEq,
    BinaryOpKindNe,
    BinaryOpKindAnd,
    BinaryOpKindOr,
    BinaryOpKindXor,
    BinaryOpKindAssign,
    BinaryOpKindAddAssign,
    BinaryOpKindSubAssign,
    BinaryOpKindMulAssign,
    BinaryOpKindDivAssign,
    BinaryOpKindModAssign,
    BinaryOpKindConcatAssign,
    BinaryOpKindBitLShiftAssign,
    BinaryOpKindBitRShiftAssign,
    BinaryOpKindBitOrAssign,
    BinaryOpKindXorAssign,
    BinaryOpKindBitAndAssign,
    BinaryOpKindMergeAssign,
    BinaryOpKindUnmergeAssign,
    BinaryOpKindExponentAssign,
    BinaryOpKindChain,
    BinaryOpKindMerge,
    BinaryOpKindUnmerge,
    BinaryOpKindRepeat,
    BinaryOpKindConcat,
    BinaryOpKindBitLShift,
    BinaryOpKindBitRShift,
    BinaryOpKindBitOr,
    BinaryOpKindBitAnd,
    BinaryOpKindExponent,
    BinaryOpKindCustom
};

/**
 *
 * @brief Get the precedence of each BinaryOpKind.
 */
Usize
get_precedence__BinaryOpKind(enum BinaryOpKind kind);

/**
 *
 * @brief Convert the BinaryOpKind to Str.
 */
const Str
to_str__BinaryOpKind(enum BinaryOpKind kind);

typedef struct BinaryOp
{
    enum BinaryOpKind kind;
    struct Expr *left;
    struct Expr *right;

    union
    {
        struct String *op;
    };
} BinaryOp;

/**
 *
 * @brief Construct the BinaryOp type.
 */
struct BinaryOp
__new__BinaryOp(enum BinaryOpKind kind,
                struct Expr *left,
                struct Expr *right,
                struct String *op);

/**
 *
 * @brief Convert BinaryOp to String.
 */
struct String *
to_string__BinaryOp(struct BinaryOp self);

/**
 *
 * @brief Free the BinaryOp type.
 */
void
__free__BinaryOp(struct BinaryOp self);

typedef struct FunCall
{
    struct Expr *id;
    struct Vec *params; // struct Vec<struct Tuple<struct FunParamCall*, struct
                        // Location*>*>*
} FunCall;

/**
 *
 * @brief Construct the FunCall type.
 */
struct FunCall
__new__FunCall(struct Expr *id, struct Vec *params);

/**
 *
 * @brief Convert FunCall in String.
 */
struct String *
to_string__FunCall(struct FunCall self);

/**
 *
 * @brief Free the FunCall type.
 */
void
__free__FunCall(struct FunCall self);

typedef struct FieldCall
{
    struct String *name;  // struct String&
    struct Option *value; // struct Option<struct Expr*>*
} FieldCall;

/**
 *
 * @brief Construct the FieldCall type.
 */
struct FieldCall *
__new__FieldCall(struct String *name, struct Option *value);

/**
 *
 * @brief Free the FieldCall type.
 */
void
__free__FieldCall(struct FieldCall *self);

typedef struct RecordCall
{
    struct Expr *id;
    struct Vec *
      fields; // struct Vec<struct Tuple<struct FieldCall*, struct Location*>*>*
} RecordCall;

/**
 * @brief Construct the RecordCall type.
 */
struct RecordCall
__new__RecordCall(struct Expr *id, struct Vec *fields);

/**
 *
 * @brief Free the RecordCall type.
 */
void
__free__RecordCall(struct RecordCall self);

typedef struct ArrayAccess
{
    struct Expr *id;
    struct Vec *access; // struct Vec<struct Expr*>*
} ArrayAccess;

/**
 *
 * @brief Construct the ArrayAccess type.
 */
inline struct ArrayAccess
__new__ArrayAccess(struct Expr *id, struct Vec *access)
{
    struct ArrayAccess self = { .id = id, .access = access };

    return self;
}

/**
 *
 * @brief Free the ArrayAccess type.
 */
void
__free__ArrayAccess(struct ArrayAccess self);

typedef struct TupleAccess
{
    struct Expr *id;
    struct Vec *access; // struct Vec<struct Expr*>*
} TupleAccess;

/**
 *
 * @brief Construct the TupleAccess type.
 */
inline struct TupleAccess
__new__TupleAccess(struct Expr *id, struct Vec *access)
{
    struct TupleAccess self = { .id = id, .access = access };

    return self;
}

/**
 *
 * @brief Free the TupleAccess type.
 */
void
__free__TupleAccess(struct TupleAccess self);

typedef struct Lambda
{
    struct Vec *params;         // struct Vec<struct FunParam*>*
    struct Option *return_type; // struct Option<struct DataType*>*
    struct Vec *body;           // struct Vec<struct FunBodyItem*>*
    bool instantly_call;
} Lambda;

/**
 *
 * @brief Construct the Lambda type.
 */
struct Lambda
__new__Lambda(struct Vec *params,
              struct Option *return_type,
              struct Vec *body,
              bool instantly_call);

/**
 *
 * @brief Free the Lambda type.
 */
void
__free__Lambda(struct Lambda self);

typedef struct Variant
{
    struct Expr *id;
    struct Option *value; // struct Option<struct Expr*>*
} Variant;

/**
 *
 * @brief Construct the Variant type.
 */
inline struct Variant
__new__Variant(struct Expr *id, struct Option *value)
{
    struct Variant self = { .id = id, .value = value };

    return self;
}

/**
 *
 * @brief Free the Variant type.
 */
void
__free__Variant(struct Variant self);

enum ExprKind
{
    ExprKindUnaryOp,
    ExprKindBinaryOp,
    ExprKindFunCall,
    ExprKindRecordCall,
    ExprKindIdentifier,
    ExprKindIdentifierAccess,
    ExprKindGlobalAccess,
    ExprKindArrayAccess,
    ExprKindTupleAccess,
    ExprKindLambda,
    ExprKindTuple,
    ExprKindArray,
    ExprKindVariant,
    ExprKindTry,
    ExprKindIf,
    ExprKindBlock,
    ExprKindQuestionMark,
    ExprKindDereference,
    ExprKindRef,
    ExprKindSelf,
    ExprKindUndef,
    ExprKindNil,
    ExprKindNone,
    ExprKindWildcard,
    ExprKindLiteral,
    ExprKindVariable,
    ExprKindGrouping
};

typedef struct Expr
{
    enum ExprKind kind;
    struct Location loc;

    union
    {
        struct UnaryOp unary_op;
        struct BinaryOp binary_op;
        struct FunCall fun_call;
        struct RecordCall record_call;
        struct String *identifier;     // struct String&
        struct Vec *identifier_access; // struct Vec<struct Expr*>*
        struct Vec *global_access;     // struct Vec<struct Expr*>*
        struct ArrayAccess array_access;
        struct TupleAccess tuple_access;
        struct Lambda lambda;
        struct Vec *array; // struct Vec<struct Expr*>*
        struct Vec *tuple; // struct Vec<struct Expr*>*
        struct Variant variant;
        struct Expr *try;
        struct IfCond *if_;
        struct Vec *block; // struct Vec<struct FunBodyItem*>*
        struct Expr *question_mark;
        struct Expr *dereference;
        struct Expr *ref;
        struct Literal literal;
        struct VariableDecl variable;
        struct Expr *grouping;
    } value;
} Expr;

/**
 *
 * @brief Construct the Expr type.
 */
struct Expr *
__new__Expr(enum ExprKind kind, struct Location loc);

/**
 *
 * @brief Construct the Expr type (UnaryOp variant).
 */
struct Expr *
__new__ExprUnaryOp(struct UnaryOp unary_op, struct Location loc);

/**
 *
 * @brief Construct the Expr type (BinaryOp variant).
 */
struct Expr *
__new__ExprBinaryOp(struct BinaryOp binary_op, struct Location loc);

/**
 *
 * @brief Construct the Expr type (FunCall variant).
 */
struct Expr *
__new__ExprFunCall(struct FunCall fun_call, struct Location loc);

/**
 *
 * @brief Construct the Expr type (RecordCall variant).
 */
struct Expr *
__new__ExprRecordCall(struct RecordCall record_call, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Identifier variant).
 */
struct Expr *
__new__ExprIdentifier(struct String *identifier, struct Location loc);

/**
 * @brief Construct the Expr type (IdentifierAccess variant).
 */
struct Expr *
__new__ExprIdentifierAccess(struct Vec *identifier_access, struct Location loc);

/**
 * @brief Construct the Expr type (GlobalAccess variant).
 */
struct Expr *
__new__ExprGlobalAccess(struct Vec *global_access, struct Location loc);

/**
 *
 * @brief Construct the Expr type (ArrayAccess variant).
 */
struct Expr *
__new__ExprArrayAccess(struct ArrayAccess array_access, struct Location loc);

/**
 *
 * @brief Construct the Expr type (TupleAccess variant).
 */
struct Expr *
__new__ExprTupleAccess(struct TupleAccess tuple_access, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Lambda variant).
 */
struct Expr *
__new__ExprLambda(struct Lambda lambda, struct Location loc);

/**
 * @brief Construct the Expr type (Array variant).
 */
struct Expr *
__new__ExprArray(struct Vec *array, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Tuple variant).
 */
struct Expr *
__new__ExprTuple(struct Vec *tuple, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Variant variant).
 */
struct Expr *
__new__ExprVariant(struct Variant variant, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Try variant).
 */
struct Expr *
__new__ExprTry(struct Expr *try, struct Location loc);

/**
 *
 * @brief Construct the Expr type (If variant).
 */
struct Expr *
__new__ExprIf(struct IfCond *if_, struct Location loc);

/**
 *
 *
 * @brief Construct the Expr type (Block variant).
 */
struct Expr *
__new__ExprBlock(struct Vec *block, struct Location loc);

/**
 *
 *
 * @brief Construct the Expr type (QuestionMark variant).
 */
struct Expr *
__new__ExprQuestionMark(struct Expr *question_mark, struct Location loc);

/**
 *
 *
 * @brief Construct the Expr type (Dereference variant).
 */
struct Expr *
__new__ExprDereference(struct Expr *dereference, struct Location loc);

/**
 *
 *
 * @brief Construct the Expr type (Ref variant).
 */
struct Expr *
__new__ExprRef(struct Expr *ref, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Literal variant).
 */
struct Expr *
__new__ExprLiteral(struct Literal literal, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Variable variant).
 */
struct Expr *
__new__ExprVariable(struct VariableDecl variable, struct Location loc);

/**
 *
 * @brief Construct the Expr type (Grouping variant).
 */
struct Expr *
__new__ExprGrouping(struct Expr *grouping, struct Location loc);

/**
 *
 * @brief Get the precedence of each expression (1 is the value with more
 * priority).
 */
UInt32
get_precedence__Expr(struct Expr *expr);

/**
 *
 * @brief Convert Expr in String.
 */
struct String *
to_string__Expr(struct Expr self);

/**
 *
 * @brief Free the Expr type (UnaryOp variant).
 */
inline void
__free__ExprUnaryOp(struct Expr *self)
{
    FREE(UnaryOp, self->value.unary_op);
    free(self);
}

/**
 *
 * @brief Free the Expr type (BinaryOp variant).
 */
inline void
__free__ExprBinaryOp(struct Expr *self)
{
    FREE(BinaryOp, self->value.binary_op);
    free(self);
}

/**
 *
 * @brief Free the Expr type (FunCall variant).
 */
inline void
__free__ExprFunCall(struct Expr *self)
{
    FREE(FunCall, self->value.fun_call);
    free(self);
}

/**
 *
 * @brief Free the Expr type (RecordCall variant).
 */
inline void
__free__ExprRecordCall(struct Expr *self)
{
    FREE(RecordCall, self->value.record_call);
    free(self);
}

/**
 *
 * @brief Free the Expr type (Identifier variant).
 */
inline void
__free__ExprIdentifier(struct Expr *self)
{
    free(self);
}

/**
 *
 * @brief Free the Expr type (IdentifierAccess variant).
 */
void
__free__ExprIdentifierAccess(struct Expr *self);

/**
 *
 * @brief Free the Expr type (GlobalAccess variant).
 */
void
__free__ExprGlobalAccess(struct Expr *self);

/**
 *
 * @brief Free the Expr type (SelfAccess variant).
 */
void
__free__ExprSelfAccess(struct Expr *self);

/**
 *
 * @brief Free the Expr type (ArrayAccess variant).
 */
inline void
__free__ExprArrayAccess(struct Expr *self)
{
    FREE(ArrayAccess, self->value.array_access);
    free(self);
}

/**
 *
 * @brief Free the Expr type (TupleAccess variant).
 */
inline void
__free__ExprTupleAccess(struct Expr *self)
{
    FREE(TupleAccess, self->value.tuple_access);
    free(self);
}

/**
 *
 * @brief Free the Expr type (Lambda variant).
 */
inline void
__free__ExprLambda(struct Expr *self)
{
    FREE(Lambda, self->value.lambda);
    free(self);
}

/**
 *
 * @brief Free the Expr type (Array variant).
 */
void
__free__ExprArray(struct Expr *self);

/**
 *
 * @brief Free the Expr type (Tuple variant).
 */
void
__free__ExprTuple(struct Expr *self);

/**
 *
 * @brief Free the Expr type (Variant variant).
 */
inline void
__free__ExprVariant(struct Expr *self)
{
    FREE(Variant, self->value.variant);
    free(self);
}

/**
 *
 * @brief Free the Expr type (Try variant).
 */
void
__free__ExprTry(struct Expr *self);

/**
 *
 * @brief Free the Expr type (If variant).
 */
void
__free__ExprIf(struct Expr *self);

/**
 *
 * @brief Free the Expr type (Block variant).
 */
void
__free__ExprBlock(struct Expr *self);

/**
 *
 * @brief Free the Expr type (QuestionMark variant).
 */
void
__free__ExprQuestionMark(struct Expr *self);

/**
 *
 * @brief Free the Expr type (Dereference variant).
 */
void
__free__ExprDereference(struct Expr *self);

/**
 *
 * @brief Free the Expr type (Ref variant).
 */
void
__free__ExprRef(struct Expr *self);

/**
 *
 * @brief Free the Expr type (Literal variant).
 */
inline void
__free__ExprLiteral(struct Expr *self)
{
    FREE(LiteralAll, self->value.literal);
    free(self);
}

/**
 *
 * @brief Free the Expr type (Variable variant).
 */
inline void
__free__ExprVariable(struct Expr *self)
{
    FREE(VariableDecl, self->value.variable);
    free(self);
}

/**
 *
 * @brief Free the Expr type (Grouping variant).
 */
void
__free__ExprGrouping(struct Expr *self);

/**
 *
 * @brief Free the Expr type.
 */
inline void
__free__Expr(struct Expr *self)
{
    free(self);
}

/**
 *
 * @brief Free the Expr type (all variants).
 */
void
__free__ExprAll(struct Expr *self);

typedef struct MatchStmt
{
    struct Expr *matching;
    struct Vec *pattern; // struct Vec<struct Tuple<struct Expr*, struct Expr*,
                         // struct Expr*>*>*
} MatchStmt;

/**
 *
 * @brief Construct the MatchStmt type.
 */
struct MatchStmt *
__new__MatchStmt(struct Expr *matching, struct Vec *pattern);

/**
 *
 * @brief Convert MatchStmt in String.
 */
struct String *
to_string__MatchStmt(struct MatchStmt self);

/**
 *
 * @brief Free the MatchStmt type.
 */
void
__free__MatchStmt(struct MatchStmt *self);

typedef struct IfBranch
{
    struct Expr *cond;
    struct Vec *body; // struct Vec<struct FunBodyItem*>*
} IfBranch;

/**
 *
 * @brief Construct the IfBranch type.
 */
struct IfBranch *
__new__IfBranch(struct Expr *cond, struct Vec *body);

/**
 *
 * @brief Free the IfBranch type.
 */
void
__free__IfBranch(struct IfBranch *self);

typedef struct IfCond
{
    struct IfBranch *if_;
    struct Option *elif;  // struct Option<struct Vec<struct IfBranch*>*>*
    struct Option *else_; // struct Option<struct Vec<struct FunBodyItem*>*>*
} IfCond;

/**
 *
 * @brief Construct the IfCond type.
 */
struct IfCond *
__new__IfCond(struct IfBranch *if_, struct Option *elif, struct Option *else_);

/**
 *
 * @brief Convert IfCond type in String.
 */
struct String *
to_string__IfCond(struct IfCond self);

/**
 *
 * @brief Free the IfCond type.
 */
void
__free__IfCond(struct IfCond *self);

typedef struct TryStmt
{
    struct Vec *try_body;      // struct Vec<struct FunBodyItem*>*
    struct Option *catch_expr; // struct Option<struct Expr*>*
    struct Option
      *catch_body; // struct Option<struct Vec<struct FunBodyItem*>*>*
} TryStmt;

/**
 *
 * @brief Construct the TryStmt type.
 */
struct TryStmt *
__new__TryStmt(struct Vec *try_body,
               struct Option *catch_expr,
               struct Option *catch_body);

/**
 *
 * @brief Convert TryStmt in String.
 */
struct String *
to_string__TryStmt(struct TryStmt self);

/**
 *
 * @brief Free the TryStmt type.
 */
void
__free__TryStmt(struct TryStmt *self);

typedef struct WhileStmt
{
    struct Expr *cond;
    struct Vec *body; // struct Vec<struct FunBodyItem*>*
} WhileStmt;

/**
 *
 * @brief Construct the WhileStmt type.
 */
struct WhileStmt *
__new__WhileStmt(struct Expr *cond, struct Vec *body);

/**
 *
 * @brief Convert WhileStmt in String.
 */
struct String *
to_string__WhileStmt(struct WhileStmt self);

/**
 *
 * @brief Free the WhileStmt type.
 */
void
__free__WhileStmt(struct WhileStmt *self);

enum ForStmtExprKind
{
    ForStmtExprKindRange,
    ForStmtExprKindTraditional,
};

typedef struct ForStmtExprTraditional
{
    struct Option
      *id; // struct Option<struct Tuple<struct String*, struct Expr*>*>*
    struct Option *cond;   // struct Option<struct Expr*>*
    struct Option *action; // struct Option<struct Expr*>*
} ForStmtExprTraditional;

/**
 *
 * @brief Construct the ForStmtExprTraditional type.
 */
struct ForStmtExprTraditional *
__new__ForStmtExprTraditional(struct Option *id,
                              struct Option *cond,
                              struct Option *action);

/**
 *
 * @brief Free the ForStmtExprTraditional type.
 */
void
__free__ForStmtExprTraditional(struct ForStmtExprTraditional *self);

typedef struct ForStmtExpr
{
    enum ForStmtExprKind kind;
    struct Location loc;

    union
    {
        struct Tuple *range; // struct Tuple<struct String*, struct Expr*>*
        struct ForStmtExprTraditional *traditional;
    } value;
} ForStmtExpr;

/**
 *
 * @brief Construct the ForStmtExpr type (Range variant).
 */
struct ForStmtExpr *
__new__ForStmtExprRange(struct Tuple *range, struct Location loc);

/**
 *
 * @brief Construct the ForStmtExpr type (Traditional variant).
 */
struct ForStmtExpr *
__new__ForStmtExprTraditionalVar(struct ForStmtExprTraditional *traditional,
                                 struct Location loc);

/**
 *
 * @brief Convert ForStmtExpr type in String.
 */
struct String *
to_string__ForStmtExpr(struct ForStmtExpr self);

/**
 *
 * @brief Free the ForStmtExpr type (Range variant).
 */
void
__free__ForStmtExprRange(struct ForStmtExpr *self);

/**
 *
 * @brief Free the ForStmtExpr type (Traditional variant).
 */
void
__free__ForStmtExprTraditionalVar(struct ForStmtExpr *self);

/**
 *
 * @brief Free the ForStmtExpr type (all variants).
 */
void
__free__ForStmtExprAll(struct ForStmtExpr *self);

typedef struct ForStmt
{
    struct ForStmtExpr *expr;
    struct Vec *body; // struct Vec<struct FunBodyItem*>*
} ForStmt;

/**
 *
 * @brief Construct the ForStmt type.
 */
struct ForStmt *
__new__ForStmt(struct ForStmtExpr *expr, struct Vec *body);

/**
 *
 * @brief Convert ForStmt type in String.
 */
struct String *
to_string__ForStmt(struct ForStmt self);

/**
 *
 * @brief Free the ForStmt type.
 */
void
__free__ForStmt(struct ForStmt *self);

enum ImportStmtValueKind
{
    ImportStmtValueKindStd,
    ImportStmtValueKindCore,
    ImportStmtValueKindBuiltin,
    ImportStmtValueKindFile,
    ImportStmtValueKindUrl,
    ImportStmtValueKindAccess,
    ImportStmtValueKindSelector,
    ImportStmtValueKindWildcard
};

typedef struct ImportStmtValue
{
    enum ImportStmtValueKind kind;

    union
    {
        struct String *access;
        struct String *url;
        struct String *file;
        struct Vec
          *selector; // struct Vec<struct Vec<struct ImportStmtValue*>*>*
    } value;
} ImportStmtValue;

/**
 *
 * @brief Construct the ImportStmtValue type (Std variant).
 */
inline struct ImportStmtValue *
__new__ImportStmtValueStd()
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindStd;
    return self;
}

/**
 *
 * @brief Construct the ImportStmtValue type (Core variant).
 */
inline struct ImportStmtValue *
__new__ImportStmtValueCore()
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindCore;
    return self;
}

/**
 *
 * @brief Construct the ImportStmtValue type (Builtin variant).
 */
inline struct ImportStmtValue *
__new__ImportStmtValueBuiltin()
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindBuiltin;
    return self;
}

/**
 *
 * @brief Construct the ImportStmtValue type (File variant).
 */
inline struct ImportStmtValue *
__new__ImportStmtValueFile(struct String *file)
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindFile;
    self->value.file = file;
    return self;
}

/**
 *
 * @brief Construct the ImportStmtValue type (Url variant).
 */
inline struct ImportStmtValue *
__new__ImportStmtValueUrl(struct String *url)
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindUrl;
    self->value.url = url;
    return self;
}

/**
 *
 * @brief Construct the ImportStmtValue type (Access variant).
 */
struct ImportStmtValue *
__new__ImportStmtValueAccess(struct String *access);

/**
 *
 * @brief Construct the ImportStmtValue type (Selector variant).
 */
struct ImportStmtValue *
__new__ImportStmtValueSelector(struct Vec *selector);

/**
 *
 * @brief Construct the ImportStmtValue type (Wildcard variant).
 */
inline struct ImportStmtValue *
__new__ImportStmtValueWildcard()
{
    struct ImportStmtValue *self = malloc(sizeof(struct ImportStmtValue));
    self->kind = ImportStmtValueKindWildcard;
    return self;
}

/**
 *
 * @brief Free the ImportStmtValue type (Access variant).
 */
void
__free__ImportStmtValueAccess(struct ImportStmtValue *self);

/**
 *
 * @brief Free the ImportStmtValue type (Selector variant).
 */
void
__free__ImportStmtValueSelector(struct ImportStmtValue *self);

/**
 *
 * @brief Free the ImportStmtValue type (all variants).
 */
void
__free__ImportStmtValueAll(struct ImportStmtValue *self);

typedef struct ImportStmt
{
    struct Vec *import_value; // struct Vec<struct ImportStmtValue*>*
    bool is_pub;
    struct Option *as; // struct Option<struct String*>*
} ImportStmt;

/**
 *
 * @brief Construct the ImportStmt type.
 */
struct ImportStmt *
__new__ImportStmt(struct Vec *import_value, bool is_pub, struct Option *as);

/**
 *
 * @brief Convert the ImportStmt type in String.
 */
struct String *
to_string__ImportStmt(struct ImportStmt self);

/**
 *
 * @brief Free the ImportStmt type.
 */
void
__free__ImportStmt(struct ImportStmt *self);

enum StmtKind
{
    StmtKindReturn,
    StmtKindIf,
    StmtKindAwait,
    StmtKindTry,
    StmtKindMatch,
    StmtKindWhile,
    StmtKindFor,
    StmtKindNext,
    StmtKindBreak,
    StmtKindImport,
};

typedef struct Stmt
{
    enum StmtKind kind;
    struct Location loc;

    union
    {
        struct Expr *return_;
        struct IfCond *if_;
        struct Expr *await;
        struct TryStmt *try;
        struct MatchStmt *match;
        struct WhileStmt *while_;
        struct ForStmt *for_;
        struct ImportStmt *import;
    } value;
} Stmt;

/**
 *
 * @brief Construct the Stmt type.
 */
struct Stmt *
__new__Stmt(enum StmtKind kind, struct Location loc);

/**
 *
 * @brief Construct the Stmt type (Return variant).
 */
struct Stmt *
__new__StmtReturn(struct Location loc, struct Expr *return_);

/**
 *
 * @brief Construct the Stmt type (If variant).
 */
struct Stmt *
__new__StmtIf(struct Location loc, struct IfCond *if_);

/**
 *
 * @brief Construct the Stmt type (Await variant).
 */
struct Stmt *
__new__StmtAwait(struct Location loc, struct Expr *await);

/**
 *
 * @brief Construct the Stmt type (Try variant).
 */
struct Stmt *
__new__StmtTry(struct Location loc, struct TryStmt *try);

/**
 *
 * @brief Construct the Stmt type (Match variant).
 */
struct Stmt *
__new__StmtMatch(struct Location loc, struct MatchStmt *match);

/**
 *
 * @brief Construct the Stmt type (While variant).
 */
struct Stmt *
__new__StmtWhile(struct Location loc, struct WhileStmt *while_);

/**
 *
 * @brief Construct the Stmt type (For variant).
 */
struct Stmt *
__new__StmtFor(struct Location loc, struct ForStmt *for_);

/**
 *
 * @brief Construct the Stmt type (Import variant).
 */
struct Stmt *
__new__StmtImport(struct Location loc, struct ImportStmt *import);

/**
 *
 * @brief Convert Stmt type in String.
 */
struct String *
to_string__Stmt(struct Stmt self);

/**
 *
 * @brief Free the Stmt type.
 */
inline void
__free__Stmt(struct Stmt *self)
{
    free(self);
}

/**
 *
 * @brief Free the Stmt type (Return variant).
 */
inline void
__free__StmtReturn(struct Stmt *self)
{
    FREE(ExprAll, self->value.return_);
    free(self);
}

/**
 *
 * @brief Free the Stmt type (If variant).
 */
inline void
__free__StmtIf(struct Stmt *self)
{
    FREE(IfCond, self->value.if_);
    free(self);
}

/**
 *
 * @brief Free the Stmt type (Await variant).
 */
inline void
__free__StmtAwait(struct Stmt *self)
{
    FREE(ExprAll, self->value.await);
    free(self);
}

/**
 *
 * @brief Free the Stmt type (Try variant).
 */
inline void
__free__StmtTry(struct Stmt *self)
{
    FREE(TryStmt, self->value.try);
    free(self);
}

/**
 *
 * @brief Free the Stmt type (Match variant).
 */
inline void
__free__StmtMatch(struct Stmt *self)
{
    FREE(MatchStmt, self->value.match);
    free(self);
}

/**
 *
 * @brief Free the Stmt type (While variant).
 */
inline void
__free__StmtWhile(struct Stmt *self)
{
    FREE(WhileStmt, self->value.while_);
    free(self);
}

/**
 *
 * @brief Free the Stmt type (For variant).
 */
inline void
__free__StmtFor(struct Stmt *self)
{
    FREE(ForStmt, self->value.for_);
    free(self);
}

inline void
__free__StmtImport(struct Stmt *self)
{
    FREE(ImportStmt, self->value.import);
    free(self);
}

/**
 *
 * @brief Free the Stmt type (all variant).
 */
void
__free__StmtAll(struct Stmt *self);

enum FunBodyItemKind
{
    FunBodyItemKindExpr,
    FunBodyItemKindStmt,
};

// FunBodyItem: For function.
typedef struct FunBodyItem
{
    enum FunBodyItemKind kind;

    union
    {
        struct Expr *expr;
        struct Stmt *stmt;
    };
} FunBodyItem;

/**
 *
 * @brief Construct the FunBodyItem type (Expr variant).
 */
struct FunBodyItem *
__new__FunBodyItemExpr(struct Expr *expr);

/**
 *
 * @brief Construct the FunBodyItem type (Stmt variant).
 */
struct FunBodyItem *
__new__FunBodyItemStmt(struct Stmt *stmt);

/**
 *
 * @brief Convert FunBodyItem in String.
 */
struct String *
to_string__FunBodyItem(struct FunBodyItem self);

/**
 *
 * @brief Free the FunBodyItem type (Expr variant).
 */
inline void
__free__FunBodyItemExpr(struct FunBodyItem *self)
{
    FREE(ExprAll, self->expr);
    free(self);
}

/**
 *
 * @brief Free the FunBodyItem type (Stmt variant).
 */
inline void
__free__FunBodyItemStmt(struct FunBodyItem *self)
{
    FREE(StmtAll, self->stmt);
    free(self);
}

/**
 *
 * @brief Free the FunBodyItem type (all variants).
 */
void
__free__FunBodyItemAll(struct FunBodyItem *self);

enum FunParamKind
{
    FunParamKindDefault,
    FunParamKindNormal,
    FunParamKindSelf
};

typedef struct FunParamCall
{
    enum FunParamKind kind;
    struct Expr *value;

    union
    {
        struct String *name; // struct String&
    };
} FunParamCall;

/**
 *
 * @brief Construct the FunParamCall type.
 */
struct FunParamCall *
__new__FunParamCall(struct Expr *value);

/**
 *
 * @brief Construct the FunParamCall type (Default variant).
 */
struct FunParamCall *
__new__FunParamCallDefault(struct Expr *value, struct String *name);

/**
 *
 * @brief Convert FunParamCall in String.
 */
struct String *
to_string__FunParamCall(struct FunParamCall self);

/**
 *
 * @brief Free the FunParamCall type.
 */
inline void
__free__FunParamCall(struct FunParamCall *self)
{
    FREE(ExprAll, self->value);
    free(self);
}

/**
 *
 * @brief Free the FunParamCall type (Default variant).
 */
inline void
__free__FunParamCallDefault(struct FunParamCall *self)
{
    FREE(ExprAll, self->value);
    FREE(String, self->name);
    free(self);
}

/**
 *
 * @brief Free the FunParamCall type (all variants).
 */
void
__free__FunParamCallAll(struct FunParamCall *self);

typedef struct FunParam
{
    enum FunParamKind kind;
    struct Tuple
      *param_data_type; // struct Tuple<struct DataType*, struct Location*>*
    struct Location loc;

    union
    {
        struct String *name; // struct String&
    };

    union
    {
        struct Expr *default_;
    } value;
} FunParam;

/**
 *
 * @brief Construct FunParam type (Default variant).
 */
struct FunParam *
__new__FunParamDefault(struct String *name,
                       struct Tuple *param_data_type,
                       struct Location loc,
                       struct Expr *default_);

/**
 *
 * @brief Construct FunParam type (Normal variant).
 */
struct FunParam *
__new__FunParamNormal(struct String *name,
                      struct Tuple *param_data_type,
                      struct Location loc);

/**
 * @brief Construct FunParam type (Self variant).
 */
struct FunParam *
__new__FunParamSelf(struct Location loc);

/**
 *
 * @brief Convert FunParam in String.
 */
struct String *
to_string__FunParam(struct FunParam self);

/**
 *
 * @brief Free the FunParam type (Default variant).
 */
void
__free__FunParamDefault(struct FunParam *self);

/**
 *
 * @brief Free the FunParam type (Normal variant).
 */
void
__free__FunParamNormal(struct FunParam *self);

/**
 *
 * @brief Free the FunParam type (Self variant).
 */
void
__free__FunParamSelf(struct FunParam *self);

/**
 *
 * @brief Free the FunParam type (all variants).
 */
void
__free__FunParamAll(struct FunParam *self);

typedef struct FunDecl
{
    struct Vec *doc;
    struct String *name; // struct String&
    struct Vec
      *tags; // struct Vec<struct Tuple<struct DataType*, struct Location&>*>*
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct Vec *params;         // struct Vec<struct FunParam*>*
    struct Tuple
      *return_type;   // struct Tuple<struct DataType*, struct Location*>*
    struct Vec *body; // struct Vec<struct FunBodyItem*>*
    bool is_pub;
    bool is_async;
} FunDecl;

/**
 *
 * @brief Construct the FunDecl type.
 */
struct FunDecl *
__new__FunDecl(struct String *name,
               struct Vec *tags,
               struct Vec *generic_params,
               struct Vec *params,
               struct Tuple *return_type,
               struct Vec *body,
               bool is_pub,
               bool is_async);

/**
 *
 * @brief Free the FunDecl type.
 */
void
__free__FunDecl(struct FunDecl *self);

typedef struct ConstantDecl
{
    struct String *name; // struct String&
    struct DataType *data_type;
    struct Expr *expr;
    bool is_pub;
} ConstantDecl;

/**
 *
 * @brief Construct the ConstantDecl type.
 */
struct ConstantDecl *
__new__ConstantDecl(struct String *name,
                    struct DataType *data_type,
                    struct Expr *expr,
                    bool is_pub);

/**
 *
 * @brief Free the ConstantDecl type.
 */
void
__free__ConstantDecl(struct ConstantDecl *self);

enum ModuleBodyItemKind
{
    ModuleBodyItemKindDecl,
    ModuleBodyItemKindImport,
    ModuleBodyItemKindInclude
};

typedef struct ModuleBodyItem
{
    enum ModuleBodyItemKind kind;
    union
    {
        struct Decl *decl;
        struct Tuple
          *import; // struct Tuple<struct ImportStmt*, struct Location*>*
    } value;
} ModuleBodyItem;

/**
 *
 * @brief Construct the ModuleBodyItem type (Decl variant).
 */
struct ModuleBodyItem *
__new__ModuleBodyItemDecl(struct Decl *decl);

/**
 *
 * @brief Construct the ModuleBodyItem type (Import variant).
 */
struct ModuleBodyItem *
__new__ModuleBodyItemImport(struct Tuple *import);

/**
 *
 * @brief Free the ModuleBodyItem type (Decl variant).
 */
void
__free__ModuleBodyItemDecl(struct ModuleBodyItem *self);

/**
 *
 * @brief Free the ModuleBodyItem type (Import variant).
 */
inline void
__free__ModuleBodyItemImport(struct ModuleBodyItem *self)
{
    FREE(ImportStmt, self->value.import->items[0]);
    FREE(Tuple, self->value.import);
    free(self);
}

/**
 *
 * @brief Free the ModuleBodyItem type (all variant).
 */
void
__free__ModuleBodyItemAll(struct ModuleBodyItem *self);

typedef struct ModuleDecl
{
    struct String *name;
    struct Vec *body; // struct Vec<struct ModuleBodyItem*>*
    bool is_pub;
} ModuleDecl;

/**
 *
 * @brief Construct the ModuleDecl type.
 */
struct ModuleDecl *
__new__ModuleDecl(struct String *name, struct Vec *body, bool is_pub);

/**
 *
 * @brief Free the ModuleDecl type.
 */
void
__free__ModuleDecl(struct ModuleDecl *self);

typedef struct AliasDecl
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct DataType *data_type;
    bool is_pub;
} AliasDecl;

/**
 *
 * @brief Construct the AliasDecl type.
 */
struct AliasDecl *
__new__AliasDecl(struct String *name,
                 struct Vec *generic_params,
                 struct DataType *data_type,
                 bool is_pub);

/**
 *
 * @brief Free the AliasDecl type.
 */
void
__free__AliasDecl(struct AliasDecl *self);

typedef struct FieldRecord
{
    struct String *name; // struct String&
    struct DataType *data_type;
    struct Expr *value;
    bool is_pub;
    struct Location loc;
} FieldRecord;

/**
 *
 * @brief Construct the FieldRecord type.
 */
struct FieldRecord *
__new__FieldRecord(struct String *name,
                   struct DataType *data_type,
                   struct Expr *value,
                   bool is_pub,
                   struct Location loc);

/**
 *
 * @brief Free the FieldRecord type.
 */
void
__free__FieldRecord(struct FieldRecord *self);

typedef struct RecordDecl
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct Vec *fields;         // struct Vec<struct FieldRecord*>*
    bool is_pub;
    bool is_object;
} RecordDecl;

/**
 *
 * @brief Construct the RecordDecl type.
 */
struct RecordDecl *
__new__RecordDecl(struct String *name,
                  struct Vec *generic_params,
                  struct Vec *fields,
                  bool is_pub,
                  bool is_object);

/**
 *
 * @brief Free the RecordDecl type.
 */
void
__free__RecordDecl(struct RecordDecl *self);

typedef struct VariantEnum
{
    struct String *name;        // struct String&
    struct DataType *data_type; // struct DataType*
    struct Location loc;
} VariantEnum;

/**
 *
 * @brief Construct the VariantEnum type.
 */
struct VariantEnum *
__new__VariantEnum(struct String *name,
                   struct DataType *data_type,
                   struct Location loc);

/**
 *
 * @brief Free the VariantEnum type.
 */
void
__free__VariantEnum(struct VariantEnum *self);

typedef struct EnumDecl
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct Vec *variants;       // struct Vec<struct VariantEnum*>*
    struct DataType *type_value;
    bool is_pub;
    bool is_object;
    bool is_error;
} EnumDecl;

/**
 *
 * @brief Construct the EnumDecl type.
 */
struct EnumDecl *
__new__EnumDecl(struct String *name,
                struct Vec *generic_params,
                struct Vec *variants,
                struct DataType *type_value,
                bool is_pub,
                bool is_object,
                bool is_error);

/**
 *
 * @brief Free the EnumDecl type.
 */
void
__free__EnumDecl(struct EnumDecl *self);

typedef struct ErrorDecl
{
    struct String *name;
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct DataType *data_type;
    bool is_pub;
} ErrorDecl;

/**
 *
 * @brief Construct the ErrorDecl type.
 */
struct ErrorDecl *
__new__ErrorDecl(struct String *name,
                 struct Vec *generic_params,
                 struct DataType *data_type,
                 bool is_pub);

/**
 *
 * @brief Free the ErrorDecl type.
 */
void
__free__ErrorDecl(struct ErrorDecl *self);

enum ClassBodyItemKind
{
    ClassBodyItemKindMethod,
    ClassBodyItemKindProperty,
    ClassBodyItemKindImport
};

typedef struct PropertyDecl
{
    struct String *name; // struct String&
    struct DataType *data_type;
    bool is_pub;
} PropertyDecl;

/**
 *
 * @brief Construct the PropertyDecl type.
 */
struct PropertyDecl *
__new__PropertyDecl(struct String *name,
                    struct DataType *data_type,
                    bool is_pub);

/**
 *
 * @brief Free the PropertyDecl type.
 */
void
__free__PropertyDecl(struct PropertyDecl *self);

typedef struct MethodDecl
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct Vec *params;         // struct Vec<struct FunParam*>*
    struct DataType *return_type;
    struct Vec *body; // struct Vec<struct FunBodyItem*>*
    bool has_first_self_param;
    bool is_async;
    bool is_pub;
} MethodDecl;

/**
 *
 * @brief Construct the MethodDecl type.
 */
struct MethodDecl *
__new__MethodDecl(struct String *name,
                  struct Vec *generic_params,
                  struct Vec *params,
                  struct DataType *return_type,
                  struct Vec *body,
                  bool has_first_self_param,
                  bool is_async,
                  bool is_pub);

/**
 *
 * @brief Free the MethodDecl type.
 */
void
__free__MethodDecl(struct MethodDecl *self);

typedef struct ClassBodyItem
{
    enum ClassBodyItemKind kind;
    struct Location loc;

    union
    {
        struct PropertyDecl *property;
        struct MethodDecl *method;
        struct ImportStmt *import;
    } value;
} ClassBodyItem;

/**
 *
 * @brief Construct the ClassBodyItem type (Property variant).
 */
struct ClassBodyItem *
__new__ClassBodyItemProperty(struct PropertyDecl *property,
                             struct Location loc);

/**
 *
 * @brief Construct the ClassBodyItem type (Method variant).
 */
struct ClassBodyItem *
__new__ClassBodyItemMethod(struct MethodDecl *method, struct Location loc);

/**
 *
 * @brief Construct the ClassBodyItem type (Import variant).
 */
struct ClassBodyItem *
__new__ClassBodyItemImport(struct ImportStmt *import, struct Location loc);

/**
 *
 * @brief Get name of ClassBodyItem.
 */
struct String *
get_name__ClassBodyItem(struct ClassBodyItem *self);

/**
 *
 * @brief Free the ClassBodyItem type (Property variant).
 */
inline void
__free__ClassBodyItemProperty(struct ClassBodyItem *self)
{
    FREE(PropertyDecl, self->value.property);
    free(self);
}

/**
 *
 * @brief Free the ClassBodyItem type (Method variant).
 */
inline void
__free__ClassBodyItemMethod(struct ClassBodyItem *self)
{
    FREE(MethodDecl, self->value.method);
    free(self);
}

/**
 *
 * @brief Free the ClassBodyItem type (Import variant).
 */
inline void
__free__ClassBodyItemImport(struct ClassBodyItem *self)
{
    FREE(ImportStmt, self->value.import);
    free(self);
}

/**
 *
 * @brief Free the ClassBodyItem type (all variants).
 */
void
__free__ClassBodyItemAll(struct ClassBodyItem *self);

typedef struct ClassDecl
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct Vec *inheritance; // struct Vec<struct Tuple<struct DataType*, struct
                             // Location*>*>*
    struct Vec
      *impl; // struct Vec<struct Tuple<struct DataType*, struct Location*>*>*
    struct Vec *body; // struct Vec<struct ClassBodyItem*>*
    bool is_pub;
} ClassDecl;

/**
 *
 * @brief Construct the ClassDecl type.
 */
struct ClassDecl *
__new__ClassDecl(struct String *name,
                 struct Vec *generic_params,
                 struct Vec *inheritance,
                 struct Vec *impl,
                 struct Vec *body,
                 bool is_pub);

/**
 *
 * @brief Free the ClassDecl type.
 */
void
__free__ClassDecl(struct ClassDecl *self);

enum TraitBodyItemKind
{
    TraitBodyItemKindPrototype,
    TraitBodyItemKindImport
};

typedef struct Prototype
{
    struct String *name;     // struct String&
    struct Vec *params_type; // struct Vec<struct DataType*>*
    struct DataType *return_type;
    bool is_async;
    bool has_first_self_param;
} Prototype;

/**
 *
 * @brief Construct the Prototype type.
 */
struct Prototype *
__new__Prototype(struct String *name,
                 struct Vec *params_type,
                 struct DataType *return_type,
                 bool is_async,
                 bool has_first_self_param);

/**
 *
 * @brief Free the Prototype type.
 */
void
__free__Prototype(struct Prototype *self);

typedef struct TraitBodyItem
{
    enum TraitBodyItemKind kind;
    struct Location loc;

    union
    {
        struct Prototype *prototype;
        struct ImportStmt *import;
    } value;
} TraitBodyItem;

/**
 *
 * @brief Construct the TraitBodyItem type (Prototype variant).
 */
struct TraitBodyItem *
__new__TraitBodyItemPrototype(struct Location loc, struct Prototype *prototype);

/**
 *
 * @brief Construct the TraitBodyItem type (Import variant).
 */
struct TraitBodyItem *
__new__TraitBodyItemImport(struct Location loc, struct ImportStmt *import);

/**
 *
 * @brief Free the TraitBodyItem type (Prototype variant).
 */
inline void
__free__TraitBodyItemPrototype(struct TraitBodyItem *self)
{
    FREE(Prototype, self->value.prototype);
    free(self);
}

/**
 *
 * @brief Free the TraitBodyItem type (Import variant).
 */
inline void
__free__TraitBodyItemImport(struct TraitBodyItem *self)
{
    FREE(ImportStmt, self->value.import);
    free(self);
}

/**
 *
 * @brief Free the TraitBodyItem type (all variants).
 */
void
__free__TraitBodyItemAll(struct TraitBodyItem *self);

typedef struct TraitDecl
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct Vec
      *inh; // struct Vec<struct Tuple<struct DataType*, struct Location*>*>*
    struct Vec *body; // struct Vec<struct TraitBodyItem*>*
    bool is_pub;
} TraitDecl;

/**
 *
 * @brief Construct the TraitDecl type.
 */
struct TraitDecl *
__new__TraitDecl(struct String *name,
                 struct Vec *generic_params,
                 struct Vec *inh,
                 struct Vec *body,
                 bool is_pub);

/**
 *
 * @brief Free the TraitDecl type.
 */
void
__free__TraitDecl(struct TraitDecl *self);

typedef struct TagDecl
{
    struct String *name;
    struct Vec *generic_params; // struct Vec<struct Generic*>*
    struct Vec *body;           // struct Vec<struct ModuleBodyItem*>*
} TagDecl;

/**
 *
 * @brief Construct the TagDecl type.
 */
struct TagDecl *
__new__TagDecl(struct String *name,
               struct Vec *generic_params,
               struct Vec *body);

/**
 *
 * @brief Free the TagDecl type.
 */
void
__free__TagDecl(struct TagDecl *self);

enum DeclKind
{
    DeclKindFun,
    // DeclKindVariable,
    DeclKindConstant,
    DeclKindModule,
    DeclKindAlias,
    DeclKindRecord,
    DeclKindEnum,
    DeclKindError,
    DeclKindClass,
    DeclKindTrait,
    DeclKindTag,
    DeclKindImport,
};

typedef struct Decl
{
    enum DeclKind kind;
    struct Location loc;

    union
    {
        struct FunDecl *fun;
        struct ConstantDecl *constant;
        struct ModuleDecl *module;
        struct AliasDecl *alias;
        struct RecordDecl *record;
        struct EnumDecl *enum_;
        struct ErrorDecl *error;
        struct ClassDecl *class;
        struct TraitDecl *trait;
        struct TagDecl *tag;
        struct ImportStmt *import;
    } value;
} Decl;

/**
 *
 * @brief Construct the Decl type (Fun variant).
 */
struct Decl *
__new__DeclFun(struct Location loc, struct FunDecl *fun);

/**
 *
 * @brief Construct the Decl type (Constant variant).
 */
struct Decl *
__new__DeclConstant(struct Location loc, struct ConstantDecl *constant);

/**
 *
 * @brief Construct the Decl type (Module variant).
 */
struct Decl *
__new__DeclModule(struct Location loc, struct ModuleDecl *module);

/**
 *
 * @brief Construct the Decl type (Alias variant).
 */
struct Decl *
__new__DeclAlias(struct Location loc, struct AliasDecl *alias);

/**
 *
 * @brief Construct the Decl type (Record variant).
 */
struct Decl *
__new__DeclRecord(struct Location loc, struct RecordDecl *record);

/**
 *
 * @brief Construct the Decl type (Enum variant).
 */
struct Decl *
__new__DeclEnum(struct Location loc, struct EnumDecl *enum_);

/**
 *
 * @brief Construct the Decl type (Error variant).
 */
struct Decl *
__new__DeclError(struct Location loc, struct ErrorDecl *error);

/**
 *
 * @brief Construct the Decl type (Class variant).
 */
struct Decl *
__new__DeclClass(struct Location loc, struct ClassDecl *class);

/**
 *
 * @brief Construct the Decl type (Trait variant).
 */
struct Decl *
__new__DeclTrait(struct Location loc, struct TraitDecl *trait);

/**
 *
 * @brief Construct the Decl type (Tag variant).
 */
struct Decl *
__new__DeclTag(struct Location loc, struct TagDecl *tag);

/**
 *
 * @brief Construct the Decl type (Import variant).
 */
struct Decl *
__new__DeclImport(struct Location loc, struct ImportStmt *import);

/**
 *
 * @brief Get name of Decl.
 */
struct String *
get_name__Decl(struct Decl *decl);

/**
 *
 * @brief Free the Decl type (Fun variant).
 */
inline void
__free__DeclFun(struct Decl *self)
{
    FREE(FunDecl, self->value.fun);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Constant variant).
 */
inline void
__free__DeclConstant(struct Decl *self)
{
    FREE(ConstantDecl, self->value.constant);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Module variant).
 */
inline void
__free__DeclModule(struct Decl *self)
{
    FREE(ModuleDecl, self->value.module);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Alias variant).
 */
inline void
__free__DeclAlias(struct Decl *self)
{
    FREE(AliasDecl, self->value.alias);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Record variant).
 */
inline void
__free__DeclRecord(struct Decl *self)
{
    FREE(RecordDecl, self->value.record);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Enum variant).
 */
inline void
__free__DeclEnum(struct Decl *self)
{
    FREE(EnumDecl, self->value.enum_);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Error variant).
 */
inline void
__free__DeclError(struct Decl *self)
{
    FREE(ErrorDecl, self->value.error);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Class variant).
 */
inline void
__free__DeclClass(struct Decl *self)
{
    FREE(ClassDecl, self->value.class);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Trait variant).
 */
inline void
__free__DeclTrait(struct Decl *self)
{
    FREE(TraitDecl, self->value.trait);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Tag variant).
 */
inline void
__free__DeclTag(struct Decl *self)
{
    FREE(TagDecl, self->value.tag);
    free(self);
}

/**
 *
 * @brief Free the Decl type (Import variant).
 */
inline void
__free__DeclImport(struct Decl *self)
{
    FREE(ImportStmt, self->value.import);
    free(self);
}

/**
 *
 * @brief Free the Decl type (all variants).
 */
void
__free__DeclAll(struct Decl *self);

enum ContractKind
{
    ContractKindByValue,
    ContractKindByDataType,
    ContractKindByGeneric,
};

typedef struct Contract
{
    enum ContractKind kind;

    union
    {
        struct Expr *value;
        struct Expr *data_type;
        struct Expr *generic;
    } contract;
} Contract;

/**
 *
 * @brief Construct the Contract type (Value variant).
 */
struct Contract *
__new__ContractValue(struct Expr *value);

/**
 *
 * @brief Construct the Contract type (DataType variant).
 */
struct Contract *
__new__ContractDataType(struct Expr *data_type);

/**
 *
 * @brief Construct the Contract type (Generic variant).
 */
struct Contract *
__new__ContractGeneric(struct Expr *generic);

/**
 *
 * @brief Free the Contract type (Value variant).
 */
inline void
__free__ContractValue(struct Contract *self)
{
    FREE(ExprAll, self->contract.value);
    free(self);
}

/**
 *
 * @brief Free the Contract type (DataType variant).
 */
inline void
__free__ContractDataType(struct Contract *self)
{
    FREE(ExprAll, self->contract.data_type);
    free(self);
}

/**
 *
 * @brief Free the Contract type (Generic variant).
 */
inline void
__free__ContractGeneric(struct Contract *self)
{
    FREE(ExprAll, self->contract.data_type);
    free(self);
}

/**
 *
 * @brief Free the Contract type (all variants).
 */
void
__free__ContractAll(struct Contract *self);

/**
 *
 * @brief Construct the Contract type (Generic variant).
 */
struct Contract *
__new__ContractGeneric(struct Expr *value);

enum CommentDocKind
{
    CommentDocKindAuthor,
    CommentDocKindDesc,
    CommentDocKindVersion
};

typedef struct CommentDoc
{
    enum DocKind kind;
    struct String *s;
} CommentDoc;

/**
 *
 * @brief Construct the CommentDoc type.
 */
struct CommentDoc *
__new__CommentDoc(enum DocKind kind, struct String *s);

/**
 *
 * @brief Free the CommentDoc type.
 */
inline void
__free__CommentDoc(struct CommentDoc *self)
{
    FREE(String, self->s);
    free(self);
}

#endif // LILY_AST_H
