/*
 * test_reader_listener.h
 *
 *  Created on: 23 Jul 2015
 *      Author: tglman
 */

#ifndef TEST_TEST_READER_LISTENER_H_
#define TEST_TEST_READER_LISTENER_H_
#include "../src/orientc_reader.h"

using namespace Orient;

class TrackerListener: public RecordParseListener {
public:
	virtual void startDocument(char * name) {
		this->class_name = strdup(name);
	}
	virtual void endDocument() {

	}
	virtual void startField(const char * name, OType type) {
		this->field_count++;
		this->balanced_count++;
	}
	virtual void endField(const char * name) {
		this->balanced_count--;
	}
	virtual void stringValue(char * value) {
		a_string_value = strdup(value);
	}
	virtual void intValue(long value) {
		integer_value = value;
	}
	virtual void longValue(long long value) {
		long_value =value;
	}
	virtual void shortValue(short value) {
		short_value =value;
	}
	virtual void byteValue(char value) {
		byte_value = value;
	}
	virtual void booleanValue(bool value) {
		boolean_value =value;
	}
	virtual void floatValue(float value) {
		float_value =value;
	}
	virtual void doubleValue(double value) {
		double_value = value;
	}
	virtual void binaryValue(char * value, int length) {
		binary_value = (char *)malloc(length);
		memcpy(binary_value,value,length);
		binary_size = length;
	}
	virtual void dateValue(long long value) {
		date_value =value;
	}
	virtual void dateTimeValue(long long value) {
		date_time_value = value;
	}
	virtual void linkValue(struct Link &value) {
		link_value.cluster =value.cluster;
		link_value.position=value.position;
	}

	virtual void startCollection(int size) {
		collectionSize = size;
	}
	virtual void startMap(int size) {}
	virtual void mapKey(char *size) {}
	virtual void endMap() {}
	virtual void endCollection() {}

	int field_count;
	int balanced_count;
	char * class_name;
	char * a_string_value;
	long integer_value;
	bool boolean_value;
	long long long_value;
	short short_value;
	char byte_value;
	float float_value;
	double double_value;
	char * binary_value;
	int binary_size;
	long long date_value;
	long long date_time_value;
	struct Link link_value;
	int collectionSize;
	TrackerListener() :
	field_count(0), balanced_count(0), class_name(0), a_string_value(0), integer_value(0),double_value(0),binary_value(0),collectionSize(0) {
	}
	~TrackerListener() {
		free((void *)class_name);
		free((void *)a_string_value);
		free((void *)binary_value);
	}

};
#endif /* TEST_TEST_READER_LISTENER_H_ */
