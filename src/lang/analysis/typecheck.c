#include <lang/analysis/symbol_table.h>
#include <lang/analysis/typecheck.h>

struct Typecheck
__new__Typecheck(struct Parser parser)
{
    struct Typecheck self = {
        .parser = parser,
        .funs = NEW(Vec, sizeof(struct FunSymbol)),
    };

    return self;
}

void
__free__Typecheck(struct Typecheck self)
{
    for (Usize i = 0; i < len__Vec(*self.funs); i++)
        FREE(FunSymbol, (struct FunSymbol *)get__Vec(*self.funs, i));

    FREE(Vec, self.funs);
    FREE(Parser, self.parser);
}