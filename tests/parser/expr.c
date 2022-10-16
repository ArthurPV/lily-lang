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
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_binaryop.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := 3 + 3;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "B := 3 - 2;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 2));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "C := 2 * 2;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 3));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "D := 1 / 2;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 4));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "E := 10 / 2;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 5));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "F := 21 % 7;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 6));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "G := 0 .. 10;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 7));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "H := 1 < 10;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 8));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "I := 1 > 10;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 9));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "J := 1 <= 10;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 10));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "K := 1 >= 10;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 11));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "L := 1 == 20;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 12));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "M := 1 not= 10;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 13));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "N := true and false;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 14));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "O := true or true;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 15));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "P := false xor true;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 16));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "Q := 3 ** 2;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 17));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "R := 2 << 3;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 18));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "S := 3 >> 2;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 19));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "T := 2 `++` 20;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 20));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun main =\n"
                            "\ta := 20\n"
                            "\ta = 120\n"
                            "\ta += 20\n"
                            "\ta -= 10\n"
                            "\ta *= 20\n"
                            "\ta /= 2\n"
                            "\ta %= 20\n"
                            "\ta <<= 2\n"
                            "\ta >>= 1\n"
                            "\ta |= 2\n"
                            "\ta xor= 30\n"
                            "\ta &= 9\n"
                            "\tb := \"hello\" ^ \"world\"\n"
                            "\tc := [1, 2, 3, 4]\n"
                            "\td := [5, 6, 7, 8]\n"
                            "\te := c ++ d\n"
                            "\tf := c -- d\n"
                            "\tg := \"hello\" $ 3\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_fun_call()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_fun_call.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := call();"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_record_call()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_record_call.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(
          !strcmp(output_str, "A := Person {name := \"John\", age := 45};"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_identifier()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_identifier.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := a;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_identifier_access()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_identifier_access.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := a.b.c;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_global_access()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_global_access.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := global.a.b.c;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_array_access()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_array_access.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := a.b.c.d[0];"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_tuple_access()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_tuple_access.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := a.b.c#0;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_lambda()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_lambda.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := fun (x) -> (x);"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "B := fun (x := 2) -> (x)();"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_tuple()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_tuple.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := (1, true, \"hello\");"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_array()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_array.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := [1, 2, 3];"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_variant()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_variant.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "VAR := Letter.A:$;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "VAR2 := Value.Integer:3;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_try()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/expr_try.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "A := try add(3);"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_if()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/expr_if.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(
          !strcmp(output_str, "B := if x > 0 do\n\t1\nelse\n\t0\nend;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_block()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_block.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "fun main =\n"
                            "\tbegin =\n"
                            "\t\ta := 3\n"
                            "\tend\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}

static int
test_expr_question_mark()
{
    struct Source src =
      NEW(Source, NEW(File, "./tests/parser/expr_question_mark.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "B := A.?;"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str, "C := Z.A.?.?.?;"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
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
