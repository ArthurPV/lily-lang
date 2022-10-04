configure:
	@mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -s Debug/compile_commands.json . 

build:
	@cd src/lang/runtime/js && @tsc --build
	@cd build && ninja .
	@cd build/Debug && ninja .

test:
	@./build/scanner_test
	@cd tests/runtime/js && npm run-script vitest

check_leaks:
	export DEBUGINFOD_URLS="https://debuginfod.archlinux.org" && valgrind --leak-check=full ./build/Debug/lily compile ./test.lily
