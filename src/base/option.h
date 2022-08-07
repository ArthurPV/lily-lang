#ifndef LILY_OPTION_H
#define LILY_OPTION_H

#include <stdbool.h>

typedef struct Option
{
    enum
    {
        Some,
        None
    } kind;
    union
    {
        void *some;
    };
} Option;

#define Some(some) __new__Some__Option(some)
#define None() __new__None__Option()

/**
 * @brief Construct Some option.
 * @param some
 * @return struct Option*
 */
struct Option *
__new__Some__Option(void *some);

/**
 * @brief Construct None option;
 * @return struct Option*
 */
struct Option *
__new__None__Option();

/**
 * @brief Returns true if the Option kind is Some else returns false.
 * @param self
 * @return bool
 */
bool
is_Some__Option(struct Option *self);

/**
 * @brief Return true if the Option kind is None else returns false.
 * @param self
 * @return bool
 */
bool
is_None__Option(struct Option *self);

/**
 *
 * @return if true if the value is equal.
 */
bool eq__Option(struct Option self, void *ptr);

/**
 * @brief Get the value of Some.
 * @param self
 * @return void*
 */
void *
get__Option(struct Option *self);

/**
 * 
 * @brief Free Option type.
 */
void __free__Option(struct Option *self);

#endif // LILY_OPTION_H