# Lily

## Table of contents

- <a href="#introduction">Introduction</a>
- <a href="#hello-world">Hello World</a>
- <a href="#comments"><Comments</a>
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
import "@std.io"

fun main =
	io.println("Hello, World!")
end
```

</section>

<section id="comments">

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

</section>
