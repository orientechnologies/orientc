#ifndef TEST_TEST_READER_LISTENER_MAP_H_
#define TEST_TEST_READER_LISTENER_MAP_H_
#include "../src/orientc_reader.h"

using namespace Orient;

class MapListener: public RecordParseListener {
public:
	virtual void startDocument(const char * name, size_t class_name_length) {
	}
	virtual void endDocument() {

	}
	virtual void startField(const char * name,size_t name_length, OType type) {
	}
	virtual void endField(const char * name,size_t name_length) {
	}
	virtual void stringValue(const char * value,size_t value_lenght) {
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
	virtual void binaryValue(const char * value, int length) {
	}
	virtual void decimalValue(int scale, const char * value, int length) {
	}
	virtual void dateValue(long long value) {
	}
	virtual void dateTimeValue(long long value) {
	}
	virtual void linkValue(struct Link &value) {
		if(value.cluster == count)
		count = value.cluster;
	}

	virtual void startCollection(int size,OType type) {
	}
	virtual void startMap(int size,OType type) {
		mapSize =size;
	}
	virtual void mapKey(const char *key ,size_t key_length) {
		std::string exp("key");
		exp+=count;
		if(std::string(key,key_lenght) == exp)
		count++;
	}
	virtual void endMap(OType type) {}
	virtual void endCollection(OType type) {}
	virtual void ridBagTreeKey(long long fileId,long long pageIndex,long pageOffset){}
	virtual void nullValue(){}
	int mapSize;
	int count;
	MapListener(): mapSize(0), count(0)
	{
	}
	~MapListener() {
	}

};
#endif /* TEST_TEST_READER_LISTENER_MAP_H_ */
