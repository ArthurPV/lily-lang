#ifndef LILY_RUNTIME_GENERIC_H
#define LILY_RUNTIME_GENERIC_H

#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct Generic
    {
        union
        {
            void *ptr; // *
            void *ref; // &
						MutStr str; // Str
						MutChar char_; // Char
						MutI8 i8; // Int8
						MutI16 i16; // Int16
						MutI32 i32; // Int32
						MutI64 i64; // Int64
						// MutI128 i128; // Int128
            MutU8 u8; // Uint8
            MutU16 u16; // Uint16
            MutU32 u32; // Uint32
            MutU64 u64; // Uint64
						// MutU128 u128; // Uint128
            MutF32 f32; // Float32
            MutF64 f64; // Float64
            MutBool bool_; // Bool
            MutUsize usize; // Usize
            MutIsize isize; // Isize
            void *any; // Any
            struct Optional op; // Optional
            struct Exception ext; // Exception
            struct Generic *mut; // Mut
        };
    } Generic;

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_GENERIC_H
