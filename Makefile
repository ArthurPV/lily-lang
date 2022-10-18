configure:
	@mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -s Debug/compile_commands.json . 

build:
	@cd src/lang/runtime/js && @tsc --build
	@cd build && ninja .
	@cd build/Debug && ninja .

test:
	@./build/scanner_test
	@./build/parser_test
	@./build/analysis_test
	@cd tests/runtime/js && npm run-script vitest

check_leaks:
	export DEBUGINFOD_URLS="https://debuginfod.archlinux.org" && valgrind --leak-check=full ./build/Debug/lily compile ./test.lily

format:
	@clang-format -i src/base/*.h
	@clang-format -i src/base/*.c
	@clang-format -i src/bin/*.c
	@clang-format -i src/command/*.h
	@clang-format -i src/command/*.c
	@clang-format -i src/lang/analysis/*.c
	@clang-format -i src/lang/analysis/*.h
	@clang-format -i src/lang/builtin/*.c
	@clang-format -i src/lang/builtin/*.h
	@clang-format -i src/lang/diagnostic/*.c
	@clang-format -i src/lang/diagnostic/*.h
	@clang-format -i src/lang/generate/*.c
	@clang-format -i src/lang/generate/*.h
	@clang-format -i src/lang/parser/*.c
	@clang-format -i src/lang/parser/*.h
	@clang-format -i src/lang/runtime/c/*.c
	@clang-format -i src/lang/runtime/c/*.h
	@clang-format -i src/lang/runtime/cpp/*.cpp
	@clang-format -i src/lang/runtime/cpp/*.hpp
	@clang-format -i src/lang/scanner/*.c
	@clang-format -i src/lang/scanner/*.h
	@clang-format -i tests/scanner/*.c
	@clang-format -i tests/scanner/*.h
	@clang-format -i tests/parser/*.c
