/* DO NOT EDIT: File automatically generated. */
#include "unity.h"

void UnityCustomTestRun(void (*Func)(void), void (*SetUp)(void), void (*TearDown)(void), const char *FuncName, const int FuncLineNum) {
    Unity.CurrentTestName = FuncName;
    Unity.CurrentTestLineNumber = (UNITY_LINE_TYPE)FuncLineNum;
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches()) {
        return;
    }
#endif
    Unity.NumberOfTests++;
#ifndef UNITY_EXCLUDE_DETAILS
#    ifdef UNITY_DETAIL_STACK_SIZE
    Unity.CurrentDetailStackSize = 0;
#    else
    UNITY_CLR_DETAILS();
#    endif
#endif
    UNITY_EXEC_TIME_START();
    if (TEST_PROTECT()) {
        if (SetUp != NULL)
            SetUp();
        Func();
    }
    if (TEST_PROTECT()) {
        if (TearDown != NULL)
            TearDown();
    }
    UNITY_EXEC_TIME_STOP();
    UnityConcludeTest();
}

void setUp(void) {
}

void tearDown(void) {
}

extern void runner_test_mylib(void);
extern void runner_test_mylib_extra(void);

int main(int argc, char **argv) {
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    int ret = UnityParseOptions(argc, argv);
    if (ret > 0) {
        return ret;
    }
#else
    (void)argc;
    (void)argv;
#endif

    UnityBegin(NULL);

    runner_test_mylib();
    runner_test_mylib_extra();

    return UnityEnd();
}
