#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int *data = NULL;
    int chunkSize;
    int *localData = NULL;
    int localSize;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double execution_times[9]; // Array to store execution times for powers of 2

    int arraySizes[] = {(1 << 24), (1 << 26), (1 << 28)}; // 2^24, 2^26, 2^28

    for (int arrayIdx = 0; arrayIdx < 3; arrayIdx++) {
        int currentArraySize = arraySizes[arrayIdx];

        if (rank == 0) {
            data = (int *)malloc(sizeof(int) * currentArraySize);
            for (int i = 0; i < currentArraySize; i++) {
                data[i] = rand() % 100;
            }
        }

        double start_time, end_time;
        double local_time = 0.0;
        double max_time = 0.0;

        for (int p = 0; p <= 8; p++) { // 1, 2, 4, ..., 256 processors
            int num_procs = (1 << p);
            if (num_procs > size) break;

            MPI_Barrier(MPI_COMM_WORLD);
            start_time = MPI_Wtime();

            MPI_Comm new_comm;
            MPI_Comm_split(MPI_COMM_WORLD, rank < num_procs, rank, &new_comm);

            if (rank < num_procs) {
                chunkSize = currentArraySize / num_procs;
                localData = (int *)malloc(sizeof(int) * chunkSize);

                MPI_Scatter(data, chunkSize, MPI_INT, localData, chunkSize, MPI_INT, 0, new_comm);

                mergeSort(localData, 0, chunkSize - 1);

                MPI_Gather(localData, chunkSize, MPI_INT, data, chunkSize, MPI_INT, 0, new_comm);

                free(localData);
            }

            MPI_Barrier(MPI_COMM_WORLD);
            end_time = MPI_Wtime();
            local_time = end_time - start_time;

            MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

            if (rank == 0) {
                execution_times[p] = max_time; // Store execution time
                printf("Time taken to sort: %lf seconds\n", max_time);
            }

            MPI_Comm_free(&new_comm);
        }

        if (rank == 0) {
            for (int p = 0; p <= 8; p++) {
                int num_procs = (1 << p);
                if (num_procs > size) break;
                printf("Processors: %d, Time: %lf seconds\n", num_procs, execution_times[p]);
            }

            free(data);
        }
    }

    MPI_Finalize();

    return 0;
}
