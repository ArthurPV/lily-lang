#include <base/file.h>
#include <base/new.h>
#include <base/print.h>
#include <base/test.h>
#include <lang/parser/parser.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wunused-function"

static int
test_module()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/module.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "module Calc =\n"
                            "\tpub fun add(x, y) =\n"
                            "\t\tx + y\n"
                            "\tend\n"
                            "\tpub fun sub(x, y) =\n"
                            "\t\tx - y\n"
                            "\tend\n"
                            "\tpub fun mul(x, y) =\n"
                            "\t\tx * y\n"
                            "\tend\n"
                            "\tpub fun div(x, y) =\n"
                            "\t\tx / y\n"
                            "\tend\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}
