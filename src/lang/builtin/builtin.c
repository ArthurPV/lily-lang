#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>
#include <lang/builtin/builtin.h>
#include <lang/parser/ast.h>
#include <stdlib.h>

struct BuiltinFun *
__new__BuiltinFun(const Str name, struct Vec *params)
{
    struct BuiltinFun *self = malloc(sizeof(struct BuiltinFun));
    self->name = name;
    self->params = params;
    return self;
}

void
__free__BuiltinFun(struct BuiltinFun *self)
{
    for (Usize i = 0; i < len__Vec(*self->params); i++)
        FREE(DataTypeAll, get__Vec(*self->params, i));

    FREE(Vec, self->params);
    free(self);
}

struct BuiltinModule *
__new__BuiltinModule(const Str name, struct Vec *items)
{
    struct BuiltinModule *self = malloc(sizeof(struct BuiltinModule));
    self->name = name;
    self->items = items;
    return self;
}

void
__free__BuiltinModule(struct BuiltinModule *self)
{
    for (Usize i = 0; i < len__Vec(*self->items); i++)
        FREE(BuiltinAll, get__Vec(*self->items, i));

    FREE(Vec, self->items);
    free(self);
}

struct Builtin *
__new__BuiltinFunVar(struct BuiltinFun *fun)
{
    struct Builtin *self = malloc(sizeof(struct Builtin));
    self->kind = BuiltinKindFun;
    self->fun = fun;
    return self;
}

struct Builtin *
__new__BuiltinModuleVar(struct BuiltinModule *module)
{
    struct Builtin *self = malloc(sizeof(struct Builtin));
    self->kind = BuiltinKindModule;
    self->module = module;
    return self;
}

void
__free__BuiltinFunVar(struct Builtin *self)
{
    FREE(BuiltinFun, self->fun);
    free(self);
}

void
__free__BuiltinModuleVar(struct Builtin *self)
{
    FREE(BuiltinModule, self->module);
    free(self);
}

void
__free__BuiltinAll(struct Builtin *self)
{
    switch (self->kind) {
        case BuiltinKindFun:
            FREE(BuiltinFunVar, self);
            break;
        case BuiltinKindModule:
            FREE(BuiltinModuleVar, self);
            break;
    }
}