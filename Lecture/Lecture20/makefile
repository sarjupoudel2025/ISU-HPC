CC = gcc

CFLAGS = -Wall -O2 -std=c99 -fopenmp
LDFLAGS = -fopenmp -lm

adaptivequadrature: main_Lect20.c
	$(CC) $(CFLAGS) main_Lect20.c -o adaptivequadrature $(LDFLAGS)

run: adaptivequadrature
	./adaptivequadrature 1 1e-6
		./adaptivequadrature 2 1e-6
		./adaptivequadrature 4 1e-6
	./adaptivequadrature 8 1e-6
	./adaptivequadrature 12 1e-6
	./adaptivequadrature 16 1e-6

clean:
	rm -f adaptivequadrature