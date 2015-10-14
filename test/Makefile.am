
TESTS=test_simple_reader test_varint test_write_readed test_helpers

check_PROGRAMS=$(TESTS)

test_simple_reader_SOURCES=test_simple_reader.cpp ../src/orientc_reader.cpp ../src/helpers.cpp ../src/parse_exception.cpp
test_simple_reader_CFLAGS =
test_simple_reader_LDADD = 


test_varint_SOURCES=test_varint.cpp ../src/helpers.cpp ../src/parse_exception.cpp
test_varint_CFLAGS =
test_varint_LDADD = 



test_write_readed_SOURCES=test_write_read.cpp ../src/helpers.cpp ../src/orientc_reader.cpp ../src/orientc_writer.cpp ../src/parse_exception.cpp
test_write_readed_CFLAGS =
test_write_readed_LDADD = 

test_helpers_SOURCES=test_helpers.cpp ../src/helpers.cpp ../src/parse_exception.cpp
test_helpers_CFLAGS =
test_helpers_LDADD = 




