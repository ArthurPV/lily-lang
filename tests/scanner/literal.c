#include "literal.h"
#include <base/string.h>
#include <base/new.h>
#include <base/test.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

int test_int() {
    struct File file = NEW(File, "./tests/scanner/int.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 0))->lit,
                           from__String("3333"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 1))->lit,
                           from__String("3333"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 2))->lit,
                           from__String("0xff"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 3))->lit,
                           from__String("0o33"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 4))->lit,
                           from__String("0b0111"),
                           true));

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}

int test_float() {
    struct File file = NEW(File, "./tests/scanner/float.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 0))->lit,
                           from__String("33.333"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 1))->lit,
                           from__String("3.323321002"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 2))->lit,
                           from__String("3e+3"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 3))->lit,
                           from__String("3333.3"),
                           true));

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}

int test_char() {
    struct File file = NEW(File, "./tests/scanner/char.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 0))->lit,
                           from__String("c"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 1))->lit,
                           from__String("\\t"),
                           true));
    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 2))->lit,
                           from__String("\\n"),
                           true));

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}

int test_string() {
    struct File file = NEW(File, "./tests/scanner/string.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 0))->lit,
                           from__String("hello world"),
                           true));

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}

int test_bit_char() {
    struct File file = NEW(File, "./tests/scanner/bit_char.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 0))->lit,
                           from__String("c"),
                           true));

    FREE(Scanner, scanner);

    return TEST_SUCCESS;

}

int test_bit_string() {
    struct File file = NEW(File, "./tests/scanner/bit_string.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT(eq__String(((struct Token *)get__Vec(*scanner.tokens, 0))->lit,
                           from__String("hello world"),
                           true));

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}
