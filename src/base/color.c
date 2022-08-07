#include <base/color.h>
#include <stdio.h>

Str
red__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[31m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[31m%s\x1b[0m", s);
    return res;
}

Str
green__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[32m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[32m%s\x1b[0m", s);
    return res;
}

Str
yellow__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[33m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[33m%s\x1b[0m", s);
    return res;
}

Str
blue__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[34m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[34m%s\x1b[0m", s);
    return res;
}

Str
magenta__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[35m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[35m%s\x1b[0m", s);
    return res;
}

Str
cyan__Color(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[36m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[36m%s\x1b[0m", s);
    return res;
}

Str
bold__Style(Str s)
{
    size_t size = snprintf(NULL, 0, "\x1b[1m%s\x1b[0m", s) + 1;
    char *res = malloc(size);
    snprintf(res, size, "\x1b[1m%s\x1b[0m", s);
    return res;
}