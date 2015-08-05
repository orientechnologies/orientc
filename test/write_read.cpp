#include "../src/orientc.h"
#include <iostream>
#include <check.h>
#include <assert.h>
#include <stdlib.h>
#include "test_reader_listener.h"
#include "test_reader_listener_link_list.h"
using namespace Orient;

START_TEST(test_simple_write_read)
	try {
		RecordWriter writer("ORecordSerializerBinary");
		writer.startDocument("Test");

		writer.startField("test", STRING);
		writer.stringValue("test");

		writer.endField("test", STRING);
		writer.startField("number", INTEGER);
		writer.intValue(10);

		writer.endField("number", INTEGER);

		int size;

		const char * content = writer.writtenContent(&size);
		RecordParser reader("ORecordSerializerBinary");

		TrackerListener listener;
		reader.parse((char *) content, size, listener);
		free((void *) content);
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

START_TEST(test_all_simple_write_read)
	try {
		RecordWriter writer("ORecordSerializerBinary");
		writer.startDocument("Test");
		writer.startField("test", STRING);
		writer.stringValue("test");
		writer.endField("test", STRING);

		writer.startField("number", INTEGER);
		writer.intValue(10);
		writer.endField("number", INTEGER);

		writer.startField("long", LONG);
		writer.longValue(30);
		writer.endField("long", LONG);

		writer.startField("bool", BOOLEAN);
		writer.booleanValue(true);
		writer.endField("bool", BOOLEAN);

		writer.startField("short", SHORT);
		writer.shortValue(20);
		writer.endField("short", SHORT);

		writer.startField("byte", BYTE);
		writer.byteValue(40);
		writer.endField("byte", BYTE);

		writer.startField("float", FLOAT);
		writer.floatValue(50.04f);
		writer.endField("float", FLOAT);

		writer.startField("double", DOUBLE);
		writer.doubleValue(60.043);
		writer.endField("double", DOUBLE);

		writer.startField("binary", BINARY);
		writer.binaryValue("bla", 3);
		writer.endField("binary", BINARY);

		writer.startField("date", DATE);
		writer.dateValue(2073600000);
		writer.endField("date", DATE);

		writer.startField("datetime", DATETIME);
		writer.dateTimeValue(2073600100);
		writer.endField("datetime", DATETIME);

		writer.startField("link", LINK);
		struct Link l;
		l.cluster = 10;
		l.position = 20;
		writer.linkValue(l);
		writer.endField("link", LINK);

		int size;

		const char * content = writer.writtenContent(&size);
		RecordParser reader("ORecordSerializerBinary");

		TrackerListener listener;
		reader.parse((char *) content, size, listener);
		free((void *) content);

		assert(listener.field_count == 12);
		assert(listener.class_name != 0);
		assert(std::string(listener.class_name) == std::string("Test"));
		assert(std::string(listener.a_string_value) == std::string("test"));
		assert(listener.integer_value == 10);
		assert(listener.long_value == 30);
		assert(listener.boolean_value == true);
		assert(listener.short_value == 20);
		assert(listener.byte_value == 40);
		assert(listener.float_value == (float )50.04);
		assert(listener.double_value == (double )60.043);
		assert(listener.binary_size == 3);
		assert(listener.binary_value[0] == 'b' && listener.binary_value[1] == 'l' && listener.binary_value[2] == 'a');
		assert(listener.date_value == 2073600000);
		assert(listener.date_time_value == 2073600100);
		assert(listener.link_value.cluster == 10 && listener.link_value.position == 20);
		assert(listener.balanced_count == 0);

	} catch (const char * oh) {
		std::cout << "oh" << oh;
		std::cout.flush();

	}

END_TEST

START_TEST(test_embedded_collection_read_write)
	try {
		RecordWriter writer("ORecordSerializerBinary");
		writer.startDocument("Test");
		writer.startField("testCollection", EMBEDDEDLIST);
		writer.startCollection(12);
		writer.stringValue("test");
		writer.intValue(10);
		writer.longValue(30);
		writer.booleanValue(true);
		writer.shortValue(20);
		writer.byteValue(40);
		writer.floatValue(50.04f);
		writer.doubleValue(60.043);
		writer.binaryValue("bla", 3);
		writer.dateValue(2073600000);
		writer.dateTimeValue(2073600100);
		struct Link l;
		l.cluster = 10;
		l.position = 20;
		writer.linkValue(l);
		writer.endCollection();
		writer.endField("testCollection", EMBEDDEDLIST);
		writer.endDocument();
		int size;

		const char * content = writer.writtenContent(&size);
		RecordParser reader("ORecordSerializerBinary");

		TrackerListener listener;
		reader.parse((char *) content, size, listener);
		free((void *) content);

		assert(listener.field_count == 1);
		assert(listener.collectionSize == 12);
		assert(listener.class_name != 0);
		assert(std::string(listener.class_name) == std::string("Test"));
		assert(std::string(listener.a_string_value) == std::string("test"));
		assert(listener.integer_value == 10);
		assert(listener.long_value == 30);
		assert(listener.boolean_value == true);
		assert(listener.short_value == 20);
		assert(listener.byte_value == 40);
		assert(listener.float_value == (float )50.04);
		assert(listener.double_value == (double )60.043);
		assert(listener.binary_size == 3);
		assert(listener.binary_value[0] == 'b' && listener.binary_value[1] == 'l' && listener.binary_value[2] == 'a');
		assert(listener.date_value == 2073600000);
		assert(listener.date_time_value == 2073600100);
		assert(listener.link_value.cluster == 10 && listener.link_value.position == 20);
		assert(listener.balanced_count == 0);

	} catch (const char * oh) {
		std::cout << "oh" << oh;
		std::cout.flush();

	}

END_TEST

START_TEST(test_link_collection_read_write)
	try {
		RecordWriter writer("ORecordSerializerBinary");
		writer.startDocument("Test");
		writer.startField("testCollection", LINKLIST);
		writer.startCollection(10);
		struct Link lnk;
		lnk.cluster = 0;
		lnk.position = 1;
		for (; lnk.position < 10; lnk.position++) {
			writer.linkValue(lnk);
			lnk.cluster = lnk.position;
		}
		writer.endCollection();
		writer.endField("testCollection", LINKLIST);
		writer.endDocument();
		int size;

		const char * content = writer.writtenContent(&size);
		RecordParser reader("ORecordSerializerBinary");

		LinkListListener listener;
		reader.parse((char *) content, size, listener);
		free((void *) content);

		assert(listener.collectionSize == 10);
		assert(listener.count == 10);

	} catch (const char * oh) {
		std::cout << "oh" << oh;
		std::cout.flush();

	}

END_TEST

START_TEST(test_embedded_map_read_write)
	try {
		RecordWriter writer("ORecordSerializerBinary");
		writer.startDocument("Test");
		writer.startField("testCollection", EMBEDDEDMAP);
		writer.startMap(12);
		writer.mapKey("key0");
		writer.stringValue("test");
		writer.mapKey("key1");
		writer.intValue(10);
		writer.mapKey("key2");
		writer.longValue(30);
		writer.mapKey("key3");
		writer.booleanValue(true);
		writer.mapKey("key4");
		writer.shortValue(20);
		writer.mapKey("key5");
		writer.byteValue(40);
		writer.mapKey("key6");
		writer.floatValue(50.04f);
		writer.mapKey("key7");
		writer.doubleValue(60.043);
		writer.mapKey("key8");
		writer.binaryValue("bla", 3);
		writer.mapKey("key9");
		writer.dateValue(2073600000);
		writer.mapKey("key10");
		writer.dateTimeValue(2073600100);
		writer.mapKey("key11");
		struct Link l;
		l.cluster = 10;
		l.position = 20;
		writer.linkValue(l);

		writer.endMap();
		writer.endField("testCollection", EMBEDDEDMAP);
		writer.endDocument();
		int size;

		const char * content = writer.writtenContent(&size);
		RecordParser reader("ORecordSerializerBinary");

		TrackerListener listener;
		reader.parse((char *) content, size, listener);
		free((void *) content);

		assert(listener.mapSize == 12);
		assert(listener.mapCount == 12);

	} catch (const char * oh) {
		std::cout << "oh" << oh;
		std::cout.flush();

	}

END_TEST

Suite * file_suite(void) {
	Suite *s = suite_create("file");
	TCase *tc_core = tcase_create("simmetric_writer_reader");
	tcase_add_test(tc_core, test_simple_write_read);
	tcase_add_test(tc_core, test_all_simple_write_read);
	tcase_add_test(tc_core, test_embedded_collection_read_write);
	tcase_add_test(tc_core, test_link_collection_read_write);
	tcase_add_test(tc_core, test_embedded_map_read_write);
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
