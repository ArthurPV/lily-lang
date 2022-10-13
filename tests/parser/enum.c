#include "base/vec.h"
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
test_enum()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/enum.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "type ColorRepr: enum =\n"
                            "\tHex Uint64,\n"
                            "\tRgb (Uint8, Uint8, Uint8)\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}
