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

#include <base/color.h>
#include <base/format.h>
#include <base/print.h>
#include <base/test.h>
#include <time.h>

struct Case *
__new__Case(Str name, void(*f))
{
    struct Case *self = malloc(sizeof(struct Case));
    self->name = name;
    self->f = f;
    return self;
}

int
run__Case(struct Case *self)
{
    int res = self->f();

    if (res == TEST_FAILED) {
        Str failed = RED("failed");
        println("case %s ... %s", self->name, failed);

        free(failed);

        return TEST_FAILED;
    } else if (res == TEST_SKIPPED) {
        Str skipped = YELLOW("skipped");
        println("case %s ... %s", self->name, skipped);

        free(skipped);

        return TEST_SKIPPED;
    }

    Str ok = GREEN("ok");
    println("case %s ... %s", self->name, ok);

    free(ok);

    return TEST_SUCCESS;
}

void
__free__Case(struct Case *self)
{
    free(self);
}

struct Suite *
__new__Suite(Str name)
{
    struct Suite *self = malloc(sizeof(struct Suite));
    self->name = name;
    self->time = 0.0;
    self->cases = NEW(Vec, sizeof(struct Case));
    return self;
}

void
add_case__Suite(struct Suite *self, struct Case *case_)
{
    push__Vec(self->cases, case_);
}

void
run__Suite(struct Suite *self,
           Usize *passed_case,
           Usize *skipped_case,
           Usize *passed_suite)
{
    int failed = 0;
    clock_t start, end;

    start = clock();

    for (Usize i = 0; i < len__Vec(*self->cases); i++) {
        int res = run__Case(get__Vec(*self->cases, i));

        if (res == TEST_FAILED)
            failed = TEST_FAILED;
        else if (res == TEST_SKIPPED)
            *skipped_case += 1;
        else
            *passed_case += 1;
    }

    end = clock();
    self->time = ((double)end - start) / CLOCKS_PER_SEC;

    if (failed) {
        Str failed_str = RED("failed");
        println("suite %s ... %s\n", self->name, failed_str);

        free(failed_str);

#ifdef FATAL_TEST // An option for stop case when it's failed
        exit(1);
#endif
    } else {
        *passed_suite += 1;

        Str ok = GREEN("ok");

        if (len__Vec(*self->cases) > 1) {
            println("%s suite run %zu cases in %.2fs ... %s\n",
                    self->name,
                    len__Vec(*self->cases),
                    self->time,
                    ok);
        } else {
            println("%s suite run %zu case in %.2fs ... %s\n",
                    self->name,
                    len__Vec(*self->cases),
                    self->time,
                    ok);
        }

        free(ok);
    }
}

void
__free__Suite(struct Suite *self)
{
    for (Usize i = 0; i < len__Vec(*self->cases); i++)
        FREE(Case, get__Vec(*self->cases, i));

    FREE(Vec, self->cases);
    free(self);
}

struct Test *
__new__Test()
{
    struct Test *self = malloc(sizeof(struct Test));
    self->suites = NEW(Vec, sizeof(struct Suite));
    self->time = 0;
    return self;
}

void
add_suite__Test(struct Test *self, struct Suite *suite)
{
    push__Vec(self->suites, suite);
}

void
run__Test(struct Test *self)
{
    clock_t start, end;
    start = clock();
    Usize count_case = 0;
    Usize count_suite = len__Vec(*self->suites);
    Usize count_passed_case = 0;
    Usize count_skipped_case = 0;
    Usize count_passed_suite = 0;

    for (Usize i = 0; i < len__Vec(*self->suites); i++) {
        struct Suite *suite = get__Vec(*self->suites, i);

        run__Suite(
          suite, &count_passed_case, &count_skipped_case, &count_passed_suite);

        count_case += len__Vec(*suite->cases);
    }

    end = clock();
    self->time = ((double)end - start) / CLOCKS_PER_SEC;

    // Print the test summary
    Println("\x1b[1mSuites: {d} {sa}, {d} {sa}, {d} total\x1b[0m",
            count_passed_suite,
            GREEN("passed"),
            count_suite - count_passed_suite,
            RED("failed"),
            count_suite);

    // Bug: Buffer overflow when print
    /*
            Println("\x1b[1mCases: {d} {sa}, {d} {sa}, {d} {sa}, {d}
       total\x1b[0m", count_passed_case, GREEN("passed"), count_case -
       (count_passed_case + count_skipped_case), RED("failed"),
       count_skipped_case, YELLOW("skipped"), count_case);
    */

    // Bug fix: temporary fix
    // TODO: try to find a solution to this bug.
    // --------------
    struct String *s =
      format("\x1b[1mCases: {d} {sa}, {d} {sa}, {d} {sa}, {d} total\x1b[0m",
             count_passed_case,
             GREEN("passed"),
             count_case - (count_passed_case + count_skipped_case),
             RED("failed"),
             count_skipped_case,
             YELLOW("skipped"),
             count_case);
    Println("{Sr}", s);
    // --------------

    // Print the time of execution
    printf("\x1b[1mTime: %.2fs\x1b[0m\n", self->time);
}

void
__free__Test(struct Test *self)
{
    for (Usize i = 0; i < len__Vec(*self->suites); i++)
        FREE(Suite, get__Vec(*self->suites, i));

    FREE(Vec, self->suites);
    free(self);
}
