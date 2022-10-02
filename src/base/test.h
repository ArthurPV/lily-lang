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
run__Suite(struct Suite *self);

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