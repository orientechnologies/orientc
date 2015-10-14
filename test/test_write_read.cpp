#include "../src/orientc.h"
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <execinfo.h>
#include <stdio.h>
#include <unistd.h>
#include <../src/parse_exception.h>
#include "test_reader_listener.h"
#include "test_reader_listener_link_list.h"
using namespace Orient;

void test_simple_write_read() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");

		writer.startField("test");
		writer.stringValue("test");

		writer.endField("test");
		writer.startField("number");
		writer.intValue(10);

		writer.endField("number");

		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse((const unsigned char *) content, size, listener);
		delete[] content;
		assert(listener.field_count == 2);
		assert(listener.class_name != 0);
		assert(std::string(listener.class_name) == std::string("Test"));
		assert(std::string(listener.a_string_value) == std::string("test"));
		assert(listener.integer_value == 10);
		assert(listener.balanced_count == 0);
	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}

}

void test_all_simple_write_read() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("test");
		writer.stringValue("test");
		writer.endField("test");

		writer.startField("number");
		writer.intValue(10);
		writer.endField("number");

		writer.startField("long");
		writer.longValue(30);
		writer.endField("long");

		writer.startField("bool");
		writer.booleanValue(true);
		writer.endField("bool");

		writer.startField("short");
		writer.shortValue(20);
		writer.endField("short");

		writer.startField("byte");
		writer.byteValue(40);
		writer.endField("byte");

		writer.startField("float");
		writer.floatValue(50.04f);
		writer.endField("float");

		writer.startField("double");
		writer.doubleValue(60.043);
		writer.endField("double");

		writer.startField("binary");
		writer.binaryValue("bla", 3);
		writer.endField("binary");

		writer.startField("date");
		writer.dateValue(2073600000);
		writer.endField("date");

		writer.startField("datetime");
		writer.dateTimeValue(2073600100);
		writer.endField("datetime");

		writer.startField("link");
		struct Link l;
		l.cluster = 10;
		l.position = 20;
		writer.linkValue(l);
		writer.endField("link");

		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse((const unsigned char *) content, size, listener);
		delete[] content;

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

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}

}

void test_embedded_collection_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("testCollection");
		writer.startCollection(12, EMBEDDEDLIST);
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
		writer.endCollection(EMBEDDEDLIST);
		writer.endField("testCollection");
		writer.endDocument();
		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse((const unsigned char *) content, size, listener);
		delete[] content;

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

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}

}

void test_link_collection_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("testCollection");
		writer.startCollection(10, LINKLIST);
		struct Link lnk;
		lnk.cluster = 0;
		lnk.position = 1;
		for (; lnk.position <= 10; lnk.position++) {
			writer.linkValue(lnk);
			lnk.cluster = lnk.position;
		}
		writer.endCollection(LINKLIST);
		writer.endField("testCollection");
		writer.endDocument();
		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		LinkListListener listener;
		reader.parse((const unsigned char *) content, size, listener);
		delete[] content;

		assert(listener.collectionSize == 10);
		assert(listener.count == 11);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

void test_embedded_map_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("testEmbeddedMap");
		writer.startMap(12, EMBEDDEDMAP);
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

		writer.endMap(EMBEDDEDMAP);
		writer.endField("testEmbeddedMap");
		writer.endDocument();
		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse((const unsigned char *) content, size, listener);
		delete[] content;

		assert(listener.mapSize == 12);
		assert(listener.mapCount == 12);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

void test_link_map_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("testLinkMap");
		writer.startMap(2, LINKMAP);

		writer.mapKey("key0");
		struct Link link;
		link.cluster = 10;
		link.position = 20;
		writer.linkValue(link);

		writer.mapKey("key1");
		link.cluster = 10;
		link.position = 22;
		writer.linkValue(link);

		writer.endMap(LINKMAP);
		writer.endField("testLinkMap");
		writer.endDocument();
		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse((const unsigned char *) content, size, listener);
		delete[] content;

		assert(listener.mapSize == 2);
		assert(listener.mapCount == 2);
		assert(listener.link_value.cluster == 10 && listener.link_value.position == 22);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

void test_link_bag_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("testLinkBag");
		writer.startCollection(2, LINKBAG);

		struct Link link;
		link.cluster = 10;
		link.position = 20;
		writer.linkValue(link);

		link.cluster = 10;
		link.position = 22;
		writer.linkValue(link);

		writer.endCollection(LINKBAG);
		writer.endField("testLinkBag");
		writer.endDocument();
		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse(content, size, listener);
		delete[] content;

		assert(listener.collectionSize == 2);
		assert(listener.link_value.cluster == 10 && listener.link_value.position == 22);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

