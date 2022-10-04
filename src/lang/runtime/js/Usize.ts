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

const MIN: bigint = 0n;
const MAX: bigint = 0xFFFFFFFFFFFFFFFFn;

function checkOverflow(n: bigint): void {
    if (n > MIN && n < MAX)
        return;
    else
        throw reportError(`Usize overflow: ${n} < ${MIN} or ${n} > ${MAX}`);
}

export class Usize {
    private _: bigint;

    constructor(_: bigint) {
        this._ = _;
        checkOverflow(this._);
    }

    public add(y: Usize): Usize {
        return new Usize(this._ + y._);
    }

    public sub(y: Usize): Usize {
        return new Usize(this._ - y._);
    }

    public mul(y: Usize): Usize {
        return new Usize(this._ * y._);
    }

    public div(y: Usize): Usize {
        return new Usize(this._ / y._);
    }

    public mod(y: Usize): Usize {
        return new Usize(this._ % y._);
    }

    public exp(y: Usize): Usize {
        return new Usize(this._ ** y._);
    }

    public lShift(y: Usize): Usize {
        return new Usize(this._ << y._);
    }

    public rShift(y: Usize): Usize {
        return new Usize(this._ >> y._);
    }

    public bitOr(y: Usize): Usize {
        return new Usize(this._ | y._);
    }

    public bitAnd(y: Usize): Usize {
        return new Usize(this._ & y._);
    }

    public bitXor(y: Usize): Usize {
        return new Usize(this._ ^ y._);
    }

    public bitNot(): Usize {
        return new Usize(~this._);
    }

    public assign(y: Usize): void {
        checkOverflow(this._ = y._);
    }

    public add_assign(y: Usize): void {
        checkOverflow(this._ += y._);
    }

    public sub_assign(y: Usize): void {
        checkOverflow(this._ -= y._);
    }

    public mul_assign(y: Usize): void {
        checkOverflow(this._ *= y._);
    }

    public div_assign(y: Usize): void {
        checkOverflow(this._ /= y._);
    }

    public mod_assign(y: Usize): void {
        checkOverflow(this._ %= y._);
    }

    public exp_assign(y: Usize): void {
        checkOverflow(this._ **= y._);
    }

    public l_shift_assign(y: Usize): void {
        checkOverflow(this._ <<= y._);
    }

    public r_shift_assign(y: Usize): void {
        checkOverflow(this._ >>= y._);
    }

    public bit_or_assign(y: Usize): void {
        checkOverflow(this._ |= y._);
    }

    public bit_and_assign(y: Usize): void {
        checkOverflow(this._ &= y._);
    }

    public bit_xor_assign(y: Usize): void {
        checkOverflow(this._ ^= y._);
    }

    public eq(y: Usize): boolean {
        return this._ === y._;
    }

    public ne(y: Usize): boolean {
        return this._ !== y._;
    }

    public lt(y: Usize): boolean {
        return this._ < y._;
    }

    public gt(y: Usize): boolean {
        return this._ > y._;
    }

    public le(y: Usize): boolean {
        return this._ <= y._;
    }

    public ge(y: Usize): boolean {
        return this._ >= y._;
    }
};
