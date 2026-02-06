/*
 * Unity Test Runner Generator - https://github.com/djboni/unity-generator
 * Copyright (c) 2026 Djones A. Boni
 * SPDX-License-Identifier: MIT
 */
#ifndef GENERATOR_H_
#define GENERATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#ifndef bool
#    define bool int
#endif
#ifndef true
#    define true 1
#endif
#ifndef false
#    define false 0
#endif

typedef struct {
    uint8_t *items;
    uint32_t count;
    uint32_t capacity;
} da_u8_t;

typedef struct {
    da_u8_t data;
    const char *filename;
    uint32_t pos;
    uint32_t line;
    uint32_t column;
} tolkenizer_t;

typedef struct {
    uint8_t *items;
    uint32_t count;
    uint32_t capacity;
    const char *filename;
    uint32_t pos;
    uint32_t line;
    uint32_t column;
} token_t;

/**
 * Tokenizer
 */

void tolken_init(tolkenizer_t *t, const char *filename);
void tolken_init_str(tolkenizer_t *t, const char *filename, const void *in, uint32_t len);
void tolken_deinit(tolkenizer_t *t);

void tolken_token_init(token_t *s);
void tolken_token_deinit(token_t *s);

void tolken_restart(tolkenizer_t *t);
bool tolken_next(tolkenizer_t *t, token_t *out);

/**
 * File utilities
 */

bool startswith(const void *haystack, const void *needle);

const char *basename(const char *path);
const char *extension(const char *path);

bool read_entire_file(da_u8_t *array, const char *file);
bool write_entire_file(const char *file, const void *ptr, uint32_t size);
bool write_entire_file_if_necessary(const char *file, const void *ptr, uint32_t size);

FILE *fp_open(const char *file, const char *mode);
bool fp_close(FILE *fp);
int32_t fp_size(FILE *fp);
bool fp_read(da_u8_t *array, uint32_t limit, FILE *fp);
void fp_read_all(da_u8_t *array, FILE *fp);
bool fp_write(FILE *fp, const void *data, uint32_t len);

#define da_init(array) \
    do { \
        (array)->items = NULL; \
        (array)->count = 0; \
        (array)->capacity = 0; \
    } while (0)

#define da_free(array) \
    do { \
        free((array)->items); \
        da_init(array); \
    } while (0)

#define da_alloc_capacity(array, new_capacity) \
    do { \
        uint32_t a_new_capacity = (new_capacity); \
        if ((array)->capacity < a_new_capacity) { \
            *(void **)&(array)->items = realloc((array)->items, a_new_capacity * sizeof((array)->items[0])); \
            assert((array)->items); \
            (array)->capacity = a_new_capacity; \
        } else { \
            /* Should we shrink in this case? */ \
        } \
    } while (0)

#define da_alloc_double_capacity(array) \
    do { \
        uint32_t new_capacity = 2 * (array)->capacity; \
        if (new_capacity == 0) \
            new_capacity = 4; \
        da_alloc_capacity((array), new_capacity); \
    } while (0)

#define da_append(array, new_item) \
    do { \
        if ((array)->capacity < (array)->count + 1) { \
            da_alloc_double_capacity(array); \
        } \
        (array)->items[(array)->count] = (new_item); \
        (array)->count += 1; \
    } while (0)

#define da_append_many(array, ptr, a__count) \
    do { \
        uint32_t a_count = (a__count); \
        da_alloc_capacity(array, (array)->count + a_count); \
        memcpy(&(array)->items[(array)->count], (ptr), a_count * sizeof((array)->items[0])); \
        (array)->count += a_count; \
    } while (0)

