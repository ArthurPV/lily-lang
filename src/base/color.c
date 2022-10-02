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

#include <base/color.h>
#include <stdio.h>

Str
red__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[31m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[31m%s\x1b[0m", s);
    return res;
}

Str
green__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[32m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[32m%s\x1b[0m", s);
    return res;
}

Str
yellow__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[33m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[33m%s\x1b[0m", s);
    return res;
}

Str
blue__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[34m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[34m%s\x1b[0m", s);
    return res;
}

Str
magenta__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[35m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[35m%s\x1b[0m", s);
    return res;
}

Str
cyan__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[36m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[36m%s\x1b[0m", s);
    return res;
}

Str
bold__Style(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[1m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[1m%s\x1b[0m", s);
    return res;
}
