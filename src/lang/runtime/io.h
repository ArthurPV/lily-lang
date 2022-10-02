#include <stdio.h>

#define print(...) printf(__VA_ARGS__)
#define println(...)         \
    {                        \
        printf(__VA_ARGS__); \
        printf("\n");        \
    }
