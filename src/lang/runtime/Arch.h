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
