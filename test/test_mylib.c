#include "mylib.h"
#include "unity.h"

void test_mylib_myadd_ZeroAndZero_shouldReturnZero(void) {
    TEST_ASSERT_EQUAL(0, myadd(0, 0));
}

void test_mylib_myadd_OneAndOne_shouldReturnTwo(void) {
    TEST_ASSERT_EQUAL(2, myadd(1, 1));
}
