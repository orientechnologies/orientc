#include <iostream>

namespace Orient {

enum OType {
	BOOLEAN =0,
	INTEGER =1,
	SHORT =2,
	LONG =3,
	FLOAT =4,
	DOUBLE =5,
	DATETIME=6,
	STRING=7,
	BINARY=8,
	EMBEDDED=9,
	EMBEDDEDLIST=10,
	EMBEDDEDSET=11,
	EMBEDDEDMAP=12,
	LINK =13,
	LINKLIST=14,
	LINKSET =15,
	LINKMAP =16,
	BYTE = 17,
	TRANSIENT = 18,
	DATE =19,
	CUSTOM = 20,
	DECIMAL = 21,
	RIDBAG = 22,
	ANY = 23
};
struct Link {
	long cluster;
	long long position;
};

class RecordParseListener {
public:
	virtual void className(char * name)=0;
	virtual void startField(char * name,OType type)=0;
	virtual void endField(char * name)=0;
	virtual void stringValue(char * value)=0;
	virtual void intValue(long value)=0;
	virtual void longValue(long long value)=0;
	virtual void shortValue(short value)=0;
	virtual void byteValue(char value)=0;
	virtual void booleanValue(char value)=0;
	virtual void floatValue(float value)=0;
	virtual void doubleValue(long double value)=0;
	virtual void binaryValue(char * value, int length)=0;
	virtual void dateValue(long long value)=0;
	virtual void dateTimeValue(long long value)=0;
	virtual void linkValue(struct Link value)=0;
	virtual ~RecordParseListener() {}
};

class RecordParser {
public:
	RecordParser(std::string format);
	void parse(char * content,int content_size, RecordParseListener & listener);

};

class RecordWriter {

public:
	void className(char * name);
	void startField(char * name,OType type);
	void endField(char * name);
	void stringValue(char * value);
	void intValue(long value);
	void longValue(long long value);
	void shortValue(short value);
	void byteValue(char value);
	void booleanValue(char value);
	void floatValue(float value);
	void doubleValue(long double value);
	void binaryValue(char * value, int length);
	void dateValue(long long value);
	void dateTimeValue(long long value);
	void linkValue(struct Link value);
};

}

