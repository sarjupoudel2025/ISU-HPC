#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0-8:0:0
#SBATCH --qos=instruction
#SBATCH --job-name="SERIAL_2D_Fluid_GMRES"
#SBATCH --output="serial_log.out"
#SBATCH --error="serial_error.out"
#SBATCH --partition=instruction

# -------------------------------
# Environment setup
# -------------------------------
module purge
module load intel
module load gcc
# -------------------------------
# Build
# -------------------------------
make clean
make

# -------------------------------
# Executable
# -------------------------------
EXE=./solve_serial

# -------------------------------
# Run serial case
# -------------------------------
echo "Running SERIAL simulation..."
$EXE
