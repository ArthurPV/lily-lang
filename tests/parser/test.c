#include "constant.h"
#include "fun.h"
#include <base/new.h>
#include <base/test.h>

int
main()
{
    struct Test *t = NEW(Test);
    struct Suite *fun = NEW(Suite, "fun");
    struct Suite *constant = NEW(Suite, "constant");

    CASE(fun, simple, test_fun);
    CASE(constant, simple, test_constant);

    SUITE(t, fun);
    SUITE(t, constant);

    RUN_TEST(t);
}
