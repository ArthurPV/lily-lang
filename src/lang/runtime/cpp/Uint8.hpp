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

#ifndef LILY_RUNTIME_UINT8_HPP
#define LILY_RUNTIME_UINT8_HPP

#include "Mut.hpp"
#include <cstdint>
#include <math.h>
#include <stdexcept>

class U8
{
    uint8_t _;

  public:
	static const uint8_t MIN = 0;
	static const uint8_t MAX = 0xFF;

    U8(uint8_t _)
      : _(_)
    {
    }

    auto add(U8 y) const -> U8
    {
		auto res = _ + y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U8(res);
    }

    auto sub(U8 y) const -> U8
    {
		auto res = _ - y._;
        return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U8(res);
    }

    auto mul(U8 y) const -> U8
    {
		auto res = _ * y._;
		return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U8(res);
    }

    auto div(U8 y) const noexcept -> U8 { return U8(_ / y._); }

    auto mod(U8 y) const noexcept -> U8 { return U8(_ % y._); }

    auto exp(U8 y) const -> U8
    {
		auto res = std::pow(_, y._);
		return (_ != 0 && y._ != 0 && res == 0)
                 ? throw std::overflow_error("Uint8 overflow")
                 : U8(res);
    }

    auto lShift(U8 y) const noexcept -> U8 { return U8(_ << y._); }

    auto rShift(U8 y) const noexcept -> U8 { return U8(_ >> y._); }

    auto bitOr(U8 y) const noexcept -> U8 { return U8(_ | y._); }

    auto bitAnd(U8 y) const noexcept -> U8 { return U8(_ & y._); }

    auto bitXor(U8 y) const noexcept -> U8 { return U8(_ ^ y._); }

    auto bitNot() const noexcept -> U8 { return U8(~_); }

    static auto assign(Mut<U8> x, U8 y) noexcept -> void { x = y; }

    static auto addAssign(Mut<U8> x, U8 y) -> void { x = x.getValue().add(y); }

    static auto subAssign(Mut<U8> x, U8 y) -> void { x = x.getValue().sub(y); }

    static auto mulAssign(Mut<U8> x, U8 y) -> void { x = x.getValue().mul(y); }

    static auto divAssign(Mut<U8> x, U8 y) noexcept -> void
    {
        x = x.getValue().div(y);
    }

    static auto modAssign(Mut<U8> x, U8 y) noexcept -> void
    {
        x = x.getValue().mod(y);
    }

    static auto expAssign(Mut<U8> x, U8 y) -> void { x = x.getValue().exp(y); }

    static auto lShiftAssign(Mut<U8> x, U8 y) noexcept -> void
    {
        x = x.getValue().lShift(y);
    }

    static auto rShiftAssign(Mut<U8> x, U8 y) noexcept -> void
    {
        x = x.getValue().rShift(y);
    }

    static auto bitOrAssign(Mut<U8> x, U8 y) noexcept -> void
    {
        x = x.getValue().bitOr(y);
    }

    static auto bitAndAssign(Mut<U8> x, U8 y) noexcept -> void
    {
        x = x.getValue().bitAnd(y);
    }

    static auto bitXorAssign(Mut<U8> x, U8 y) noexcept -> void
    {
        x = x.getValue().bitXor(y);
    }

    auto eq(U8 y) noexcept -> bool { return _ == y._; }

    auto ne(U8 y) noexcept -> bool { return _ != y._; }

    auto lt(U8 y) noexcept -> bool { return _ < y._; }

    auto gt(U8 y) noexcept -> bool { return _ > y._; }

    auto le(U8 y) noexcept -> bool { return _ <= y._; }

    auto ge(U8 y) noexcept -> bool { return _ >= y._; }

    auto toValue() noexcept -> uint8_t { return _; }
};

#endif // LILY_RUNTIME_UINT8_HPP
