#ifndef LILY_SYMBOL_TABLE_H
#define LILY_SYMBOL_TABLE_H

#include <lang/parser/ast.h>

enum ScopeItemKind
{
    ScopeItemKindVariable,
    ScopeItemKindParam,
    ScopeItemKindRecord,
    ScopeItemKindAlias,
    ScopeItemKindEnum,
    ScopeItemKindRecordObj,
    ScopeItemKindEnumObj,
    ScopeItemKindConstant,
    ScopeItemKindVariant,
    ScopeItemKindFun,
    ScopeItemKindError,
    ScopeItemKindClass,
    ScopeItemKindModule,
    ScopeItemKindTrait
};

enum ScopeKind
{
    ScopeKindGlobal,
    ScopeKindLocal
};

typedef struct Scope
{
    Str filename;
    struct String *name; // struct String&
    struct Vec *id;      // struct Vec<Usize*>*
    enum ScopeItemKind item_kind;
    enum ScopeKind kind;
    bool is_checked;
} ScopdId;

/**
 *
 * @brief Construct the Scope type.
 */
struct Scope
__new__Scope(const Str filename,
             struct String *name,
             struct Vec *id,
             enum ScopeItemKind item_kind,
             enum ScopeKind kind);

/**
 *
 * @brief Free the Scope type.
 */
void
__free__Scope(struct Scope scope);

enum Visibility
{
    VisibilityPublic,
    VisibilityPrivate
};

#define VISIBILITY(self) \
    self->is_pub == true ? VisibilityPublic : VisibilityPrivate;

typedef struct DataTypeSymbol
{
    enum DataTypeKind kind;
    struct Scope *scope; // struct Scope&
    union
    {
        struct DataTypeSymbol *ptr;
        struct DataTypeSymbol *ref;
        struct DataTypeSymbol *optional;
        struct DataTypeSymbol *exception;
        struct Tuple
          *lambda; // struct Tuple<struct Vec*, struct DataTypeSymbol*>*
        struct Tuple *array;  // struct Tuple<struct DataTypeSymbol*, Usize*>*
        struct Tuple *custom; // struct Tuple<struct String&, struct Vec<struct
                              // GenericParams*>*>*
        struct Vec *tuple;    // struct Vec<struct DataTypeSymbol*>*
    } value;
} DataTypeSymbol;

/**
 *
 * @brief Construct the DataTypeSymbol type.
 */
struct DataTypeSymbol *
__new__DataTypeSymbol(enum DataTypeKind kind);

/**
 *
 * @brief Construct the DataTypeSymbol type (Ptr variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolPtr(struct DataTypeSymbol *ptr);

/**
 *
 * @brief Construct the DataTypeSymbol type (Ref variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolRef(struct DataTypeSymbol *ref);

/**
 *
 * @brief Construct the DataTypeSymbol type (Optional variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolOptional(struct DataTypeSymbol *optional);

/**
 *
 * @brief Construct the DataTypeSymbol type (Exception variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolException(struct DataTypeSymbol *exception);

/**
 *
 * @brief Construct the DataTypeSymbol type (Lambda variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolLambda(struct Vec *params,
                            struct DataTypeSymbol *return_type);

/**
 *
 * @brief Construct the DataTypeSymbol type (Array variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolArray(struct DataTypeSymbol *data_type,
                           struct Option *size);

/**
 *
 * @brief Construct the DataTypeSymbol type (Custom variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolCustom(struct String *name,
                            struct Vec *generic_params,
                            struct Scope *scope);

/**
 *
 * @brief Construct the DataTypeSymbol type (Tuple variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolTuple(struct Vec *tuple);

/**
 *
 * @brief Free the DataTypeSymbol type.
 */
void
__free__DataTypeSymbol(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Ptr variant).
 */
void
__free__DataTypeSymbolPtr(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Ref variant).
 */
void
__free__DataTypeSymbolRef(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Optional variant).
 */
void
__free__DataTypeSymbolOptional(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Exception variant).
 */
