CC = gcc
CFLAGS = -O3
LFLAGS = -lm
OBJECTS = main.o polynomialLL_Lib.o

.PHONY: clean help

output.txt: main.exe
	./main.exe > output.txt

main.exe: $(OBJECTS)
	$(CC) $(CFLAGS) -o main.exe $(OBJECTS) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main.exe output.txt

help:
	@echo "Makefile for compiling and running the polynomial evaluation program."
	@echo "Usage:"
	@echo "  make output.txt   - Compiles the program and generates output.txt"
	@echo "  make clean        - Removes compiled files and output"
	@echo "  make help         - Displays this help message"