#ifndef TEST_TEST_READER_LISTENER_LINK_LIST_H_
#define TEST_TEST_READER_LISTENER_LINK_LIST_H_
#include "../src/orientc_reader.h"

using namespace Orient;

class LinkListListener: public RecordParseListener {
public:
	virtual void startDocument(char * name) {
	}
	virtual void endDocument() {

	}
	virtual void startField(const char * name, OType type) {
	}
	virtual void endField(const char * name) {
	}
	virtual void stringValue(char * value) {
	}
	virtual void intValue(long value) {
	}
	virtual void longValue(long long value) {
	}
	virtual void shortValue(short value) {
	}
	virtual void byteValue(char value) {
	}
	virtual void booleanValue(bool value) {
	}
	virtual void floatValue(float value) {
	}
	virtual void doubleValue(double value) {
	}
	virtual void binaryValue(char * value, int length) {
	}
	virtual void dateValue(long long value) {
	}
	virtual void dateTimeValue(long long value) {
	}
	virtual void linkValue(struct Link &value) {
		if(value.position == count)
			count++;
	}

	virtual void startCollection(int size) {
		collectionSize =size;
	}
	virtual void startMap(int size) {}
	virtual void mapKey(char *size) {}
	virtual void endMap() {}
	virtual void endCollection() {}

	int collectionSize;
	int count;
	LinkListListener(): collectionSize(0), count(1)
	{
	}
	~LinkListListener() {
	}

};
#endif /* TEST_TEST_READER_LISTENER_LINK_LIST_H_ */
