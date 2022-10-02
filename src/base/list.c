#include <stdlib.h>
#include <base/list.h>

struct List *__new__List() {
    struct List *self = malloc(sizeof(struct List));
    self->value = NULL;
    self->next = NULL;
    return self;
}

void __free__List(struct List *self) {
    free(self);
}