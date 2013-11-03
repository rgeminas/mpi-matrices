CC=mpicc
CFLAGS = -Wall -g -lm -I./src

SOURCES = src/main.c src/read_matrix.c
HEADERS = src/read_matrix.h

main: $(SOURCES) $(HEADERS)
	${CC} ${CFLAGS} $(SOURCES) -o main
clean:
	rm -f main
