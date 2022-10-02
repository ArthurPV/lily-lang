#ifndef LILY_GENERATE_H
#define LILY_GENERATE_H

#include <base/string.h>
#include <lang/analysis/typecheck.h>

typedef struct Generate {
    struct String *ouput;
    struct Typecheck tc;
} Generate;

struct Generate
__new__Generate(struct Typecheck tc);

void
__free__Generate(struct Generate self);

#endif // LILY_GENERATE_H