#include "List.h"
#include <stdio.h>

Unit
push__List(struct List *l, struct Generic *item)
{
    struct List *p = l;

    while (p->next != NULL)
        p = p->next;

    p->next->next = NULL;
    p->next->value = item;
}

struct Generic *
pop__List(struct List *l)
{
    struct List *p = l;
    struct Generic *value = NULL;

    while (p->next->next != NULL)
        p = p->next;

    value = p->next->value;
    p->next = NULL;

    return value;
}

Unit
replace__List(struct List *l, Usize idx, struct Generic *item)
{
    struct List *p = l;
    MutUsize i = 0;

    while (p->next != NULL) {
        p = p->next;

        if (i++ == idx)
            break;
    }

    p->value = item;
}

struct Generic *
get__List(struct List l, Usize idx)
{
    struct List *p = &l;
    MutUsize i = 0;

    while (p->next != NULL) {
        if (i == idx)
            return p->value;

        ++i;
    }

    RUNTIME_ERROR("index out of bounds");
}

Usize
len__List(struct List l)
{
    struct List *p = &l;
    MutUsize len = 0;

    while (p->next != NULL) {
        p = p->next;
        ++len;
    }

    return len;
}
