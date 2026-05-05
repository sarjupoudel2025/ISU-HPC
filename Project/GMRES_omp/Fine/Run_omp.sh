#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks-per-node=32
#SBATCH --time=0-0:30:0
#SBATCH --qos=instruction
#SBATCH --job-name="heat2d_mpi"
#SBATCH --output="scaling_result.out"
#SBATCH --error="errors_heat2d_mpi.out"
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
EXE=./solve_mpi

# -------------------------------
# Strong scaling (high → low)
# -------------------------------
for cores in 64 32 16 8 4 2 1; do
    echo "===================================="
    echo "Running with $cores MPI ranks"
    echo "===================================="

    srun -n $cores $EXE
done