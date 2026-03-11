CC = gcc
CFLAGS = -O3
LFLAGS = -lm
OBJECTS = main_Lect15.o matrix.o trimatrix.o FindZero.o GetShift.o Hessenberg.o QRA.o QRdecomp.o

.PHONY: clean help

output.txt: main_Lect15.exe
	./main_Lect15.exe > output.txt

main_Lect15.exe: $(OBJECTS)
	$(CC) $(CFLAGS) -o main_Lect15.exe $(OBJECTS) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main_Lect15.exe output.txt

help:
	@echo "Makefile for compiling and running the bissection method program."
	@echo "Usage:"
	@echo "  make output.txt   - Compiles the program and generates output.txt"
	@echo "  make clean        - Removes compiled files and output"
	@echo "  make help         - Displays this help message"