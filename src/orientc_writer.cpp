#include "orientc_writer.h"
#include "helpers.h"
#include <list>
#include <stdlib.h>
#include <string.h>
namespace Orient {

void writeString(ContentBuffer & buffer, char *string);
void writeFlatInteger(ContentBuffer & buffer, long string);

class DocumentWriter {
public:
	ContentBuffer header;
	ContentBuffer data;
	std::list<std::pair<int, int> > pointers;
	void writeRecordVersion();
	void writeClass(char * name);
	void startField(char *name, OType type);
	void endField(char *name, OType type);
	char * writtenContent(int *size);
};

class InternalWriter {
public:
	std::list<DocumentWriter *> nested;
};

void DocumentWriter::writeRecordVersion() {
	header.prepare(1);
	header.content[header.cursor] = 0;
}

void DocumentWriter::writeClass(char * name) {
	writeString(header, name);
}

void DocumentWriter::startField(char *name, OType type) {
	writeString(header, name);
	header.prepare(4);
	std::pair<int, int> toAdd(header.cursor, data.prepared);
	pointers.push_back(toAdd);
	header.prepare(1);
	header.content[header.cursor] = type;
}

void DocumentWriter::endField(char *name, OType type) {
}

char * DocumentWriter::writtenContent(int * size) {
	writeVarint(header, 0);
	int headerSize = header.prepared;
	int dataSize = data.prepared;
	int wholeSize = headerSize + dataSize;
	std::list<std::pair<int, int> >::iterator i;
	for (i = pointers.begin(); i != pointers.end(); ++i) {
		std::pair<int, int> & pair = *i;
		header.force_cursor(pair.first);
		writeFlatInteger(header, pair.second + headerSize);
	}
	char * all = (char *) malloc(wholeSize);
	memcpy(all, header.content, headerSize);
	memcpy(all + headerSize, data.content, dataSize);
	*size = wholeSize;
	return all;
}

RecordWriter::RecordWriter(std::string formatter) :
		writer(new InternalWriter) {
	if (formatter != "ORecordSerializerBinary")
		throw "Formatter not supported";
	writer->nested.push_front(new DocumentWriter());
	DocumentWriter * doc = writer->nested.front();
	doc->writeRecordVersion();

}

void RecordWriter::className(char * name) {
	DocumentWriter * doc = writer->nested.front();
	doc->writeClass(name);

}

void RecordWriter::startField(char *name, OType type) {
	DocumentWriter *front = writer->nested.front();
	front->startField(name, type);
	if (type == EMBEDDED) {
		writer->nested.push_front(new DocumentWriter());
	}

}

void RecordWriter::stringValue(char * value) {
	DocumentWriter *front = writer->nested.front();
	writeString(front->data, value);
}

void RecordWriter::intValue(long value) {
	DocumentWriter *front = writer->nested.front();
	writeVarint(front->data, value);
}

void RecordWriter::shortValue(short value) {
	DocumentWriter *front = writer->nested.front();
	writeVarint(front->data, value);
}

void RecordWriter::longValue(long long value) {
	DocumentWriter *front = writer->nested.front();
	writeVarint(front->data, value);
}

void RecordWriter::dateValue(long long value) {
	DocumentWriter *front = writer->nested.front();
	//TODO:Reduce the size
	writeVarint(front->data, value);
}

void RecordWriter::dateTimeValue(long long value) {
	DocumentWriter *front = writer->nested.front();
	writeVarint(front->data, value);
}

void RecordWriter::byteValue(char value) {
	DocumentWriter *front = writer->nested.front();
	front->data.prepare(1);
	front->data.content[front->data.cursor] = value;
}

void RecordWriter::booleanValue(bool value) {
	DocumentWriter *front = writer->nested.front();
	front->data.prepare(1);
	front->data.content[front->data.cursor] = value ? 1 : 0;
}

void RecordWriter::endField(char *name, OType type) {
	DocumentWriter *front = writer->nested.front();
	front->endField(name, type);
	if (type == EMBEDDED) {
		int size;
		char * content = front->writtenContent(&size);
		writer->nested.pop_front();
		delete front;
		DocumentWriter *front1 = writer->nested.front();
		front1->data.prepare(size);
		memcpy(front1->data.content + front1->data.cursor, content, size);
	}
}

const char * RecordWriter::writtenContent(int * size) {
	return writer->nested.front()->writtenContent(size);
}

void writeString(ContentBuffer & buffer, char *string) {
	int size = strlen(string);
	writeVarint(buffer, size);
	buffer.prepare(size);
	memcpy(buffer.content + buffer.cursor, string, size);
}

void writeFlatInteger(ContentBuffer & buffer, long value) {
	buffer.prepare(4);
	buffer.content[buffer.cursor] = (char) ((value >> 24) & 0xFF);
	buffer.content[buffer.cursor + 1] = (char) ((value >> 16) & 0xFF);
	buffer.content[buffer.cursor + 2] = (char) ((value >> 8) & 0xFF);
	buffer.content[buffer.cursor + 3] = (char) ((value >> 0) & 0xFF);
}

}
