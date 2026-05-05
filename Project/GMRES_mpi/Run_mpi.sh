#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks-per-node=32
#SBATCH --time=0-8:0:0
#SBATCH --qos=instruction
#SBATCH --job-name="2D_Fluid_MPI_GMRES"
#SBATCH --output="log.out"
#SBATCH --error="error.out"
#SBATCH --partition=instruction

# -------------------------------
# Environment setup
# -------------------------------
module purge

module load intel
module load gcc
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
    echo "Running with $cores MPI ranks"

    srun -n $cores $EXE
done