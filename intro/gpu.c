#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#define MAX_NUM 100000

//tested with icx intel compiler and Intel® Iris® Xe graphics gpu
//run with icx gpu.c -fiopenmp -fopenmp-targets=spir64 -o gpu

int main(int argc, char *argv[]) {
    int num_of_terms = 0, num_of_threads = 0, i = 0;
    int arr[MAX_NUM] = {0};
    float average = 0.0, total_sum = 0.0;

    struct timespec end, start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    int num_devices = omp_get_num_devices();
    printf("Number of available devices %d\n", num_devices);

    if (argc < 3) {
        printf("Too few arguments. Usage: %s <number of threads> <number of terms>\n", argv[0]);
        return 1;
    } else if (!(num_of_threads = atoi(argv[1]))) {
        printf("Invalid argument for number of threads\n");
        return 1;
    } else if (!(num_of_terms = atoi(argv[2]))) {
        printf("Invalid argument for number of terms\n");
        return 1;
    } else {
        for (i = 0; i < num_of_terms; i++) {
            arr[i] = i + 1;
        }
    }

    omp_set_num_threads(num_of_threads);

    double start_time = 0.0, end_time = 0.0, elapsed_time = 0.0;

    start_time = omp_get_wtime(); 

    #pragma omp target teams distribute parallel for reduction(+:total_sum) map(to: arr[:MAX_NUM]) \
                                                                            map(tofrom: total_sum)
        for (i = 0; i < num_of_terms; i++) {
            total_sum += arr[i];
        }

    average = total_sum / num_of_terms;
    
    printf("Average = %lf\n", average);

    end_time = omp_get_wtime();

    printf("Elapsed time ~ %lf * 10^(-6) seconds (~us)\n", (end_time - start_time) * 1000000);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    unsigned long int delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("Elapsed time for the whole program %lu us\n", delta_us);

    return EXIT_SUCCESS;
}

