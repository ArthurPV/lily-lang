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

#ifndef LILY_RUNTIME_UINT32_HPP
#define LILY_RUNTIME_UINT32_HPP

#include "Mut.hpp"
#include <cstdint>
#include <math.h>
#include <stdexcept>

class U32
{
    uint32_t _;

  public:
    static const uint32_t MIN = 0;
    static const uint32_t MAX = 0xFFFFFFFF;

    U32(uint32_t _)
      : _(_)
    {
    }

    auto add(U32 y) const -> U32
    {
        auto res = _ + y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U32(res);
    }

    auto sub(U32 y) const -> U32
    {
        auto res = _ - y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U32(res);
    }

    auto mul(U32 y) const -> U32
    {
        auto res = _ * y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U32(res);
    }

    auto div(U32 y) const noexcept -> U32 { return U32(_ / y._); }

    auto mod(U32 y) const noexcept -> U32 { return U32(_ % y._); }

    auto exp(U32 y) const -> U32
    {
        auto res = std::pow(_, y._);
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U32(res);
    }

    auto lShift(U32 y) const noexcept -> U32 { return U32(_ << y._); }

    auto rShift(U32 y) const noexcept -> U32 { return U32(_ >> y._); }

    auto bitOr(U32 y) const noexcept -> U32 { return U32(_ | y._); }

    auto bitAnd(U32 y) const noexcept -> U32 { return U32(_ & y._); }

    auto bitXor(U32 y) const noexcept -> U32 { return U32(_ ^ y._); }

    auto bitNot() const noexcept -> U32 { return U32(~_); }

    static auto assign(Mut<U32> x, U32 y) noexcept -> void { x = y; }

    static auto addAssign(Mut<U32> x, U32 y) -> void
    {
        x = x.getValue().add(y);
    }

    static auto subAssign(Mut<U32> x, U32 y) -> void
    {
        x = x.getValue().sub(y);
    }

    static auto mulAssign(Mut<U32> x, U32 y) -> void
    {
        x = x.getValue().mul(y);
    }

    static auto divAssign(Mut<U32> x, U32 y) noexcept -> void
    {
        x = x.getValue().div(y);
    }

    static auto modAssign(Mut<U32> x, U32 y) noexcept -> void
    {
        x = x.getValue().mod(y);
    }

    static auto expAssign(Mut<U32> x, U32 y) -> void
    {
        x = x.getValue().exp(y);
    }

    static auto lShiftAssign(Mut<U32> x, U32 y) noexcept -> void
    {
        x = x.getValue().lShift(y);
    }

    static auto rShiftAssign(Mut<U32> x, U32 y) noexcept -> void
    {
        x = x.getValue().rShift(y);
    }

    static auto bitOrAssign(Mut<U32> x, U32 y) noexcept -> void
    {
        x = x.getValue().bitOr(y);
    }

    static auto bitAndAssign(Mut<U32> x, U32 y) noexcept -> void
    {
        x = x.getValue().bitAnd(y);
    }

    static auto bitXorAssign(Mut<U32> x, U32 y) noexcept -> void
    {
        x = x.getValue().bitXor(y);
    }

    auto eq(U32 y) noexcept -> bool { return _ == y._; }

    auto ne(U32 y) noexcept -> bool { return _ != y._; }

    auto lt(U32 y) noexcept -> bool { return _ < y._; }

    auto gt(U32 y) noexcept -> bool { return _ > y._; }

    auto le(U32 y) noexcept -> bool { return _ <= y._; }

    auto ge(U32 y) noexcept -> bool { return _ >= y._; }

    auto toValue() noexcept -> uint32_t { return _; }
};

#endif // LILY_RUNTIME_UINT32_HPP
