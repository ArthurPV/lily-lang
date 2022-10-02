#ifndef LILY_STRING_H
#define LILY_STRING_H

#include <base/vec.h>

typedef struct String
{
    struct Vec *content;
} String;

/**
 *
 * @brief Construct String type.
 * @return the String instance.
 */
struct String *
__new__String();

/**
 *
 * @brief Push each character of s parameter in new String.
 * @return the String.
 */
struct String *
from__String(const Str s);

/**
 *
 * @brief Push character to String type.
 */
void
push__String(struct String *self, const char *c);

/**
 *
 * @brief Append string to string.
 */
void
append__String(struct String *self, struct String *self2, bool drop);

/**
 *
 * @brief Push Str in String.
 */
void
push_str__String(struct String *self, const Str s);

/**
 *
 * @return the character at idx.
 */
char *
get__String(struct String self, Usize idx);

/**
 *
 * @return the code of character at idx.
 */
char *
get_code__String(struct String self, Usize idx);

/**
 *
 * @brief Merge two Strings.
 * @return the modify String.
 */
struct String
concat__String(struct String self, struct String self2);

/**
 *
 * @return true if the String ends by search_string else return false.
 */
bool
ends_with__String(struct String self,
                  struct String search_string,
                  const Usize *len);

/**
 *
 * @brief Convert String to Str type.
 */
Str
to_Str__String(struct String self);

/**
 *
 * @brief Repeat count times Str parameter.
 * @return the repeated String
 */
struct String *
repeat__String(const Str s, Usize count);

/**
 *
 * @brief Insert String at idx.
 */
struct String *
insert__String(struct String self, struct String self2, Usize idx);

/**
 *
 * @brief Replace an index value at idx by c value.
 */
void
replace__String(struct String *self, const char *c, Usize idx);

/**
 *
 * @brief Remove a character at idx.
 */
void
remove__String(struct String *self, Usize idx);

/**
 *
 * @return the removed character.
 */
char *
pop__String(struct String *self);

/**
 *
 * @return true if the len of String is zero and false otherwise.
 */
bool
is_empty__String(struct String self);

/**
 *
 * @brief Transform Vec in String.
 */
struct Vec *
split__String(struct String self, char c);

/**
 *
 * @param drop Free the second parameter.
 */
bool
eq__String(struct String *self, struct String *self2, bool drop);

/**
 *
 * @brief Copy the String type.
 */
struct String *
copy__String(struct String *self);

/**
 *
 * @return the len of String.
 */
Usize
len__String(struct String self);

/**
 *
 * @return the capacity of String.
 */
Usize
capacity__String(struct String self);

/**
 *
 * @brief Free String type.
 */
void
__free__String(struct String *self);

#endif // LILY_STRING_H
