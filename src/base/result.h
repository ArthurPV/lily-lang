#ifndef LILY_RESULT_H
#define LILY_RESULT_H

#include <stdbool.h>

typedef struct Result
{
    enum
    {
        Ok,
        Err
    } kind;
    union
    {
        void *ok;
        void *err;
    };
} Result;

#define Ok(ok) __new__ResultOk(ok)
#define Err(err) __new__ResultErr(err)

/**
 *
 * @return new Result instance (Ok).
 */
struct Result *
__new__ResultOk(void *ok);

/**
 *
 * @return new Result instance (Err).
 */
struct Result *
__new__ResultErr(void *err);

/**
 *
 * @brief Returns true if the result kind is an Err otherwise returns false.
 */
bool
is_err__Result(struct Result self);

/**
 *
 * @brief Returns true if the result kind is Ok otherwise returns false.
 */
bool
is_ok__Result(struct Result self);

/**
 *
 * @brief Get the ok result value.
 */
void *
get_ok__Result(struct Result self);

/**
 *
 * @brief Get the err result value.
 */
void *
get_err__Result(struct Result self);

/**
 *
 * @brief Free the Result type.
 */
void
__free__Result(struct Result *self);

#endif // LILY_RESULT_H