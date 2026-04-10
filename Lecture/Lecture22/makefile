CC = gcc

CFLAGS = -Wall -O2 -std=c99 -fopenmp
LDFLAGS = -fopenmp -lm

pi_OP: main_Lect22_OP.c
	$(CC) $(CFLAGS) main_Lect22_OP.c -o pi_OP $(LDFLAGS)

run: pi_OP
	./pi_OP 1 100000000
	./pi_OP 2 100000000
	./pi_OP 4 100000000
	./pi_OP 8 100000000
	./pi_OP 12 100000000
	./pi_OP 16 10000000

clean:
	rm -f pi_OP