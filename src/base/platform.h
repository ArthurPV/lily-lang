#ifndef LILY_PLATFORM_H
#define LILY_PLATFORM_H

#if defined(__linux__)
#define OS_NAME "linux"
#define LILY_LINUX_OS
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
  defined(__bsdi__) || defined(__DragonFly__)
#defined OS_NAME "BSD"
#define LILY_BSD_OS
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_NAME "macos"
#define LILY_APPLE_OS
#elif defined(_WIN32) || defined(_WIN64)
#define OS_NAME "windows"
#define LILY_WINDOWS_OS
#else
#define OS_NAME "unknown"
#define LILY_UNKNOWN_OS
#endif

#if defined(__x86_64__)
#define ARCH_NAME "x86_64"
#define LILY_X86_64_ARCH
#elif defined(__i386__)
#define ARCH_NAME "x86"
#define LILY_X86_ARCH
#elif defined(__arm__)
#define ARCH_NAME "arm"
#define LILY_ARM_ARCH
#elif defined(__mips__)
#define ARCH_NAME "mips"
#define LILY_MIPS_ARCH
#else
#define ARCH_NAME "unknown"
#define LILY_UNKNOWN_ARCH
#endif

#endif // LILY_PLATFORM_H