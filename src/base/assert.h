#ifndef LILY_ASSERT_H
#define LILY_ASSERT_H

#include <assert.h>
#include <base/print.h>
#include <stdlib.h>

#define ASSERT_EQ(e1, e2)                                                      \
    assert(e1 == e2 && "left expression is not equal to right expression")
#define ASSERT_NE(e1, e2)                                                      \
    assert(e1 != e2 && "left expression is equal to right expression")

#endif // LILY_ASSERT_H