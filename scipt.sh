#!/bin/bash
#SBATCH -p main       # Queue name
#SBATCH -n256         # Number of processes
module load openmpi
mpicc -o mergeSort mergeSort.c
mpirun -np 256 mergeSort
