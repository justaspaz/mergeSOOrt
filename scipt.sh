#!/bin/bash
#SBATCH -p main       # Queue name
#SBATCH -n256         # Number of processes

# Compile the program
mpicc -o mergeSort mergeSort.c

# Run the program with increasing number of processes
mpirun -np 1 mergeSort
mpirun -np 2 mergeSort
mpirun -np 4 mergeSort
mpirun -np 8 mergeSort
mpirun -np 16 mergeSort
mpirun -np 32 mergeSort
mpirun -np 64 mergeSort
mpirun -np 128 mergeSort
mpirun -np 256 mergeSort