bool da_appendf(da_u8_t *str, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* GENERATOR_H_ */

#ifndef GENERATOR_NO_IMPLEMENTATION

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

extern int vsnprintf(char *str, size_t size, const char *format, va_list ap);

bool startswith(const void *haystack, const void *needle) {
    return strstr(haystack, needle) == haystack;
}

const char *basename(const char *path) {
    uint32_t pos = 0;
    uint32_t i;
    for (i = 0; path[i]; ++i) {
        if (path[i] == '/') {
            pos = i + 1;
        } else if (path[i] == '\\') {
            /* Windows */
            pos = i + 1;
        }
    }
    return &path[pos];
}

const char *extension(const char *path) {
    uint32_t pos = 0;
    uint32_t i;
    for (i = 0; path[i]; ++i) {
        if (path[i] == '.') {
            pos = i + 1;
        } else if (path[i] == '\\') {
            /* Windows */
            pos = i + 1;
        }
    }
    return &path[pos];
}

bool read_entire_file(da_u8_t *array, const char *file) {
    int32_t size = -1;
    FILE *fp = fp_open(file, "rb");
    if (fp == NULL) {
        goto err_defer;
    }

    size = fp_size(fp);
    if (size < 0) {
        goto err_defer;
    }

    if (!fp_read(array, size, fp)) {
        goto err_defer;
    }

    if (!fp_close(fp)) {
        fp = NULL;
        goto err_defer;
    }
    return true;
err_defer:
    if (fp != NULL) {
        fp_close(fp);
    }
    return false;
}

bool write_entire_file(const char *file, const void *ptr, uint32_t size) {
    FILE *fp = fp_open(file, "wb");
    if (fp == NULL) {
        goto err_defer;
    }

    if (!fp_write(fp, ptr, size)) {
        goto err_defer;
    }

    if (!fp_close(fp)) {
        fp = NULL;
        goto err_defer;
    }
    return true;
err_defer:
    if (fp != NULL) {
        fp_close(fp);
    }
    return false;
}

bool write_entire_file_if_necessary(const char *file, const void *ptr, uint32_t size) {
    int32_t current_size = -1;
    da_u8_t current_content = {0};

    /* Check if the sizes are different */
    FILE *fp = fp_open(file, "rb");
    if (fp == NULL) {
        goto write_file;
    }
    current_size = fp_size(fp);
    if (current_size < 0 || size != (uint32_t)current_size) {
        /* Could not read the size or the size differs */
        goto write_file;
    }

    /* Check if the contents are different */
    fp_read_all(&current_content, fp);
    if (size != current_content.count) {
        goto write_file;
    }
    if (memcmp(ptr, current_content.items, size)) {
        /* The content differs */
        goto write_file;
    }

    da_free(&current_content);
    fp_close(fp);
    return true;

write_file:
    da_free(&current_content);
    if (fp != NULL) {
        fp_close(fp);
    }
    return write_entire_file(file, ptr, size);
}

FILE *fp_open(const char *file, const char *mode) {
    FILE *fp = fopen(file, mode);
    if (fp == NULL) {
    }
    return fp;
}

bool fp_close(FILE *fp) {
    if (fclose(fp) == 0) {
        return true;
    }
    return false;
}

int32_t fp_size(FILE *fp) {
    int32_t size = -1;
    int32_t starting_position = ftell(fp);
    if (starting_position < 0) {
        goto err_defer;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        goto err_defer;
    }

    size = ftell(fp);
    if (size < 0) {
        goto err_defer;
    }

    if (fseek(fp, starting_position, SEEK_SET) != 0) {
        goto err_defer;
    }

    return size;

err_defer:
    return -1;
}

void fp_read_all(da_u8_t *array, FILE *fp) {
    int ch;
    while ((ch = fgetc(fp)) >= 0)
        da_append(array, ch);

    /* Add an uncounted null terminator. */
    da_append(array, '\0');
    array->count -= 1;
}

bool fp_read(da_u8_t *array, uint32_t limit, FILE *fp) {
    uint32_t num_read = -1;
    if (limit <= 0)
        return true;

    da_alloc_capacity(array, array->count + limit + 1);

    num_read = fread(&array->items[array->count], 1, limit, fp);
    assert(num_read <= limit);
    array->count += num_read;

    /* Add an uncounted null terminator. */
    da_append(array, '\0');
    array->count -= 1;
    return num_read == limit;
}

bool fp_write(FILE *fp, const void *data, uint32_t len) {
    uint32_t num_write = fwrite(data, 1, len, fp);
    assert(num_write <= len);
    if (num_write < len) {
        return false;
    }
    return true;
}

int da_appendf(da_u8_t *str, const char *format, ...) {
    if (str->capacity == 0)
        da_alloc_capacity(str, 4096);
    if (str->count == 0 && str->items[0] != '\0')
        str->items[0] = '\0';
    do {
        const uint32_t remaining_capacity = str->capacity - str->count;
        va_list va;
        uint32_t necessary;
        va_start(va, format);
        necessary = vsnprintf((char *)&str->items[str->count], remaining_capacity, format, va);
        va_end(va);
        if (necessary < remaining_capacity) {
            /* The current capacity is enough */
            str->count += necessary;
            return necessary;
        } else {
            /* Not enough capacity */
            uint32_t new_capacity = str->count + necessary + 1;
            if (new_capacity <= 2 * str->capacity) {
                da_alloc_double_capacity(str);
            } else {
                da_alloc_capacity(str, new_capacity);
            }
            continue;
        }
    } while (1);
    assert(0 && "Unreachable");
    return 0;
}

static void *memdup(const void *in, size_t len) {
    void *dest = malloc(len);
    if (dest == NULL) {
        return NULL;
    }
    return memcpy(dest, in, len);
}

static void tolken_init_(tolkenizer_t *t, const char *filename, da_u8_t *data) {
    memset(t, 0, sizeof(*t));
    t->filename = filename;
    t->data = *data;
}

void tolken_init(tolkenizer_t *t, const char *filename) {
    da_u8_t data = {0};
    read_entire_file(&data, filename);
    tolken_init_(t, filename, &data);
}

void tolken_init_str(tolkenizer_t *t, const char *filename, const void *vin, uint32_t len) {
    const char *in = (const char *)vin;
    da_u8_t data = {0};
    data.items = memdup(in, len);
    assert(data.items != NULL);
    data.count = len;
    data.capacity = len;
    tolken_init_(t, filename, &data);
}

void tolken_deinit(tolkenizer_t *t) {
    da_free(&t->data);
    memset(t, 0, sizeof(*t));
}

void tolken_token_init(token_t *s) {
    memset(s, 0, sizeof(*s));
}

void tolken_token_deinit(token_t *s) {
    da_free(s);
    memset(s, 0, sizeof(*s));
}

static void tolken_skip_whitespace(tolkenizer_t *t) {
    while (t->pos < t->data.count) {
        uint8_t ch = t->data.items[t->pos];
        if (!isspace(ch)) {
            break;
        }
        t->pos += 1;
        if (ch == '\n') {
            /* LF */
            t->column = 0;
            t->line += 1;
        } else if (ch == '\r') {
            /* CR or CRLF */
            if (t->pos < t->data.count) {
                uint8_t next_ch = t->data.items[t->pos];
                if (next_ch == '\n') {
                    /* CRLF */
                    t->pos += 1;
                }
            }
            t->column = 0;
            t->line += 1;
        } else {
            t->column += 1;
        }
    }
}

static bool tolken_advance_to_str(tolkenizer_t *t, const char *str, uint32_t str_len, const char *ign, uint32_t ign_len) {
    while (t->pos < t->data.count) {
        uint8_t ch = t->data.items[t->pos];
        if (ign_len != 0 && ch == ign[0] && t->pos + ign_len <= t->data.count && !memcmp(&t->data.items[t->pos], ign, ign_len)) {
            t->pos += ign_len;
            continue;
        }
        if (ch == str[0] && t->pos + str_len <= t->data.count && !memcmp(&t->data.items[t->pos], str, str_len)) {
            t->pos += str_len;
            return true;
        }
        t->pos += 1;
        if (ch == '\n') {
            /* LF */
            t->column = 0;
            t->line += 1;
        } else if (ch == '\r') {
            /* CR or CRLF */
            if (t->pos < t->data.count) {
                uint8_t next_ch = t->data.items[t->pos];
                if (next_ch == '\n') {
                    /* CRLF */
                    t->pos += 1;
                }
            }
            t->column = 0;
            t->line += 1;
        } else {
            t->column += 1;
        }
    }
    return false;
}

static bool tolken_advance_to_eol(tolkenizer_t *t) {
    while (t->pos < t->data.count) {
        uint8_t ch = t->data.items[t->pos];
        if (ch == '\n') {
            /* LF */
            break;
        } else if (ch == '\r') {
            /* CR or CRLF: it does not matter which one */
            break;
        } else {
            t->pos += 1;
            t->column += 1;
        }
    }
    return true;
}

void tolken_restart(tolkenizer_t *t) {
    t->pos = 0;
    t->line = 0;
    t->column = 0;
}

bool tolken_next(tolkenizer_t *t, token_t *out) {
    bool ret = 0;
    uint32_t count = 0;
    tolken_skip_whitespace(t);

    /* Set the start of the token */
    out->filename = t->filename;
    out->pos = t->pos;
    out->line = t->line + 1;
    out->column = t->column + 1;

    /* Get token */
    while (t->pos < t->data.count) {
        uint8_t ch = t->data.items[t->pos];

        /* C comment */
        if (ch == '/' && t->pos + 2 <= t->data.count && !memcmp(&t->data.items[t->pos], "/*", 2)) {
            t->pos += 2;
            ret = tolken_advance_to_str(t, "*/", 2, "", 0);
            goto end;
        }

        /* C++ comment */
        if (ch == '/' && t->pos + 2 <= t->data.count && !memcmp(&t->data.items[t->pos], "//", 2)) {
            t->pos += 2;
            ret = tolken_advance_to_eol(t);
            goto end;
        }

        /* String */
        if (ch == '"' && t->pos + 1 <= t->data.count && !memcmp(&t->data.items[t->pos], "\"", 1)) {
            t->pos += 1;
            ret = tolken_advance_to_str(t, "\"", 1, "\\\"", 2);
            goto end;
        }

        /* Character */
        if (ch == '\'' && t->pos + 1 <= t->data.count && !memcmp(&t->data.items[t->pos], "'", 1)) {
            t->pos += 1;
            ret = tolken_advance_to_str(t, "'", 1, "\\'", 2);
            goto end;
        }

        /* Pre-processor */
        if (ch == '#') {
            t->pos += 1;
            while (1) {
                ret = tolken_advance_to_eol(t);
                if (!ret)
                    break;
                ch = t->data.items[t->pos - 1];
                if (ch == '\\') {
                    /* Escaped new line */
                    tolken_skip_whitespace(t);
                    continue;
                }
                break;
            }
            goto end;
        }

        /* Generic token */
        if (isspace(ch)) {
            /* Token ended: whitespace */
            break;
        } else if (ispunct(ch) && ch != '_') {
            /* Token ended: single character token */
            if (count == 0) {
                /* No token yet: this is the token */
                count += 1;
                t->pos += 1;
                t->column += 1;
            }
            break;
        } else {
            /* Continue the token */
            count += 1;
            t->pos += 1;
            t->column += 1;
        }
    }

    ret = count > 0;
    assert(count == t->pos - out->pos);
end:

    /* Copy token */
    count = t->pos - out->pos;
    out->count = 0;
    da_append_many(out, &t->data.items[out->pos], count);

    /* Add a handy uncounted null terminator */
    da_append(out, '\0');
    out->count -= 1;

    return ret;
}

#endif /* GENERATOR_NO_IMPLEMENTATION */

#ifndef GENERATOR_NO_MAIN

#include <string.h>

#define SHIFT_ARGS(c, v) (assert((c) > 0), (c)--, (v)++[0])

typedef struct {
    const char **items;
    uint32_t count;
    uint32_t capacity;
} strings_not_owned_t;

typedef struct {
    char **items; /* Owns the pointers. */
    uint32_t count;
    uint32_t capacity;
} strings_owned_t;

typedef struct {
    char *name; /* Owns the pointers. */
    uint32_t line;
    bool end_of_header_detector;
} test_functions_t;

typedef struct {
    test_functions_t *items; /* Owned pointers. */
    uint32_t count;
    uint32_t capacity;
} test_functions_list_t;

static const char *program = NULL;

static void generate_test_runner(const char *testfile, const char *module, const char *runnerfile) {
    uint32_t i;
    uint32_t start_of_functions = 0;
    da_u8_t data = {0}, *d = &data;
    tolkenizer_t t = {0};
    token_t s = {0};

    char *suitesetup = NULL;
    char *suiteteardown = NULL;
    char *setup = NULL;
    char *teardown = NULL;
    test_functions_list_t test_functions = {0};

    int func_state = 0;
    int preproc_if_depth = 0;
    int last_preproc_if_pos = -1;
    da_u8_t func_name = {0};
    int func_line = 0;

    tolken_init(&t, testfile);
    tolken_token_init(&s);

    /* Process the whole file to find the suite setup, suite teardown, setup,
     * teardown and test functions.
     */
    func_state = 0;
    preproc_if_depth = 0;
    last_preproc_if_pos = -1;
    while (tolken_next(&t, &s)) {
        if (startswith(s.items, "#")) {
            tolkenizer_t preprocessor = {0};

            /* Add the preprocessor to the list */
            test_functions_t func = {0};
            func.name = memdup(s.items, s.count + 1);
            assert(func.name != NULL);
            func.line = s.line;
            da_append(&test_functions, func);

            /* Count the preprocessor #if depth */
            tolken_init_str(&preprocessor, "preprocessor", &s.items[1], s.count - 1);
            if (tolken_next(&preprocessor, &s)) {
                if (!strcmp((const char *)s.items, "if") ||
                    !strcmp((const char *)s.items, "ifdef") ||
                    !strcmp((const char *)s.items, "ifndef")) {
                    if (preproc_if_depth == 0) {
                        /* Position of the depth 0 #if. If a test function is
                         * defined inside this #if we set the flag
                         * end_of_header_detector to close the header section
                         * and start the test runner, calling the test
                         * functions.
                         */
                        last_preproc_if_pos = test_functions.count - 1;
                    }
                    preproc_if_depth += 1;
                } else if (!strcmp((const char *)s.items, "endif")) {
                    preproc_if_depth -= 1;
                    if (preproc_if_depth == 0) {
                        /* End of the depth 0 #if */
                        last_preproc_if_pos = -1;
                    }
                } else {
                    /* Nothing to do */
                }
                tolken_deinit(&preprocessor);
            }

            continue;
        }

        /* Find test, spec, setUp, tearDown, suiteSetUp functions (`void function_name(void)`)
         * and suiteTearDown functions (`int function_name(int num_errors)`).
         */
        switch (func_state) {
        case 0:
            if (!strcmp("void", (const char *)s.items)) {
                /* void */
                func_line = s.line;
                func_state = 1;
            } else if (!strcmp("int", (const char *)s.items)) {
                /* int */
                func_line = s.line;
                func_state = 101;
            } else {
                func_state = 0;
            }
            break;
        case 1:
            /* void function_name */
            func_name.count = 0;
            da_appendf(&func_name, "%s", (const char *)s.items);
            func_state += 1;
            break;
        case 2:
            if (!strcmp("(", (const char *)s.items)) {
                /* void function_name( */
                func_state += 1;
            } else {
                func_state = 0;
            }
            break;
        case 3:
            if (!strcmp("void", (const char *)s.items)) {
                /* void function_name(void */
                func_state += 1;
            } else if (!strcmp(")", (const char *)s.items)) {
                /* void function_name() */
                func_state += 2; /* Skip to state 5 */
            } else {
                func_state = 0;
            }
            break;
        case 4:
            if (!strcmp(")", (const char *)s.items)) {
                /* void function_name(void) */
                func_state += 1;
            } else {
                func_state = 0;
            }
            break;
        case 5:
            if (!strcmp("{", (const char *)s.items)) {
                /* void function_name(void) { */
                /* void function_name() { */
                func_state = 0;
                goto found_void_func_void;
            } else {
                func_state = 0;
            }
            break;
        case 101:
            /* int function_name */
            func_name.count = 0;
            da_appendf(&func_name, "%s", (const char *)s.items);
            func_state += 1;
            break;
        case 102:
            if (!strcmp("(", (const char *)s.items)) {
                /* int function_name( */
                func_state += 1;
            } else {
                func_state = 0;
            }
            break;
        case 103:
            if (!strcmp("int", (const char *)s.items)) {
                /* int function_name(int */
                func_state += 1;
            } else if (!strcmp(")", (const char *)s.items)) {
                /* int function_name() */
                func_state += 3; /* Skip to state 106 */
            } else {
                func_state = 0;
            }
            break;
        case 104:
            if (!strcmp(")", (const char *)s.items)) {
                /* int function_name(int) */
                func_state += 2; /* Skip to state 106 */
            } else {
                /* int function_name(int num_errors */
                bool valid = true;
                for (i = 0; i < s.count; ++i) {
                    uint8_t ch = s.items[i];
                    if (!isalnum(ch) && ch != '_') {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    func_state += 1;
                } else {
                    func_state = 0;
                }
            }
            break;
        case 105:
            if (!strcmp(")", (const char *)s.items)) {
                /* int function_name(int num_errors) */
                func_state += 1;
            } else {
                func_state = 0;
            }
            break;
        case 106:
            if (!strcmp("{", (const char *)s.items)) {
                /* int function_name(int num_errors) { */
                /* int function_name(int) { */
                /* int function_name() { */
                func_state = 0;
                goto found_int_func_int;
            } else {
                func_state = 0;
            }
            break;
        default:
            func_state = 0;
        }
        continue;

    found_void_func_void:
        if (startswith(func_name.items, "test") ||
            startswith(func_name.items, "spec")) {
            /* Add the function to the list */
            test_functions_t func = {0};
            func.name = memdup(func_name.items, func_name.count + 1);
            assert(func.name != NULL);
            func.line = func_line;
            da_append(&test_functions, func);

            /* Found a test function. Use the position of the depth 0 #if
             * to set the flag end_of_header_detector to close the header
             * section and start the test runner, calling the test functions.
             */
            if (last_preproc_if_pos >= 0) {
                test_functions.items[last_preproc_if_pos].end_of_header_detector = true;
            }
        } else if (startswith(func_name.items, "setUp")) {
            if (setup != NULL) {
                free(setup);
                fprintf(stderr, "%s: warning: more than one setUp*() functions in '%s'\n", program, testfile);
            }
            setup = memdup(func_name.items, func_name.count + 1);
            assert(setup != NULL);
        } else if (startswith(func_name.items, "tearDown")) {
            if (teardown != NULL) {
                free(teardown);
                fprintf(stderr, "%s: warning: more than one tearDown*() functions in '%s'\n", program, testfile);
            }
            teardown = memdup(func_name.items, func_name.count + 1);
            assert(teardown != NULL);
        } else if (startswith(func_name.items, "suiteSetUp")) {
            if (suitesetup != NULL) {
                free(suitesetup);
                fprintf(stderr, "%s: warning: more than one suiteSetUp*() functions in '%s'\n", program, testfile);
            }
            suitesetup = memdup(func_name.items, func_name.count + 1);
            assert(suitesetup != NULL);
        } else {
            /* Ignore function */
        }
        continue;

    found_int_func_int:
        if (startswith(func_name.items, "suiteTearDown")) {
            if (suiteteardown != NULL) {
                free(suiteteardown);
                fprintf(stderr, "%s: warning: more than one suiteTearDown*() functions in '%s'\n", program, testfile);
            }
            suiteteardown = memdup(func_name.items, func_name.count + 1);
            assert(suiteteardown != NULL);
        } else {
            /* Ignore function */
        }
        continue;
    }

    da_appendf(d,
        "/* DO NOT EDIT: File automatically generated. */\n"
        "#include \"unity.h\"\n"
        "\n"
        "extern void UnityCustomTestRun(void (*Func)(void), void (*SetUp)(void), void (*TearDown)(void), const char *FuncName, const int FuncLineNum);\n"
        "\n"
        "");

    da_appendf(d,
        "/* Headers and definitions from the test file. */\n"
        "");
    for (i = 0; i < test_functions.count; ++i) {
        test_functions_t func = test_functions.items[i];
        if (func.end_of_header_detector) {
            break;
        }
        if (startswith(func.name, "#")) {
            da_appendf(d, "%s\n", func.name);
            continue;
        }
        break;
    }
    start_of_functions = i;

    da_appendf(d,
        "\n"
        "/* Set up, tear down, and test functions from the test file. */\n"
        "");

    if (suitesetup != NULL) {
        da_appendf(d, "extern void %s(void);\n", suitesetup);
    }
    if (suiteteardown != NULL) {
        da_appendf(d, "extern int %s(int num_errors);\n", suiteteardown);
    }

    if (setup != NULL) {
        da_appendf(d, "extern void %s(void);\n", setup);
    }
    if (teardown != NULL) {
        da_appendf(d, "extern void %s(void);\n", teardown);
    }

    for (i = start_of_functions; i < test_functions.count; ++i) {
        test_functions_t func = test_functions.items[i];
        if (startswith(func.name, "#")) {
            continue;
        }
        da_appendf(d, "extern void %s(void);\n", func.name);
    }
    if (test_functions.count == 0) {
        fprintf(stderr, "%s: warning: no test functions in '%s'\n", program, testfile);
    }

    da_appendf(d,
        "\n"
        "void runner_test_%s(void) {\n"
        "",
        module);

    if (suiteteardown != NULL) {
        da_appendf(d,
            "    UNITY_COUNTER_TYPE failures_at_start = Unity.TestFailures;\n"
            "",
            suiteteardown);
    }

    da_appendf(d,
        "    UnitySetTestFile(\"%s\");\n"
        "\n"
        "",
        testfile);

    if (suitesetup != NULL) {
        da_appendf(d,
            "    %s();\n"
            "\n"
            "",
            suitesetup);
    }

    for (i = start_of_functions; i < test_functions.count; ++i) {
        test_functions_t func = test_functions.items[i];
        if (startswith(func.name, "#")) {
            da_appendf(d, "%s\n", func.name);
            continue;
        }
        da_appendf(d,
            "    UnityCustomTestRun(%s, %s, %s, \"%s\", %d);\n"
            "",
            func.name, (setup ? setup : "NULL"), (teardown ? teardown : "NULL"), func.name, func.line);
    }

    if (suiteteardown != NULL) {
        da_appendf(d,
            "\n"
            "    (void)%s(Unity.TestFailures - failures_at_start);\n"
            "",
            suiteteardown);
    }

    da_appendf(d,
        "}\n"
        "");

    write_entire_file_if_necessary(runnerfile, d->items, d->count);

    if (suitesetup != NULL) {
        free(suitesetup);
    }
    if (suiteteardown != NULL) {
        free(suiteteardown);
    }
    if (setup != NULL) {
        free(setup);
    }
    if (teardown != NULL) {
        free(teardown);
    }
    for (i = 0; i < test_functions.count; ++i) {
        test_functions_t func = test_functions.items[i];
        free(func.name);
    }
    da_free(&test_functions);
    da_free(&func_name);

    tolken_token_deinit(&s);
    tolken_deinit(&t);
    da_free(d);
}

static void generate_global_runner(strings_owned_t *modules, const char *runnerfile) {
    uint32_t i;
    da_u8_t data = {0}, *d = &data;

    da_appendf(d,
        "/* DO NOT EDIT: File automatically generated. */\n"
        "#include \"unity.h\"\n"
        "\n"
        "");

    da_appendf(d,
        "void UnityCustomTestRun(void (*Func)(void), void (*SetUp)(void), void (*TearDown)(void), const char *FuncName, const int FuncLineNum) {\n"
        "");
    da_appendf(d,
        "    Unity.CurrentTestName = FuncName;\n"
        "    Unity.CurrentTestLineNumber = (UNITY_LINE_TYPE)FuncLineNum;\n"
        "#ifdef UNITY_USE_COMMAND_LINE_ARGS\n"
        "    if (!UnityTestMatches()) {\n"
        "        return;\n"
        "    }\n"
        "#endif\n"
        "    Unity.NumberOfTests++;\n"
        "#ifndef UNITY_EXCLUDE_DETAILS\n"
        "#    ifdef UNITY_DETAIL_STACK_SIZE\n"
        "    Unity.CurrentDetailStackSize = 0;\n"
        "#    else\n"
        "    UNITY_CLR_DETAILS();\n"
        "#    endif\n"
        "#endif\n"
        "");
    da_appendf(d,
        "    UNITY_EXEC_TIME_START();\n"
        "    if (TEST_PROTECT()) {\n"
        "        if (SetUp != NULL)\n"
        "            SetUp();\n"
        "        Func();\n"
        "    }\n"
        "    if (TEST_PROTECT()) {\n"
        "        if (TearDown != NULL)\n"
        "            TearDown();\n"
        "    }\n"
        "    UNITY_EXEC_TIME_STOP();\n"
        "    UnityConcludeTest();\n"
        "}\n"
        "");

    da_appendf(d,
        "\n"
        "void setUp(void) {\n"
        "}\n"
        "\n"
        "void tearDown(void) {\n"
        "}\n"
        "\n"
        "");

    for (i = 0; i < modules->count; ++i) {
        da_appendf(d,
            "extern void runner_test_%s(void);\n"
            "",
            modules->items[i]);
    }

    da_appendf(d,
        "\n"
        "int main(int argc, char **argv) {\n"
        "#ifdef UNITY_USE_COMMAND_LINE_ARGS\n"
        "    int ret = UnityParseOptions(argc, argv);\n"
        "    if (ret > 0) {\n"
        "        return ret;\n"
        "    }\n"
        "#else\n"
        "    (void)argc;\n"
        "    (void)argv;\n"
        "#endif\n"
        "\n"
        "    UnityBegin(NULL);\n"
        "\n"
        "");

    for (i = 0; i < modules->count; ++i) {
        da_appendf(d,
            "    runner_test_%s();\n"
            "",
            modules->items[i]);
    }

    da_appendf(d,
        "\n"
        "    return UnityEnd();\n"
        "}\n"
        "");

    write_entire_file_if_necessary(runnerfile, d->items, d->count);

    da_free(d);
}

static const char *global_runner_path = NULL;
static strings_not_owned_t test_files_paths = {0};
static strings_owned_t test_runners_paths = {0};
static strings_owned_t test_module_names = {0};

int main(int argc, char **argv) {
    uint32_t i;
    program = SHIFT_ARGS(argc, argv);
    if (argc < 1) {
        fprintf(stderr, "%s: error: no output file\n", program);
        return 1;
    }
    for (i = 0; (int)i < argc; ++i) {
        if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i])) {
            fprintf(stderr, "Usage: %s [-h|--help] GLOBAL_RUNNER TEST_FILES...\n", program);
            return 0;
        }
    }

    global_runner_path = SHIFT_ARGS(argc, argv);
    da_alloc_capacity(&test_files_paths, argc);
    for (i = 0; argc > 0; ++i) {
        const char *path = SHIFT_ARGS(argc, argv);
        da_append(&test_files_paths, path);
    }

    if (test_files_paths.count == 0) {
        fprintf(stderr, "%s: error: no test files\n", program);
        return 1;
    }

    fprintf(stderr, "Listing files:\n");
    fprintf(stderr, "- Global Runner: %s\n", global_runner_path);
    for (i = 0; i < test_files_paths.count; ++i) {
        const char *path = test_files_paths.items[i];
        fprintf(stderr, "- Test File %d: %s\n", i + 1, path);
    }

    fprintf(stderr, "Processing:\n");
    for (i = 0; i < test_files_paths.count; ++i) {
        const char *begin = "test_";
        const char *path = test_files_paths.items[i];
        uint32_t path_len = strlen(path);
        const char *base = basename(path);
        uint32_t base_len = strlen(base);
        const char *ext = extension(base);
        uint32_t ext_len = strlen(ext);
        char *module = NULL;
        uint32_t module_len = base_len - (5 + 1 + ext_len); /* 5="test_" - 1="." */
        char *runner = NULL;
        uint32_t runner_len = path_len + 7; /* 7="runner_" */

        fprintf(stderr, "- Processing Test File %d:\n", i + 1);
        fprintf(stderr, "    - Test File Path:   %s\n", path);

        if (!startswith(base, begin)) {
            fprintf(stderr, "%s: error: test file name is expected to start with '%s': '%s'\n", program, begin, global_runner_path);
            return 1;
        }
        if (strcmp(ext, "c") && strcmp(ext, "C") && strcmp(ext, "cpp") && strcmp(ext, "CPP")) {
            fprintf(stderr, "%s: error: test file name extension is expected to be '.c|.C|.cpp|.CPP': '%s'\n", program, path);
            return 1;
        }
        if ((int32_t)module_len <= 0) {
            fprintf(stderr, "%s: error: test file name does not provide a module name 'test_<module>.<ext>: '%s'\n", program, path);
            return 1;
        }

        runner = malloc(runner_len + 1);
        assert(runner != NULL);
        memcpy(runner, &path[0], path_len - base_len);
        runner[path_len - base_len] = '\0';
        strcat(runner, "runner_");
        strcat(runner, &path[path_len - base_len]);
        assert(runner_len == strlen(runner));
        da_append(&test_runners_paths, runner);
        fprintf(stderr, "    - Runner File Path: %s\n", runner);

        module = malloc(module_len + 1);
        assert(module != NULL);
        memcpy(module, &base[5], module_len);
        module[module_len] = '\0';
        assert(module_len == strlen(module));
        da_append(&test_module_names, module);

        fprintf(stderr, "    - Module Name:      %s\n", module);

        generate_test_runner(path, module, runner);
    }

    fprintf(stderr, "- Processing Global Runner:\n");
    fprintf(stderr, "    - Global Runner Path: %s\n", global_runner_path);

    {
        const char *begin = "runner_";
        const char *base = basename(global_runner_path);
        const char *ext = extension(global_runner_path);
        if (!startswith(base, begin)) {
            fprintf(stderr, "%s: error: test file name is expected to start with '%s': '%s'\n", program, begin, global_runner_path);
            return 1;
        }
        if (strcmp(ext, "c") && strcmp(ext, "C") && strcmp(ext, "cpp") && strcmp(ext, "CPP")) {
            fprintf(stderr, "%s: error: test file name extension is expected to be '.c|.C|.cpp|.CPP': '%s'\n", program, global_runner_path);
            return 1;
        }
    }

    generate_global_runner(&test_module_names, global_runner_path);

    /* NOTE: These deallocations are not actually necessary since the program will exit. */
    for (i = 0; i < test_module_names.count; ++i) {
        free(test_module_names.items[i]);
    }
    da_free(&test_module_names);
    for (i = 0; i < test_runners_paths.count; ++i) {
        free(test_runners_paths.items[i]);
    }
    da_free(&test_runners_paths);
    da_free(&test_files_paths);

    return 0;
}

#endif /* GENERATOR_NO_MAIN */
