# [Unity Test Runner Generator](https://github.com/djboni/unity-generator)

Unity test runner generator written in C.

- Completely self contained `generator.c` file
- Only a C compiler is needed
- Allows multiple test files in one test executable
- Works with conditional compilation of tests (inside `#if`)
- Works with tests inside comments
- Compatible with the original Unity runner generator:
  - Test functions:
    - `void test*(void)`
    - `void spec*(void)`
  - Test set up and tear down functions:
    - `void setUp*(void)`
    - `void tearDown*(void)`
  - Suite set up and tear down functions:
    - `void suiteSetUp*(void)`
    - `int suiteTearDown*(int num_errors)`

The main file of this repository is `test/unity/generator.c`.

The remaining are supporting files and examples.

You can also use this repository as the starting point for your project.

## Using the Generator

Build the generator with one command:

```sh
gcc -o generator src/generator.c
```

Generate test runners for multiple test files:

```sh
./generator test/runner_global.c test/test_mylib.c test/test_mylib_extra.c
```

- The first argument is the output global runner, which calls the file runners
  - Creates one file `test/runner_global.c`
- The other arguments are the test files to generate file runners
  - Creates files `test/runner_test_*.c`

Build the test executable:

```sh
gcc -o test_mylib -Iinclude -Itest/unity src/mylib.c test/*.c test/unity/unity.c
```

Run the test executable:

```sh
./test_mylib
```

## Makefile

There is a makefile you can try:

```sh
make test
```

## Testing the Generator

The steps are pretty much the same.

These are in the directory test2/ to avoid conflicts.

```sh
gcc -o generator src/generator.c
./generator test2/runner_generator.c test2/test_generator.c
gcc -o test_generator -Itest/unity/ -Isrc/ test2/test_generator.c test2/runner_test_generator.c test2/runner_generator.c test/unity/unity.c
./test_generator
```

## Why this Project?

TL;DR The best is to use a single language for the whole project.

TL;DR We do not want to install Ruby (and Python, and Zig, and C++, ...)
just to build the tests.

Some time ago I needed to unit-test an embedded C project I was working on.
I tried several test frameworks. All had their advantages and, unfortunately,
also drawbacks.

At that time I settled on CppUTest. Unlike in C based test frameworks, we did
not need to do the extra work of registering the new tests. The C++
constructors took care of that. It was nice.

However, for another project, we had a very strict C++98 compiler, that could
not compile CppUTest. And, even if it would compile, it looked like the C++
executable size would be a problem too. We found about the automated conversion
from CppUTest to the Unity test framework. The former was for development and
the latter for testing in the embedded hardware.

The conversion tool was written in Ruby. It was a bit annoying having to
install it just to run the conversion. But the bigger issue was maintaining the
tests written in a compatible way for both test frameworks.

Soon we left CppUtest and the conversion tool and started using only Unity.
As any C test framework, Unity does not have automatic registering of new
tests, but it has its own test runner generator, written in Ruby. But it forced
one executable for each test file. And we had a lot of test files.
And each one of them needed to be flashed in the embedded hardware.
That was painfully slow.

Another pain point were the commented test functions and the conditional
compilation of test functions. The Unity generator added them anyways and we
got linker errors.

So we had the great idea to write ourselves a test runner generator tool that
solves our problems:

- Unity (C, small footprint)
- One executable for several test files
- Ignore test functions inside comments
- Work with functions inside conditional compilation

We wrote it in Python, because we knew how to use it and already used it for
other things too. After a few iterations on this idea we were happy with the
tool.

I reimplemented it in Zig while learning that language and used it since.

What I discovered is that it is really annoying working in a multi language
repository. Even if "only a few helper tools" are written outside of the main
language.

Soon everyone has a great idea for a tool and wants to use his own favorite
language. And then everyone have to install all those pesky dependencies.

Ruby, Python, Zig, C++, Haskel, Scheme, Bash, Powershell, ... and the list goes on.

Each developer uses a different Operating System.
Now all of those tools, in different languages, need to work on several
Operating Systems. And of course, in all OS versions.

The new language version is out and some people upgraded and others did not.
The new Debian or Ubuntu was released and all languages changed versions.
Or the same but because the Arch guy upgraded after lunch.
So now we also have to maintain for all versions of the language.

Then something with glib-c incompatibility.

And then the Windows installer download page does not exist anymore, so the new
guy cannot install the dependencies. And his PATH is messed up.

It is simply a nightmare to keep the tools and the documentation up to date.

Now I need the same tool; and also peace.
