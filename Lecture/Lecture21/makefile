# Use MPI compiler
CC = mpicc

# Compiler flags
CFLAGS = -Wall -O2 -std=c99

# Executable name
TARGET = main_Lect21

# Source file
SRC = main_Lect21.c

# Default target: build
all: $(TARGET)

# Compile the MPI program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Run the program with 4 processes (change NP as needed)
run: $(TARGET)
	mpirun -np 4 ./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET)