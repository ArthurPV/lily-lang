configure:
	@mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -s Debug/compile_commands.json . 

test:
	@./build/scanner_test

check_leaks:
	export DEBUGINFOD_URLS="https://debuginfod.archlinux.org" && valgrind --leak-check=full ./build/Debug/lily compile ./test.lily
