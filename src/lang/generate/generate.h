#ifndef LILY_GENERATE_H
#define LILY_GENERATE_H

#include <base/string.h>
#include <lang/analysis/typecheck.h>

typedef struct Generate
{
    struct String *output;
    struct Typecheck tc;
} Generate;

struct Generate
__new__Generate(struct Typecheck tc);

void
write_on_file__Generate(struct Generate self);

void
__free__Generate(struct Generate self);

#endif // LILY_GENERATE_H
