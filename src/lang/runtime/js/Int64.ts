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

import { Mut } from "./Mut";

const MIN: bigint = -0x8000000000000000n;
const MAX: bigint = 0x7FFFFFFFFFFFFFFFn;

function checkOverflow(n: bigint): void {
    if (n > MIN && n < MAX)
        return;
    else
        throw reportError(`Int64 overflow: ${n} < ${MIN} or ${n} > ${MAX}`);
}

export class I64 {
    private _: bigint;

    constructor(_: bigint) {
        this._ = _;
        checkOverflow(this._);
    }

    public add(y: I64): I64 {
        return new I64(this._ + y._);
    }

    public sub(y: I64): I64 {
        return new I64(this._ - y._);
    }

    public mul(y: I64): I64 {
        return new I64(this._ * y._);
    }

    public div(y: I64): I64 {
        return new I64(this._ / y._);
    }

    public mod(y: I64): I64 {
        return new I64(this._ % y._);
    }

    public exp(y: I64): I64 {
        return new I64(this._ ** y._);
    }

    public lShift(y: I64): I64 {
        return new I64(this._ << y._);
    }

    public rShift(y: I64): I64 {
        return new I64(this._ >> y._);
    }

    public bitOr(y: I64): I64 {
        return new I64(this._ | y._);
    }

    public bitAnd(y: I64): I64 {
        return new I64(this._ & y._);
    }

    public bitXor(y: I64): I64 {
        return new I64(this._ ^ y._);
    }

    public bitNot(): I64 {
        return new I64(~this._);
    }

	public static assign(x: Mut<I64>, y: I64): void {
		x.assign(y);
		checkOverflow(x.toValue().toBigInt());
    }

    public static addAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().add(y));
    }

    public static subAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().sub(y));
    }

    public static mulAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().mul(y));
    }

    public static divAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().div(y));
    }

    public static modAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().mod(y));
    }

    public static expAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().exp(y));
    }

    public static lShiftAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().lShift(y));
    }

    public static rShiftAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().rShift(y));
    }

    public static bitOrAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().bitOr(y));
    }

    public static bitAndAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().bitAnd(y));
    }

    public static bitXorAssign(x: Mut<I64>, y: I64): void {
		x.assign(x.toValue().bitXor(y));
    }
 
    public eq(y: I64): boolean {
        return this._ === y._;
    }

    public ne(y: I64): boolean {
        return this._ !== y._;
    }

    public neg(): I64 {
        return new I64(-this._);
    }

    public lt(y: I64): boolean {
        return this._ < y._;
    }

    public gt(y: I64): boolean {
        return this._ > y._;
    }

    public le(y: I64): boolean {
        return this._ <= y._;
    }

    public ge(y: I64): boolean {
        return this._ >= y._;
    }

    public toBigInt(): bigint {
        return this._;
    }
};
