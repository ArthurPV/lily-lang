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

#ifndef LILY_PARSER_H
#define LILY_PARSER_H

#include <lang/parser/ast.h>
#include <lang/scanner/scanner.h>

typedef struct ParseBlock
{
    struct Scanner scanner;
    struct Vec *blocks;    // struct Vec<struct Vec<struct ParseContext*>*>*
    struct Token *current; // struct Token&
    struct Vec *disable_warning; // struct Vec<Str>*
    Usize pos;
} ParseBlock;

/**
 *
 * @brief Construct the ParseBlock type.
 */
struct ParseBlock
__new__ParseBlock(struct Scanner scanner);

/**
 *
 * @brief Run parse block.
 */
void
run__ParseBlock(struct ParseBlock *self);

/**
 *
 * @brief Free the ParseBlock type.
 */
void
__free__ParseBlock(struct ParseBlock self);

typedef struct FunParseContext
{
    bool is_pub;
    bool is_async;
    bool has_generic_params;
    bool has_tag;
    bool has_tags;
    bool has_params;
    bool has_return_type;
    bool is_operator;
    bool in_tag;
    struct String *name;        // struct String&
    struct Vec *tags;           // struct Vec<struct Token&>*
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *params;         // struct Vec<struct Token&>*
    struct Vec *return_type;    // struct Vec<struct Token&>*
    struct Vec *body;           // struct Vec<struct Token&>*
} FunParseContext;

/**
 *
 * @brief Construct the FunParseContext type.
 */
struct FunParseContext
__new__FunParseContext();

/**
 *
 * @brief Free the FunParseContext type.
 */
void
__free__FunParseContext(struct FunParseContext self);

typedef struct EnumParseContext
{
    bool is_pub;
    bool has_generic_params;
    bool has_data_type;
    bool is_error;
    struct String *name;        // struct String&
    struct Vec *data_type;      // struct Vec<struct Token&>*
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *variants;       // struct Vec<struct Token&>*
} EnumParseContext;

/**
 *
 * @brief Construct the EnumParseContext type.
 */
struct EnumParseContext
__new__EnumParseContext();

/**
 *
 * @brief Free the EnumParseContext type.
 */
void
__free__EnumParseContext(struct EnumParseContext self);

typedef struct RecordParseContext
{
    bool is_pub;
    bool has_generic_params;
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *fields;         // struct Vec<struct Token&>*
} RecordParseContext;

/**
 *
 * @brief Construct the RecordParseContext type.
 */
struct RecordParseContext
__new__RecordParseContext();

/**
 *
 * @brief Free the RecordParseContext type.
 */
void
__free__RecordParseContext(struct RecordParseContext self);

typedef struct AliasParseContext
{
    bool is_pub;
    bool has_generic_params;
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *data_type;      // struct Vec<struct Token&>*
} AliasParseContext;

/**
 *
 * @brief Construct the AliasParseContext type.
 */
struct AliasParseContext
__new__AliasParseContext();

/**
 *
 * @brief Free the AliasParseContext type.
 */
void
__free__AliasParseContext(AliasParseContext self);

typedef struct TraitParseContext
{
    bool is_pub;
    bool has_generic_params;
    bool has_inheritance;
    struct String *name;        // struct String&
    struct Vec *inheritance;    // struct Vec<struct Token&>*
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *body;           // struct Vec<struct Token&>*
} TraitParseContext;

/**
 *
 * @brief Contruct the TraitParseContext type.
 */
struct TraitParseContext
__new__TraitParseContext();

/**
 *
 * @brief Free the TraitParseContext type.
 */
void
__free__TraitParseContext(struct TraitParseContext self);

typedef struct ClassParseContext
{
    bool is_pub;
    bool has_generic_params;
    bool has_inheritance;
    bool has_impl;
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *inheritance;    // struct Vec<struct Token&>*
    struct Vec *impl;           // struct Vec<struct Token&>*
    struct Vec *body;           // struct Vec<struct ParseContext*>*
} ClassParseContext;

/**
 *
 * @brief Contruct the ClassParseContext type.
 */
struct ClassParseContext
__new__ClassParseContext();

/**
 *
 * @brief Free the ClassParseContext type.
 */
void
__free__ClassParseContext(struct ClassParseContext self);

