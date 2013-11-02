CC=mpicc
CFLAGS = -Wall -g -lm

main: main.c
	${CC} ${CFLAGS} main.c -o main
clean:
	rm -f main
