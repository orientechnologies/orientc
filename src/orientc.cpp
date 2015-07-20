#include "orientc.h"
#include <malloc.h>
#include <memory.h>
namespace Orient {


class ContentReader{
	public:
	ContentReader(const char * content,const int content_size):content(content),size(content_size),cursor(0),prepared(0){}
	void prepare_read(int next) {
		cursor=prepared;
		prepared+=next;
		if(prepared>this->size)
			throw " out of content size";
	}
	const char *content;
	const int size;
	int cursor;
	int prepared;
};
long long readVarint(ContentReader &reader);
void readString(ContentReader & reader, char * str, int size);

RecordParser::RecordParser(std::string formatter) {
	if(formatter != "ORecordSerializerBinary")
		throw "Formatter not supported";
}

void RecordParser::parse(char * content,int content_size,RecordParseListener &listener) {
	ContentReader reader(content,content_size);
	reader.prepare_read(1);
	if(reader.content[reader.cursor] != 0)
		throw "unsupported version";
	long long class_size = readVarint(reader);
	std::cout<<"size:"<<class_size<<"\n";
	char * class_name = reinterpret_cast<char *>(malloc(class_size));
	readString(reader,class_name,class_size);
	listener.className(class_name);
}


void readString(ContentReader & reader, char * str, int size){
	reader.prepare_read(size);
	memcpy(str,reader.content+reader.cursor,size);
	str[size]=0;
}


long long readVarint(ContentReader &reader) {
	long long value = 0;
	int i = 0;
	char b;
	do
	{
		reader.prepare_read(1);
		b = reader.content[reader.cursor];
		value |= (b & 0x7F) << i;
		i += 7;
		if (i > 63)
			throw "Variable length quantity is too long (must be <= 63)";
	} while((b &  0x80L) != 0);

	long long temp = (((value << 63) >> 63) ^ value) >> 1;

    	return temp ^ (value & (1L << 63));
}


}



