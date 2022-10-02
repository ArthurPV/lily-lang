#ifndef LILY_SCANNER_H
#define LILY_SCANNER_H

#include <base/types.h>
#include <base/vec.h>
#include <lang/scanner/token.h>

#define HIDDEN_UNUSED_COMMENT // CommentOne and CommentMulti

typedef struct File
{
    Str name;
    struct String *content;
} File;

typedef struct Source
{
    struct File file;
    Usize pos;
    char *c;
} Source;

typedef struct Scanner
{
    struct Source *src;
    UInt64 line;
    UInt64 col;
    struct Location loc;
    struct Vec *tokens;
    Usize count_error;
} Scanner;

/**
 *
 * @brief Construct File type.
 */
struct File
__new__File(Str name);

/**
 *
 * @brief Free File type.
 */
void
__free__File(struct File self);

/**
 *
 * @brief Construct Source type.
 */
struct Source
__new__Source(struct File file);

/**
 *
 * @brief Free Source type.
 */
void
__free__Source(struct Source self);

/**
 *
 * @brief Construct Scanner type.
 */
struct Scanner
__new__Scanner(struct Source *src);

/**
 *
 * @brief Run scanner phase.
 */
void
run__Scanner(struct Scanner *self);

/**
 *
 * @brief Free Scanner type.
 */
void
__free__Scanner(struct Scanner self);

#endif // LILY_SCANNER_H