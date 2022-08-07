#ifndef LILY_TRY_H
#define LILY_TRY_H

#include <assert.h>
#include <base/option.h>

#define TRY_SOME(op) get__Option(op)
#define TRY_RESULT(res) 1
#define TRY_PTR(ptr) ptr != NULL ? *ptr : assert(0 && "failed: ptr is NULL")

#endif // LILY_TRY_H