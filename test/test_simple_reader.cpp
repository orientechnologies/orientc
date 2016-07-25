#include <iostream>
#include "../src/orientc.h"
#include <fstream>
#include <stdlib.h>
#include <assert.h>
#include "test_reader_listener.h"

using namespace Orient;

void test_simple_reader() {
	try {
		RecordParser parser("onet_ser_v0");
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
		assert(listener.balanced_document_count == 0);
	} catch (...) {
		assert(false);
	}

}

void test_simple_reader_many() {
	try {
		RecordParser parser("onet_ser_v0");
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
		RecordParser parser("onet_ser_v0");
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
		RecordParser parser("onet_ser_v0");
		char content[10000];
		std::fstream document_data("data/all_fields.data");
		document_data.read(content, 10000);
		SimpleTrackerListener listener;
		parser.parse((unsigned char *) content, 10000, listener);
		assert(listener.field_count == 22);
		assert(listener.balanced_count == 0);
		assert(listener.integer_value == 21);
		assert(listener.float_value == (float )10.2);
		assert(listener.double_value == (double )11.2);
		assert(listener.short_value == 2);
		assert(listener.long_value == 32);
		assert(listener.binary_size == 4);
		assert(listener.date_value == 1341273600000);
		assert(listener.date_time_value == 1446808223000);
		assert(listener.link_value.cluster == 10 && listener.link_value.position == 20);
		assert(listener.binary_value[0] == 'a' && listener.binary_value[1] == 'd' && listener.binary_value[2] == 'a' && listener.binary_value[3] == 'd');
		//Checking that the class name if the one of the embedded document.
		assert(std::string(listener.class_name) == std::string("test1"));
		assert(std::string(listener.a_string_value) == std::string("value2"));
		assert(listener.linkList.size() == 3);
		assert(listener.linkSet.size() == 3);
		//TODO: implement linkmap
		assert(listener.linkMap.size() == 2);
		assert(listener.embeddedList.size() == 2);
		assert(listener.embeddedSet.size() == 2);
		assert(listener.embeddedMap.size() == 2);
		assert(listener.embeddedRidbag.size() == 3);
		assert(listener.scale == 16);
		assert(listener.decimal_size == 15);

	} catch (...) {
		assert(false);
	}
}

void test_reader_nested() {
	try {
		RecordParser parser("onet_ser_v0");
		char content[10000];
		std::fstream document_data("data/deep_embedded.data");
		document_data.read(content, 10000);
		SimpleTrackerListener listener;
		parser.parse((unsigned char *) content, 10000, listener);
		assert(listener.startDocumentCount == 6);
		assert(listener.field_count == 9);
	} catch (...) {
		assert(false);
	}
}

void test_reader_mix_nested() {
	try {
		RecordParser parser("onet_ser_v0");
		char content[10000];
		std::fstream document_data("data/deep_mix_embedded.data");
		document_data.read(content, 10000);
		SimpleTrackerListener listener;
		parser.parse((unsigned char *) content, 10000, listener);
		assert(listener.startDocumentCount == 7);
		//it keep only the strings of all the embedded lists.
		assert(listener.embeddedList.size() == 4);
		assert(std::string(listener.a_string_value) == std::string("text_last"));
		assert(listener.balanced_count == 0);
		assert(listener.field_count == 12);
	} catch (...) {
		assert(false);
	}
}

void test_reader_ridbag_tree_nested() {
	try {
		RecordParser parser("onet_ser_v0");
		char content[10000];
		std::fstream document_data("data/ridbag_tree.data");
		document_data.read(content, 10000);
		SimpleTrackerListener listener;
		parser.parse((unsigned char *) content, 10000, listener);
		assert(listener.startDocumentCount == 1);
		assert(listener.balanced_count == 0);
		assert(listener.field_count == 1);
		assert(listener.ridbagKey);
	} catch (...) {
		assert(false);
	}
}

void test_reader_null_nested() {
	try {
		RecordParser parser("onet_ser_v0");
		char content[10000];
		std::fstream document_data("data/with_null.data");
		document_data.read(content, 10000);
		SimpleTrackerListener listener;
		parser.parse((unsigned char *) content, 10000, listener);
		assert(listener.startDocumentCount == 1);
		assert(listener.balanced_count == 0);
		assert(listener.field_count == 8);
		std::cout<<listener.nullReadCount ;
		assert(listener.nullReadCount == 7 );
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
	test_reader_nested();
	test_reader_mix_nested();
	test_reader_ridbag_tree_nested();
	test_reader_null_nested();
	return 0;
}

