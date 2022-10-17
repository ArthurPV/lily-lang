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

#include <stdlib.h>
#include <string.h>

#include <base/platform.h>
#include <base/print.h>
#include <base/util.h>
#include <command/command.h>
#include <command/help.h>
#include <lang/analysis/typecheck.h>
#include <lang/generate/generate.h>
#include <lang/generate/generate_c.h>
#include <lang/parser/parser.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

#if defined(__clang__)
#if __clang_major__ < 9
#error "Use Clang v9.0 or later"
#endif
#elif defined(__GNUC__)
#if __GNUC__ < 8
#error "Use GCC v8.0 or later"
#endif
#else
#error "unknown C compiler"
#endif

int
main(int argc, char **argv)
{
    int status = -1;

    if (argc > 1) {
        if (!strcmp(argv[1], "build")) {
            status = BUILD_COMMAND;
        } else if (!strcmp(argv[1], "compile")) {
            status = COMPILE_COMMAND;
        } else if (!strcmp(argv[1], "help")) {
            status = HELP_COMMAND;
        } else if (!strcmp(argv[1], "init")) {
            status = INIT_COMMAND;
        } else if (!strcmp(argv[1], "new")) {
            status = NEW_COMMAND;
        } else if (!strcmp(argv[1], "version")) {
            status = VERSION_COMMAND;
        }
    }

    switch (status) {
        case BUILD_COMMAND:
            Println("Build");
            break;
        case COMPILE_COMMAND:
            if (argc > 2) {
#ifdef LILY_WINDOWS_OS
#include <windows.h>
#else
#include <time.h>
#endif

#ifdef LILY_WINDOWS_OS
                DWORD start = GetTickCount();
#else
                clock_t start = clock();
#endif

                struct File file = NEW(File, argv[2]);
                struct Source src = NEW(Source, file);
                struct Scanner scanner = NEW(Scanner, &src);
                struct ParseBlock parse_block = NEW(ParseBlock, scanner);
                struct Parser parser = NEW(Parser, parse_block);
                struct Typecheck tc = NEW(Typecheck, parser);

                run__Typecheck(&tc, NULL);

                struct Generate gen = NEW(Generate, tc);

                write_main_function(&gen);
                // run__GenerateC(gen);

                FREE(Generate, gen);

#ifdef LILY_WINDOWS_OS
                double total_t = (double)(GetTickCount() - start);
#else
                double total_t = (double)(clock() - start) / CLOCKS_PER_SEC;
#endif

                println("compiled in %.3fs", total_t);
            }

            break;
        case HELP_COMMAND:
            printf("%s\n", MAIN_HELP);
            break;
        case INIT_COMMAND:
            Println("Init");
            break;
        case NEW_COMMAND:
            Println("New");
            break;
        case VERSION_COMMAND:
            Println("Version");
            break;
        case UNKNOWN_COMMAND: {
#include <assert.h>
            assert(0 && "unknown command");
        }
    }
}
