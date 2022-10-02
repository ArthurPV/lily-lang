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

#ifndef LILY_FILE_H
#define LILY_FILE_H

#include <base/string.h>
#include <base/types.h>
#include <stdbool.h>

typedef struct Path
{
    struct String *path;
} Path;

/**
 *
 * @param path
 * @return struct Path*
 */
struct Path *
__new__Path(Str path);

/**
 *
 * @brief Determine if the path is a directory.
 * @param self
 * @return bool
 */
bool
is_directory__Path(struct Path self);

/**
 *
 * @brief Get the extension of file.
 * @param self
 * @return Str
 */
Str
get_extension__Path(struct Path self);

/**
 *
 * @brief The function try to read the file.
 * @param self
 * @return char*
 */
struct String *
read_file__Path(struct Path self);

/**
 *
 * @brief The function try to write on file.
 * @param self
 * @return void
 */
void
write_file__Path(struct Path self, const Str content);

/**
 *
 * @brief Free Path type.
 */
void
__free__Path(struct Path *self);

#endif // LILY_FILE_H
