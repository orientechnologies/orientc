
TESTS=simple_reader test_varint

check_PROGRAMS=$(TESTS)

simple_reader_SOURCES=simple_reader.cpp ../src/orientc_reader.cpp ../src/helpers.cpp
simple_reader_CFLAGS = @CHECK_CFLAGS@
simple_reader_LDADD = @CHECK_LIBS@


test_varint_SOURCES=test_varint.cpp ../src/helpers.cpp
test_varint_CFLAGS = @CHECK_CFLAGS@
test_varint_LDADD = @CHECK_LIBS@