void test_embedded_deep_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("embed");
		writer.startDocument("");
		writer.startField("as");
		writer.stringValue("string");
		writer.endField("as");
		writer.startField("embed");
		writer.startDocument("");
		writer.startField("embed");
		writer.startDocument("");
		writer.startField("field");
		writer.stringValue("string");
		writer.endField("field");
		writer.endDocument();
		writer.endField("embed");
		writer.endDocument();
		writer.endField("embed");
		writer.endDocument();
		writer.endField("embed");
		writer.endDocument();

		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse(content, size, listener);
		delete[] content;

		assert(listener.startDocumentCount == 4);
		assert(listener.field_count == 5);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

void test_embedded_deep_collections_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("embed");
		writer.startDocument("");
		writer.startField("field");

		writer.startCollection(2, EMBEDDEDLIST);
		writer.stringValue("string");
		writer.startMap(3, EMBEDDEDMAP);
		writer.mapKey("key0");
		writer.stringValue("value");
		writer.mapKey("key1");
		writer.startDocument("");
		writer.startField("bla");
		writer.intValue(10);
		writer.endDocument();
		writer.mapKey("key2");
		writer.stringValue("string");
		writer.endMap(EMBEDDEDMAP);
		writer.endCollection(EMBEDDEDLIST);
		writer.endField("field");

		writer.endDocument();
		writer.endField("embed");
		writer.endDocument();

		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse(content, size, listener);
		delete[] content;

		assert(listener.collectionSize == 2);
		assert(listener.mapCount == 3);
		assert(listener.startDocumentCount == 3);
		assert(listener.field_count == 3);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

void test_link_bag_tree_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");
		writer.startField("testLinkBag");
		writer.ridBagTreeKey(20, 20, 20);
		writer.endField("testLinkBag");
		writer.endDocument();
		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse(content, size, listener);
		delete[] content;

		assert(listener.fileId == 20);
		assert(listener.pageIndex == 20);
		assert(listener.pageOffset == 20);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

void test_null_read_write() {
	try {
		RecordWriter writer("onet_ser_v0");
		writer.startDocument("Test");

		writer.startField("embeddedList");
		writer.startCollection(2, EMBEDDEDLIST);
		writer.nullValue();
		writer.stringValue("bla");
		writer.endCollection(EMBEDDEDLIST);
		writer.endField("embeddedList");

		writer.startField("embeddedSet");
		writer.startCollection(2, EMBEDDEDSET);
		writer.nullValue();
		writer.stringValue("bla");
		writer.endCollection(EMBEDDEDSET);
		writer.endField("embeddedSet");

		writer.startField("linklist");
		writer.startCollection(2, LINKLIST);
		writer.nullValue();
		struct Link l;
		l.cluster = 0;
		l.position = 1;
		writer.linkValue(l);
		writer.endCollection(LINKLIST);
		writer.endField("linklist");

		writer.startField("linkset");
		writer.startCollection(2, LINKSET);
		writer.nullValue();
		struct Link l1;
		l1.cluster = 0;
		l1.position = 1;
		writer.linkValue(l);
		writer.endCollection(LINKSET);
		writer.endField("linkset");
//
		writer.startField("embeddedMap");
		writer.startMap(2, EMBEDDEDMAP);
		writer.mapKey("key0");
		writer.nullValue();
		writer.mapKey("key1");
		writer.stringValue("value");
		writer.endMap(EMBEDDEDMAP);
		writer.endField("embeddedMap");

		writer.startField("linkMap");
		writer.startMap(2, LINKMAP);
		writer.mapKey("key2");
		writer.nullValue();
		writer.mapKey("key3");
		struct Link l2;
		l2.cluster = 0;
		l2.position = 1;
		writer.linkValue(l2);
		writer.endMap(LINKMAP);
		writer.endField("linkMap");
//
		writer.startField("nullField");
		writer.nullValue();
		writer.endField("nullField");

		writer.endDocument();
		int size;

		const unsigned char * content = writer.writtenContent(&size);
		RecordParser reader("onet_ser_v0");

		TrackerListener listener;
		reader.parse(content, size, listener);
		delete[] content;

		std::cout<<listener.field_count <<std::endl;
		assert(listener.field_count == 7);
		std::cout<<listener.nullReadCount <<std::endl;
		assert(listener.nullReadCount == 7);

	} catch (parse_exception & oh) {
		std::cout << "oh" << oh.what();
		std::cout.flush();
		assert(false);
	}
}

int main() {
	test_simple_write_read();
	test_all_simple_write_read();
	test_link_collection_read_write();
	test_embedded_map_read_write();
	test_embedded_collection_read_write();
	test_link_map_read_write();
	test_link_bag_read_write();
	test_embedded_deep_read_write();
	test_embedded_deep_collections_read_write();
	test_link_bag_tree_read_write();
	test_null_read_write();
	return 0;
}
