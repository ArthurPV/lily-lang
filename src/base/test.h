/*
 * MIT License
 *
 * Copyright (c) 2022 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_TEST_H
#define LILY_TEST_H

#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#define TEST_SUCCESS 0
#define TEST_FAILED 1

#define TEST_ASSERT_EQ(l, r) \
    if (l != r)              \
    return TEST_FAILED
#define TEST_ASSERT_NE(l, r) \
    if (l == r)              \
    return TEST_FAILED
#define TEST_ASSERT(expr) \
    if (!expr)            \
    return TEST_FAILED

#define CASE(suite, name, f) add_case__Suite(suite, NEW(Case, #name, f))
#define SUITE(test, suite) add_suite__Test(test, suite)
#define RUN_TEST(test)    \
    {                     \
        run__Test(test);  \
        FREE(Test, test); \
    }

typedef struct Case
{
    Str name;
    int (*f)(void);
} Case;

typedef struct Suite
{
    Str name;
    double time;
    struct Vec *cases;
} Suite;

typedef struct Test
{
    struct Vec *suites;
    double time;
} Test;

/**
 *
 * @return new Case.
 */
struct Case *
__new__Case(Str name, void(*f));

/**
 *
 * @brief Run a single case.
 */
int
run__Case(struct Case *self);

/**
 *
 * @brief Free the Case type.
 */
void
__free__Case(struct Case *self);

/**
 *
 * @return new Suite.
 */
struct Suite *
__new__Suite(Str name);

/**
 *
 * @brief Push new case to suite.
 */
void
add_case__Suite(struct Suite *self, struct Case *case_);

/**
 *
 * @brief Run a single suite.
 */
void
run__Suite(struct Suite *self, Usize *passed_test, Usize *passed_suite);

/**
 *
 * @brief Free the Suite type.
 */
void
__free__Suite(struct Suite *self);

/**
 *
 * @return new Test.
 */
struct Test *
__new__Test();

/**
 *
 * @brief Push new suite to test.
 */
void
add_suite__Test(struct Test *self, struct Suite *suite);

/**
 *
 * @brief Run all tests.
 */
void
run__Test(struct Test *self);

/**
 *
 * @brief Free the Test type.
 */
void
__free__Test(struct Test *self);

#endif // LILY_TEST_H
