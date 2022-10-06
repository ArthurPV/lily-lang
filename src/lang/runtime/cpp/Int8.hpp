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

#ifndef LILY_RUNTIME_INT8_HPP
#define LILY_RUNTIME_INT8_HPP

#include "Mut.hpp"
#include <cstdint>
#include <math.h>
#include <stdexcept>

class I8
{
    int8_t _;

    auto checkOverflow(int8_t y, int8_t res) const -> I8
    {
        return (_ != 0 && y != 0 && res == 0)
                 ? throw std::overflow_error("Int8 overflow")
                 : I8(res);
    }

  public:
    static const int8_t MIN = -0x80;
    static const int8_t MAX = 0x7F;

    I8(int8_t _)
      : _(_)
    {
    }

    auto add(I8 y) const -> I8 { return checkOverflow(y._, _ + y._); }

    auto sub(I8 y) const -> I8 { return checkOverflow(y._, _ - y._); }

    auto mul(I8 y) const -> I8 { return checkOverflow(y._, _ * y._); }

    auto div(I8 y) const noexcept -> I8 { return I8(_ / y._); }

    auto mod(I8 y) const noexcept -> I8 { return I8(_ % y._); }

    auto exp(I8 y) const -> I8 { return checkOverflow(y._, std::pow(_, y._)); }

    auto lShift(I8 y) const -> I8 { return checkOverflow(y._, _ << y._); }

    auto rShift(I8 y) const -> I8 { return checkOverflow(y._, _ >> y._); }

    auto bitOr(I8 y) const -> I8 { return checkOverflow(y._, _ | y._); }

    auto bitAnd(I8 y) const -> I8 { return checkOverflow(y._, _ & y._); }

    auto bitXor(I8 y) const -> I8 { return checkOverflow(y._, _ ^ y._); }

    auto bitNot() const -> I8 { return checkOverflow(0, ~_); }

    static auto assign(Mut<I8> x, I8 y) noexcept -> void { x = y; }

    static auto addAssign(Mut<I8> x, I8 y) -> void { x = x.getValue().add(y); }

    static auto subAssign(Mut<I8> x, I8 y) -> void { x = x.getValue().sub(y); }

    static auto mulAssign(Mut<I8> x, I8 y) -> void { x = x.getValue().mul(y); }

    static auto divAssign(Mut<I8> x, I8 y) -> void { x = x.getValue().div(y); }

    static auto modAssign(Mut<I8> x, I8 y) -> void { x = x.getValue().mod(y); }

    static auto expAssign(Mut<I8> x, I8 y) -> void { x = x.getValue().exp(y); }

    static auto lShiftAssign(Mut<I8> x, I8 y) -> void
    {
        x = x.getValue().lShift(y);
    }

    static auto rShiftAssign(Mut<I8> x, I8 y) -> void
    {
        x = x.getValue().rShift(y);
    }

    static auto bitOrAssign(Mut<I8> x, I8 y) -> void
    {
        x = x.getValue().bitOr(y);
    }

    static auto bitAndAssign(Mut<I8> x, I8 y) -> void
    {
        x = x.getValue().bitAnd(y);
    }

    static auto bitXorAssign(Mut<I8> x, I8 y) -> void
    {
        x = x.getValue().bitXor(y);
    }

    auto eq(I8 y) const noexcept -> bool { return _ == y._; }

    auto ne(I8 y) const noexcept -> bool { return _ != y._; }

    auto lt(I8 y) const noexcept -> bool { return _ < y._; }

    auto gt(I8 y) const noexcept -> bool { return _ > y._; }

    auto le(I8 y) const noexcept -> bool { return _ <= y._; }

    auto ge(I8 y) const noexcept -> bool { return _ >= y._; }

    auto toValue() const noexcept -> uint8_t { return _; }
};

#endif // LILY_RUNTIME_INT8_HPP
