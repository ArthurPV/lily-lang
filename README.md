# Lily

```lily
import "@std.Io"

fun main =
	Io.println("Hello")
end
```

## Build the project

To build this project, it is recommended to have at least Clang version 12 or GCC version 12. However the versions of Clang and GCC which do not support the C23 standard, it is obligatory to modify the version of C used in the CMake config. Fortunately to go faster it is possible to apply a patch to make CMake compatible with C11 standard.

```bash
./scripts/patch_cmake.sh
```

Follow these instructions to build the project:

```bash
make
cd build/Debug
ninja
```

## Platforms support

| Platform / Architecture | x86        | x86_64     | arm        | mips       |
|-------------------------|------------|------------|------------|------------|
| Windows (7, 8, 10, ..)  | not tested | not tested | not tested | ---------- |
| Linux                   | not tested | tested     | not tested | not tested |
| BSD                     | not tested | not tested | not tested | not tested |
| macOS                   | ---------  | not tested | not tested | ---------  |

## Features

The Lily language is a multi-paradigm, object-oriented, functional, generic and imperative language. It has strong typing, static and embeds type inference.

### Function

When you declare a function you don't have to add a type to the parameters of the function. The compiler reserves the right to assign abstract types to describe an ambiguous type. Moreover, it is logical that you are not obliged to declare the generic parameters, as in the OCaml and Haskell languages.

Here are some examples of function declaration:

```lily
fun add(x) = x;
```

```lily
fun add[T](x T) T = x;
```

```lily
fun add(x) =
	return x
end
```

These are all ways to declare functions in Lily.

### Type

There are several types such as `record`, `enum` and `alias`.

NOTE: It is not possible to omit generic parameters like for functions. 

#### Record

Lily records are the simplest structure for recording information in a functional way. It is possible to have public or private fields (private by default). 

Here are some examples of `record` declaration in Lily:

```lily
type Car: record =
	name Str,
	price Uint64
end
```

```lily
type Custom[T]: record =
	pub v T
end
```

#### Enum

Lily enums are very similar to those found in the Haskell or OCaml language. Except that it is possible to natively add default values to each variant or make error enums.

Here are some examples of `enum` declaration in Lily:

```lily
type Animal: enum =
	Cat,
	Dog,
end
```

```lily
type Digit: enum(Uint8) =
	Zero,
	One,
	Two,
	Tree,
	Fore,
	Five,
	Six,
	Seven,
	Eight,
	Nine
end
```

```lily
type Error: enum(error) =
	Panic,
	Fatal,
end
```

```lily
type Expr: enum =
	Add (Expr, Expr),
	Sub (Expr, Expr),
	Mul (Expr, Expr),
	Div (Expr, Expr),
	Int Int32
end
```

#### Alias

Aliases are really the simplest types, they simply allow you to rename a type.

Here are some examples of `alias` declarations in Lily:

```lily
type Number: alias = Uint64;
```

```lily
type Custom[T] = T;
```

### Object

There are several types such as `record`, `enum`, `trait` and `class`.

#### Record

The records defined with the `object` keyword are similar to the records defined with the `type` keyword, except that the only difference is that it is possible to attach functions to the record.

Here are an example of `record` (object) declaration in Lily:

```lily
object Car: record =
	name Str,
	price Uint64
end

tag Car =
	fun get_name(self) = self.name;

	fun get_price(self) = self.price;

	fun update_price(mut self, price) = self.price = price;
end

// or

fun#Car get_name(self) = self.name;

fun#Car get_price(self) = self.price;

fun#Car update_price(mut self, price) = self.price = price;

```

#### Enum 

#### Trait

#### Class

### Error

### Module

### Constant

### Import

Have fun coding :)
