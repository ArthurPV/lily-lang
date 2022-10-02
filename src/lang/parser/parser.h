#ifndef LILY_PARSER_H
#define LILY_PARSER_H

#include <lang/parser/ast.h>
#include <lang/scanner/scanner.h>

typedef struct ParseBlock
{
    struct Scanner *scanner;
    struct Vec *blocks;    // struct Vec<struct Vec<struct ParseContext*>*>*
    struct Token *current; // struct Token&
    struct Vec *disable_warning; // struct Vec<Str>*
    Usize pos;
    Usize count_error;
    Usize count_warning;
} ParseBlock;

/**
 *
 * @brief Construct the ParseBlock type.
 */
struct ParseBlock *
__new__ParseBlock(struct Scanner *scanner);

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
__free__ParseBlock(struct ParseBlock *self);

typedef struct FunParseContext
{
    bool is_pub;
    bool is_async;
    bool has_generic_params;
    bool has_tag;
    bool has_tags;
    bool has_params;
    struct String *name;        // struct String&
    struct Vec *tags;           // struct Vec<struct Token&>*
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *params;         // struct Vec<struct Token&>*
    struct Vec *body;           // struct Vec<struct Token&>*
} FunParseContext;

/**
 *
 * @brief Construct the FunParseContext type.
 */
struct FunParseContext *
__new__FunParseContext();

/**
 *
 * @brief Free the FunParseContext type.
 */
void
__free__FunParseContext(struct FunParseContext *self);

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
struct EnumParseContext *
__new__EnumParseContext();

/**
 *
 * @brief Free the EnumParseContext type.
 */
void
__free__EnumParseContext(struct EnumParseContext *self);

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
struct RecordParseContext *
__new__RecordParseContext();

/**
 *
 * @brief Free the RecordParseContext type.
 */
void
__free__RecordParseContext(struct RecordParseContext *self);

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
struct AliasParseContext *
__new__AliasParseContext();

/**
 *
 * @brief Free the AliasParseContext type.
 */
void
__free__AliasParseContext(AliasParseContext *self);

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
struct TraitParseContext *
__new__TraitParseContext();

/**
 *
 * @brief Free the TraitParseContext type.
 */
void
__free__TraitParseContext(struct TraitParseContext *self);

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
struct ClassParseContext *
__new__ClassParseContext();

/**
 *
 * @brief Free the ClassParseContext type.
 */
void
__free__ClassParseContext(struct ClassParseContext *self);

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
struct TagParseContext *
__new__TagParseContext();

/**
 *
 * @brief Free the TagParseContext type.
 */
void
__free__TagParseContext(struct TagParseContext *self);

typedef struct MethodParseContext
{
    bool is_pub;
    bool is_async;
    bool has_generic_params;
    bool has_params;
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Token&>*
    struct Vec *params;         // struct Vec<struct Token&>*
    struct Vec *body;           // struct Vec<struct Token&>*
} MethodParseContext;

/**
 *
 * @brief Contruct the MethodParseContext type.
 */
struct MethodParseContext *
__new__MethodParseContext();

/**
 *
 * @brief Free the MethodParseContext type.
 */
void
__free__MethodParseContext(struct MethodParseContext *self);

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
struct PropertyParseContext *
__new__PropertyParseContext();

/**
 *
 * @brief Free the PropertyParseContext type.
 */
void
__free__PropertyParseContext(struct PropertyParseContext *self);

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
    ParseContextKindProperty
};

typedef struct ParseContext
{
    enum ParseContextKind kind;

    union
    {
        struct FunParseContext *fun;
        struct EnumParseContext *enum_;
        struct RecordParseContext *record;
        struct AliasParseContext *alias;
        struct TraitParseContext *trait;
        struct ClassParseContext *class;
        struct MethodParseContext *method;
        struct PropertyParseContext *property;
    } value;
} ParseContext;

/**
 *
 * @brief Contruct the ParseContext type (Fun variant).
 */
struct ParseContext *
__new__ParseContextFun(struct FunParseContext *fun);

/**
 *
 * @brief Contruct the ParseContext type (Enum variant).
 */
struct ParseContext *
__new__ParseContextEnum(struct EnumParseContext *enum_, bool is_object);

/**
 *
 * @brief Contruct the ParseContext type (Record variant).
 */
struct ParseContext *
__new__ParseContextRecord(struct RecordParseContext *record, bool is_object);

/**
 *
 * @brief Contruct the ParseContext type (Alias variant).
 */
struct ParseContext *
__new__ParseContextAlias(struct AliasParseContext *alias);

/**
 *
 * @brief Contruct the ParseContext type (Trait variant).
 */
struct ParseContext *
__new__ParseContextTrait(struct TraitParseContext *trait);

/**
 *
 * @brief Contruct the ParseContext type (Class variant).
 */
struct ParseContext *
__new__ParseContextClass(struct ClassParseContext *class);

/**
 *
 * @brief Contruct the ParseContext type (Method variant).
 */
struct ParseContext *
__new__ParseContextMethod(struct MethodParseContext *method);

/**
 *
 * @brief Contruct the ParseContext type (Property variant).
 */
struct ParseContext *
__new__ParseContextProperty(struct PropertyParseContext *property);

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
 * @brief Free the ParseContext type (all variants).
 */
void
__free__ParseContextAll(struct ParseContext *self);

typedef struct Parser
{
    struct ParseBlock *parse_block;
} Parser;

/**
 *
 * @brief Construct the Parser type.
 */
struct Parser *
__new__Parser(struct ParseBlock *parse_block);

/**
 *
 * @brief Run parser.
 */
void
run__Parser(struct Parser *self);

void
run_without_multi_thread__Parser(struct Parser *self);

/**
 *
 * @brief Free the Parser type.
 */
void
__free__Parser(struct Parser *self);

#endif // LILY_PARSER_H
