#ifndef LILY_RUNTIME_ALLOCATOR_H
#define LILY_RUNTIME_ALLOCATOR_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

	LILY_API void* allocate__Allocator(Usize n, Usize t_size);
	inline LILY_API void deallocate__Allocator(void* const p);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_ALLOCATOR_H
