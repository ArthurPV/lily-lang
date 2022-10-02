# Lily

## Table of contents

- [Introduction](###Introduction)
- [Hello World](###Hello-World)
- [Comments](###Comments)
- [Values](###Values)
- [Variables](###Variables)
- [Integers](###Integers)
- [Floats](###Floats)
- [Operators](###Operators)
- [Arrays](###Arrays)
- [fun](###fun)
- [alias](###alias)
- [record (type)](###record-(type))
- [enum (type)](###enum-(type))
- [record (object)](###record-(object))
- [enum (object)](###enum-(object))
- [class](###class)
- [trait](###trait)
- [match](###match)
- [while](###while)
- [for](###for)
- [if](###if)
- [try](###try)
- [errror](###error)
- [Optionals](###Optionals)
- [Generics](###Generics)
- [async](###async)

### Introduction

Lily is a general-purpose programming language.

### Hello World

```lily
import "@std.io"

fun main =
	io.println("Hello, World!")
end
```

### Comments

```lily
import "@std.io"

fun main =
	// To create a comment in Lily you must start with "//".

	io.println("Hello, World!", [])
end
```

#### Doc comments

```lily
import "@std.io"

// To create a doc comment in Lily you must start with "///".

/// @prot{Int32 -> Int32 -> Int32}
fun add(x, y) = x + y;

fun main =
	io.println("Add: {d}", [add(2, 3)])
end
```
