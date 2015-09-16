
TESTS=simple_reader test_varint test_write_readed

check_PROGRAMS=$(TESTS)

simple_reader_SOURCES=simple_reader.cpp ../src/orientc_reader.cpp ../src/helpers.cpp ../src/parse_exception.cpp
simple_reader_CFLAGS =
simple_reader_LDADD = 


test_varint_SOURCES=test_varint.cpp ../src/helpers.cpp ../src/parse_exception.cpp
test_varint_CFLAGS =
test_varint_LDADD = 



test_write_readed_SOURCES=write_read.cpp ../src/helpers.cpp ../src/orientc_reader.cpp ../src/orientc_writer.cpp ../src/parse_exception.cpp
test_write_readed_CFLAGS =
test_write_readed_LDADD = 


