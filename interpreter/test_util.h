/* Copyright 2020 Charles Daniels */

/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef TEST_UTIL
#define TEST_UTIL

#include<math.h>

#define fail(fmt, ...) do { \
		fprintf(stderr, "TEST FAILED (%s, %s:L%i): ", __func__, __FILE__, __LINE__); \
		fprintf(stderr, fmt, __VA_ARGS__); \
		fprintf(stderr, "\n"); \
		exit(1); \
	} while(0);

#define str_should_equal(str1, str2) do { \
		if (strcmp((str1), (str2)) != 0) { fail("'%s' ('%s') should equal '%s' ('%s')", \
			str1, #str1, str2, #str2); } \
	} while(0)

#define str_should_not_equal(str1, str2) do { \
		if (strcmp((str1), (str2)) == 0) { fail("'%s' ('%s') should no equal '%s' ('%s')", \
			str1, #str1, str2, #str2); } \
	} while(0)

#define should_equal(v1, v2) do { \
		if ( (v1) != (v2) ) { fail("'%s' should equal '%s'", #v1, #v2); } \
	} while(0)

#define should_equal_epsilon(v1, v2, epsilon) do { \
		if ( fabs(v1 - v2) >= epsilon ) { fail("'%s' should equal '%s' within bound %f", #v1, #v2, epsilon); } \
	} while(0)

#define should_not_equal(v1, v2) do { \
		if ( (v1) == (v2) ) { fail("'%s' should not equal '%s'", #v1, #v2); } \
	} while(0)

#define should_be_true(expr) do { \
		if (!(expr)) { fail("'%s' should have been true and was not", #expr); } \
	} while(0)

#define should_be_false(expr) do { \
		if (!(expr)) { fail("'%s' should have been false and was not", #expr); } \
	} while(0)

#define should_be_null(expr) do { \
		if ((expr) != NULL) { fail("'%s' should have been NULL and was not", #expr); } \
	} while(0)

#define should_not_be_null(expr) do { \
		if ((expr) == NULL) { fail("'%s' should not have been NULL and was not", #expr); } \
	} while(0)

#endif /* TEST_UTIL */
