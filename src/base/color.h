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

#ifndef LILY_COLOR_H
#define LILY_COLOR_H

#include <base/types.h>

#define RED(s) red__Color(s)
#define GREEN(s) green__Color(s)
#define YELLOW(s) yellow__Color(s)
#define BLUE(s) blue__Color(s)
#define MAGENTA(s) magenta__Color(s)
#define CYAN(s) cyan__Color(s)
#define BOLD(s) bold__Style(s)

/**
 *
 * @return the formatted Str in red color.
 */
Str
red__Color(Str s);

/**
 *
 * @return the formatted Str in green color.
 */
Str
green__Color(Str s);

/**
 *
 * @return the formatted Str in yellow color.
 */
Str
yellow__Color(Str s);

/**
 * @return the formatted Str in blue color.
 */
Str
blue__Color(Str s);

/**
 *
 * @return the formatted Str in magenta color.
 */
Str
magenta__Color(Str s);

/**
 * @return the formatted Str in cyan color.
 */
Str
cyan__Color(Str s);

/**
 * @return the formatted Str in bold style.
 */
Str
bold__Style(Str s);

#endif // LILY_COLOR_H
