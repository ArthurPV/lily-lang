#ifndef LILY_NEW_H
#define LILY_NEW_H

#define NEW(name, ...) __new__##name(__VA_ARGS__)
#define FREE(name, ...) __free__##name(__VA_ARGS__)

#endif // LILY_NEW_H