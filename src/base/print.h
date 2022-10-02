#ifndef LILY_PRINT_H
#define LILY_PRINT_H

#include <base/types.h>
#include <stdio.h>

#define print(format, ...) printf(format, __VA_ARGS__)
#define println(...)         \
    {                        \
        printf(__VA_ARGS__); \
        printf("\n");        \
    }

int
Print(const Str format_, ...);
int
Println(const Str format_, ...);

#endif // LILY_PRINT_H