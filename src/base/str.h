#ifndef LILY_STR_H
#define LILY_STR_H

#include <base/types.h>
#include <stdbool.h>

/**
 *
 * @brief Convert bool in Str.
 */
Str
of_bool__Str(bool b);

/**
 *
 * @brief Convert int in Str.
 */
Str
of_int__Str(int i);

/**
 *
 * @brief Convert u8 in Str.
 */
Str of_u8__Str(UInt8 u);

/**
 *
 * @brief Convert u16 in Str.
 */
Str of_u16__Str(UInt16 u);

/**
 *
 * @brief Convert u32 in Str.
 */
Str of_u32__Str(UInt32 u);

/**
 *
 * @brief Convert u64 in Str.
 */
Str of_u64__Str(UInt64 u);

/**
 *
 * @brief Convert i64 in Str.
 */
Str of_i64__Str(Int64 i);

/**
 *
 * @brief Convert i128 in Str.
 */
Str of_i128__Str(Int128 i);

/**
 *
 * @brief Convert double in Str.
 */
Str
of_double__Str(double d);

/**
 * @brief Convert char in Str.
 * 
 */
Str
of_char__Str(char c);

#endif // LILY_STR_H