CC = gcc
CFLAGS = -O3
LFLAGS = -lm
OBJECTS = Main_Lec14.o Matrix_lib_Lec14.o

.PHONY: clean help

output.txt: main_lec14.exe
	./main_lec14.exe > output.txt

main_lec14.exe: $(OBJECTS)
	$(CC) $(CFLAGS) -o main_lec14.exe $(OBJECTS) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main_lec14.exe output.txt

help:
	@echo "Makefile for compiling and running the bissection method program."
	@echo "Usage:"
	@echo "  make output.txt   - Compiles the program and generates output.txt"
	@echo "  make clean        - Removes compiled files and output"
	@echo "  make help         - Displays this help message"