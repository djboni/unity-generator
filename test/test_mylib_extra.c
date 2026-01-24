#include "mylib.h"
#include "unity.h"

void suiteSetUp_mylib_extra(void) {
    /* Suite Set Up - Runs once before the first test. */
}

int suiteTearDown_mylib_extra(int num_errors) {
    /* Suite Tear Down - Runs once after the last test.
     *
     * Called with the number of errors in the suite.
     *
     * Return 0 for success and !=0 for failure (for compatibility with other
     * Unity generators). Currently the runner ignores the return value.
     */
    return num_errors;
}

void setUp_mylib_extra(void) {
    /* Test Set Up - Runs before each test. */
}

void tearDown_mylib_extra(void) {
    /* Test Tear Down - Runs after each test, even if ignored or failed. */
}

void test_pass(void) {
    /* This test passes. */
}

#if 0
/* Works with tests inside conditional compilation. */
void test_ignore(void) {
    TEST_IGNORE_MESSAGE("this test is ignored");
}
#endif

/* Works with commented tests.
void test_fail(void) {
    TEST_FAIL_MESSAGE("this test fails");
}
*/
