#include "identifier.h"
#include <base/new.h>
#include <base/string.h>
#include <base/test.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

int
test_identifier()
{
    struct File file = NEW(File, "./tests/scanner/identifier.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 0))->lit,
                           from__String("ee3"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 1))->lit,
                           from__String("e2"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 2))->lit,
                           from__String("ee__33"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 3))->lit,
                           from__String("eqqqqAA"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 4))->lit,
                           from__String("Zasdd_____ffff5533"),
                           true));

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}