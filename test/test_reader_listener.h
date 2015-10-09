#ifndef TEST_TEST_READER_LISTENER_H_
#define TEST_TEST_READER_LISTENER_H_
#include "../src/orientc_reader.h"
#include <string.h>
#include <sstream>
#include <list>
#include <set>
#include <map>
using namespace Orient;

class TrackerListener: public RecordParseListener {
public:
	virtual void startDocument(const char * name, size_t class_name_length) {
		if(this->class_name != 0) free(this->class_name);
		this->class_name = strndup(name,class_name_length);
		startDocumentCount++;
	}
	virtual void endDocument() {

	}
	virtual void startField(const char * name,size_t name_length, OType type) {
		this->field_count++;
		this->balanced_count++;
	}
	virtual void endField(const char * name,size_t name_length) {
		this->balanced_count--;
	}
	virtual void stringValue( const char * value, size_t value_lenght) {
		if(a_string_value != 0) free(a_string_value);
		a_string_value = strndup(value,value_lenght);
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
	virtual void binaryValue(const char * value, int length) {
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

	virtual void startCollection(int size,OType type) {
		collectionSize = size;
	}
	virtual void startMap(int size,OType type) {
		mapSize = size;
	}
	virtual void mapKey(const char *key ,size_t key_length) {
		std::stringstream ss;
		ss<<"key"<<mapCount;
		assert(std::string(key,key_length) == ss.str());
		mapCount++;
	}
	virtual void endMap(OType type) {}
	virtual void endCollection(OType type) {}
	virtual void ridBagTreeKey(long long fileId,long long pageIndex,long pageOffset) {
		this->fileId =fileId;
		this->pageIndex= pageIndex;
		this->pageOffset = pageOffset;
	}
	virtual void nullValue(){
		nullRead =true;
	}

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
	int mapSize;
	int mapCount;
	int startDocumentCount;
	long long fileId;
	long long pageIndex;
	long pageOffset;
	bool nullRead;
	TrackerListener() :
	field_count(0), balanced_count(0), class_name(0), a_string_value(0), integer_value(0),double_value(0),binary_value(0),collectionSize(0),mapSize(0),mapCount(0),startDocumentCount(0),fileId(0),pageIndex(0),pageOffset(0),nullRead(false) {
	}
	~TrackerListener() {
		free((void *)class_name);
		free((void *)a_string_value);
		free((void *)binary_value);
	}

};

class SimpleTrackerListener: public RecordParseListener {
public:
	virtual void startDocument(const char * name, size_t class_name_length) {
		if(this->class_name != 0) free(this->class_name);
		this->class_name = strndup(name,class_name_length);
		startDocumentCount++;
	}
	virtual void endDocument() {

	}
	virtual void startField(const char * name,size_t name_length, OType type) {
		this->field_count++;
		this->balanced_count++;
		if(type != EMBEDDEDLIST && type != EMBEDDEDMAP && type != EMBEDDEDSET && type != LINKSET && type != LINKMAP && type != LINKLIST && type != LINKBAG)
		this->types.push_front(type);
	}
	virtual void endField(const char * name,size_t name_length) {
		this->balanced_count--;
	}
	virtual void stringValue( const char * value, size_t value_lenght) {
		if(types.front() == EMBEDDEDLIST) {
			embeddedList.push_back(std::string(value,value_lenght));
		} else if( types.front() == EMBEDDEDSET) {
			embeddedSet.insert(std::string(value,value_lenght));
		} else if( types.front() == EMBEDDEDMAP) {
			embeddedMap[key] == std::string(value,value_lenght);
		}
		if(a_string_value != 0) free(a_string_value);
		a_string_value = strndup(value,value_lenght);

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
	virtual void binaryValue(const char * value, int length) {
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
		if(types.front() == LINK) {
			link_value.cluster =value.cluster;
			link_value.position=value.position;
		} else if(types.front() == LINKLIST) {
			linkList.push_back(value);
		} else if(types.front() == LINKSET) {
			linkSet.push_back(value);
		} else if(types.front() == LINKMAP) {
			linkMap[key] = value;
		} else if(types.front() == LINKBAG) {
			embeddedRidbag.push_back(value);
		}
	}

	virtual void startCollection(int size,OType type) {
		collectionSize = size;
		this->types.push_front(type);
	}
	virtual void startMap(int size,OType type) {
		mapSize = size;
		this->types.push_front(type);
	}
	virtual void mapKey(const char *key ,size_t key_length) {
		mapCount++;
		this->key = std::string(key,key_length);
	}
	virtual void endMap(OType type) {}
	virtual void endCollection(OType type) {}
	virtual void ridBagTreeKey(long long fileId,long long pageIndex,long pageOffset) {
		ridbagKey= true;
	}
	virtual void nullValue(){
		nullRead = true;
	}

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
	int mapSize;
	int mapCount;
	int startDocumentCount;
	std::list<OType> types;
	std::list<struct Link> linkList;
	std::list<struct Link> linkSet;
	std::list<struct Link> embeddedRidbag;
	std::map<std::string,struct Link> linkMap;
	std::list<std::string> embeddedList;
	std::set<std::string> embeddedSet;
	std::map<std::string,std::string> embeddedMap;
	std::string key;
	bool ridbagKey;
	bool nullRead;
	SimpleTrackerListener() :
	field_count(0), balanced_count(0), class_name(0), a_string_value(0), integer_value(0),double_value(0),binary_value(0),collectionSize(0),mapSize(0),mapCount(0),startDocumentCount(0) ,ridbagKey(false),nullRead(false) {
	}
	~SimpleTrackerListener() {
		free((void *)class_name);
		free((void *)a_string_value);
		free((void *)binary_value);
	}

};

#endif /* TEST_TEST_READER_LISTENER_H_ */
