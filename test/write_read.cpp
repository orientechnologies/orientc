#include "../src/orientc.h"
#include <iostream>
#include <check.h>
#include <assert.h>
#include <stdlib.h>
#include "test_reader_listener.h"
using namespace Orient;

START_TEST(test_simple_write_read)
	try {
		RecordWriter writer("ORecordSerializerBinary");
		writer.className("Test");

		writer.startField("test", STRING);
		writer.stringValue("test");

		writer.endField("test", STRING);
		writer.startField("number", INTEGER);
		writer.intValue(10);

		writer.endField("number", INTEGER);

		int size;

		const char * content = writer.writtenContent(&size);
		RecordParser reader("ORecordSerializerBinary");

		std::cout.flush();
		TrackerListener listener;
		reader.parse((char *) content, size, listener);

		assert(listener.field_count == 2);
		assert(listener.class_name != 0);
		assert(std::string(listener.class_name) == std::string("Test"));
		assert(std::string(listener.a_string_value) == std::string("test"));
		assert(listener.integer_value == 10);
		assert(listener.balanced_count == 0);
	} catch (const char * oh) {
		std::cout << "oh" << oh;
		std::cout.flush();

	}

END_TEST

Suite * file_suite(void) {
	Suite *s = suite_create("file");
	TCase *tc_core = tcase_create("simmetric_writer_reader");
	tcase_add_test(tc_core, test_simple_write_read);
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
