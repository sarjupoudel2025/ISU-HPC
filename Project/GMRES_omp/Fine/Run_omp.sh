#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64
#SBATCH --time=0-8:0:0
#SBATCH --qos=instruction
#SBATCH --job-name="2D_Fluid_OMP_GMRES"
#SBATCH --output="log.out"
#SBATCH --error="error.out"
#SBATCH --partition=instruction

# -------------------------------
# Environment setup
# -------------------------------
module purge
module load intel
module load openmpi

# -------------------------------
# Build
# -------------------------------
make clean
make

# -------------------------------
# Executable
# -------------------------------
EXE=./solve_omp

# -------------------------------
# OpenMP scaling test
# -------------------------------
for threads in 64 32 16 8 4 2 1; do
    echo "===================================="
    echo "Running with $threads OpenMP threads"
    echo "===================================="

    export OMP_NUM_THREADS=$threads
    $EXE $threads
done