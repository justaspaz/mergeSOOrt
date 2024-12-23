#!/bin/bash
#SBATCH -p main       # Queue name
#SBATCH -n64         # Number of processes
mpicc -o mergeSort mergeSort.c
mpirun -np 128 mergeSort
