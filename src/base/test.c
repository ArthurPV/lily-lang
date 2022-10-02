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
    if (self->f() == 1) {
        Str failed = RED("failed");
        println("test %s ... %s", self->name, failed);

        free(failed);

        return TEST_FAILED;
    }
    Str ok = GREEN("ok");
    println("test %s ... %s", self->name, ok);

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
run__Suite(struct Suite *self)
{
    int failed = 0;
    clock_t start, end;

    start = clock();

    for (Usize i = 0; i < len__Vec(*self->cases); i++) {
        if (run__Case(get__Vec(*self->cases, i)) == 1) {
            failed = 1;
        }
    }

    end = clock();
    self->time = ((double)end - start) / CLOCKS_PER_SEC;

    if (failed) {
        Str failed_str = RED("failed");
        println("suite %s ... %s", self->name, failed_str);

        free(failed_str);

        exit(1);
    } else {
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

    for (Usize i = 0; i < len__Vec(*self->suites); i++) {
        run__Suite(get__Vec(*self->suites, i));
    }

    end = clock();
    self->time = ((double)end - start) / CLOCKS_PER_SEC;

    if (len__Vec(*self->suites) > 1) {
        println(
          "%zu suites run in %.2fs.", len__Vec(*self->suites), self->time);
    } else {
        println("%zu suite run in %.2fs.", len__Vec(*self->suites), self->time);
    }
}

void
__free__Test(struct Test *self)
{
    for (Usize i = 0; i < len__Vec(*self->suites); i++)
        FREE(Suite, get__Vec(*self->suites, i));

    FREE(Vec, self->suites);
    free(self);
}
