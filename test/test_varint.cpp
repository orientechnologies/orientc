#include "../src/helpers.h"
#include <stdlib.h>
#include <assert.h>
#include <climits>
#include <iostream>

using namespace Orient;

int64_t ser_deser_varint(int64_t number) {
	unsigned char content[1024];
	ContentBuffer reader(content, 1024);
	writeVarint(reader, number);
	reader.prepared = 0;
	reader.cursor = 0;
	return readVarint(reader);
}

void test_write_read_varint() {
	assert(12 == ser_deser_varint(12));
	assert(234 == ser_deser_varint(234));
	assert(43234 == ser_deser_varint(43234));
	assert(46576443234 == ser_deser_varint(46576443234));
	assert(534 == ser_deser_varint(534));
	assert(-534 == ser_deser_varint(-534));
	assert(LLONG_MAX-10 == ser_deser_varint(LLONG_MAX-10));
	assert(LLONG_MAX == ser_deser_varint(LLONG_MAX));
	assert(LLONG_MIN == ser_deser_varint(LLONG_MIN));
}

int main() {
	test_write_read_varint();
	return 0;
}

