#ifndef LILY_VEC_H
#define LILY_VEC_H

#include <base/types.h>
#include <stdbool.h>

typedef struct Vec
{
    void **items;
    Usize len;
    Usize capacity;
    Usize default_capacity;
    Usize item_size;
} Vec;

/**
 *
 * @brief construct Vec type.
 * @return the Vec instance.
 */
struct Vec *
__new__Vec(Usize item_size);

/**
 *
 * @brief Pass void* array to initialize Vec type.
 * @return the Vec instance.
 */
struct Vec *
from__Vec(void **items, Usize item_size, Usize len);

/**
 *
 * @return Vec.
 */
struct Vec *
create__Vec(void *item, Usize item_size, Usize len);

/**
 *
 * @brief Resize allocated memory.
 */
void
resize__Vec(struct Vec *self);

/**
 *
 * @brief push value to Vec items field.
 */
void
push__Vec(struct Vec *self, void *item);

/**
 *
 * @brief get items value at idx value.
 */
void *
get__Vec(struct Vec self, Usize idx);

/**
 *
 * @brief remove item of Vec at idx value.
 * @param idx index to remove the value of the index desire.
 */
void
remove__Vec(struct Vec *self, Usize idx);

/**
 *
 * @brief remove last item and return it.
 * @return the last element of the Vec.
 */
void *
pop__Vec(struct Vec *self);

/**
 *
 * @brief Remove the first element and return it.
 * @return the first element of the Vec.
 */
void *
shift__Vec(struct Vec *self);

/**
 *
 * @brief Modify the index desired with the new value.
 */
void
modify_item__Vec(struct Vec *self, void *value, Usize idx);

/**
 *
 * @brief Merge two Vecs.
 * @return the modify Vec.
 */
struct Vec *
concat__Vec(struct Vec *self, struct Vec *self2);

/**
 *
 * @brief Fill Vec with specified value.
 * @param start index can take NULL value (default value: 0).
 * @param end index can take NULL value (default value: length of Vec).
 * @return the modify Vec.
 */
struct Vec
fill__Vec(struct Vec self, void *value, const Usize *start, const Usize *end);

/**
 *
 * @brief Keep just the item value in Vec which corresponded to the condition.
 * @return the modify Vec.
 */
struct Vec
filter__Vec(struct Vec self, bool(f)(void *));

/**
 *
 * @brief Modify value with function passed in argument.
 * @return the modify Vec.
 */
struct Vec
map__Vec(struct Vec self, void *(f)(void *));

/**
 *
 * @return the first element which match with find condition.
 */
void *
find__Vec(struct Vec self, bool(f)(void *));

/**
 *
 * @return the index which value at n index match with find condition.
 */
Usize *
find_index__Vec(struct Vec self, bool(f)(void *));

/**
 * @brief Apply function on each element of Vec item.
 */
void
for_each__Vec(struct Vec self, void(f)(void *));

/**
 *
 * @return true if the element(s) are/is into the Vec.
 */
bool
includes__Vec(struct Vec self, void *item);

/**
 *
 * @param start begin to search at this index (default value if start == NULL:
 * 0).
 * @return the first index of value which corresponded with value passed in
 * parameter.
 */
Usize *
index_of__Vec(struct Vec self, void *value, const Usize *start);

/**
 *
 * @brief Insert item at specified idx.
 * @return the modify Vec.
 */
void
insert__Vec(struct Vec *self, void *item, Usize idx);

/**
 *
 * @brief Join Str Vec.
 * @return the joined character array.
 */
Str
join__Vec(struct Vec *self, Str separator);

/**
 *
 * @brief Reverse Vec values.
 * @return the reversed Vec.
 */
struct Vec *
reverse__Vec(struct Vec self);

/**
 *
 * @brief Class number by croissant order (if reverse is false).
 * @return the sorted Vec.
 */
struct Vec *
sort__Vec(struct Vec self, bool reverse);

/**
 *
 * @return the reduced value.
 */
void *
reduce__Vec(struct Vec self, void *(f)(void *, void *, void *));

/**
 *
 * @brief Shortens the Vec.
 */
void
truncate__Vec(struct Vec *self, Usize len);

/**
 *
 * @brief Clear the Vec.
 */
void
clear__Vec(struct Vec *self);

/**
 *
 * @return true if the Vec is clear and otherwise false.
 */
bool
is_clear__Vec(struct Vec self);

/**
 *
 * @return the length of the Vec.
 */
Usize
len__Vec(struct Vec self);

/**
 *
 * @return the capacity of the Vec.
 */
Usize
capacity__Vec(struct Vec self);

/**
 *
 * @brief Free Vec type.
 */
void
__free__Vec(struct Vec *self);

#endif // LILY_VEC_H
