#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#define n 1024

//TODO: set num of teams, transpose

int A[n][n], B[n][n], C[n][n];
int i = 0, j = 0, k = 0;

int main(int argc, char *argv[]) {

    int num_of_threads = 0;

    if (argc < 2) {
        printf("Too few arguments. Usage: %s <number of threads>\n", argv[0]);
        return 1;
    } else if (!(num_of_threads = atoi(argv[1]))) {
        printf("Invalid argument for number of threads\n");
        return 1;
    } else {
        goto Init;
    }

    Init:

    omp_set_num_threads(num_of_threads);

    int num_devices = omp_get_num_devices();
    printf("Number of available devices %d\n", num_devices);
    
    struct timespec end, start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++) {
            A[i][j] = (int)rand()/ (int)RAND_MAX;
	        B[i][j] = (int)rand()/ (int)RAND_MAX;
	        C[i][j] = 0;
        }

    double start_time = 0.0, end_time = 0.0, elapsed_time = 0.0;
    start_time = omp_get_wtime();

    #pragma omp target map(to: A, B, C, i, j, k)
    #pragma omp teams distribute parallel for private(i,j,k) shared(A,B,C)
    //#pragma omp parallel for private(i,j,k) shared(A,B,C) 
        for(i = 0; i < n; i++) {
            for(k = 0; k < n; k++) { 
                for(j = 0; j < n; j++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

    end_time = omp_get_wtime();

    printf("Elapsed time ~ %lf * 10^(-6) seconds (~us)\n", (end_time - start_time) * 1000000);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    unsigned long int delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("Elapsed time for the whole program %lu us\n", delta_us);

    return EXIT_SUCCESS;
}
