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
test_stmt_return()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_return.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun add(x, y) =\n"
                            "\treturn x + y\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_if()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/stmt_if.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun is_zero(x) =\n"
                            "\tif x == 0 do\n"
                            "\t\ttrue\n"
                            "\telse\n"
                            "\t\tfalse\n"
                            "\tend\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_await()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_await.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun add(x, y) =\n"
                            "\tawait some_expr\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_try()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/stmt_try.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun try_it(x) =\n"
                            "\ttry do\n"
                            "\t\tcall(x)\n\n"
                            "\tcatch err do\n"
                            "\t\tprintln(\"error\")\n\n"
                            "\tend\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_match()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_match.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun is_zero(x) =\n"
                            "\tmatch x\n"
                            "\t\t0 => true,\n"
                            "\t\t_ => false,\n"
                            "\tend\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_while()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_while.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun loop_to_10 =\n"
                            "\ti := 0\n"
                            "\twhile i < 10 do\n"
                            "\t\ti += 1\n\n"
                            "\tend\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_for()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_for.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun loop_to_10 =\n"
							"\tfor i in 10 do\n"
							"\t\tprintln(\"hey\")\n"
							"\tend\n"
							"end"));

        FREE(String, output);
        free(output_str);
    }

	{
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun loop_to_10_2 =\n"
							"\tfor i := 0, i < 10, i += 1 do\n"
							"\t\tprintln(\"hey\")\n"
							"\tend\n"
							"end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_next()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_next.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun loop_next =\n"
							"\tfor i in 10 do\n"
							"\t\tnext\n"
							"\tend\n"
							"end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_break()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_break.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun loop_break =\n"
							"\tfor i in 10 do\n"
							"\t\tbreak\n"
							"\tend\n"
							"end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt_import()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt_import.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun main =\n"
							"\timport \"@std.io\" as Io\n"
							"end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_stmt()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/stmt.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun main =\n"
							"\tfor i in 10 do\n"
							"\t\tprintln(\"hey\")\n"
							"\tend\n"
							"\treturn 0\n"
							"end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}
