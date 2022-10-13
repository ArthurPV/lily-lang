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
test_class()
{
    struct Source src = NEW(Source, NEW(File, "./tests/parser/class.lily"));
    struct Parser parser = NEW(Parser, NEW(ParseBlock, NEW(Scanner, &src)));
    run__Parser(&parser);

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 0));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(!strcmp(output_str,
                            "object Person: class =\n"
                            "\t@name Str;\n"
                            "\t@age Uint8;\n"
                            "\tpub @new(name, age) =\n"
                            "\t\t@.name = name\n"
                            "\t\t@.age = age\n"
                            "\tend\n"
                            "end"));

        FREE(String, output);
        free(output_str);
    }

    {
        struct String *output =
          to_String__Decl(*(struct Decl *)get__Vec(*parser.decls, 1));
        Str output_str = to_Str__String(*output);

        TEST_ASSERT(
          !strcmp(output_str,
                  "object Work => [Person]: class =\n"
                  "\t@name Str;\n"
                  "\t@salary Uint64;\n"
                  "\tpub @new(name#Person, age#Person, name, salary) =\n"
                  "\t\t@.name = name\n"
                  "\t\t@.salary = salary\n"
                  "\tend\n"
                  "end"));

        FREE(String, output);
        free(output_str);
    }

    FREE(Parser, parser);

    return TEST_SUCCESS;
}
