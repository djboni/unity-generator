/* DO NOT EDIT: File automatically generated. */
#include "unity.h"

/* Use GENERATOR_RUNNER to control definitions in the test and in the runner.
 * Useful to disable *_IMPLEMENTATION definitions in the runner when
 * using STB style header only libraries in the test file.
 */
#define GENERATOR_RUNNER 1
extern void UnityCustomTestRun(void (*Func)(void), void (*SetUp)(void), void (*TearDown)(void), const char *FuncName, const int FuncLineNum);

/* Headers and definitions from the test file. */
#include "mylib.h"
#include "unity.h"

/* Set up, tear down, and test functions from the test file. */
extern void test_mylib_myadd_ZeroAndZero_shouldReturnZero(void);
extern void test_mylib_myadd_OneAndOne_shouldReturnTwo(void);

void runner_test_mylib(void) {
    UnitySetTestFile("test/test_mylib.c");

    UnityCustomTestRun(test_mylib_myadd_ZeroAndZero_shouldReturnZero, NULL, NULL, "test_mylib_myadd_ZeroAndZero_shouldReturnZero", 4);
    UnityCustomTestRun(test_mylib_myadd_OneAndOne_shouldReturnTwo, NULL, NULL, "test_mylib_myadd_OneAndOne_shouldReturnTwo", 8);
}
