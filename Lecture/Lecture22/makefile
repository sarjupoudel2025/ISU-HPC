# Compilers
CC = gcc
MPICC = mpicc

# Flags
CFLAGS = -Wall -O2 -std=c99
OMPFLAGS = -fopenmp
LDFLAGS = -lm

# ----------------------
# OpenMP build
# ----------------------
pi_OP: main_Lect22_OP.c
	$(CC) $(CFLAGS) $(OMPFLAGS) main_Lect22_OP.c -o pi_OP $(OMPFLAGS) $(LDFLAGS)

# ----------------------
# MPI build
# ----------------------
pi_MPI: main_Lect22_MPI.c
	$(MPICC) $(CFLAGS) main_Lect22_MPI.c -o pi_MPI $(LDFLAGS)

# ----------------------
# Run OpenMP tests
# ----------------------
run_op: pi_OP
	./pi_OP 1 10000000000
	./pi_OP 2 10000000000
	./pi_OP 4 10000000000
	./pi_OP 8 10000000000
	./pi_OP 12 10000000000
	./pi_OP 16 10000000000

# ----------------------
# Run MPI test
# ----------------------
run_mpi: pi_MPI
	mpirun -np 6 ./pi_MPI 10000000000

# ----------------------
# Build everything
# ----------------------
all: pi_OP pi_MPI

# ----------------------
# Clean
# ----------------------
clean:
	rm -f pi_OP pi_MPI