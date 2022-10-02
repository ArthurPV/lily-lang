#include <stdlib.h>
#include <string.h>

#include <base/print.h>
#include <command/command.h>
#include <lang/analysis/typecheck.h>
#include <lang/generate/generate.h>
#include <lang/generate/generate_c.h>
#include <lang/parser/parser.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>
#include <command/command.h>

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

    if (status == BUILD_COMMAND)
        Println("Build");
    else if (status == COMPILE_COMMAND) {
        struct File file = NEW(File, "./app.lily");
        struct Source src = NEW(Source, file);
        struct Scanner scanner = NEW(Scanner, &src);

        run__Scanner(&scanner);

        struct ParseBlock parse_block = NEW(ParseBlock, scanner);

        run__ParseBlock(&parse_block);

        struct Parser parser = NEW(Parser, parse_block);

        run__Parser(&parser);

        struct Typecheck tc = NEW(Typecheck, parser);

        struct Generate gen = NEW(Generate, tc);

        write_main_function(&gen);

        FREE(Generate, gen);
    } else if (status == INIT_COMMAND)
        Println("Init");
    else if (status == NEW_COMMAND)
        Println("New");
    else if (status == VERSION_COMMAND)
        Println("Version");
    else if (status == UNKNOWN_COMMAND) {
#include <assert.h>
        assert(0 && "unknown command");
    }
}
