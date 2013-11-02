CC=mpicc
CFLAGS = -Wall -g -lm

SOURCES = main.c
HEADERS = read_matrix.h

main: $(SOURCES) $(HEADERS)
	${CC} ${CFLAGS} $(SOURCES) -I . -o main
clean:
	rm -f main
