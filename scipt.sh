#!/bin/bash
#SBATCH -p main       # Queue name
#SBATCH -n128         # Number of processes
module load openmpi
mpicc -o mergeSort mergeSort.c
mpirun -np 128 mergeSort
