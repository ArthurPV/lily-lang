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

export class Exception<T, U> {
    private value: T | null;
    private ext: U | null;

    constructor(value: T, ext: U) {
        if (value === null) {
            this.value = null;
            this.ext = ext;
        } else if (ext === null) {
            this.value = value;
            this.ext = null;
        } else
            throw reportError("unreachable: this situation is absolutely impossible");
    }

    public get get(): T | null {
        if (this.ext !== null)
            return this.value;
        else
            throw reportError("Exception: impossible to get the value because it's off");
    }

    public eq(y: Exception<T, U>): boolean {
        if (this.value !== null && y.value !== null)
            return this.value === y.value;
        else
            throw reportError("Exception: impossible to get the value because it's off");
    }

    public ne(y: Exception<T, U>): boolean {
        if (this.value !== null && y.value !== null)
            return this.value !== y.value;
        else
            throw reportError("Exception: impossible to get the value because it's off");
    }
};
