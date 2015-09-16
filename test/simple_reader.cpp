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
	return 0;
}

