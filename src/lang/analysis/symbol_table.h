#ifndef LILY_SYMBOL_TABLE_H
#define LILY_SYMBOL_TABLE_H

#include <lang/parser/ast.h>

typedef struct Scope
{
    Str filename;
    Usize id;
    Usize sub_id;
} ScopdId;

/**
 *
 * @brief Construct the Scope type.
 */
struct Scope
__new__Scope(const Str filename, Usize id);

enum Visibility
{
    VisibilityPublic,
    VisibilityPrivate
};

typedef struct FunSymbol
{
    struct String *name;        // struct String&
    struct Vec *taged_type;     // struct Vec<struct SymbolTable*>*
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *params;         // struct Vec<struct FunParam*>&
    enum Visibility visibility;
    bool is_async;
    struct DataType *return_type;
    struct Vec *body; // struct Vec<struct SymbolTable*>*
    struct Scope scope;
    struct Decl *fun_decl; // struct Decl&
} FunSymbol;

/**
 *
 * @brief Construct the FunSymbol type.
 */
struct FunSymbol *
__new__FunSymbol(struct Decl *fun_decl, const Str filename, Usize id);

/**
 *
 * @brief Free the FunSymbol type.
 */
void
__free__FunSymbol(struct FunSymbol *self);

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
 * @brief Free the BinaryOpSymbol type.
 */
void
__free__BinaryOpSymbol(struct BinaryOpSymbol self);

typedef struct FunCallSymbol
{
    struct Scope loc;
    struct DataType *data_type;
    struct Vec *params; // struct Vec<struct Tuple<struct SymbolFunParamCall*,
                        // struct Location&>*>*
} FunCallSymbol;

typedef struct ExprSymbol
{
    enum ExprKind kind;
    struct Location loc;

    union
    {
        struct UnaryOpSymbol unary_op;
        struct BinaryOpSymbol binary_op;
        struct FunCallSymbol fun_call;
    } value;
} ExprSymbol;

/**
 *
 * @brief Construct the ExprSymbol type (Unary variant).
 */
struct ExprSymbol
__new__ExprSymbolUnary(struct Expr expr, struct UnaryOpSymbol unary_op);

/**
 *
 * @brief Free the ExprSymbol type (Unary variant).
 */
void
__free__ExprSymbolUnary(struct ExprSymbol self);

/**
 *
 * @brief Free the ExprSymbol type (all variants).
 */
void
__free__ExprSymbolAll(struct ExprSymbol self);

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
 * @brief Free the SymbolTable type (Fun variant).
 */
void
__free__SymbolTableFun(struct SymbolTable *self);

/**
 *
 * @brief Free the SymbolTable type (all variants).
 */
void
__free__SymbolTableAll(struct SymbolTable *self);

#endif // LILY_SYMBOL_TABLE_H