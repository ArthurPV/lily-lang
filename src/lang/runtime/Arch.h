#ifndef LILY_RUNTIME_ARCH_H
#define LILY_RUNTIME_ARCH_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    enum Arch
    {
        ArchArm,
        ArchArm64,
        ArchX86,
        ArchX86_64,
        ArchIA_64,
        ArchMIPS,
        ArchPowerPC,
        ArchUnknown
    };

#if defined(__arm__)
    const enum Arch ArchName = ArchArm;
    const char ArchStr[] = "arm";
#elif defined(__aarch64__)
const enum Arch ArchName = ArchArm64;
const char ArchStr[] = "arm64";
#elif defined(__i386)
const enum Arch ArchName = ArchX86;
const char ArchStr[] = "x86";
#elif defined(__x86_64__)
const enum Arch ArchName = ArchX86_64;
const char ArchStr[] = "x86_64";
#elif defined(__ia64__)
const enum Arch ArchName = ArchIA_64;
const char ArchStr[] = "IA_64";
#elif defined(__mips__)
const enum Arch ArchName = ArchMIPS;
const char ArchStr[] = "MIPS";
#elif defined(__powerpc__)
const enum Arch ArchName = ArchPowerPC;
const char ArchStr[] = "PowerPC";
#else
const enum Arch ArchName = ArchUnknown;
const char ArchStr[] = "Unknown";
#endif

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_ARCH_H
