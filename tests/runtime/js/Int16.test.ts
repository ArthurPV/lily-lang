import { I16 } from "../../../src/lang/runtime/js/Int16";
import { Mut } from "../../../src/lang/runtime/js/Mut";

test("should do Int16 overflow", () => {
    let failed = false;

    try {
        new I16(2233333);
    } catch (err) {
        failed = true;
    }

    expect(failed).toBe(true);
});

test("add() should do basic addition", () => {
    const res = (new I16(20)).add(new I16(10)).toNumber(); // 20 + 10
    expect(res).toEqual(30);
});

test("sub() should do basic substraction", () => {
    const res = (new I16(20)).sub(new I16(10)).toNumber(); // 20 - 10
    expect(res).toEqual(10);
});

test("mul() should do basic multiplication", () => {
    const res = (new I16(20)).mul(new I16(4)).toNumber(); // 20 * 4
    expect(res).toEqual(80);
});

test("div() should do basic division", () => {
    const res = (new I16(20).div(new I16(10))).toNumber(); // 20 / 10
    expect(res).toEqual(2);
});

test("mod() should do basic modulo", () => {
    const res = (new I16(20).mod(new I16(10))).toNumber(); // 20 % 10
    expect(res).toEqual(0);
});

test("exp() should do basic exponent", () => {
    const res = (new I16(2).exp(new I16(3))).toNumber(); // 2 ** 3
    expect(res).toEqual(8);
})

test("lShift() should do basic left shift", () => {
    const res = (new I16(2).lShift(new I16(3))).toNumber(); // 2 << 3
    expect(res).toEqual(16);
});

test("rShift() should do basic right shift", () => {
    const res = (new I16(2).rShift(new I16(3))).toNumber(); // 2 >> 3
    expect(res).toEqual(0);
});

test("bitOr() should do basic bit or", () => {
    const res = (new I16(2).bitOr(new I16(3))).toNumber(); // 2 | 3
    expect(res).toEqual(3);
});

test("bitAnd() should do basic bit and", () => {
    const res = (new I16(2).bitAnd(new I16(3))).toNumber(); // 2 & 3
    expect(res).toEqual(2);
});

test("bitXor() should do basic bit xor", () => {
    const res = (new I16(2).bitXor(new I16(3))).toNumber(); // 2 ^ 3
    expect(res).toEqual(1);
});

test("bitNot() should do basic bit not", () => {
    const res = (new I16(2)).bitNot().toNumber(); // ~2
    expect(res).toEqual(-3);
});

test("assign() should do basic assign", () => {
    const res = new Mut<I16>(new I16(20));
    expect(res.toValue().toNumber()).toEqual(20);

    I16.assign(res, new I16(10)); // res = 10
    expect(res.toValue().toNumber()).toEqual(10);
});

test("addAssign() should do basic assign (addition)", () => {
    const res = new Mut<I16>(new I16(20));
    expect(res.toValue().toNumber()).toEqual(20);

    I16.addAssign(res, new I16(10)); // res += 10
    expect(res.toValue().toNumber()).toEqual(30);
});

test("subAssign() should do basic assign (substraction)", () => {
    const res = new Mut<I16>(new I16(20));
    expect(res.toValue().toNumber()).toEqual(20);

    I16.subAssign(res, new I16(10)); // res -= 10
    expect(res.toValue().toNumber()).toEqual(10);
});

test("mulAssign() should do basic assign (multiplication)", () => {
    const res = new Mut<I16>(new I16(20));
    expect(res.toValue().toNumber()).toEqual(20);

    I16.mulAssign(res, new I16(2)); // res *= 2
    expect(res.toValue().toNumber()).toEqual(40);
});

test("divAssign() should do basic assign (division)", () => {
    const res = new Mut<I16>(new I16(20));
    expect(res.toValue().toNumber()).toEqual(20);

    I16.divAssign(res, new I16(2)); // res /= 2
    expect(res.toValue().toNumber()).toEqual(10);
});

test("modAssign() should do basic assign (modulo)", () => {
    const res = new Mut<I16>(new I16(20));
    expect(res.toValue().toNumber()).toEqual(20);

    I16.modAssign(res, new I16(2)); // res %= 2
    expect(res.toValue().toNumber()).toEqual(0);
});

test("expAssign() should do basic assign (exponent)", () => {
    const res = new Mut<I16>(new I16(2));
    expect(res.toValue().toNumber()).toEqual(2);

    I16.expAssign(res, new I16(3)); // res **= 3
    expect(res.toValue().toNumber()).toEqual(8);
});

test("lShiftAssign() should do basic assign (left shift)", () => {
    const res = new Mut<I16>(new I16(2));
    expect(res.toValue().toNumber()).toEqual(2);

    I16.lShiftAssign(res, new I16(3)); // res <<= 3
    expect(res.toValue().toNumber()).toEqual(16);
});

test("rShiftAssign() should do basic assign (right shift)", () => {
    const res = new Mut<I16>(new I16(2));
    expect(res.toValue().toNumber()).toEqual(2);

    I16.rShiftAssign(res, new I16(3)); // res >>= 3
    expect(res.toValue().toNumber()).toEqual(0);
});

test("bitOrAssign() should do basic assign (bit or)", () => {
    const res = new Mut<I16>(new I16(2));
    expect(res.toValue().toNumber()).toEqual(2);

    I16.bitOrAssign(res, new I16(3)); // res |= 3
    expect(res.toValue().toNumber()).toEqual(3);
});

test("bitAndAssign() should do basic assign (bit and)", () => {
    const res = new Mut<I16>(new I16(2));
    expect(res.toValue().toNumber()).toEqual(2);

    I16.bitAndAssign(res, new I16(3)); // res &= 3
    expect(res.toValue().toNumber()).toEqual(2);
});

test("bitXorAssign() should do basic assign (bit xor)", () => {
    const res = new Mut<I16>(new I16(2));
    expect(res.toValue().toNumber()).toEqual(2);

    I16.bitXorAssign(res, new I16(3)); // res ^= 3
    expect(res.toValue().toNumber()).toEqual(1);
});

test("eq() should do basic equal", () => {
    const res = (new I16(2)).eq(new I16(2)); // 2 == 2
    expect(res).toBe(true);
});

test("ne() should do basic not equal", () => {
    const res = (new I16(2)).ne(new I16(2)); // 2 not= 2
    expect(res).toBe(false);
});

test("neg() should do basic negative", () => {
    const res = (new I16(2)).neg().toNumber(); // -2
    expect(res).toEqual(-2);
});

test("lt() should do basic less", () => {
    const res = (new I16(2)).lt(new I16(10)); // 2 < 10
    expect(res).toBe(true);
});

test("gt() should do basic greater", () => {
    const res = (new I16(2)).gt(new I16(10)); // 2 > 10
    expect(res).toBe(false);
});

test("le() should do basic less equal", () => {
    const res = (new I16(2)).le(new I16(10)); // 2 <= 10
    expect(res).toBe(true);
});

test("ge() should do basic greater equal", () => {
    const res = (new I16(2)).ge(new I16(10)); // 2 >= 10
    expect(res).toBe(false);
});
