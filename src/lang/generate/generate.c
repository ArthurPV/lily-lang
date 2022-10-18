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

#include <base/file.h>
#include <base/platform.h>
#include <base/macros.h>
#include <lang/generate/generate.h>
#include <stdio.h>

struct Generate
__new__Generate(struct Typecheck tc)
{
    struct Generate self = { .output = NEW(String), .tc = tc };

    return self;
}

void
write_on_file__Generate(struct Generate self)
{
    Usize size =
      snprintf(
        NULL, 0, "%s.c", self.tc.parser.parse_block.scanner.src->file.name) +
      1;
    Str fw = malloc(size);

    snprintf(
      fw, size, "%s.c", self.tc.parser.parse_block.scanner.src->file.name);

    {
        struct Path *pw = NEW(Path, fw);

        Str output_str = to_Str__String(*self.output);

        write_file__Path(*pw, output_str);

        FREE(Path, pw);
        free(output_str);
    }

    Usize exe_size = snprintf(NULL, 0, "%s", fw) - 6;
    Str exe = malloc(exe_size);

    snprintf(exe, exe_size, "%s", fw);

#ifdef LILY_WINDOWS_OS
#else
#ifdef LOCAL
    {
        Usize command_size =
          snprintf(NULL,
                   0,
                   "./scripts/compile.sh %s src/lang/runtime/c "
                   "build/src/lang/runtime/c/liblily_runtime_c.so %s",
                   exe) +
          1;
		Str command = malloc(command_size);

		setenv("LD_LIBRARY_PATH=./lily_cache/lib");
		snprintf(command, command_size, "./scripts/compile.sh %s src/lang/runtime/c "
                   "build/src/lang/runtime/c/liblily_runtime_c.so %s",
                   exe);

        system(command);
    }
#endif
#ifdef INSTALL
    {
        system("");
    }
#endif
#endif

    free(fw);
}

void
__free__Generate(struct Generate self)
{
    FREE(String, self.output);
    FREE(Typecheck, self.tc);
}
