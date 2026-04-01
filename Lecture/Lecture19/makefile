CC = gcc

CFLAGS = -Wall -O2 -std=c99 -fopenmp
LDFLAGS = -fopenmp -lm

normalize: main_Lect19.c
	$(CC) $(CFLAGS) main_Lect19.c -o normalize $(LDFLAGS)

run: normalize
	./normalize 1 100000000
	./normalize 2 100000000
	./normalize 4 100000000
	./normalize 8 100000000
	./normalize 16 100000000

clean:
	rm -f normalize