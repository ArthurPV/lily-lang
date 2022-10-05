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

#ifndef LILY_RUNTIME_UINT16_HPP
#define LILY_RUNTIME_UINT16_HPP

#include "Mut.hpp"
#include <cstdint>
#include <math.h>
#include <stdexcept>

class U16
{
    uint16_t _;

  public:
    static const uint16_t MIN = 0;
    static const uint16_t MAX = 0xFFFF;

    U16(uint16_t _)
      : _(_)
    {
    }

    auto add(U16 y) const -> U16
    {
        auto res = _ + y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U16(res);
    }

    auto sub(U16 y) const -> U16
    {
        auto res = _ - y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U16(res);
    }

    auto mul(U16 y) const -> U16
    {
        auto res = _ * y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U16(res);
    }

    auto div(U16 y) const noexcept -> U16 { return U16(_ / y._); }

    auto mod(U16 y) const noexcept -> U16 { return U16(_ % y._); }

    auto exp(U16 y) const -> U16
    {
        auto res = std::pow(_, y._);
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U16(res);
    }

    auto lShift(U16 y) const noexcept -> U16 { return U16(_ << y._); }

    auto rShift(U16 y) const noexcept -> U16 { return U16(_ >> y._); }

    auto bitOr(U16 y) const noexcept -> U16 { return U16(_ | y._); }

    auto bitAnd(U16 y) const noexcept -> U16 { return U16(_ & y._); }

    auto bitXor(U16 y) const noexcept -> U16 { return U16(_ ^ y._); }

    auto bitNot() const noexcept -> U16 { return U16(~_); }

    static auto assign(Mut<U16> x, U16 y) noexcept -> void { x = y; }

    static auto addAssign(Mut<U16> x, U16 y) -> void
    {
        x = x.getValue().add(y);
    }

    static auto subAssign(Mut<U16> x, U16 y) -> void
    {
        x = x.getValue().sub(y);
    }

    static auto mulAssign(Mut<U16> x, U16 y) -> void
    {
        x = x.getValue().mul(y);
    }

    static auto divAssign(Mut<U16> x, U16 y) noexcept -> void
    {
        x = x.getValue().div(y);
    }

    static auto modAssign(Mut<U16> x, U16 y) noexcept -> void
    {
        x = x.getValue().mod(y);
    }

    static auto expAssign(Mut<U16> x, U16 y) -> void
    {
        x = x.getValue().exp(y);
    }

    static auto lShiftAssign(Mut<U16> x, U16 y) noexcept -> void
    {
        x = x.getValue().lShift(y);
    }

    static auto rShiftAssign(Mut<U16> x, U16 y) noexcept -> void
    {
        x = x.getValue().rShift(y);
    }

    static auto bitOrAssign(Mut<U16> x, U16 y) noexcept -> void
    {
        x = x.getValue().bitOr(y);
    }

    static auto bitAndAssign(Mut<U16> x, U16 y) noexcept -> void
    {
        x = x.getValue().bitAnd(y);
    }

    static auto bitXorAssign(Mut<U16> x, U16 y) noexcept -> void
    {
        x = x.getValue().bitXor(y);
    }

    auto eq(U16 y) noexcept -> bool { return _ == y._; }

    auto ne(U16 y) noexcept -> bool { return _ != y._; }

    auto lt(U16 y) noexcept -> bool { return _ < y._; }

    auto gt(U16 y) noexcept -> bool { return _ > y._; }

    auto le(U16 y) noexcept -> bool { return _ <= y._; }

    auto ge(U16 y) noexcept -> bool { return _ >= y._; }

    auto toValue() noexcept -> uint16_t { return _; }
};

#endif // LILY_RUNTIME_UINT16_HPP
