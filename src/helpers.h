#include <iostream>

namespace Orient {

class ContentReader {
public:
	ContentReader(char * content, const int content_size);
	void prepare_read(int next);
	char *content;
	const int size;
	int cursor;
	int prepared;
};

long long readVarint(ContentReader &reader);

void writeVarint(ContentReader &reader,long long value);


}  // namespace Orient
