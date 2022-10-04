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

import { I8 } from "./Int8";
import { I16 } from "./Int16";
import { I32 } from "./Int32";
import { I64 } from "./Int64";
import { I128 } from "./Int128";
import { Isize as Isize_ } from "./Isize";
import { U8 } from "./Uint8";
import { U16 } from "./Uint16";
import { U32 } from "./Uint32";
import { U64 } from "./Uint64";
import { U128 } from "./Uint128";
import { Usize as Usize_ } from "./Usize";
import { F32 } from "./Float32";
import { F64 } from "./Float64";
import { Char as Char_ } from "./Char";
import { Optional as Optional_ } from "./Optional";
import { Exception as Exception_ } from "./Exception";
import { Mut as Mut_ } from "./Mut";
import { Ref as Ref_ } from "./Ref";
import { Ptr as Ptr_ } from "./Ptr";
import { Tuple as Tuple_ } from "./Tuple";

export type Int8 = I8;
export type Int16 = I16;
export type Int32 = I32;
export type Int64 = I64;
export type Int128 = I128;
export type Isize = Isize_;
export type Uint8 = U8;
export type Uint16 = U16;
export type Uint32 = U32;
export type Uint64 = U64;
export type Uint128 = U128;
export type Usize = Usize_;
export type Float32 = F32;
export type Float64 = F64;
export type Bool = boolean;
export type Char = Char_;
export type Mut<T> = Mut_<T>;
export type Optional<T> = Optional_<T>;
export type Exception<T, U> = Exception_<T, U>;
export type Ref<T> = Ref_<T>;
export type Ptr<T> = Ptr_<T>;
export type Tuple<T extends unknown[]> = Tuple_<T>;
export type Unit = void;
