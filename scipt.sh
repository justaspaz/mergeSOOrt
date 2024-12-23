#!/bin/bash
#SBATCH -p main       # Queue name
#SBATCH -n256         # Number of processes
module load openmpi
# Compile the program
mpicc -o mergeSort mergeSort.c

# Run the program with increasing number of processes
mpirun -np 256 mergeSort
