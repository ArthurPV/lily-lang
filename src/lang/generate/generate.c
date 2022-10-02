#include <lang/generate/generate.h>

struct Generate
__new__Generate(struct Typecheck tc)
{
    struct Generate self = {
        .ouput = NEW(String),
        .tc = tc
    };

    return self;
}

void
__free__Generate(struct Generate self)
{
    FREE(String, self.ouput);
    FREE(Typecheck, self.tc);
}