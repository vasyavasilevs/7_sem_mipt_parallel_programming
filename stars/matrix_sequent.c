#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 1024

int A[n][n], B[n][n], C[n][n];
int i = 0, j = 0, k = 0;

int main(int argc, char *argv[]) {

    struct timespec end, start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++) {
            A[i][j] = (int)rand()/ (int)RAND_MAX;
	        B[i][j] = (int)rand()/ (int)RAND_MAX;
	        C[i][j] = 0;
        }

    //transpose
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++) {
            int tmp = B[j][i];
            B[j][i] = B[i][j];
            B[i][j] = tmp;
        }

    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            for(k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[j][k];

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    unsigned long int delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("Elapsed time for the whole program %lu us\n", delta_us);

    return EXIT_SUCCESS;
}
