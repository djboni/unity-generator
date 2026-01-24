CC = gcc
CFLAGS = -ggdb3 -Wall -Wextra -std=c90 -pedantic -DUNITY_USE_COMMAND_LINE_ARGS
INCLUDES = -I./include/ -I./test/unity/

# Only build
all: ./build/generator ./build/test_mylib

# Build the test executable and run the tests
test: ./build/test_mylib
	./build/test_mylib

# Clean command
clean:
	rm -fr ./build ./generator ./test_mylib
	find ./test -type f -name 'runner_*.c' -delete

# Build artifacts directory
./build:
	mkdir -p ./build

# Executable that generates test runners
./build/generator: ./test/unity/generator.c | ./build
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

# Generate test runners: global runner and all test files
./test/runner_global.c: \
		./test/test_mylib.c \
		./test/test_mylib_extra.c \
		| ./build/generator
	./build/generator ./test/runner_global.c $^

# Build the test executable
./build/test_mylib: \
		./src/mylib.c \
		./test/runner_global.c \
		./test/test_mylib.c ./test/runner_test_mylib.c \
		./test/test_mylib_extra.c ./test/runner_test_mylib_extra.c \
		./test/unity/unity.c
	$(CC) $(CFLAGS) -DGENERATOR_NO_MAIN $(INCLUDES) $^ -o $@
