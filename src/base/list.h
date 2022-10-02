#ifndef LILY_LIST_H
#define LILY_LIST_H

typedef struct List
{
    void *value;
    struct List *next;
} List;

/**
 *
 * @brief Construct the List type.
 */
struct List *
__new__List();

/**
 *
 * @brief Free the List type.
 */
void
__free__List(struct List *self);

#endif // LILY_LIST_H