#ifndef LILY_RUNTIME_MEM_H
#define LILY_RUNTIME_MEM_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    LILY_API void *malloc__Mem(size_t n);
    LILY_API inline Unit free__Mem(void *p);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_MEM_H
