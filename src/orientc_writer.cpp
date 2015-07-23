#include "orientc_writer.h"
#include "helpers.h"

namespace Orient {

RecordWriter::RecordWriter(std::string formatter) :
		reader(new ContentBuffer()) {
	if (formatter != "ORecordSerializerBinary")
		throw "Formatter not supported";
}

void RecordWriter::className(char * name) {

}

}
