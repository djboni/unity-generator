CC = gcc
OPTIMIZATION = -O0
CFLAGS = $(OPTIMIZATION) -ggdb3 -Wall -Wextra
UNITY_CFLAGS = -I./test/unity/ -DUNITY_USE_COMMAND_LINE_ARGS
LDFLAGS =
INCLUDES = -I./include/
EXT =

OS := $(shell uname -s)
ifneq (,$(findstring Windows,$(OS)))
    EXT = .exe
else ifneq (,$(findstring MINGW,$(OS)))
    EXT = .exe
else
endif

# Source, object and dependency files
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SOURCES := $(call rwildcard,include src test,*.c)
DIRS := $(sort $(patsubst %,build/%,$(dir $(SOURCES))))
OBJS := $(SOURCES:%.c=./build/%.o)
DEPS := $(SOURCES:%.c=./build/%.d)

# Only build
all: prerequisites \
	./build/generator$(EXT) \
	./build/test_generator$(EXT) \
	./build/test_mylib$(EXT) \

# Build and run the tests
test: all
	./build/test_generator$(EXT)
	./build/test_mylib$(EXT)

# Clean command
clean:
	rm -fr ./build

prerequisites: ./build ./build/compile_commands.json

# Build artifacts directory
./build:
	mkdir -p $(DIRS)

# Build object files
./build/test/%.o: ./test/%.c | ./build
	$(CC) $(INCLUDES) $(CFLAGS) $(UNITY_CFLAGS) -MMD -MP -o $@ -c $<
./build/%.o: ./%.c | ./build
	$(CC) $(INCLUDES) $(CFLAGS) -MMD -MP -o $@ -c $<

# Compilation commands for LSP
./build/compile_commands.json: Makefile | ./test/runner_global.c ./build
	which grep jq >/dev/null 2>&1 && make --always-make --dry-run \
	    | grep -wE 'gcc|g\+\+|clang|clang\+\+|zig cc|zig c\+\+|filcc|fil\+\+' \
	    | grep -w '\-c' \
	    | jq -nR '[inputs|{directory:"$(PWD)", command:., file: match(" [^ ]+$$").string[1:]}]' \
	    > ./build/compile_commands.json || touch ./build/compile_commands.json

# Executable that generates test runners
./build/generator$(EXT): ./build/src/generator.o | ./build
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Generate test runners: global runner and all test files
./test/runner_global.c: \
		./test/test_mylib.c \
		./test/test_mylib_extra.c \
		| ./build/generator$(EXT)
	./build/generator$(EXT) ./test/runner_global.c $^

# Build the test executable
./build/test_generator$(EXT): \
		test2/test_generator.c test2/runner_test_generator.c \
		test2/runner_generator.c \
		test/unity/unity.c
	gcc -o $@ -Itest/unity/ -Isrc/ test2/test_generator.c test2/runner_test_generator.c test2/runner_generator.c test/unity/unity.c

# Build the test executable
./build/test_mylib$(EXT): \
		./build/src/mylib.o \
		./build/test/runner_global.o \
		./build/test/test_mylib.o ./build/test/runner_test_mylib.o \
		./build/test/test_mylib_extra.o ./build/test/runner_test_mylib_extra.o \
		./build/test/unity/unity.o \
		| ./build
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Include the dependency files
-include $(DEPS)
