cmake_minimum_required(VERSION 3.11)
project(lily C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_PATH src)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -O -O2 -O3")

set(BASE_SRC
        src/base/algorithm.c
        src/base/color.c
        src/base/file.c
        src/base/format.c
        src/base/list.c
        src/base/map.c
        src/base/mem.c
        src/base/option.c
        src/base/print.c
        src/base/result.c
        src/base/str.c
        src/base/string.c
        src/base/test.c
        src/base/tuple.c
        src/base/u128.c
        src/base/util.c
        src/base/vec.c)

set(LANG_SRC
        src/lang/analysis/symbol_table.c
        src/lang/analysis/typecheck.c
        src/lang/builtin/builtin_c.c
        src/lang/builtin/builtin.c
        src/command/parse.c
        src/lang/diagnostic/diagnostic.c
        src/lang/diagnostic/summary.c
        src/lang/generate/generate_c.c
        src/lang/generate/generate.c
        src/lang/parser/ast.c
        src/lang/parser/parser.c
        src/lang/scanner/scanner.c
        src/lang/scanner/token.c)

add_library(lily_base ${BASE_SRC})
target_include_directories(lily_base PRIVATE src)

add_library(lily_lang ${LANG_SRC})
target_link_libraries(lily_lang lily_base)
target_include_directories(lily_lang PRIVATE src)

add_executable(lily src/bin/main.c
                    src/command/parse.c)
target_link_libraries(lily lily_lang lily_base)
target_include_directories(lily PRIVATE src)

add_executable(scanner_test
        tests/scanner/test.c
        tests/scanner/operator.c
        tests/scanner/separator.c
        tests/scanner/keyword.c
        tests/scanner/identifier.c
        tests/scanner/literal.c)
target_link_libraries(scanner_test lily_base lily_lang)
target_include_directories(scanner_test PRIVATE src)

add_test(NAME scanner_test COMMAND scanner_test)

add_executable(parser_test
	tests/parser/alias.c
	tests/parser/class.c
	tests/parser/constant.c
	tests/parser/enum.c
	tests/parser/error.c
	tests/parser/fun.c
	tests/parser/import.c
	tests/parser/module.c
	tests/parser/record.c
	tests/parser/tag.c
	tests/parser/test.c
	tests/parser/trait.c)
target_link_libraries(parser_test lily_base lily_lang)
target_include_directories(parser_test PRIVATE src)

add_test(NAME parser_test COMMAND parser_test)

add_subdirectory(src/lang/runtime/c)
add_subdirectory(src/lang/runtime/cpp)
