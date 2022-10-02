#ifndef LILY_RUNTIME_INT8_H
#define LILY_RUNTIME_INT8_H

#include <lang/runtime/runtime.h>

I8 add(MutI8 x, MutI8 y);
I8 sub(MutI8 x, MutI8 y);
I8 mul(I8 x, I8 y);
I8 div_(I8 x, I8 y);
I8 mod(I8 x, I8 y);
void add_assign(MutI8 *x, I8 y);
void sub_assign(MutI8 *x, I8 y);
void mul_assign(MutI8 *x, I8 y);

#endif // LILY_RUNTIME_INT8_H
