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
        throw reportError(`Uint64 overflow: ${n} < ${MIN} or ${n} > ${MAX}`);
}

export class U64 {
    private _: bigint;

    constructor(_: bigint) {
        this._ = _;
        checkOverflow(this._);
    }

    public add(y: U64): U64 {
        return new U64(this._ + y._);
    }

    public sub(y: U64): U64 {
        return new U64(this._ - y._);
    }

    public mul(y: U64): U64 {
        return new U64(this._ * y._);
    }

    public div(y: U64): U64 {
        return new U64(this._ / y._);
    }

    public mod(y: U64): U64 {
        return new U64(this._ % y._);
    }

    public exp(y: U64): U64 {
        return new U64(this._ ** y._);
    }

    public lShift(y: U64): U64 {
        return new U64(this._ << y._);
    }

    public rShift(y: U64): U64 {
        return new U64(this._ >> y._);
    }

    public bitOr(y: U64): U64 {
        return new U64(this._ | y._);
    }

    public bitAnd(y: U64): U64 {
        return new U64(this._ & y._);
    }

    public bitXor(y: U64): U64 {
        return new U64(this._ ^ y._);
    }

    public bitNot(): U64 {
        return new U64(~this._);
    }

    public assign(y: U64): void {
        checkOverflow(this._ = y._);
    }

    public add_assign(y: U64): void {
        checkOverflow(this._ += y._);
    }

    public sub_assign(y: U64): void {
        checkOverflow(this._ -= y._);
    }

    public mul_assign(y: U64): void {
        checkOverflow(this._ *= y._);
    }

    public div_assign(y: U64): void {
        checkOverflow(this._ /= y._);
    }

    public mod_assign(y: U64): void {
        checkOverflow(this._ %= y._);
    }

    public exp_assign(y: U64): void {
        checkOverflow(this._ **= y._);
    }

    public l_shift_assign(y: U64): void {
        checkOverflow(this._ <<= y._);
    }

    public r_shift_assign(y: U64): void {
        checkOverflow(this._ >>= y._);
    }

    public bit_or_assign(y: U64): void {
        checkOverflow(this._ |= y._);
    }

    public bit_and_assign(y: U64): void {
        checkOverflow(this._ &= y._);
    }

    public bit_xor_assign(y: U64): void {
        checkOverflow(this._ ^= y._);
    }

    public eq(y: U64): boolean {
        return this._ === y._;
    }

    public ne(y: U64): boolean {
        return this._ !== y._;
    }

    public lt(y: U64): boolean {
        return this._ < y._;
    }

    public gt(y: U64): boolean {
        return this._ > y._;
    }

    public le(y: U64): boolean {
        return this._ <= y._;
    }

    public ge(y: U64): boolean {
        return this._ >= y._;
    }
};
