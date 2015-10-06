#include "orientc_reader.h"

#include <cstring>
#include "helpers.h"
#include "parse_exception.h"
#include <arpa/inet.h>
#include <endian.h>

namespace Orient {

void readSimpleValue(ContentBuffer &reader, OType type, RecordParseListener & listener);
inline void readValueString(ContentBuffer & reader, RecordParseListener & listener);
void readValueLinkCollection(ContentBuffer & reader, RecordParseListener & listener);
void readValueEmbeddedCollection(ContentBuffer & reader, RecordParseListener & listener);
void readValueEmbeddedMap(ContentBuffer & reader, RecordParseListener & listener);
void readValueLinkMap(ContentBuffer & reader, RecordParseListener & listener);
inline void readValueLink(ContentBuffer & reader, RecordParseListener & listener);
void readDocument(ContentBuffer &reader, RecordParseListener & listener);
int32_t readFlat32Integer(ContentBuffer & reader);

RecordParser::RecordParser(std::string formatter) {
	if (formatter != "ORecordSerializerBinary")
		throw parse_exception("Formatter not supported");
}

void RecordParser::parse(const unsigned char * content, int content_size, RecordParseListener &listener) {
	ContentBuffer reader(content, content_size);
	reader.prepare(1);
	if (reader.content[reader.cursor] != 0)
		throw parse_exception("unsupported version");
	readDocument(reader, listener);
}

void readDocument(ContentBuffer &reader, RecordParseListener & listener) {
	int64_t class_size = readVarint(reader);
	if (class_size != 0) {
		reader.prepare(class_size);
		char * class_name = (char *) reader.content + reader.cursor;
		listener.startDocument(class_name, class_size);
	} else
		listener.startDocument("", 0);
	int64_t size = 0;
	while ((size = readVarint(reader)) != 0) {
		if (size > 0) {
			reader.prepare(size);
			char * field_name = (char *) reader.content + reader.cursor;
			int32_t position = readFlat32Integer(reader);
			reader.prepare(1);
			OType type = (OType) reader.content[reader.cursor];
			listener.startField(field_name, size, type);
			int temp = reader.prepared;
			reader.force_cursor(position);
			readSimpleValue(reader, type, listener);
			reader.force_cursor(temp);
			listener.endField(field_name, size);
		} else {
			throw new parse_exception("property id not supported by network serialization");
		}
	}
}

void readSimpleValue(ContentBuffer &reader, OType type, RecordParseListener & listener) {

	switch (type) {
	case STRING:
		readValueString(reader, listener);
		break;
	case INTEGER: {
		int64_t value = readVarint(reader);
		listener.intValue(value);
	}
		break;
	case LONG: {
		int64_t value = readVarint(reader);
		listener.longValue(value);
	}
		break;
	case SHORT: {
		int64_t value = readVarint(reader);
		listener.shortValue(value);
	}
		break;
	case BYTE: {
		reader.prepare(1);
		listener.byteValue(reader.content[reader.cursor]);
	}
		break;

	case BOOLEAN: {
		reader.prepare(1);
		listener.booleanValue(reader.content[reader.cursor] != 0);
	}
		break;
	case DATE: {
		int64_t read = readVarint(reader);
		read *= 86400000;
		listener.dateValue(read);
	}
		break;
	case FLOAT: {
		int32_t i_val = readFlat32Integer(reader);
		float fl;
		memcpy(&fl, &i_val, 4);
		listener.floatValue(fl);
	}
		break;
	case DOUBLE: {
		int64_t i_val;
		reader.prepare(8);
		memcpy(&i_val, reader.content + reader.cursor, 8);
		i_val = be64toh(i_val);
		double db;
		memcpy(&db, &i_val, 8);
		listener.doubleValue(db);
	}
		break;
	case DATETIME: {
		int64_t value = readVarint(reader);
		listener.dateTimeValue(value);
	}
		break;
	case LINK: {
		readValueLink(reader, listener);
	}
		break;
	case LINKSET:
	case LINKLIST: {
		readValueLinkCollection(reader, listener);
	}
		break;
	case BINARY: {
		int64_t value_size = readVarint(reader);
		reader.prepare(value_size);
		listener.binaryValue((char *) reader.content + reader.cursor, value_size);
	}
		break;
	case EMBEDDEDLIST:
	case EMBEDDEDSET: {
		readValueEmbeddedCollection(reader, listener);
	}
		break;
	case EMBEDDEDMAP: {
		readValueEmbeddedMap(reader, listener);
	}
		break;
	case LINKMAP: {
		readValueLinkMap(reader, listener);
	}
		break;
	case EMBEDDED: {
		readDocument(reader, listener);
	}
		break;
	default:
		break;
	}

}

void readValueString(ContentBuffer & reader, RecordParseListener & listener) {
	int64_t value_size = readVarint(reader);
	reader.prepare(value_size);
	listener.stringValue((char *) reader.content + reader.cursor, value_size);
}

void readValueLink(ContentBuffer & reader, RecordParseListener & listener) {
	Link link;
	link.cluster = readVarint(reader);
	link.position = readVarint(reader);
	listener.linkValue(link);
}

void readValueLinkCollection(ContentBuffer & reader, RecordParseListener & listener) {
	int size = readVarint(reader);
	listener.startCollection(size);
	while (size-- > 0) {
		//TODO: handle null
		readValueLink(reader, listener);
	}
	listener.endCollection();

}
void readValueEmbeddedCollection(ContentBuffer & reader, RecordParseListener & listener) {
	int size = readVarint(reader);
	listener.startCollection(size);
	reader.prepare(1);
	OType type = (OType) reader.content[reader.cursor];
	if (ANY == type) {
		while (size-- > 0) {
			reader.prepare(1);
			OType entryType = (OType) reader.content[reader.cursor];
			if (ANY == entryType)
				;	//todo handle null
			else
				readSimpleValue(reader, entryType, listener);
		}
	}
	listener.endCollection();
	//For now else is impossible
}

void readValueEmbeddedMap(ContentBuffer & reader, RecordParseListener & listener) {
	int64_t size = readVarint(reader);
	listener.startMap(size);
	while (size-- > 0) {
		//Skipping because is everytime string
		reader.prepare(1);
		int key_size = readVarint(reader);
		reader.prepare(key_size);
		char * key_name = (char *) reader.content + reader.cursor;
		listener.mapKey(key_name, key_size);
		long position = readFlat32Integer(reader);
		reader.prepare(1);
		OType type = (OType) reader.content[reader.cursor];
		int temp = reader.prepared;
		reader.force_cursor(position);
		readSimpleValue(reader, type, listener);
		reader.force_cursor(temp);
	}
	listener.endMap();
}

void readValueLinkMap(ContentBuffer & reader, RecordParseListener & listener) {
	int64_t size = readVarint(reader);
	listener.startMap(size);
	while (size-- > 0) {
		//Skipping because is everytime string
		reader.prepare(1);
		int key_size = readVarint(reader);
		reader.prepare(key_size);
		char * key_name = (char *) reader.content + reader.cursor;
		listener.mapKey(key_name, key_size);
		readValueLink(reader, listener);
	}
}

int32_t readFlat32Integer(ContentBuffer & reader) {
	int32_t value;
	reader.prepare(4);
	memcpy(&value, reader.content + reader.cursor, 4);
	value = be32toh(value);
	return value;
}

}

