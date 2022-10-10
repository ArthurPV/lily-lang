#include "fun.h"
#include <base/new.h>
#include <base/test.h>

int
main()
{
    struct Test *t = NEW(Test);
    struct Suite *fun = NEW(Suite, "fun");

    CASE(fun, simple, test_fun);

    SUITE(t, fun);

    RUN_TEST(t);
}
