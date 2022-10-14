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
test_expr_unaryop()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_unaryop.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := -3;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "B := not true;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 2));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "C := &a;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 3));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "D := ~23;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 4));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "E := `++++`3;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_binaryop()
{
    return TEST_SKIPPED;
}

static int
test_expr_fun_call()
{
    return TEST_SKIPPED;
}

static int
test_expr_record_call()
{
    return TEST_SKIPPED;
}

static int
test_expr_identifier()
{
    return TEST_SKIPPED;
}

static int
test_expr_identifier_access()
{
    return TEST_SKIPPED;
}

static int
test_expr_global_access()
{
    return TEST_SKIPPED;
}

static int
test_expr_array_access()
{
    return TEST_SKIPPED;
}

static int
test_expr_tuple_access()
{
    return TEST_SKIPPED;
}

static int
test_expr_lambda()
{
    return TEST_SKIPPED;
}

static int
test_expr_tuple()
{
    return TEST_SKIPPED;
}

static int
test_expr_array()
{
    return TEST_SKIPPED;
}

static int
test_expr_variant()
{
    return TEST_SKIPPED;
}

static int
test_expr_try()
{
    return TEST_SKIPPED;
}

static int
test_expr_if()
{
    return TEST_SKIPPED;
}

static int
test_expr_block()
{
    return TEST_SKIPPED;
}

static int
test_expr_question_mark()
{
    return TEST_SKIPPED;
}

static int
test_expr_dereference()
{
    return TEST_SKIPPED;
}

static int
test_expr_ref()
{
    return TEST_SKIPPED;
}

static int
test_expr_self()
{
    return TEST_SKIPPED;
}

static int
test_expr_undef()
{
    return TEST_SKIPPED;
}

static int
test_expr_nil()
{
    return TEST_SKIPPED;
}

static int
test_expr_none()
{
    return TEST_SKIPPED;
}

static int
test_expr_wildcard()
{
    return TEST_SKIPPED;
}

static int
test_expr_literal()
{
    return TEST_SKIPPED;
}

static int
test_expr_variable()
{
    return TEST_SKIPPED;
}

static int
test_expr_grouping()
{
    return TEST_SKIPPED;
}
