# Lily

Fast, no visibile alloc, no garbage collection.

## Table of contents

- <a href="#introduction">Introduction</a>
- <a href="#hello-world">Hello World</a>
- <a href="#comments">Comments</a>
- <a href="#values">Values</a>
- <a href="#variables">Variables</a>
- <a href="#integers">Integers</a>
- <a href="#floats">Floats</a>
- <a href="#operators">Operators</a>
- <a href="#arrays">Arrays</a>
- <a href="#fun">Fun</a>
- <a href="#alias">Alias</a>
- <a href="#record-type">Record (type)</a>
- <a href="#enum-type">Enum (type)</a>
- <a href="#record-object">Record (object)</a>
- <a href="#enum-object">Enum (object)</a>
- <a href="#class">Class</a>
- <a href="#trait">Trait</a>
- <a href="#match">Match</a>
- <a href="#while">While</a>
- <a href="#for">For</a>
- <a href="#if">If</a>
- <a href="#try">Try</a>
- <a href="#error">Error</a>
- <a href="#optionals">Optionals</a>
- <a href="#generics">Generics</a>
- <a href="#async">async</a>

<section id="introduction">

### Introduction

Lily is a general-purpose programming language.

</section>

<section id="hello-world">

### Hello World

```lily
import "@std.Io"

fun main =
	Io.println("Hello, World!")
end
```

</section>

<section id="comments">

### Comments

```lily
import "@std.Io"

fun main =
	// To create a comment in Lily you must start with "//".

	Io.println("Hello, World!", [])
end
```

#### Doc comments

```lily
import "@std.Io"

// To create a doc comment in Lily you must start with "///".

/// @prot{Int32 -> Int32 -> Int32}
fun add(x, y) = x + y;

fun main =
	Io.println("Add: {d}", [add(2, 3)])
end
```

</section>

<section id="values">

### Values

```lily
import "@std.Array"

fun main =
	a := 3.33F32 // Cast Float32
	b := 3.222F64 // Cast Float64
	c := 3.322 // Float64
	d := 3. // Float64

	d := "Hello" // Str

	e := true // Bool
	f := false // Bool
	g :: Bool := undef // Bool

	h := 333 // Int32
	i :: Uint8 := 3 // Uint8
	j := 3 + 3 // Int32
	k := -2 // Int32

	l := [1, 2, 3, 4, 5] // [5]Int32
	m :: [_]Int32 := [] // [_]Int32 -> dynamic array

	Array.push(m, 3) // Anticipate allocation of memory
	n :: Usize := m |> Array.length()

	o :: (Int32, Str) := (3, "hello")
	p := (true, "hello", 9) // (Bool, Str, Int32)

	q := None // ?`T -> type defined by the compiler
	r := undef // `T -> type defined by the compiler
	s := nil // *`T -> type defined by the compiler
end
```

#### Primitive types

| Type    	| Description                            |
|-----------|----------------------------------------|
| `Int8`    | signed 8-bit integer                   |
| `Int16`   | signed 16-bit integer                  |
| `Int32`   | signed 32-bit integer                  |
| `Int64`   | signed 64-bit integer                  |
| `Int128`  | signed 128-bit integer                 |
| `Uint8`   | unsigned 8-bit integer                 |
| `Uint16`  | unsigned 16-bit integer                |
| `Uint32`  | unsigned 32-bit integer                |
| `Uint64`  | unsigned 64-bit integer                |
| `Uint128` | unsigned 128-bit integer               |
| `Usize`   | unsigned pointer size integer          |
| `Isize`   | signed pointer size integer            |
| `Float32` | 32-bit floating point                  |
| `Float64` | 64-bit floating point                  |
| `Bool`    | type which has the value true or false |
| `Unit`    | like void C type                       |
| `Str`     | like char* C type                      |

#### Primitive values

| Name                 | Description        |
|----------------------|--------------------|
| `true` and `false`   | Bool values        |
| `nil`                | Value for pointer  |
| `None`               | Value for optional |
| `undef`              | Unspecified value  |

</section>

<section id="variables">

### Variables

In the Lily language, variables are immutable by default, as explained in this example:

```lily
fun main =
	a := 3 // Int32
	
	// a = 30
	// ^^^^^^ -> cannot assign an other value because the variable is immutable by default
end
```

#### mutable variable

For variables to be mutable, you must add `mut` before the variable name.

```lily
fun main =
	mut a := 3 // Int32

	a = 30 // Ok
	// now the variable's value is equal to 30
end
```

</section>

<section id="integers">

### Integers

List of integer types in the Lily language:

signed:

- Int8
- Int16
- Int32
- Int64
- Int128

unsigned:

- Uint8
- Uint16
- Uint32
- Uint32
- Uint64
- Uint128

```lily
fun main =
	a := 33I8 // Cast Int8
	b := 10I16 // Cast Int16
	c := 300I32 // Cast Int32
	d := 200I64 // Cast Int64
	e := 10000I128 // Cast Int128

	f := 33U8 // Cast Uint8
	g := 10U16 // Cast Uint16
	h := 300U32 // Cast Uint32
	i := 400U64 // Cast Uint64
	j := 500000U128 // Cast Uint128

	k := 1_000_000 // Int32
	l := 0xFF_FF_FF // Int32
	m := 0b111_111 // Int32
	n := 0o123_123 // Int32
end
```

</section>

<section id="floats">

### Floats

List of floating point types in the Lily language:

- Float32
- Float64


```lily
fun main =
	a := 3.33F32; // Cast Float32
	b := 3.322F64; // Cast Float64

	c := 3.33_333; // Float64
	d := 2.3E+2; // Float64
	e := 5.2e+10; // Float64
end
```

</section>

<section id="operators">

### Operators

</section>
