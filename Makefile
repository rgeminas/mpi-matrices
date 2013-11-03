CC=mpicc
CFLAGS = -Wall -g -lm -I.

SOURCES = main.c read_matrix.c
HEADERS = read_matrix.h

main: $(SOURCES) $(HEADERS)
	${CC} ${CFLAGS} $(SOURCES) -o main
clean:
	rm -f main
