#include "constant.h"
#include <base/file.h>
#include <base/new.h>
#include <base/print.h>
#include <base/test.h>
#include <lang/parser/parser.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>
#include <string.h>

int
test_constant()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/constant.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A :: Int32 := 3;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "B :: Int64 := 10;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 2));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "C :: Str := \"hello\";"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 3));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "D := true;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 4));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "E := \'c\';"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 5));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "F := b\'c\';"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}
