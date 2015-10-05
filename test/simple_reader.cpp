#include <iostream>
#include "../src/orientc.h"
#include <fstream>
#include <stdlib.h>
#include <assert.h>
#include "test_reader_listener.h"

using namespace Orient;

void test_simple_reader() {
	try {
		RecordParser parser("ORecordSerializerBinary");
		char content[1024];
		std::fstream document_data("data/document.data");
		document_data.read(content, 1024);
		TrackerListener listener;
		parser.parse((unsigned char *) content, 1024, listener);
		assert(listener.field_count == 2);
		assert(listener.class_name != 0);
		assert(std::string(listener.class_name) == std::string("TheClass"));
		assert(std::string(listener.a_string_value) == std::string("test"));
		assert(listener.integer_value == 2);
		assert(listener.balanced_count == 0);
	} catch (...) {
		assert(false);
	}

}

void test_simple_reader_many() {
	try {
		RecordParser parser("ORecordSerializerBinary");
		char content[2048];
		std::fstream document_data("data/many_field.data");
		document_data.read(content, 2048);
		TrackerListener listener;
		parser.parse((unsigned char *) content, 2048, listener);
		assert(listener.field_count > 2);
		assert(listener.balanced_count == 0);
	} catch (...) {
		assert(false);
	}
}

void test_simple_reader_big() {
	try {
		RecordParser parser("ORecordSerializerBinary");
		char content[10000];
		std::fstream document_data("data/basic_long_string.data");
		document_data.read(content, 10000);
		TrackerListener listener;
		parser.parse((unsigned char *) content, 10000, listener);
		assert(listener.field_count == 2);
		assert(listener.balanced_count == 0);
	} catch (...) {
		assert(false);
	}
}

void test_simple_reader_all() {
	try {
		RecordParser parser("ORecordSerializerBinary");
		char content[10000];
		std::fstream document_data("data/all_fields.data");
		document_data.read(content, 10000);
		SimpleTrackerListener listener;
		parser.parse((unsigned char *) content, 10000, listener);
		assert(listener.field_count == 21);
		assert(listener.balanced_count == 0);
		assert(listener.integer_value == 21);
		assert(listener.float_value == (float )10.2);
		std::cout << listener.double_value;
		assert(listener.double_value == (double )11.2);
		assert(listener.short_value == 2);
		assert(listener.long_value == 32);
		assert(listener.binary_size == 4);
		assert(listener.link_value.cluster == 10 && listener.link_value.position ==20);
		assert(listener.binary_value[0] == 'a' && listener.binary_value[1] == 'd' && listener.binary_value[2] == 'a' && listener.binary_value[3] == 'd');
		//Checking that the class name if the one of the embedded document.
		assert(std::string(listener.class_name) == std::string("test1"));
		assert(std::string(listener. a_string_value) == std::string("value2"));
		assert(listener.linkList.size() == 3);
		assert(listener.linkSet.size() == 3);
		//TODO: implement linkmap
//		assert(listener.linkMap.size() == 2);
		assert(listener.embeddedList.size() == 2);
		assert(listener.embeddedSet.size() == 2);
		assert(listener.embeddedMap.size() == 2);
	} catch (...) {
		assert(false);
	}
}

void test_fail_wrong_format() {
	try {
		RecordParser parser("sdfsdfs");
		assert(false);
	} catch (...) {
	}
}

int main() {
	test_simple_reader();
	test_fail_wrong_format();
	test_simple_reader_many();
	test_simple_reader_big();
	test_simple_reader_all();
	return 0;
}

