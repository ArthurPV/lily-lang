#ifndef LILY_RUNTIME_OS_H
#define LILY_RUNTIME_OS_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    enum Os
    {
        OsBsd,
        OsLinux,
        OsMac,
        OsWindows,
        OsUnknown
    };

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
  defined(__bsdi__) || defined(__DragonFly__)
    const enum Os OsName = OsBsd;
    const char OsStr[] = "BSD";
#elif defined(__linux__)
const enum Os OsName = OsLinux;
const char OsStr[] = "Linux";
#elif defined(__APPLE__) && defined(__MACH__)
const enum Os OsName = OsMac;
const char OsStr[] = "Mac";
#elif defined(_WIN32) || defined(_WIN64)
const enum Os OsName = OsWindows;
const char OsStr[] = "Windows";
#else
const enum Os OsName = OsUnknown;
const char OsStr[] = "Unknown";
#endif

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_OS_H
