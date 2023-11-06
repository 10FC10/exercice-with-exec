CC = gcc
CFLAGS = -Wall

main: main.c procedure.c header.h
	$(CC) $(CFLAGS) -o main main.c procedure.c

clean:
	rm -f main

