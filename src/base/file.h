#ifndef LILY_FILE_H
#define LILY_FILE_H

#include <base/string.h>
#include <base/types.h>
#include <stdbool.h>

typedef struct Path
{
    struct String *path;
} Path;

/**
 *
 * @param path
 * @return struct Path*
 */
struct Path *
__new__Path(Str path);

/**
 *
 * @brief Determine if the path is a directory.
 * @param self
 * @return bool
 */
bool
is_directory__Path(struct Path self);

/**
 *
 * @brief Get the extension of file.
 * @param self
 * @return Str
 */
Str
get_extension__Path(struct Path self);

/**
 *
 * @brief The function try to read the file.
 * @param self
 * @return char*
 */
struct String *
read_file__Path(struct Path self);

/**
 *
 * @brief Free Path type.
 */
void
__free__Path(struct Path *self);

#endif // LILY_FILE_H