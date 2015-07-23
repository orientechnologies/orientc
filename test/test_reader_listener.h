/*
 * test_reader_listener.h
 *
 *  Created on: 23 Jul 2015
 *      Author: tglman
 */

#ifndef TEST_TEST_READER_LISTENER_H_
#define TEST_TEST_READER_LISTENER_H_

using namespace Orient;

class TrackerListener: public RecordParseListener {
public:
	virtual void className(char * name) {
		this->class_name = strdup(name);
	}
	virtual void startField(char * name, OType type) {
		this->field_count++;
		this->balanced_count++;
	}
	virtual void endField(char * name) {
		this->balanced_count--;
	}
	virtual void stringValue(char * value) {
		a_string_value = strdup(value);
	}
	virtual void intValue(long value) {
		integer_value = value;
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
	virtual void doubleValue(long double value) {
	}
	virtual void binaryValue(char * value, int length) {
	}
	virtual void dateValue(long long value) {
	}
	virtual void dateTimeValue(long long value) {
	}
	virtual void linkValue(struct Link &value) {
	}
	int field_count;
	int balanced_count;
	char * class_name;
	char * a_string_value;
	long integer_value;
	TrackerListener() :
	field_count(0), balanced_count(0), class_name(0), a_string_value(0), integer_value(0) {
	}

};
#endif /* TEST_TEST_READER_LISTENER_H_ */