typedef struct TagParseContext
{
    bool has_generic_params;
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *body;           // struct Vec<struct Token&>*
} TagParseContext;

/**
 *
 * @brief Contruct the TagParseContext type.
 */
struct TagParseContext
__new__TagParseContext();

/**
 *
 * @brief Free the TagParseContext type.
 */
void
__free__TagParseContext(struct TagParseContext self);

typedef struct MethodParseContext
{
    bool is_pub;
    bool is_async;
    bool has_generic_params;
    bool has_params;
    bool has_return_type;
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *params;         // struct Vec<struct Token&>*
    struct Vec *return_type;    // struct Vec<struct Token&>*
    struct Vec *body;           // struct Vec<struct Token&>*
} MethodParseContext;

/**
 *
 * @brief Contruct the MethodParseContext type.
 */
struct MethodParseContext
__new__MethodParseContext();

/**
 *
 * @brief Free the MethodParseContext type.
 */
void
__free__MethodParseContext(struct MethodParseContext self);

typedef struct PropertyParseContext
{
    bool is_pub;
    struct String *name;   // struct String&
    struct Vec *data_type; // struct Vec<struct Token&>*
} PropertyParseContext;

/**
 *
 * @brief Contruct the PropertyParseContext type.
 */
struct PropertyParseContext
__new__PropertyParseContext();

/**
 *
 * @brief Free the PropertyParseContext type.
 */
void
__free__PropertyParseContext(struct PropertyParseContext self);

typedef struct ImportParseContext
{
    bool is_pub;
    struct String *value; // struct String&
    struct Location value_loc;
    struct String *as_value; // struct String& (Optional value)
} ImportParseContext;

/**
 *
 * @brief Contruct the ImportParseContext type.
 */
struct ImportParseContext
__new__ImportParseContext();

typedef struct ConstantParseContext
{
    bool is_pub;
    struct String *name;   // struct String&
    struct Vec *data_type; // struct Vec<struct Token&>*
    struct Vec *expr;      // struct Vec<struct Token&>*
} ConstantParseContext;

/**
 *
 * @brief Construct the ConstantParseContext type.
 */
struct ConstantParseContext
__new__ConstantParseContext();

/**
 *
 * @brief Free the ConstantParseContext type.
 */
void
__free__ConstantParseContext(struct ConstantParseContext self);

typedef struct ErrorParseContext
{
    bool is_pub;
    bool has_generic_params;
    bool has_data_type;
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *data_type;      // struct Vec<struct Token&>*
} ErrorParseContext;

/**
 *
 * @brief Construct the ErrorParseContext type.
 */
struct ErrorParseContext
__new__ErrorParseContext();

/**
 *
 * @brief Free the ErrorParseContext type.
 */
void
__free__ErrorParseContext(struct ErrorParseContext self);

typedef struct ModuleParseContext
{
    bool is_pub;
    struct String *name; // struct String&
    struct Vec *body;    // struct Vec<struct ParseContext*>*
} ModuleParseContext;

/**
 *
 * @brief Construct the ModuleParseContext type.
 */
struct ModuleParseContext
__new__ModuleParseContext();

/**
 *
 * @brief Free the ModuleParseContext type.
 */
void
__free__ModuleParseContext(struct ModuleParseContext self);

enum ParseContextKind
{
    ParseContextKindFun,
    ParseContextKindEnum,
    ParseContextKindRecord,
    ParseContextKindAlias,
    ParseContextKindEnumObject,
    ParseContextKindRecordObject,
    ParseContextKindTrait,
    ParseContextKindClass,
    ParseContextKindMethod,
    ParseContextKindProperty,
    ParseContextKindImport,
    ParseContextKindConstant,
    ParseContextKindError,
    ParseContextKindModule,
    ParseContextKindTag
};

typedef struct ParseContext
{
    enum ParseContextKind kind;
    struct Location loc;

    union
    {
        struct FunParseContext fun;
        struct EnumParseContext enum_;
        struct RecordParseContext record;
        struct AliasParseContext alias;
        struct TraitParseContext trait;
        struct ClassParseContext class;
        struct MethodParseContext method;
        struct PropertyParseContext property;
        struct ImportParseContext import;
        struct ConstantParseContext constant;
        struct ErrorParseContext error;
        struct ModuleParseContext module;
        struct TagParseContext tag;
    } value;
} ParseContext;

