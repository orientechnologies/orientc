#include "orientc.h"
#include "helpers.h"
#include <malloc.h>
#include <memory.h>
namespace Orient {

void readString(ContentReader & reader, char * str, int size);

RecordParser::RecordParser(std::string formatter) {
	if (formatter != "ORecordSerializerBinary")
		throw "Formatter not supported";
}

void RecordParser::parse(char * content, int content_size, RecordParseListener &listener) {
	ContentReader reader(content, content_size);
	reader.prepare_read(1);
	if (reader.content[reader.cursor] != 0)
		throw "unsupported version";
	long long class_size = readVarint(reader);
	char * class_name = reinterpret_cast<char *>(malloc(class_size));
	readString(reader, class_name, class_size);
	listener.className(class_name);
	free(class_name);
	long long size = 0;
	size = readVarint(reader);
	while ((size) != 0) {
		if (size > 0) {
			char * filed_name = reinterpret_cast<char *>(malloc(size));
			readString(reader, filed_name, size);
			reader.prepare_read(4);
			reader.prepare_read(1);
			listener.startField(filed_name, (OType) reader.content[reader.cursor]);
			free(filed_name);
		} else {
			// god sake
		}
		size = readVarint(reader);
	}
}

void readString(ContentReader & reader, char * str, int size) {
	reader.prepare_read(size);
	memcpy(str, reader.content + reader.cursor, size);
	str[size] = 0;
}

}

