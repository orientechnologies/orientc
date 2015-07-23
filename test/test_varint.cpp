#include "../src/helpers.h"
#include <check.h>
#include <stdlib.h>
#include <assert.h>
#include <climits>
#include <iostream>

using namespace Orient;

long long ser_deser_varint(long long number) {
	char content[1024];
	ContentBuffer reader(content, 1024);
	writeVarint(reader, number);
	reader.prepared = 0;
	reader.cursor = 0;
	return readVarint(reader);
}

START_TEST(test_write_read_varint)
	assert(12 == ser_deser_varint(12));
	assert(234 == ser_deser_varint(234));
	assert(43234 == ser_deser_varint(43234));
	assert(46576443234 == ser_deser_varint(46576443234));
	assert(534 == ser_deser_varint(534));
	assert(-534 == ser_deser_varint(-534));
	assert(LLONG_MAX-10 == ser_deser_varint(LLONG_MAX-10));
	assert(LLONG_MAX == ser_deser_varint(LLONG_MAX));
	assert(LLONG_MIN == ser_deser_varint(LLONG_MIN));

END_TEST

Suite * file_suite(void) {
	Suite *s = suite_create("file");
	TCase *tc_core = tcase_create("varint");
	tcase_add_test(tc_core, test_write_read_varint);
	suite_add_tcase(s, tc_core);
	return s;
}

int main() {
	int number_failed;
	Suite *s = file_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

