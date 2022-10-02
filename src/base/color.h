#ifndef LILY_COLOR_H
#define LILY_COLOR_H

#include <base/types.h>

#define RED(s) red__Color(s)
#define GREEN(s) green__Color(s)
#define YELLOW(s) yellow__Color(s)
#define BLUE(s) blue__Color(s)
#define MAGENTA(s) magenta__Color(s)
#define CYAN(s) cyan__Color(s)
#define BOLD(s) bold__Style(s)

/**
 *
 * @return the formatted Str in red color.
 */
Str
red__Color(Str s);

/**
 *
 * @return the formatted Str in green color.
 */
Str
green__Color(Str s);

/**
 *
 * @return the formatted Str in yellow color.
 */
Str
yellow__Color(Str s);

/**
 * @return the formatted Str in blue color.
 */
Str
blue__Color(Str s);

/**
 *
 * @return the formatted Str in magenta color.
 */
Str
magenta__Color(Str s);

/**
 * @return the formatted Str in cyan color.
 */
Str
cyan__Color(Str s);

/**
 * @return the formatted Str in bold style.
 */
Str
bold__Style(Str s);

#endif // LILY_COLOR_H