void
__free__DataTypeSymbolException(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Lambda variant).
 */
void
__free__DataTypeSymbolLambda(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Array variant).
 */
void
__free__DataTypeSymbolArray(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Custom variant).
 */
void
__free__DataTypeSymbolCustom(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Tuple variant).
 */
void
__free__DataTypeSymbolTuple(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (all variants).
 */
void
__free__DataTypeSymbolAll(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the Generic type (RestrictedDataType with DataTypeSymbol).
 */
void
__free__GenericSymbolRestrictedDataType(struct Generic *self);

/**
 *
 * @brief Free the Generic type (all variants).
 */
void
__free__GenericSymbolAll(struct Generic *self);

typedef struct VariableSymbol
{
    struct String *name; // struct String&
    struct DataTypeSymbol *data_type;
    struct ExprSymbol *expr;
    struct Scope *scope; // struct Scope&
    struct Location loc;
    bool is_mut;
} VariableSymbol;

/**
 *
 * @brief Construct the VariableDecl type.
 */
struct VariableSymbol
__new__VariableSymbol(struct VariableDecl decl, struct Location decl_loc);

/**
 *
 * @brief Free the VariableDecl type.
 */
void
__free__VariableSymbol(struct VariableSymbol self);

typedef struct FunSymbol
{
    struct String *name;        // struct String&
    struct Vec *taged_type;     // struct Vec<struct SymbolTable*>*
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *params;         // struct Vec<struct FunParam*>&
    enum Visibility visibility;
    bool is_async;
    struct DataType *return_type;
    struct Vec *body;      // struct Vec<struct SymbolTable*>*
    struct Scope *scope;   // struct Scope&
    struct Decl *fun_decl; // struct Decl&
} FunSymbol;

/**
 *
 * @brief Construct the FunSymbol type.
 */
struct FunSymbol *
__new__FunSymbol(struct Decl *fun_decl);

/**
 *
 * @brief Free the FunSymbol type.
 */
void
__free__FunSymbol(struct FunSymbol *self);

typedef struct ConstantSymbol
{
    struct String *name;        // struct String&
    struct DataType *data_type; // struct DataType&
    struct ExprSymbol *expr_symbol;
    struct Scope *scope;        // struct Scope&
    struct Decl *constant_decl; // struct Decl&
    enum Visibility visibility;
} ConstantSymbol;

/**
 *
 * @brief Construct the ConstantSymbol type.
 */
struct ConstantSymbol *
__new__ConstantSymbol(struct Decl *constant_decl);

/**
 *
 * @brief Free the ConstantSymbol type.
 */
void
__free__ConstantSymbol(struct ConstantSymbol *self);

typedef struct ModuleSymbol
{
    struct String *name;      // struct String&
    struct Vec *body;         // struct Vec<SymbolTable*>*
    struct Scope *scope;      // struct Scope&
    struct Decl *module_decl; // struct Decl&
    enum Visibility visibility;
} ModuleSymbol;

/**
 *
 * @brief Construct the ModuleSymbol type.
 */
struct ModuleSymbol *
__new__ModuleSymbol(struct Decl *module_decl);

/**
 *
 * @brief Free the ModuleSymbol type.
 */
void
__free__ModuleSymbol(struct ModuleSymbol *self);

typedef struct AliasSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct DataType *data_type; // struct DataType&
    struct Scope *scope;        // struct Scope&
    struct Decl *alias_decl;    // struct Decl&
    enum Visibility visibility;
} AliasSymbol;

/**
 *
 * @brief Construct the AliasSymbol type.
 */
struct AliasSymbol *
__new__AliasSymbol(struct Decl *alias_decl);

/**
 *
 * @brief Free the AliasSymbol type.
 */
void
__free__AliasSymbol(struct AliasSymbol *self);

typedef struct FieldRecordSymbol
{
    struct String *name;        // struct String&
    struct DataType *data_type; // struct DataType&
} FieldRecordSymbol;

enum LiteralSymbolKind
{
    LiteralSymbolKindBool,
    LiteralSymbolKindChar,
    LiteralSymbolKindBitChar,
    LiteralSymbolKindInt8,
    LiteralSymbolKindInt16,
    LiteralSymbolKindInt32,
    LiteralSymbolKindInt64,
    LiteralSymbolKindInt128,
    LiteralSymbolKindUint8,
    LiteralSymbolKindUint16,
    LiteralSymbolKindUint32,
    LiteralSymbolKindUint64,
    LiteralSymbolKindUint128,
    LiteralSymbolKindFloat32,
    LiteralSymbolKindFloat64,
    LiteralSymbolKindStr,
    LiteralSymbolKindBitStr,
    LiteralSymbolKindUnit
};

typedef struct LiteralSymbol
{
    enum LiteralSymbolKind kind;

    union
    {
        bool bool_;
        char char_;
        UInt8 bit_char;
        Int8 int8;
        Int16 int16;
        Int32 int32;
        Int64 int64;
        Int128 int128;
        UInt8 uint8;
        UInt16 uint16;
        UInt32 uint32;
        UInt64 uint64;
        Int128 uint128;
        Float32 float32;
        Float64 float64;
        Str str;         // Str&
        UInt8 **bit_str; // Uint8*&
    } value;
} LiteralSymbol;

/**
 *
 * @brief Construct LiteralSymbol (Bool variant).
 */
struct LiteralSymbol
__new__LiteralSymbolBool(bool bool_);

/**
 *
 * @brief Construct LiteralSymbol (Char variant).
 */
struct LiteralSymbol
__new__LiteralSymbolChar(char char_);

/**
 *
 * @brief Construct LiteralSymbol (BitChar variant).
 */
struct LiteralSymbol
__new__LiteralSymbolBitChar(UInt8 bit_char);

/**
 *
 * @brief Construct LiteralSymbol (Int8 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolInt8(Int8 int8);

/**
 *
 * @brief Construct LiteralSymbol (Int16 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolInt16(Int16 int16);

/**
 *
 * @brief Construct LiteralSymbol (Int32 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolInt32(Int32 int32);

/**
 *
 * @brief Construct LiteralSymbol (Int64 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolInt64(Int64 int64);

/**
 *
 * @brief Construct LiteralSymbol (Int128 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolInt128(Int128 int128);

/**
 *
 * @brief Construct LiteralSymbol (Uint8 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolUint8(UInt8 uint8);

/**
 *
 * @brief Construct LiteralSymbol (Uint16 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolUint16(UInt16 uint16);

/**
 *
 * @brief Construct LiteralSymbol (Uint32 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolUint32(UInt32 uint32);

/**
 *
 * @brief Construct LiteralSymbol (Uint64 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolUint64(UInt64 uint64);

/**
 *
 * @brief Construct LiteralSymbol (Uint128 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolUint128(Int128 uint128);

/**
 *
 * @brief Construct LiteralSymbol (Float32 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolFloat32(Float32 float32);

/**
 *
 * @brief Construct LiteralSymbol (Float64 variant).
 */
struct LiteralSymbol
__new__LiteralSymbolFloat64(Float64 float64);

/**
 *
 * @brief Construct LiteralSymbol (Str variant).
 */
struct LiteralSymbol
__new__LiteralSymbolStr(Str str);

/**
 *
 * @brief Construct LiteralSymbol (BitStr variant).
 */
struct LiteralSymbol
__new__LiteralSymbolBitStr(UInt8 **bit_str);

/**
 *
 * @brief Construct LiteralSymbol (Unit variant).
 */
struct LiteralSymbol
__new__LiteralSymbolUnit();

typedef struct UnaryOpSymbol
{
    enum UnaryOpKind kind;
    struct DataType *data_type;
    struct ExprSymbol *right;
} UnaryOpSymbol;

/**
 *
 * @brief Construct the UnaryOpSymbol type.
 */
struct UnaryOpSymbol
__new__UnaryOpSymbol(struct Expr unary_op,
                     struct DataType *data_type,
                     struct ExprSymbol *right);

/**
 *
 * @brief Convert UnaryOpKind to Str.
 */
const Str
to_Str__UnaryOpSymbol(unsigned int kind);

/**
 *
 * @brief Free the UnaryOpSymbol type.
 */
void
__free__UnaryOpSymbol(struct UnaryOpSymbol self);

typedef struct BinaryOpSymbol
{
    enum BinaryOpKind kind;
    struct DataType *data_type;
    struct ExprSymbol *left;
    struct ExprSymbol *right;
} BinaryOpSymbol;

/**
 *
 * @brief Construct the BinaryOpSymbol type.
 */
struct BinaryOpSymbol
__new__BinaryOpSymbol(struct Expr binary_op,
                      struct DataType *data_type,
                      struct ExprSymbol *left,
                      struct ExprSymbol *right);

/**
 *
 * @brief Convert BinaryOpKind to Str.
 */
const Str
to_Str__BinaryOpSymbol(unsigned int kind);

/**
 *
 * @brief Free the BinaryOpSymbol type.
 */
void
__free__BinaryOpSymbol(struct BinaryOpSymbol self);

typedef struct FunCallSymbol
{
    bool is_builtin;
    struct Scope *id;
    struct DataType *data_type;
    struct Vec *params; // struct Vec<struct Tuple<struct SymbolFunParamCall*,
                        // struct Location&>*>*
} FunCallSymbol;

/**
 *
 * @brief Construct the FunCallSymbol type.
 */
struct FunCallSymbol
__new__FunCallSymbol(bool is_builtin,
                     struct Scope *id,
                     struct DataType *data_type,
                     struct Vec *params);

/**
 *
 * @brief Free the FunCallSymbol type.
 */
void
__free__FunCallSymbol(struct FunCallSymbol self);

typedef struct FieldCallSymbol
{
    struct String *name; // struct String&
    struct ExprSymbol *value;
} FieldCallSymbol;

/**
 *
 * @brief Construct the FieldCallSymbol type.
 */
struct FieldCallSymbol *
__new__FieldCallSymbol(struct String *name, struct ExprSymbol *value);

/**
 *
 * @brief Free the FieldCallSymbol type.
 */
void
__free__FieldCallSymbol(struct FieldCallSymbol *self);

typedef struct RecordCallSymbol
{
    struct Scope id;
    struct Vec *fields; // struct Vec<struct Tuple<struct FieldCallSymbol*,
                        // struct Location&>*>*
} RecordCallSymbol;

/**
 *
 * @brief Construct the RecordCallSymbol type.
 */
struct RecordCallSymbol
__new__RecordCallSymbol(struct Scope id, struct Vec *fields);

/**
 *
 * @brief Free the RecordCallSymbol type.
 */
void
__free__RecordCallSymbol(struct RecordCallSymbol self);

typedef struct ArrayAccessSymbol
{
    struct Scope id;
    struct Vec *access; // struct Vec<struct ExprSymbol*>*
} ArrayAccessSymbol;

/**
 *
 * @brief Construct the ArrayAccessSymbol type.
 */
struct ArrayAccessSymbol
__new__ArrayAccessSymbol(struct Scope id, struct Vec *access);

/**
 *
 * @brief Free the ArrayAccessSymbol type.
 */
void
__free__ArrayAccessSymbol(struct ArrayAccessSymbol self);

typedef struct TupleAccessSymbol
{
    struct Scope id;
    struct Vec *access; // struct Vec<struct ExprSymbol*>*
} TupleAccessSymbol;

/**
 *
 * @brief Construct the TupleAccessSymbol type.
 */
struct TupleAccessSymbol
__new__TupleAccessSymbol(struct Scope id, struct Vec *access);

/**
 *
 * @brief Free the TupleAccessSymbol type.
 */
void
__free__TupleAccessSymbol(struct TupleAccessSymbol self);

typedef struct LambdaSymbol
{
    struct Vec *params; // struct Vec<struct FunParam*>* TODO!!
    struct DataTypeSymbol *return_type;
    struct Vec *body; // struct Vec<struct SymbolTable*>*
    Usize id;
} LambdaSymbol;

typedef struct VariantSymbol
{
    struct Scope id;
    struct ExprSymbol *value;
} VariantSymbol;

/**
 *
 * @brief Construct the VariantSymbol type.
 */
struct VariantSymbol
__new__VariantSymbol(struct Scope id, struct ExprSymbol *value);

/**
 *
 * @brief Free the VariantSymbol type.
 */
void
__free__VariantSymbol(struct VariantSymbol self);

typedef struct ExprSymbol
{
    enum ExprKind kind;
    struct Location loc;

    union
    {
        struct UnaryOpSymbol unary_op;
        struct BinaryOpSymbol binary_op;
        struct FunCallSymbol fun_call;
        struct RecordCallSymbol record_call;
        struct Scope identifier;
        struct Scope identifier_access;
        struct ArrayAccessSymbol array_access;
        struct TupleAccessSymbol tuple_access;
        struct LambdaSymbol lambda;
        struct Vec *array;
        struct Vec *tuple;
        struct VariantSymbol variant;
        struct ExprSymbol *try;
        // struct IfCond
        struct Vec *block; // struct Vec<struct SymbolTable*>*
        struct Scope question_mark;
        struct Scope dereference;
        struct Scope ref;
        struct LiteralSymbol literal;
        struct VariableSymbol variable;
    } value;
} ExprSymbol;

/**
 *
 * @brief Construct the ExprSymbol type.
 */
struct ExprSymbol *
__new__ExprSymbol(struct Expr *expr);

/**
 *
 * @brief Construct the ExprSymbol type (UnaryOp variant).
 */
struct ExprSymbol *
__new__ExprSymbolUnaryOp(struct Expr expr, struct UnaryOpSymbol unary_op);

/**
 *
 * @brief Construct the ExprSymbol type (BinaryOp variant).
 */
struct ExprSymbol *
__new__ExprSymbolBinaryOp(struct Expr expr, struct BinaryOpSymbol binary_op);

/**
 *
 * @brief Construct the ExprSymbol type (FunCall variant).
 */
struct ExprSymbol *
__new__ExprSymbolFunCall(struct Expr expr, struct FunCallSymbol fun_call);

/**
 *
 * @brief Construct the ExprSymbol type (RecordCall variant).
 */
struct ExprSymbol *
__new__ExprSymbolRecordCall(struct Expr expr,
                            struct RecordCallSymbol record_call);

/**
 *
 * @brief Construct the ExprSymbol type (Identifier variant).
 */
struct ExprSymbol *
__new__ExprSymbolIdentifier(struct Expr expr, struct Scope identifier);

/**
 *
 * @brief Construct the ExprSymbol type (IdentifierAccess variant).
 */
struct ExprSymbol *
__new__ExprSymbolIdentifierAccess(struct Expr expr,
                                  struct Scope identifier_access);

/**
 *
 * @brief Construct the ExprSymbol type (ArrayAccess variant).
 */
struct ExprSymbol *
__new__ExprSymbolArrayAccess(struct Expr expr,
                             struct ArrayAccessSymbol array_access);

/**
 *
 * @brief Construct the ExprSymbol type (TupleAccess variant).
 */
struct ExprSymbol *
__new__ExprSymbolTupleAccess(struct Expr expr,
                             struct TupleAccessSymbol tuple_access);

/**
 *
 * @brief Construct the ExprSymbol type (Lambda variant).
 */
struct ExprSymbol *
__new__ExprSymbolLambda(struct Expr expr, struct LambdaSymbol lambda);

/**
 *
 * @brief Construct the ExprSymbol type (Tuple variant).
 */
struct ExprSymbol *
__new__ExprSymbolTuple(struct Expr expr, struct Vec *tuple);

/**
 *
 * @brief Construct the ExprSymbol type (Array variant).
 */
struct ExprSymbol *
__new__ExprSymbolArray(struct Expr expr, struct Vec *array);

/**
 *
 * @brief Construct the ExprSymbol type (Variant variant).
 */
struct ExprSymbol *
__new__ExprSymbolVariant(struct Expr expr, struct VariantSymbol variant);

/**
 *
 * @brief Construct the ExprSymbol type (Try variant).
 */
struct ExprSymbol *
__new__ExprSymbolTry(struct Expr expr, struct ExprSymbol *try);

/**
 *
 * @brief Construct the ExprSymbol type (Block variant).
 */
struct ExprSymbol *
__new__ExprSymbolBlock(struct Expr expr, struct Vec *block);

/**
 *
 * @brief Construct the ExprSymbol type (QuestionMark variant).
 */
struct ExprSymbol *
__new__ExprSymbolQuestionMark(struct Expr expr, struct Scope question_mark);

/**
 *
 * @brief Construct the ExprSymbol type (Dereference variant).
 */
struct ExprSymbol *
__new__ExprSymbolDereference(struct Expr expr, struct Scope dereference);

/**
 *
 * @brief Construct the ExprSymbol type (Ref variant).
 */
struct ExprSymbol *
__new__ExprSymbolRef(struct Expr expr, struct Scope ref);

/**
 *
 * @brief Construct the ExprSymbol type (Literal variant).
 */
struct ExprSymbol *
__new__ExprSymbolLiteral(struct Expr expr, struct LiteralSymbol literal);

/**
 *
 * @brief Construct the ExprSymbol type (Variable variant).
 */
struct ExprSymbol *
__new__ExprSymbolVariable(struct Expr expr, struct VariableSymbol variable);

/**
 *
 * @brief Free the ExprSymbol type.
 */
void
__free__ExprSymbol(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Unary variant).
 */
void
__free__ExprSymbolUnaryOp(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Binary variant).
 */
void
__free__ExprSymbolBinaryOp(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (FunCall variant).
 */
void
__free__ExprSymbolFunCall(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (RecordCall variant).
 */
void
__free__ExprSymbolRecordCall(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Identifier variant).
 */
void
__free__ExprSymbolIdentifier(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (IdentifierAccess variant).
 */
void
__free__ExprSymbolIdentifierAccess(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (ArrayAccess variant).
 */
void
__free__ExprSymbolArrayAccess(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (TupleAccess variant).
 */
void
__free__ExprSymbolTupleAccess(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Lambda variant).
 */
void
__free__ExprSymbolLambda(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Tuple variant).
 */
void
__free__ExprSymbolTuple(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Array variant).
 */
void
__free__ExprSymbolArray(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Variant variant).
 */
void
__free__ExprSymbolVariant(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Try variant).
 */
void
__free__ExprSymbolTry(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Block variant).
 */
void
__free__ExprSymbolBlock(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (QuestionMark variant).
 */
void
__free__ExprSymbolQuestionMark(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Dereference variant).
 */
void
__free__ExprSymbolDereference(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Ref variant).
 */
void
__free__ExprSymbolRef(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Literal variant).
 */
void
__free__ExprSymbolLiteral(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Variable variant).
 */
void
__free__ExprSymbolVariable(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (all variants).
 */
void
__free__ExprSymbolAll(struct ExprSymbol *self);

typedef struct MatchSymbol
{
    struct ExprSymbol *matching;
    struct Vec *pattern; // struct Vec<struct Tuple<struct ExprSymbol*, struct
                         // Vec<struct SymbolTable*>*>*
} MatchSymbol;

/**
 *
 * @brief Construct the MatchSymbol type.
 */
struct MatchSymbol
__new__MatchSymbol(struct ExprSymbol *matching, struct Vec *pattern);

/**
 *
 * @brief Free the MatchSymbol type.
 */
void
__free_MatchSymbol(struct MatchSymbol self);

typedef struct IfBranchSymbol
{
    struct ExprSymbol *cond;
    struct Vec *body; // struct Vec<struct SymbolTable*>*
} IfBranchSymbol;

/**
 *
 * @brief Construct the IfBranchSymbol type.
 */
struct IfBranchSymbol *
__new__IfBranchSymbol(struct ExprSymbol *cond, struct Vec *body);

/**
 *
 * @brief Free the IfBranchSymbol type.
 */
void
__free__IfBranchSymbol(struct IfBranchSymbol *self);

typedef struct IfCondSymbol
{
    struct IfBranchSymbol *if_;
    struct Vec *elif; // struct Vec<struct IfBranchSymbol*>*
    struct IfBranchSymbol *else_;
} IfCondSymbol;

/**
 *
 * @brief Construct the IfCondSymbol type.
 */
struct IfCondSymbol
__new__IfCondSymbol(struct IfBranchSymbol *if_,
                    struct Vec *elif,
                    struct IfBranchSymbol *else_);

/**
 *
 * @brief Free the IfCondSymbol type.
 */
void
__free__IfCondSymbol(struct IfCondSymbol self);

typedef struct TrySymbol
{
    struct ExprSymbol *try_expr;
    struct Vec *try_body; // struct Vec<struct SymbolTable*>*
    struct ExprSymbol *catch_expr;
    struct Vec *catch_body; // struct Vec<struct SymbolTable*>*
} TrySymbol;

/**
 *
 * @brief Construct the TrySymbol type.
 */
struct TrySymbol
__new__TrySymbol(struct ExprSymbol *try_expr,
                 struct Vec *try_body,
                 struct ExprSymbol *catch_expr,
                 struct Vec *catch_body);

/**
 *
 * @brief Free the TrySymbol type.
 */
void
__free__TrySymbol(struct TrySymbol self);

typedef struct WhileSymbol
{
    struct ExprSymbol *cond;
    struct Vec *body; // struct Vec<struct SymbolTable*>*
} WhileSymbol;

/**
 *
 * @brief Construct the WhileSymbol type.
 */
struct WhileSymbol
__new__WhileSymbol(struct ExprSymbol *cond, struct Vec *body);

/**
 *
 * @brief Free the WhileSymbol type.
 */
void
__free__WhileSymbol(struct WhileSymbol self);

typedef struct StmtSymbol
{
    enum StmtKind kind;
    struct Location loc;

    union
    {
        struct IfCondSymbol if_;
        struct ExprSymbol *await;
        struct TrySymbol *try;
        struct MatchSymbol match;
        struct WhileSymbol while_;
    } value;
} StmtSymbol;

enum SymbolTableKind
{
    SymbolTableKindFun,
    SymbolTableKindConstant,
    SymbolTableKindModule,
    SymbolTableKindAlias,
    SymbolTableKindRecord,
    SymbolTableKindEnum,
    SymbolTableKindError,
    SymbolTableKindClass,
    SymbolTableKindTrait,
    SymbolTableKindRecordObject,
    SymbolTableKindEnumObject,
    SymbolTableKindTag,
    SymbolTableKindVariable,
    SymbolTableKindExpr
};

typedef struct SymbolTable
{
    enum SymbolTableKind kind;
    struct Location loc;

    union
    {
        struct FunSymbol *fun;
        struct ExprSymbol *expr;
    } value;
} SymbolTable;

/**
 *
 * @brief Construct the SymbolTable type (Fun variant).
 */
struct SymbolTable *
__new__SymbolTableFun(struct FunSymbol *fun, struct Decl *fun_decl);

/**
 *
 * @brief Construct the SymbolTable type (Expr variant).
 */
struct SymbolTable *
__new__SymbolTableExpr(struct ExprSymbol *expr, struct Expr *expr_ast);

/**
 *
 * @brief Free the SymbolTable type (Fun variant).
 */
void
__free__SymbolTableFun(struct SymbolTable *self);

/**
 *
 * @brief Free the SymbolTable type (Expr variant).
 */
void
__free__SymbolTableExpr(struct SymbolTable *self);

/**
 *
 * @brief Free the SymbolTable type (all variants).
 */
void
__free__SymbolTableAll(struct SymbolTable *self);

#endif // LILY_SYMBOL_TABLE_H
