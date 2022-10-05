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

#ifndef LILY_RUNTIME_INT16_HPP
#define LILY_RUNTIME_INT16_HPP

#include "Mut.hpp"
#include <cstdint>
#include <math.h>
#include <stdexcept>

class I16
{
    int16_t _;

  public:
    static const int16_t MIN = -0x8000;
    static const int16_t MAX = 0x7FFF;

    I16(int16_t _)
      : _(_)
    {
    }

    auto add(I16 y) const -> I16
    {
        auto res = _ + y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : I16(res);
    }

    auto sub(I16 y) const -> I16
    {
        auto res = _ - y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : I16(res);
    }

    auto mul(I16 y) const -> I16
    {
        auto res = _ * y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : I16(res);
    }

    auto div(I16 y) const noexcept -> I16 { return I16(_ / y._); }

    auto mod(I16 y) const noexcept -> I16 { return I16(_ % y._); }

    auto exp(I16 y) const -> I16
    {
        auto res = std::pow(_, y._);
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : I16(res);
    }

    auto lShift(I16 y) const noexcept -> I16 { return I16(_ << y._); }

    auto rShift(I16 y) const noexcept -> I16 { return I16(_ >> y._); }

    auto bitOr(I16 y) const noexcept -> I16 { return I16(_ | y._); }

    auto bitAnd(I16 y) const noexcept -> I16 { return I16(_ & y._); }

    auto bitXor(I16 y) const noexcept -> I16 { return I16(_ ^ y._); }

    auto bitNot() const noexcept -> I16 { return I16(~_); }

    static auto assign(Mut<I16> x, I16 y) noexcept -> void { x = y; }

    static auto addAssign(Mut<I16> x, I16 y) -> void
    {
        x = x.getValue().add(y);
    }

    static auto subAssign(Mut<I16> x, I16 y) -> void
    {
        x = x.getValue().sub(y);
    }

    static auto mulAssign(Mut<I16> x, I16 y) -> void
    {
        x = x.getValue().mul(y);
    }

    static auto divAssign(Mut<I16> x, I16 y) noexcept -> void
    {
        x = x.getValue().div(y);
    }

    static auto modAssign(Mut<I16> x, I16 y) noexcept -> void
    {
        x = x.getValue().mod(y);
    }

    static auto expAssign(Mut<I16> x, I16 y) -> void
    {
        x = x.getValue().exp(y);
    }

    static auto lShiftAssign(Mut<I16> x, I16 y) noexcept -> void
    {
        x = x.getValue().lShift(y);
    }

    static auto rShiftAssign(Mut<I16> x, I16 y) noexcept -> void
    {
        x = x.getValue().rShift(y);
    }

    static auto bitOrAssign(Mut<I16> x, I16 y) noexcept -> void
    {
        x = x.getValue().bitOr(y);
    }

    static auto bitAndAssign(Mut<I16> x, I16 y) noexcept -> void
    {
        x = x.getValue().bitAnd(y);
    }

    static auto bitXorAssign(Mut<I16> x, I16 y) noexcept -> void
    {
        x = x.getValue().bitXor(y);
    }

    auto eq(I16 y) noexcept -> bool { return _ == y._; }

    auto ne(I16 y) noexcept -> bool { return _ != y._; }

    auto lt(I16 y) noexcept -> bool { return _ < y._; }

    auto gt(I16 y) noexcept -> bool { return _ > y._; }

    auto le(I16 y) noexcept -> bool { return _ <= y._; }

    auto ge(I16 y) noexcept -> bool { return _ >= y._; }

    auto toValue() noexcept -> uint8_t { return _; }
};

#endif // LILY_RUNTIME_INT16_HPP
