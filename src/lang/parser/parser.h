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

enum ParseContextKind
{
    ParseContextKindFun
};

typedef struct ParseContext
{
    enum ParseContextKind kind;

    union
    {
        struct FunParseContext *fun;
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
 * @brief Free the ParseContext type (Fun variant).
 */
void
__free__ParseContextFun(struct ParseContext *self);

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
