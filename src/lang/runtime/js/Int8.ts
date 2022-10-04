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

const MIN: number = -0x80;
const MAX: number = 0x7F;

function checkOverflow(n: number): void {
	if (n > MIN && n < MAX)
		return;
	else
		throw reportError(`Int8 overflow: ${n} < ${MIN} or ${n} > ${MAX}`);
}

export class I8 {
	private _: number;	

	constructor(_: number) {
		this._ = _;
		checkOverflow(this._);
	}

	public add(y: I8): I8 {
		return new I8(this._ + y._);
	}

	public sub(y: I8): I8 {
		return new I8(this._ - y._);
	}

	public mul(y: I8): I8 {
		return new I8(this._ * y._);
	}

	public div(y: I8): I8 {
		return new I8(Math.floor(this._ / y._));
	}

	public mod(y: I8): I8 {
		return new I8(this._ % y._);
	}

	public exp(y: I8): I8 {
		return new I8(this._ ** y._);
	}

	public lShift(y: I8): I8 {
		return new I8(this._ << y._);
	}

	public rShift(y: I8): I8 {
		return new I8(this._ >> y._);
	}

	public bitOr(y: I8): I8 {
		return new I8(this._ | y._);
	}

	public bitAnd(y: I8): I8 {
		return new I8(this._ & y._);
	}

	public bitXor(y: I8): I8 {
		return new I8(this._ ^ y._);
	}

	public bitNot(): I8 {
		return new I8(~this._);
	}

	public assign(y: I8): void {
		checkOverflow(this._ = y._);
	}

	public addAssign(y: I8): void {
		checkOverflow(this._ += y._);
	}

	public subAssign(y: I8): void {
		checkOverflow(this._ -= y._);
	}

	public mulAssign(y: I8): void {
		checkOverflow(this._ *= y._);
	}

	public divAssign(y: I8): void {
		checkOverflow(this._ = Math.floor(this._ /= y._));
	}

	public modAssign(y: I8): void {
		checkOverflow(this._ %= y._);
	}

	public expAssign(y: I8): void {
		checkOverflow(this._ **= y._);
	}

	public lShiftAssign(y: I8): void {
		checkOverflow(this._ <<= y._);
	}

	public rShiftAssign(y: I8): void {
		checkOverflow(this._ >>= y._);
	}

	public bitOrAssign(y: I8): void {
		checkOverflow(this._ |= y._);
	}

	public bitAndAssign(y: I8): void {
		checkOverflow(this._ &= y._);
	}

	public bitXorAssign(y: I8): void {
		checkOverflow(this._ ^= y._);
	}

	public eq(y: I8): boolean {
		return this._ === y._;
	}

	public ne(y: I8): boolean {
		return this._ !== y._;
	}

	public neg(): I8 {
		return new I8(-this._);
	}

	public lt(y: I8): boolean {
		return this._ < y._;
	}

	public gt(y: I8): boolean {
		return this._ > y._;
	}

	public le(y: I8): boolean {
		return this._ <= y._;
	}

	public ge(y: I8): boolean {
		return this._ >= y._;
	}

	public toNumber(): number {
		return this._;
	}
};
