# Lily

```lily
fun add(x, y) = x+y;
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
