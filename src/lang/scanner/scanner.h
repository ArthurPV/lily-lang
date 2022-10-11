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

#ifndef LILY_SCANNER_H
#define LILY_SCANNER_H

#include <base/types.h>
#include <base/vec.h>
#include <lang/scanner/token.h>

#ifndef LILY_FORMAT_FILE      // Tool to format lily file
#define HIDDEN_UNUSED_COMMENT // CommentOne and CommentMulti
#endif

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
