import { I128 } from "../../../src/lang/runtime/js/Int128";

test("should do Int128 overflow", () => {
	let failed = false;

	try {
		new I128(20000000000000000000000000000000000000000000000000000000000000000000000n);
	} catch (err) {
		failed = true;
	}

	expect(failed).toBe(true);
});

test("add() should do basic addition", () => {
	const res = (new I128(20n)).add(new I128(10n)).toBigInt(); // 20 + 10
	expect(res).toEqual(30n);
});

test("sub() should do basic substraction", () => {
	const res = (new I128(20n)).sub(new I128(10n)).toBigInt(); // 20 - 10
	expect(res).toEqual(10n);
});

test("mul() should do basic multiplication", () => {
	const res = (new I128(20n)).mul(new I128(4n)).toBigInt(); // 20 * 4
	expect(res).toEqual(80n);
});

test("div() should do basic division", () => {
	const res = (new I128(20n).div(new I128(10n))).toBigInt(); // 20 / 10
	expect(res).toEqual(2n);
});

test("mod() should do basic modulo", () => {
	const res = (new I128(20n).mod(new I128(10n))).toBigInt(); // 20 % 10
	expect(res).toEqual(0n);
});

test("exp() should do basic exponent", () => {
	const res = (new I128(2n).exp(new I128(3n))).toBigInt(); // 2 ** 3
	expect(res).toEqual(8n);
})

test("lShift() should do basic left shift", () => {
	const res = (new I128(2n).lShift(new I128(3n))).toBigInt(); // 2 << 3
	expect(res).toEqual(16n);
});

test("rShift() should do basic right shift", () => {
	const res = (new I128(2n).rShift(new I128(3n))).toBigInt(); // 2 >> 3
	expect(res).toEqual(0n);
});

test("bitOr() should do basic bit or", () => {
	const res = (new I128(2n).bitOr(new I128(3n))).toBigInt(); // 2 | 3
	expect(res).toEqual(3n);
});

test("bitAnd() should do basic bit and", () => {
	const res = (new I128(2n).bitAnd(new I128(3n))).toBigInt(); // 2 & 3
	expect(res).toEqual(2n);
});

test("bitXor() should do basic bit xor", () => {
	const res = (new I128(2n).bitXor(new I128(3n))).toBigInt(); // 2 ^ 3
	expect(res).toEqual(1n);
});

test("bitNot() should do basic bit not", () => {
	const res = (new I128(2n)).bitNot().toBigInt(); // ~2
	expect(res).toEqual(-3n);
});

test("assign() should do basic assign", () => {
	const res = new I128(20n);
	expect(res.toBigInt()).toEqual(20n);

	res.assign(new I128(10n)); // res = 10
	expect(res.toBigInt()).toEqual(10n);
});

test("addAssign() should do basic assign (addition)", () => {
	const res = new I128(20n);
	expect(res.toBigInt()).toEqual(20n);

	res.addAssign(new I128(10n)); // res += 10
	expect(res.toBigInt()).toEqual(30n);
});

test("subAssign() should do basic assign (substraction)", () => {
	const res = new I128(20n);
	expect(res.toBigInt()).toEqual(20n);

	res.subAssign(new I128(10n)); // res -= 10
	expect(res.toBigInt()).toEqual(10n);
});

test("mulAssign() should do basic assign (multiplication)", () => {
	const res = new I128(20n);
	expect(res.toBigInt()).toEqual(20n);

	res.mulAssign(new I128(2n)); // res *= 2
	expect(res.toBigInt()).toEqual(40n);
});

test("divAssign() should do basic assign (division)", () => {
	const res = new I128(20n);
	expect(res.toBigInt()).toEqual(20n);

	res.divAssign(new I128(2n)); // res /= 2
	expect(res.toBigInt()).toEqual(10n);
});

test("modAssign() should do basic assign (modulo)", () => {
	const res = new I128(20n);
	expect(res.toBigInt()).toEqual(20n);

	res.modAssign(new I128(2n)); // res %= 2
	expect(res.toBigInt()).toEqual(0n);
});

test("expAssign() should do basic assign (exponent)", () => {
	const res = new I128(2n);
	expect(res.toBigInt()).toEqual(2n);

	res.expAssign(new I128(3n)); // res **= 3
	expect(res.toBigInt()).toEqual(8n);
});

test("lShiftAssign() should do basic assign (left shift)", () => {
	const res = new I128(2n);
	expect(res.toBigInt()).toEqual(2n);

	res.lShiftAssign(new I128(3n)); // res <<= 3
	expect(res.toBigInt()).toEqual(16n);
});

test("rShiftAssign() should do basic assign (right shift)", () => {
	const res = new I128(2n);
	expect(res.toBigInt()).toEqual(2n);

	res.rShiftAssign(new I128(3n)); // res >>= 3
	expect(res.toBigInt()).toEqual(0n);
});

test("bitOrAssign() should do basic assign (bit or)", () => {
	const res = new I128(2n);
	expect(res.toBigInt()).toEqual(2n);

	res.bitOrAssign(new I128(3n)); // res |= 3
	expect(res.toBigInt()).toEqual(3n);
});

test("bitAndAssign() should do basic assign (bit and)", () => {
	const res = new I128(2n);
	expect(res.toBigInt()).toEqual(2n);

	res.bitAndAssign(new I128(3n)); // res &= 3
	expect(res.toBigInt()).toEqual(2n);
});

test("bitXorAssign() should do basic assign (bit xor)", () => {
	const res = new I128(2n);
	expect(res.toBigInt()).toEqual(2n);

	res.bitXorAssign(new I128(3n)); // res ^= 3
	expect(res.toBigInt()).toEqual(1n);
});

test("eq() should do basic equal", () => {
	const res = (new I128(2n)).eq(new I128(2n)); // 2 == 2
	expect(res).toBe(true);
});

test("ne() should do basic not equal", () => {
	const res = (new I128(2n)).ne(new I128(2n)); // 2 not= 2
	expect(res).toBe(false);
});

test("neg() should do basic negative", () => {
	const res = (new I128(2n)).neg().toBigInt(); // -2
	expect(res).toEqual(-2n);
});

test("lt() should do basic less", () => {
	const res = (new I128(2n)).lt(new I128(10n)); // 2 < 10
	expect(res).toBe(true);
});

test("gt() should do basic greater", () => {
	const res = (new I128(2n)).gt(new I128(10n)); // 2 > 10
	expect(res).toBe(false);
});

test("le() should do basic less equal", () => {
	const res = (new I128(2n)).le(new I128(10n)); // 2 <= 10
	expect(res).toBe(true);
});

test("ge() should do basic greater equal", () => {
	const res = (new I128(2n)).ge(new I128(10n)); // 2 >= 10
	expect(res).toBe(false);
});
