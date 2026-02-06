#include "unity.h"

#include <string.h>

#ifdef GENERATOR_RUNNER
#    define GENERATOR_NO_IMPLEMENTATION
#endif
#ifndef GENERATOR_NO_MAIN
#    define GENERATOR_NO_MAIN
#endif
#include "generator.c"

tolkenizer_t t = {0};
token_t s = {0};

void suiteSetUp_tolken(void) {
}

int suiteTearDown_tolken(int num_errors) {
    return num_errors;
}

void setUp_tolken(void) {
    /* Causes segmentation fault when not initialized. */
    memset(&t, 0x5A, sizeof(t));
    memset(&s, 0x5A, sizeof(t));
}

void tearDown_tolken(void) {
    /* Test if deinitialized. */
    TEST_ASSERT_EQUAL(NULL, t.data.items);
    TEST_ASSERT_EQUAL(0, t.data.count);
    TEST_ASSERT_EQUAL(0, t.data.capacity);
    TEST_ASSERT_EQUAL(NULL, s.items);
    TEST_ASSERT_EQUAL(0, s.count);
    TEST_ASSERT_EQUAL(0, s.capacity);
}

void test_tolken_init_deinit(void) {
    const char code[] = "";
    tolken_init_str(&t, "test.c", code, strlen(code));
    tolken_token_init(&s);
    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_tokenize_one_line(void) {
    const char code[] = "a simplistic string";
    tolken_init_str(&t, "test.c", code, strlen(code));
    tolken_token_init(&s);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("a", (char *)s.items);
    TEST_ASSERT_EQUAL(1, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(0, s.pos);
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("simplistic", (char *)s.items);
    TEST_ASSERT_EQUAL(10, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(2, s.pos);
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(3, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("string", (char *)s.items);
    TEST_ASSERT_EQUAL(6, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(13, s.pos);
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(14, s.column);

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(19, s.pos);
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(20, s.column);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_tokenize_lf_lines(void) {
    const char code[] = "a \n  simplistic \n   string \n  ";
    tolken_init_str(&t, "test.c", code, strlen(code));
    tolken_token_init(&s);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("a", (char *)s.items);
    TEST_ASSERT_EQUAL(1, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(0, s.pos);
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("simplistic", (char *)s.items);
    TEST_ASSERT_EQUAL(10, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(5, s.pos);
    TEST_ASSERT_EQUAL(2, s.line);
    TEST_ASSERT_EQUAL(3, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("string", (char *)s.items);
    TEST_ASSERT_EQUAL(6, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(20, s.pos);
    TEST_ASSERT_EQUAL(3, s.line);
    TEST_ASSERT_EQUAL(4, s.column);

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(30, s.pos);
    TEST_ASSERT_EQUAL(4, s.line);
    TEST_ASSERT_EQUAL(3, s.column);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_tokenize_crlf_lines(void) {
    const char code[] = "a \r\n  simplistic \r\n   string \r\n  ";
    tolken_init_str(&t, "test.c", code, strlen(code));
    tolken_token_init(&s);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("a", (char *)s.items);
    TEST_ASSERT_EQUAL(1, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(0, s.pos);
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("simplistic", (char *)s.items);
    TEST_ASSERT_EQUAL(10, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(6, s.pos);
    TEST_ASSERT_EQUAL(2, s.line);
    TEST_ASSERT_EQUAL(3, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("string", (char *)s.items);
    TEST_ASSERT_EQUAL(6, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(22, s.pos);
    TEST_ASSERT_EQUAL(3, s.line);
    TEST_ASSERT_EQUAL(4, s.column);

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(33, s.pos);
    TEST_ASSERT_EQUAL(4, s.line);
    TEST_ASSERT_EQUAL(3, s.column);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_tokenize_cr_lines(void) {
    const char code[] = "a \r  simplistic \r   string \r  ";
    tolken_init_str(&t, "test.c", code, strlen(code));
    tolken_token_init(&s);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("a", (char *)s.items);
    TEST_ASSERT_EQUAL(1, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(0, s.pos);
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("simplistic", (char *)s.items);
    TEST_ASSERT_EQUAL(10, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(5, s.pos);
    TEST_ASSERT_EQUAL(2, s.line);
    TEST_ASSERT_EQUAL(3, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("string", (char *)s.items);
    TEST_ASSERT_EQUAL(6, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(20, s.pos);
    TEST_ASSERT_EQUAL(3, s.line);
    TEST_ASSERT_EQUAL(4, s.column);

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);
    TEST_ASSERT_EQUAL_STRING("test.c", s.filename);
    TEST_ASSERT_EQUAL(30, s.pos);
    TEST_ASSERT_EQUAL(4, s.line);
    TEST_ASSERT_EQUAL(3, s.column);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_tokenize_single_char_tokens(void) {
    const char code[] = "int test_func(void) {return a[1];}";
    const char *expected[] = {"int", "test_func", "(", "void", ")", "{", "return", "a", "[", "1", "]", ";", "}"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_c_comments(void) {
    const char code[] =
        "/* comment 1 */\n"
        "    /*comment2*/    \n"
        "/* comment \n \r\n \r 3 */\n"
        "";
    const char *expected[] = {"/* comment 1 */", "/*comment2*/", "/* comment \n \r\n \r 3 */"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_cpp_comments(void) {
    const char code[] =
        "// comment 1\n"
        "    //comment2 \n"
        "// comment 3\r\n"
        "// comment 4\r"
        "";
    const char *expected[] = {"// comment 1", "//comment2 ", "// comment 3", "// comment 4"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_c_comments_line_column(void) {
    const char code[] =
        "/* comment 1 \n"
        " */\n"
        "    /*comment2 \n"
        " */\n"
        "/* comment 3 \r\n"
        " */\r\n"
        "/* comment 4 \r"
        " */\r"
        "";

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(3, s.line);
    TEST_ASSERT_EQUAL(5, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(5, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(7, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_cpp_comments_line_column(void) {
    const char code[] =
        "// comment 1\n"
        "    //comment2 \n"
        "// comment 3\r\n"
        "// comment 4\r"
        "";

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(1, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(2, s.line);
    TEST_ASSERT_EQUAL(5, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(3, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_TRUE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL(4, s.line);
    TEST_ASSERT_EQUAL(1, s.column);

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_string(void) {
    const char code[] =
        "\"string 1\"\n"
        "    \"string2\" \n"
        "\"string \\\"3\\\"\"\r\n"
        "\" string '4'\"\r"
        "\" string \\\n5\"\n"
        "";
    const char *expected[] = {"\"string 1\"", "\"string2\"", "\"string \\\"3\\\"\"", "\" string '4'\"", "\" string \\\n5\""};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_string_bug_with_escaped_slash_before_end_of_string(void) {
    const char code[] =
        "\"\\\\\" end";
    const char *expected[] = {"\"\\\\\"", "end"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}
void test_tolken_get_character(void) {
    const char code[] =
        "'1'\n"
        "    '2' \n"
        "'\\''\r\n"
        "'4'\r"
        "";
    const char *expected[] = {"'1'", "'2'", "'\\''", "'4'"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_character_bug_with_escaped_slash_before_end_of_character(void) {
    const char code[] =
        "'\\\\' end"
        "";
    const char *expected[] = {"'\\\\'", "end"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_preprocessor_include(void) {
    const char code[] =
        "#include <stdio.h>\n"
        "#include \"myheader.h\"\n"
        "#    include  \"otherheader.h\" \n"
        "";
    const char *expected[] = {"#include <stdio.h>", "#include \"myheader.h\"", "#    include  \"otherheader.h\" "};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_preprocessor_define(void) {
    const char code[] =
        "#define N 100\n"
        "#    define M 2\n"
        "#define f(x) \\\n"
        "    do { \\\n"
        "        x \\\n"
        "    } while (0)\n"
        "";
    const char *expected[] = {"#define N 100", "#    define M 2", "#define f(x) \\\n    do { \\\n        x \\\n    } while (0)"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_preprocessor_if(void) {
    const char code[] =
        "#if 0\n"
        "#    if 1\n"
        "#    elif 2\n"
        "#    endif\n"
        "#elif 3\n"
        "#endif\n"
        "";
    const char *expected[] = {
        "#if 0",
        "#    if 1",
        "#    elif 2",
        "#    endif",
        "#elif 3",
        "#endif"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}

void test_tolken_get_preprocessor_ifdef(void) {
    const char code[] =
        "#ifdef VAL\n"
        "#    ifndef WAL\n"
        "#    endif\n"
        "#endif\n"
        "";
    const char *expected[] = {
        "#ifdef VAL",
        "#    ifndef WAL",
        "#    endif",
        "#endif"};
    uint32_t expected_len = sizeof(expected) / sizeof(expected[0]);
    uint32_t i;

    tolken_init_str(&t, "test_func.c", code, strlen(code));
    tolken_token_init(&s);

    for (i = 0; i < expected_len; ++i) {
        TEST_ASSERT_TRUE(tolken_next(&t, &s));
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)s.items);
        TEST_ASSERT_EQUAL(strlen(expected[i]), s.count);
    }

    TEST_ASSERT_FALSE(tolken_next(&t, &s));
    TEST_ASSERT_EQUAL_STRING("", (char *)s.items);
    TEST_ASSERT_EQUAL(0, s.count);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
}
