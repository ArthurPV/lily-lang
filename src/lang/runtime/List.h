#ifndef LILY_RUNTIME_LIST_H
#define LILY_RUNTIME_LIST_H

#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct List
    {
        struct List *next;
        struct Generic *value;
    } List;

    Unit push__List(struct List *l, struct Generic *item);
    Unit remove__List(struct List *l, Usize idx);
    Unit replace__List(struct List *l, Usize idx, struct Generic *item);
    struct Generic *pop__List(struct List *l);
    struct Generic *get__List(struct List l, Usize idx);
    Usize len__List(struct List l);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_ARRAY_H
