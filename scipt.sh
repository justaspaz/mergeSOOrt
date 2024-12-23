#!/bin/bash
#SBATCH -p main       # Queue name
#SBATCH -n32         # Number of processes
mpicc -o mergeSort mergeSort.c
mpirun -np 32 mergeSort
