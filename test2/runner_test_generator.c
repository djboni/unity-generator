/* DO NOT EDIT: File automatically generated. */
#include "unity.h"

/* Use GENERATOR_RUNNER to control definitions in the test and in the runner.
 * Useful to disable *_IMPLEMENTATION definitions in the runner when
 * using STB style header only libraries in the test file.
 */
#define GENERATOR_RUNNER 1
extern void UnityCustomTestRun(void (*Func)(void), void (*SetUp)(void), void (*TearDown)(void), const char *FuncName, const int FuncLineNum);

/* Headers and definitions from the test file. */
#include "unity.h"
#include <string.h>
#ifdef GENERATOR_RUNNER
#    define GENERATOR_NO_IMPLEMENTATION
#endif
#ifndef GENERATOR_NO_MAIN
#    define GENERATOR_NO_MAIN
#endif
#include "generator.c"

/* Set up, tear down, and test functions from the test file. */
extern void suiteSetUp_tolken(void);
extern int suiteTearDown_tolken(int num_errors);
extern void setUp_tolken(void);
extern void tearDown_tolken(void);
extern void test_tolken_init_deinit(void);
extern void test_tolken_tokenize_one_line(void);
extern void test_tolken_tokenize_lf_lines(void);
extern void test_tolken_tokenize_crlf_lines(void);
extern void test_tolken_tokenize_cr_lines(void);
extern void test_tolken_tokenize_single_char_tokens(void);
extern void test_tolken_get_c_comments(void);
extern void test_tolken_get_cpp_comments(void);
extern void test_tolken_get_c_comments_line_column(void);
extern void test_tolken_get_cpp_comments_line_column(void);
extern void test_tolken_get_string(void);
extern void test_tolken_get_string_bug_with_escaped_slash_before_end_of_string(void);
extern void test_tolken_get_character(void);
extern void test_tolken_get_character_bug_with_escaped_slash_before_end_of_character(void);
extern void test_tolken_get_preprocessor_include(void);
extern void test_tolken_get_preprocessor_define(void);
extern void test_tolken_get_preprocessor_if(void);
extern void test_tolken_get_preprocessor_ifdef(void);

void runner_test_generator(void) {
    UNITY_COUNTER_TYPE failures_at_start = Unity.TestFailures;
    UnitySetTestFile("test2/test_generator.c");

    suiteSetUp_tolken();

    UnityCustomTestRun(test_tolken_init_deinit, setUp_tolken, tearDown_tolken, "test_tolken_init_deinit", 39);
    UnityCustomTestRun(test_tolken_tokenize_one_line, setUp_tolken, tearDown_tolken, "test_tolken_tokenize_one_line", 47);
    UnityCustomTestRun(test_tolken_tokenize_lf_lines, setUp_tolken, tearDown_tolken, "test_tolken_tokenize_lf_lines", 88);
    UnityCustomTestRun(test_tolken_tokenize_crlf_lines, setUp_tolken, tearDown_tolken, "test_tolken_tokenize_crlf_lines", 129);
    UnityCustomTestRun(test_tolken_tokenize_cr_lines, setUp_tolken, tearDown_tolken, "test_tolken_tokenize_cr_lines", 170);
    UnityCustomTestRun(test_tolken_tokenize_single_char_tokens, setUp_tolken, tearDown_tolken, "test_tolken_tokenize_single_char_tokens", 211);
    UnityCustomTestRun(test_tolken_get_c_comments, setUp_tolken, tearDown_tolken, "test_tolken_get_c_comments", 234);
    UnityCustomTestRun(test_tolken_get_cpp_comments, setUp_tolken, tearDown_tolken, "test_tolken_get_cpp_comments", 261);
    UnityCustomTestRun(test_tolken_get_c_comments_line_column, setUp_tolken, tearDown_tolken, "test_tolken_get_c_comments_line_column", 289);
    UnityCustomTestRun(test_tolken_get_cpp_comments_line_column, setUp_tolken, tearDown_tolken, "test_tolken_get_cpp_comments_line_column", 328);
    UnityCustomTestRun(test_tolken_get_string, setUp_tolken, tearDown_tolken, "test_tolken_get_string", 363);
    UnityCustomTestRun(test_tolken_get_string_bug_with_escaped_slash_before_end_of_string, setUp_tolken, tearDown_tolken, "test_tolken_get_string_bug_with_escaped_slash_before_end_of_string", 392);
    UnityCustomTestRun(test_tolken_get_character, setUp_tolken, tearDown_tolken, "test_tolken_get_character", 415);
    UnityCustomTestRun(test_tolken_get_character_bug_with_escaped_slash_before_end_of_character, setUp_tolken, tearDown_tolken, "test_tolken_get_character_bug_with_escaped_slash_before_end_of_character", 443);
    UnityCustomTestRun(test_tolken_get_preprocessor_include, setUp_tolken, tearDown_tolken, "test_tolken_get_preprocessor_include", 468);
    UnityCustomTestRun(test_tolken_get_preprocessor_define, setUp_tolken, tearDown_tolken, "test_tolken_get_preprocessor_define", 495);
    UnityCustomTestRun(test_tolken_get_preprocessor_if, setUp_tolken, tearDown_tolken, "test_tolken_get_preprocessor_if", 525);
    UnityCustomTestRun(test_tolken_get_preprocessor_ifdef, setUp_tolken, tearDown_tolken, "test_tolken_get_preprocessor_ifdef", 561);

    (void)suiteTearDown_tolken(Unity.TestFailures - failures_at_start);
}
