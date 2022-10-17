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
test_import_std()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/import_std.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "import \"@std.Io\" as io"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_import_builtin()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/import_builtin.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "import \"@builtin.Int8\""));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_import_core()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/import_core.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "import \"@core.Int8\""));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_import_url()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/import_url.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(
          !strcmp(output_str, "import \"@url((https://example.com)\""));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_import_file()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/import_file.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "import \"@file((../../app)\""));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_import()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/import.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "import \"@std.{d, d}\""));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "import \"@core.{d, de}\""));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 2));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "import \"a.b.c.d\""));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}