/**
 *
 * @brief Contruct the ParseContext type (Fun variant).
 */
struct ParseContext *
__new__ParseContextFun(struct FunParseContext fun, struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Enum variant).
 */
struct ParseContext *
__new__ParseContextEnum(struct EnumParseContext enum_,
                        struct Location loc,
                        bool is_object);

/**
 *
 * @brief Contruct the ParseContext type (Record variant).
 */
struct ParseContext *
__new__ParseContextRecord(struct RecordParseContext record,
                          struct Location loc,
                          bool is_object);

/**
 *
 * @brief Contruct the ParseContext type (Alias variant).
 */
struct ParseContext *
__new__ParseContextAlias(struct AliasParseContext alias, struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Trait variant).
 */
struct ParseContext *
__new__ParseContextTrait(struct TraitParseContext trait, struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Class variant).
 */
struct ParseContext *
__new__ParseContextClass(struct ClassParseContext class, struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Method variant).
 */
struct ParseContext *
__new__ParseContextMethod(struct MethodParseContext method,
                          struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Property variant).
 */
struct ParseContext *
__new__ParseContextProperty(struct PropertyParseContext property,
                            struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Import variant).
 */
struct ParseContext *
__new__ParseContextImport(struct ImportParseContext import,
                          struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Constant variant).
 */
struct ParseContext *
__new__ParseContextConstant(struct ConstantParseContext constant,
                            struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Error variant).
 */
struct ParseContext *
__new__ParseContextError(struct ErrorParseContext error, struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Module variant).
 */
struct ParseContext *
__new__ParseContextModule(struct ModuleParseContext module,
                          struct Location loc);

/**
 *
 * @brief Contruct the ParseContext type (Tag variant).
 */
struct ParseContext *
__new__ParseContextTag(struct TagParseContext tag, struct Location loc);

/**
 *
 * @brief Free the ParseContext type (Fun variant).
 */
void
__free__ParseContextFun(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Enum variant).
 */
void
__free__ParseContextEnum(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Record variant).
 */
void
__free__ParseContextRecord(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Alias variant).
 */
void
__free__ParseContextAlias(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Trait variant).
 */
void
__free__ParseContextTrait(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Class variant).
 */
void
__free__ParseContextClass(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Method variant).
 */
void
__free__ParseContextMethod(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Property variant).
 */
void
__free__ParseContextProperty(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Property variant).
 */
void
__free__ParseContextImport(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Constant variant).
 */
void
__free__ParseContextConstant(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Error variant).
 */
void
__free__ParseContextError(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Module variant).
 */
void
__free__ParseContextModule(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (Tag variant).
 */
void
__free__ParseContextTag(struct ParseContext *self);

/**
 *
 * @brief Free the ParseContext type (all variants).
 */
void
__free__ParseContextAll(struct ParseContext *self);

typedef struct ParseDecl // Parse for all declaration except for class body
{
    Usize pos;
    struct Token *current;  // struct Token&
    struct Token *previous; // struct Token&
    struct Vec *tokens;     // struct Vec<struct Token&>*
} ParseDecl;

/**
 *
 * @brief Construct the ParseDecl type.
 */
struct ParseDecl
__new__ParseDecl(struct Vec *tokens);

typedef struct ParseClassBody
{
    Usize pos;
    struct ParseContext *current; // struct ParseContext&
    struct Vec *blocks;           // struct Vec<struct ParseContext*>*
} ParseClassBody;

/**
 *
 * @brief Construct the ParseDecl type.
 */
struct ParseClassBody
__new__ParseClassBody(struct Vec *blocks);

typedef struct Parser
{
    struct ParseBlock parse_block;
    Usize pos;
    struct ParseContext *current;
    struct Vec *decls; // struct Vec<struct Decl*>*
} Parser;

/**
 *
 * @brief Construct the Parser type.
 */
struct Parser
__new__Parser(struct ParseBlock parse_block);

/**
 *
 * @brief Run parser.
 */
void
run__Parser(struct Parser *self);

/**
 *
 * @brief Free the Parser type.
 */
void
__free__Parser(struct Parser self);

#endif // LILY_PARSER_H
