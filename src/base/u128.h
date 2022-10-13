#ifndef LILY_U128_H
#define LILY_U128_H

#include <stdint.h>

typedef struct UInt128
{
    uint64_t lo, hi;
} UInt128;

struct UInt128
__new__UInt128(uint64_t lo, uint64_t hi);
struct UInt128
zero__UInt128();

#endif // LILY_U128_H
