#include <base/util.h>
#include <stdbool.h>
#include <string.h>

Int128
atoi_i128(const Str s)
{
    Int128 i = 0;
    Usize pos = 0;
    bool neg = false;

    if (s[pos] == '-') {
        neg = true;
        ++pos;
    }

    while (s[pos] >= '0' && s[pos] <= '9') {
        if (neg)
            i = (10 * i) - (s[pos++] - '0');
        else
            i = (10 * i) + (s[pos++] - '0');
    }

    return i;
}