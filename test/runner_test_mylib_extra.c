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
extern void suiteSetUp_mylib_extra(void);
extern int suiteTearDown_mylib_extra(int num_errors);
extern void setUp_mylib_extra(void);
extern void tearDown_mylib_extra(void);
extern void test_pass(void);
extern void test_ignore(void);

void runner_test_mylib_extra(void) {
    UNITY_COUNTER_TYPE failures_at_start = Unity.TestFailures;
    UnitySetTestFile("test/test_mylib_extra.c");

    suiteSetUp_mylib_extra();

    UnityCustomTestRun(test_pass, setUp_mylib_extra, tearDown_mylib_extra, "test_pass", 27);
#if 0
    UnityCustomTestRun(test_ignore, setUp_mylib_extra, tearDown_mylib_extra, "test_ignore", 33);
#endif

    (void)suiteTearDown_mylib_extra(Unity.TestFailures - failures_at_start);
}
