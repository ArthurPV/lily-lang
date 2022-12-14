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

const MIN: number = 0;
const MAX: number = 0xFFFF;

function checkOverflow(n: number): void {
    if (n > MIN && n < MAX)
        return;
    else
        throw reportError(`Uint16 overflow: ${n} < ${MIN} or ${n} > ${MAX}`);
}

export class U16 {
    private _: number;

    constructor(_: number) {
        this._ = _;
        checkOverflow(this._);
    }

    public add(y: U16): U16 {
        return new U16(this._ + y._);
    }

    public sub(y: U16): U16 {
        return new U16(this._ - y._);
    }

    public mul(y: U16): U16 {
        return new U16(this._ * y._);
    }

    public div(y: U16): U16 {
        return new U16(Math.floor(this._ / y._));
    }

    public mod(y: U16): U16 {
        return new U16(this._ % y._);
    }

    public exp(y: U16): U16 {
        return new U16(this._ ** y._);
    }

    public lShift(y: U16): U16 {
        return new U16(this._ << y._);
    }

    public rShift(y: U16): U16 {
        return new U16(this._ >> y._);
    }

    public bitOr(y: U16): U16 {
        return new U16(this._ | y._);
    }

    public bitAnd(y: U16): U16 {
        return new U16(this._ & y._);
    }

    public bitXor(y: U16): U16 {
        return new U16(this._ ^ y._);
    }

    public bitNot(): U16 {
        return new U16(~this._);
    }

	public static assign(x: Mut<U16>, y: U16): void {
		x.assign(y);
		checkOverflow(x.toValue().toNumber());
    }

    public static addAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().add(y));
    }

    public static subAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().sub(y));
    }

    public static mulAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().mul(y));
    }

    public static divAssign(x: Mut<U16>, y: U16): void {
		x.assign(new U16(Math.floor(x.toValue().div(y).toNumber())));
    }

    public static modAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().mod(y));
    }

    public static expAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().exp(y));
    }

    public static lShiftAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().lShift(y));
    }

    public static rShiftAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().rShift(y));
    }

    public static bitOrAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().bitOr(y));
    }

    public static bitAndAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().bitAnd(y));
    }

    public static bitXorAssign(x: Mut<U16>, y: U16): void {
		x.assign(x.toValue().bitXor(y));
    } 

    public eq(y: U16): boolean {
        return this._ === y._;
    }

    public ne(y: U16): boolean {
        return this._ !== y._;
    }

    public lt(y: U16): boolean {
        return this._ < y._;
    }

    public gt(y: U16): boolean {
        return this._ > y._;
    }

    public le(y: U16): boolean {
        return this._ <= y._;
    }

    public ge(y: U16): boolean {
        return this._ >= y._;
    }

	public toNumber(): number {
		return this._;
	}
};
