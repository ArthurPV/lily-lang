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

const MIN: bigint = -0x8000000000000000n;
const MAX: bigint = 0x7FFFFFFFFFFFFFFFn;

function checkOverflow(n: bigint): void {
    if (n > MIN && n < MAX)
        return;
    else
        throw reportError(`Isize overflow: ${n} < ${MIN} or ${n} > ${MAX}`);
}

export class Isize {
    private _: bigint;

    constructor(_: bigint) {
        this._ = _;
        checkOverflow(this._);
    }

    public add(y: Isize): Isize {
        return new Isize(this._ + y._);
    }

    public sub(y: Isize): Isize {
        return new Isize(this._ - y._);
    }

    public mul(y: Isize): Isize {
        return new Isize(this._ * y._);
    }

    public div(y: Isize): Isize {
        return new Isize(this._ / y._);
    }

    public mod(y: Isize): Isize {
        return new Isize(this._ % y._);
    }

    public exp(y: Isize): Isize {
        return new Isize(this._ ** y._);
    }

    public lShift(y: Isize): Isize {
        return new Isize(this._ << y._);
    }

    public rShift(y: Isize): Isize {
        return new Isize(this._ >> y._);
    }

    public bitOr(y: Isize): Isize {
        return new Isize(this._ | y._);
    }

    public bitAnd(y: Isize): Isize {
        return new Isize(this._ & y._);
    }

    public bitXor(y: Isize): Isize {
        return new Isize(this._ ^ y._);
    }

    public bitNot(): Isize {
        return new Isize(~this._);
    }

    public assign(y: Isize): void {
        checkOverflow(this._ = y._);
    }

    public add_assign(y: Isize): void {
        checkOverflow(this._ += y._);
    }

    public sub_assign(y: Isize): void {
        checkOverflow(this._ -= y._);
    }

    public mul_assign(y: Isize): void {
        checkOverflow(this._ *= y._);
    }

    public div_assign(y: Isize): void {
        checkOverflow(this._ /= y._);
    }

    public mod_assign(y: Isize): void {
        checkOverflow(this._ %= y._);
    }

    public exp_assign(y: Isize): void {
        checkOverflow(this._ **= y._);
    }

    public l_shift_assign(y: Isize): void {
        checkOverflow(this._ <<= y._);
    }

    public r_shift_assign(y: Isize): void {
        checkOverflow(this._ >>= y._);
    }

    public bit_or_assign(y: Isize): void {
        checkOverflow(this._ |= y._);
    }

    public bit_and_assign(y: Isize): void {
        checkOverflow(this._ &= y._);
    }

    public bit_xor_assign(y: Isize): void {
        checkOverflow(this._ ^= y._);
    }

    public eq(y: Isize): boolean {
        return this._ === y._;
    }

    public ne(y: Isize): boolean {
        return this._ !== y._;
    }

    public neg(): Isize {
        return new Isize(-this._);
    }

    public lt(y: Isize): boolean {
        return this._ < y._;
    }

    public gt(y: Isize): boolean {
        return this._ > y._;
    }

    public le(y: Isize): boolean {
        return this._ <= y._;
    }

    public ge(y: Isize): boolean {
        return this._ >= y._;
    }
};
