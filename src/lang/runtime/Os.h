/*
 * MIT License
 *
 * Copyright (c) 2022 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
