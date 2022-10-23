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

#include <assert.h>
#include <base/format.h>
#include <base/new.h>
#include <base/str.h>
#include <stdarg.h>
#include <string.h>

struct String *
format(Str fmt, ...)
{
    struct String *s = NEW(String);
    va_list vl;

    va_start(vl, fmt);

    for (Usize i = 0; i < strlen(fmt); i++) {
        switch (fmt[i]) {
            case '{':
                assert(i++ < strlen(fmt) && "`{` is not closed");
                switch (fmt[i]) {
                    case 'b': {
                        Str bool_str = of_bool__Str(va_arg(vl, int));
                        push_str__String(s, bool_str);
                        break;
                    }
                    case 's': {
                        Str str = va_arg(vl, char *);
                        push_str__String(s, str);

                        // Str allocated
                        if (fmt[i + 1] == 'a') {
                            i++;
                            free(str);
                        }

                        break;
                    }
                    case 'S': {
                        struct String *string = va_arg(vl, struct String *);

                        // String raw
                        if (fmt[i + 1] == 'r') {
                            i++;
                            append__String(s, string, true);
                        } else {
                            append__String(s, string, false);
                        }

                        break;
                    }
                    case 'L': {
                        if (fmt[i + 1] == 'u') {
                            i++;
                            Str u64_str = of_u64__Str(va_arg(vl, UInt64));
                            push_str__String(s, u64_str);
                            free(u64_str);
                            break;
                        } else {
                            Str i64_str = of_i64__Str(va_arg(vl, Int64));
                            push_str__String(s, i64_str);
                            free(i64_str);
                            break;
                        }
                    }
                    case 'd': {
                        Str int_str = of_int__Str(va_arg(vl, int));
                        push_str__String(s, int_str);
                        free(int_str);
                        break;
                    }
                    case 'f': {
                        Str double_str = of_double__Str(va_arg(vl, double));
                        push_str__String(s, double_str);
                        free(double_str);
                        break;
                    }
                    case 'c': {
                        Str c_str = of_char__Str((char)va_arg(vl, int));
                        push_str__String(s, c_str);
                        free(c_str);
                        break;
                    }
                    case '{': {
                        push__String(s, (char *)'{');
                        break;
                    }
                    default:
                        assert(0 && "invalid format specifier");
                }
                i++;
                if (fmt[i - 1] != '{')
                    assert(fmt[i] == '}' && "expected `}`");
                break;
            default:
                push__String(s, (char *)(UPtr)fmt[i]);
        }
    }

    va_end(vl);

    return s;
}
