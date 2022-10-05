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

const MIN: number = -3.40282346638528859811704183484516925440000000000000e+38;
const MAX: number = -MIN;

function checkOverflow(n: number): void {
    if (n > MIN && n < MAX)
        return;
    else
        throw reportError(`Float32 overflow: ${n} < ${MIN} or ${n} > ${MAX}`);
}

export class F32 {
    private _: number;

    constructor(_: number) {
        this._ = _;
        checkOverflow(this._);
    }

    public add(y: F32): F32 {
        return new F32(this._ + y._);
    }

    public sub(y: F32): F32 {
        return new F32(this._ - y._);
    }

    public mul(y: F32): F32 {
        return new F32(this._ * y._);
    }

    public div(y: F32): F32 {
        return new F32(this._ / y._);
    }

    public mod(y: F32): F32 {
        return new F32(this._ % y._);
    }

    public exp(y: F32): F32 {
        return new F32(this._ ** y._);
    }

    public lShift(y: F32): F32 {
        return new F32(this._ << y._);
    }

    public rShift(y: F32): F32 {
        return new F32(this._ >> y._);
    }

    public bitOr(y: F32): F32 {
        return new F32(this._ | y._);
    }

    public bitAnd(y: F32): F32 {
        return new F32(this._ & y._);
    }

    public bitXor(y: F32): F32 {
        return new F32(this._ ^ y._);
    }

    public bitNot(): F32 {
        return new F32(~this._);
    }

	public static assign(x: Mut<F32>, y: F32): void {
		x.assign(y);
		checkOverflow(x.toValue().toNumber());
    }

    public static addAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().add(y));
    }

    public static subAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().sub(y));
    }

    public static mulAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().mul(y));
    }

    public static divAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().div(y));
    }

    public static modAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().mod(y));
    }

    public static expAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().exp(y));
    }

    public static lShiftAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().lShift(y));
    }

    public static rShiftAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().rShift(y));
    }

    public static bitOrAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().bitOr(y));
    }

    public static bitAndAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().bitAnd(y));
    }

    public static bitXorAssign(x: Mut<F32>, y: F32): void {
		x.assign(x.toValue().bitXor(y));
    } 

    public eq(y: F32): boolean {
        return this._ === y._;
    }

    public ne(y: F32): boolean {
        return this._ !== y._;
    }

    public neg(): F32 {
        return new F32(-this._);
    }

    public lt(y: F32): boolean {
        return this._ < y._;
    }

    public gt(y: F32): boolean {
        return this._ > y._;
    }

    public le(y: F32): boolean {
        return this._ <= y._;
    }

    public ge(y: F32): boolean {
        return this._ >= y._;
    }

	public toNumber(): number {
		return this._;
	}
};
