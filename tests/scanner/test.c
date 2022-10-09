#include "identifier.h"
#include "keyword.h"
#include "literal.h"
#include "operator.h"
#include "separator.h"
#include <base/new.h>
#include <base/test.h>

int
main()
{
    struct Test *t = NEW(Test);
    struct Suite *operator= NEW(Suite, "operator");
    struct Suite *separator = NEW(Suite, "separator");
    struct Suite *keyword = NEW(Suite, "keyword");
    struct Suite *identifier = NEW(Suite, "identifier");
    struct Suite *literal = NEW(Suite, "literal");

    CASE(operator, simple, test_operator);

    CASE(separator, simple, test_separator);

    CASE(keyword, simple, test_keyword);

    CASE(identifier, simple, test_identifier);

    CASE(literal, integer, test_int);
    CASE(literal, float_, test_float);
    CASE(literal, char_, test_char);
    CASE(literal, string, test_string);
    CASE(literal, bit char_, test_bit_char);
    CASE(literal, bit string, test_bit_string);

    SUITE(t, operator);
    SUITE(t, separator);
    SUITE(t, keyword);
    SUITE(t, identifier);
    SUITE(t, literal);

    RUN_TEST(t);
}
