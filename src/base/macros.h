#ifndef LILY_MACROS_H
#define LILY_MACROS_H

#include <assert.h>
#include <base/print.h>
#include <stdlib.h>

#define DEBUG
#undef DEBUG

#define UNREACHABLE(msg)                                             \
    {                                                                \
        println("UNREACHABLE: %s (%s:%d)", msg, __FILE__, __LINE__); \
        exit(1);                                                     \
    }

#define TODO(msg)                                             \
    {                                                         \
        println("TODO: %s (%s:%d)", msg, __FILE__, __LINE__); \
        exit(1);                                              \
    }

#define UNIMPLEMENTED(msg)                                             \
    {                                                                  \
        println("UNIMPLEMENTED: %s (%s:%d)", msg, __FILE__, __LINE__); \
        exit(1);                                                       \
    }

#endif // LILY_MACROS_H
