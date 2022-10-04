import { I32 } from "../../../src/lang/runtime/js/Int32";

test("should do Int32 overflow", () => {
    let failed = false;

    try {
        new I32(214748364733);
    } catch (err) {
        failed = true;
    }

    expect(failed).toBe(true);
});

test("add() should do basic addition", () => {
    const res = (new I32(20)).add(new I32(10)).toNumber(); // 20 + 10
    expect(res).toEqual(30);
});

test("sub() should do basic substraction", () => {
    const res = (new I32(20)).sub(new I32(10)).toNumber(); // 20 - 10
    expect(res).toEqual(10);
});

test("mul() should do basic multiplication", () => {
    const res = (new I32(20)).mul(new I32(4)).toNumber(); // 20 * 4
    expect(res).toEqual(80);
});

test("div() should do basic division", () => {
    const res = (new I32(20).div(new I32(10))).toNumber(); // 20 / 10
    expect(res).toEqual(2);
});

test("mod() should do basic modulo", () => {
    const res = (new I32(20).mod(new I32(10))).toNumber(); // 20 % 10
    expect(res).toEqual(0);
});

test("exp() should do basic exponent", () => {
    const res = (new I32(2).exp(new I32(3))).toNumber(); // 2 ** 3
    expect(res).toEqual(8);
})

test("lShift() should do basic left shift", () => {
    const res = (new I32(2).lShift(new I32(3))).toNumber(); // 2 << 3
    expect(res).toEqual(16);
});

test("rShift() should do basic right shift", () => {
    const res = (new I32(2).rShift(new I32(3))).toNumber(); // 2 >> 3
    expect(res).toEqual(0);
});

test("bitOr() should do basic bit or", () => {
    const res = (new I32(2).bitOr(new I32(3))).toNumber(); // 2 | 3
    expect(res).toEqual(3);
});

test("bitAnd() should do basic bit and", () => {
    const res = (new I32(2).bitAnd(new I32(3))).toNumber(); // 2 & 3
    expect(res).toEqual(2);
});

test("bitXor() should do basic bit xor", () => {
    const res = (new I32(2).bitXor(new I32(3))).toNumber(); // 2 ^ 3
    expect(res).toEqual(1);
});

test("bitNot() should do basic bit not", () => {
    const res = (new I32(2)).bitNot().toNumber(); // ~2
    expect(res).toEqual(-3);
});

test("assign() should do basic assign", () => {
    const res = new I32(20);
    expect(res.toNumber()).toEqual(20);

    res.assign(new I32(10)); // res = 10
    expect(res.toNumber()).toEqual(10);
});

test("addAssign() should do basic assign (addition)", () => {
    const res = new I32(20);
    expect(res.toNumber()).toEqual(20);

    res.addAssign(new I32(10)); // res += 10
    expect(res.toNumber()).toEqual(30);
});

test("subAssign() should do basic assign (substraction)", () => {
    const res = new I32(20);
    expect(res.toNumber()).toEqual(20);

    res.subAssign(new I32(10)); // res -= 10
    expect(res.toNumber()).toEqual(10);
});

test("mulAssign() should do basic assign (multiplication)", () => {
    const res = new I32(20);
    expect(res.toNumber()).toEqual(20);

    res.mulAssign(new I32(2)); // res *= 2
    expect(res.toNumber()).toEqual(40);
});

test("divAssign() should do basic assign (division)", () => {
    const res = new I32(20);
    expect(res.toNumber()).toEqual(20);

    res.divAssign(new I32(2)); // res /= 2
    expect(res.toNumber()).toEqual(10);
});

test("modAssign() should do basic assign (modulo)", () => {
    const res = new I32(20);
    expect(res.toNumber()).toEqual(20);

    res.modAssign(new I32(2)); // res %= 2
    expect(res.toNumber()).toEqual(0);
});

test("expAssign() should do basic assign (exponent)", () => {
    const res = new I32(2);
    expect(res.toNumber()).toEqual(2);

    res.expAssign(new I32(3)); // res **= 3
    expect(res.toNumber()).toEqual(8);
});

test("lShiftAssign() should do basic assign (left shift)", () => {
    const res = new I32(2);
    expect(res.toNumber()).toEqual(2);

    res.lShiftAssign(new I32(3)); // res <<= 3
    expect(res.toNumber()).toEqual(16);
});

test("rShiftAssign() should do basic assign (right shift)", () => {
    const res = new I32(2);
    expect(res.toNumber()).toEqual(2);

    res.rShiftAssign(new I32(3)); // res >>= 3
    expect(res.toNumber()).toEqual(0);
});

test("bitOrAssign() should do basic assign (bit or)", () => {
    const res = new I32(2);
    expect(res.toNumber()).toEqual(2);

    res.bitOrAssign(new I32(3)); // res |= 3
    expect(res.toNumber()).toEqual(3);
});

test("bitAndAssign() should do basic assign (bit and)", () => {
    const res = new I32(2);
    expect(res.toNumber()).toEqual(2);

    res.bitAndAssign(new I32(3)); // res &= 3
    expect(res.toNumber()).toEqual(2);
});

test("bitXorAssign() should do basic assign (bit xor)", () => {
    const res = new I32(2);
    expect(res.toNumber()).toEqual(2);

    res.bitXorAssign(new I32(3)); // res ^= 3
    expect(res.toNumber()).toEqual(1);
});

test("eq() should do basic equal", () => {
    const res = (new I32(2)).eq(new I32(2)); // 2 == 2
    expect(res).toBe(true);
});

test("ne() should do basic not equal", () => {
    const res = (new I32(2)).ne(new I32(2)); // 2 not= 2
    expect(res).toBe(false);
});

test("neg() should do basic negative", () => {
    const res = (new I32(2)).neg().toNumber(); // -2
    expect(res).toEqual(-2);
});

test("lt() should do basic less", () => {
    const res = (new I32(2)).lt(new I32(10)); // 2 < 10
    expect(res).toBe(true);
});

test("gt() should do basic greater", () => {
    const res = (new I32(2)).gt(new I32(10)); // 2 > 10
    expect(res).toBe(false);
});

test("le() should do basic less equal", () => {
    const res = (new I32(2)).le(new I32(10)); // 2 <= 10
    expect(res).toBe(true);
});

test("ge() should do basic greater equal", () => {
    const res = (new I32(2)).ge(new I32(10)); // 2 >= 10
    expect(res).toBe(false);
});
