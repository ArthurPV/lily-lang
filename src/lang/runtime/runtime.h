#ifndef LILY_RUNTIME_H
#define LILY_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#define MIN(x, y) (((x)<(y))?(x):(y))
#define MAX(x, y) (((x)>(y))?(x):(y))

typedef char* MutStr;
typedef const char* Str;

typedef uint8_t MutU8;
typedef uint16_t MutU16;
typedef uint32_t MutU32;
typedef uint64_t MutU64;

typedef const uint8_t U8;
typedef const uint16_t U16;
typedef const uint32_t U32;
typedef const uint64_t U64;

typedef int8_t MutI8;
typedef int16_t MutI16;
typedef int32_t MutI32;
typedef int64_t MutI64;

typedef const int8_t I8;
typedef const int16_t I16;
typedef const int32_t I32;
typedef const int64_t I64;

typedef char MutChar;
typedef const char Char;

typedef float MutF32;
typedef const float F32;

typedef double MutF64;
typedef const double F64;

typedef bool MutBool;
typedef const bool Bool;

typedef size_t MutUsize;
typedef const size_t Usize;

typedef long long MutIsize;
typedef const long long Isize;

typedef void Unit;

#define RUNTIME_ASSERT(expr) if (!expr) assert(0 && "runtime error");
#define RUNTIME_ERROR(msg) \
    printf("Runtime Error: %s", msg); \
    exit(1)

typedef struct Exception {
    void *val;
    void *ext;
} Exception;

struct Exception
__new__Exception(void *val, void *ext);

typedef Exception MutExt;
typedef const Exception Ext;

typedef struct Optional {
    void *val;
} Optional;

struct Optional 
__new__Optional(void *val);

void *
get__Optional(struct Optional self);

typedef Optional MutOp;
typedef const Optional Op;

#endif // LILY_RUNTIME_H
