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
test_fun()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/fun.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun add(x, y) =\n"
                            "\tx + y\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun sub(x, y) =\n"
                            "\tx - y\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 2));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun mul(x, y) =\n"
                            "\tx * y\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 3));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun div(x, y) =\n"
                            "\tx / y\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 4));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun add2[T](x T) =\n"
                            "\tx\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 5));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun#(Name) add(x) =\n"
                            "\tx\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}
