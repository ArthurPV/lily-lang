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
#include <base/format.h>
#include <base/new.h>
#include <base/print.h>
#include <base/string.h>
#include <lang/diagnostic/summary.h>

void
emit__Summary(Int32 count_error, Int32 count_warning, const Str msg)
{
    Str error = NULL;
    Str warning = NULL;

    if (count_error > 1)
        error = "errors";
    else
        error = "error";

    if (count_warning > 1)
        warning = "warnings";
    else
        warning = "warning";

    struct String *s = format("\n{sa}: {s} with {d} {sa} and {d} {sa}.",
                              CYAN("Summary"),
                              msg,
                              count_error,
                              RED(error),
                              count_warning,
                              YELLOW(warning));

    Println("{Sr}", s);
}
