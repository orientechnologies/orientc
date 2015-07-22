#include "../src/orientc.h"
#include <fstream>
#include <check.h>
#include <stdlib.h>
#include <assert.h>

using namespace Orient;

class TrackerListener: public RecordParseListener {
public:
	virtual void className(char * name) {
		this->class_name = strdup(name);
	}
	virtual void startField(char * name, OType type) {
		this->field_count++;
		std::cout << "field:" << name << " otype : " << type << std::endl;
	}
	virtual void endField(char * name) {
	}
	virtual void stringValue(char * value) {
	}
	virtual void intValue(long value) {
	}
	virtual void longValue(long long value) {
	}
	virtual void shortValue(short value) {
	}
	virtual void byteValue(char value) {
	}
	virtual void booleanValue(char value) {
	}
	virtual void floatValue(float value) {
	}
	virtual void doubleValue(long double value) {
	}
	virtual void binaryValue(char * value, int length) {
	}
	virtual void dateValue(long long value) {
	}
	virtual void dateTimeValue(long long value) {
	}
	virtual void linkValue(struct Link value) {
	}
	int field_count;
	char * class_name;
	TrackerListener() :
			field_count(0), class_name(0) {
	}

};

START_TEST(test_simple_reader)
	{
		RecordParser parser("ORecordSerializerBinary");
		char content[1024];
		std::fstream document_data("data/document.data");
		document_data.read(content, 1024);
		TrackerListener listener;
		parser.parse(content, 1024, listener);
		std::cout << listener.field_count;
		std::cout << listener.class_name;
		std::cout.flush();
		assert(listener.field_count == 2);
		assert(listener.class_name != 0);
		std::cout << "class name :"<<listener.class_name<<std::endl;
		assert(std::string(listener.class_name) == std::string("TheClass"));
	}END_TEST

START_TEST(test_fail_wrong_format)
	{

		try {
			RecordParser parser("sdfsdfs");
			assert(false);
		} catch (...) {
		}

	}END_TEST

Suite * file_suite(void) {
	Suite *s = suite_create("file");
	TCase *tc_core = tcase_create("simple_reader");
	tcase_add_test(tc_core, test_simple_reader);
	tcase_add_test(tc_core, test_fail_wrong_format);
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

