CC=cc
CFLAGS=-c
LDFLAGS=
SOURCES=adios_read_aio.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=adios_read_aio

adios_read_aio: adios_read_aio.c
	${CC} -g -c adios_read_aio.c -DH5_USE_16_API $<
	${CC} -g -o adios_read_aio adios_read_aio.o
clean:
	rm -f *.o core.* *~ *err *out
	rm -f adios_read_aio

