#include "helpers.h"
#include <iostream>

namespace Orient {

ContentBuffer::ContentBuffer() :
		content(0), size(0), cursor(0), prepared(0) {

}

ContentBuffer::ContentBuffer(char * content, const int content_size) :
		content(content), size(content_size), cursor(0), prepared(0) {
}

void ContentBuffer::prepare(int next) {
	if (next > this->size)
		throw " out of content size";
	cursor = prepared;
	prepared += next;
	//std::cout << "cursor:" << cursor << " prepared:" << prepared << " char:" << std::hex << (int) content[cursor] << std::dec << "\n";
}

void ContentBuffer::force_cursor(int position) {
	if (position > this->size)
		throw " out of content size";
	cursor = position;
	prepared = position;
}

long long readVarint(ContentBuffer &reader) {
	long long value = 0;
	int i = 0;
	unsigned long long b;
	do {
		reader.prepare(1);
		b = reader.content[reader.cursor];
		if ((b & 0x80) != 0) {
			value |= ((b & 0x7F) << i);
			i += 7;
			if (i > 63) {
				throw "Variable length quantity is too long (must be <= 63)";
			}
		}
	} while ((b & 0x80) != 0);
	value |= b << i;
	long long temp = ((((long long) value << 63) >> 63) ^ (long long) value) >> 1;
	return temp ^ (value & ((long long) 1 << 63));
}

void writeVarint(ContentBuffer &reader, long long value) {
	unsigned long long realValue = (value << (long long) 1) ^ (value >> (long long) 63);
	while ((realValue & 0xFFFFFFFFFFFFFF80) != 0) {
		reader.prepare(1);
		reader.content[reader.cursor] = (unsigned char) ((realValue & 0x7F) | 0x80);
		realValue >>= 7;
	}
	reader.prepare(1);
	reader.content[reader.cursor] = (unsigned char) ((realValue & 0x7F));
}

}
