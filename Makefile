configure:
	@mkdir -p build && cd build && cmake .. -G Ninja

test:
	@./build/scanner_test
