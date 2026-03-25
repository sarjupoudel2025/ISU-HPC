CC = gcc

CFLAGS = -Wall -O2 -std=c99 -fopenmp
LDFLAGS = -fopenmp

blur: main_Lect17.c
	$(CC) $(CFLAGS) main_Lect17.c -o blur $(LDFLAGS)

run: blur
	./blur 1 5000
	./blur 2 5000
	./blur 4 5000
	./blur 8 5000

clean:
	rm -f blur