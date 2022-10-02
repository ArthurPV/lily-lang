#include <stdlib.h>
#include <string.h>

#include <base/print.h>
#include <command/command.h>
#include <command/help.h>
#include <lang/analysis/typecheck.h>
#include <lang/generate/generate.h>
#include <lang/generate/generate_c.h>
#include <lang/parser/parser.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

#if defined(__clang__)
#if __clang__ < 14
#error "Use Clang v14.0 or later"
#endif
#endif

#if defined(__GNUC__)
#if __GNUC__ < 12
#error "Use GCC v12.0 or later"
#endif
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
#include <time.h>

                clock_t start = clock();

                struct File file = NEW(File, argv[2]);
                struct Source src = NEW(Source, file);
                struct Scanner scanner = NEW(Scanner, &src);

                run__Scanner(&scanner);

                struct ParseBlock parse_block = NEW(ParseBlock, scanner);

                run__ParseBlock(&parse_block);

                struct Parser parser = NEW(Parser, parse_block);

                run__Parser(&parser);

                struct Typecheck tc = NEW(Typecheck, parser);

                run__Typecheck(&tc);

                struct Generate gen = NEW(Generate, tc);

                write_main_function(&gen);
                // run__GenerateC(gen);

                FREE(Generate, gen);

                double total_t = (double)(clock() - start) / CLOCKS_PER_SEC;

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
