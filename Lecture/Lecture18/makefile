CC = gcc

CFLAGS = -Wall -O2 -std=c99 -fopenmp
LDFLAGS = -fopenmp -lm

pricecall: main_Lect18.c
	$(CC) $(CFLAGS) main_Lect18.c -o pricecall $(LDFLAGS)

run: pricecall
	./pricecall 1
	./pricecall 2
	./pricecall 4
	./pricecall 8
	./pricecall 16

clean:
	rm -f pricecall