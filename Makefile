CC=cc
CFLAGS=-c
LDFLAGS=
SOURCES=adios_read_aio.c test_read.c test_write.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=adios_read_aio test_read test_write

all: adios_read_aio test_write test_read
adios_read_aio: adios_read_aio.c
	${CC} -g -c -DH5_USE_16_API $<
	${CC} -g -o adios_read_aio adios_read_aio.o
test_write: test_write.c
	${CC} -g -c -DH5_USE_16_API test_write.c
	${CC} -g -o test_write test_write.o
test_read: test_read.c
	${CC} -g -c -DH5_USE_16_API test_read.c
	${CC} -g -o test_read test_read.o

clean:
	rm -f *.o core.* *~ *err *out
	rm -f adios_read_aio test_write test_read

