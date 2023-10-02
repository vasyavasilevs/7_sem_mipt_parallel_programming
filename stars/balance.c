#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define NUM_ITER 65

int main() { 
    int iter_count[NUM_THREADS] = {0};
    int iter_count_static_1[NUM_THREADS] = {0};
    int iter_count_static_2[NUM_THREADS] = {0};
    int iter_count_dynamic_1[NUM_THREADS] = {0};
    int iter_count_dynamic_2[NUM_THREADS] = {0};
    int iter_count_guided_1[NUM_THREADS] = {0};
    int iter_count_guided_2[NUM_THREADS] = {0};

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel for
        for (int i = 0; i < NUM_ITER; i++) {
            iter_count[omp_get_thread_num()]++;
        }

    #pragma omp parallel for num_threads(NUM_THREADS) schedule(static, 1)
    for (int i = 0; i < NUM_ITER; i++) {
        iter_count_static_1[omp_get_thread_num()]++;
        printf("thread %d operation %d\n", omp_get_thread_num(), i);
    }

    printf("\n");

    #pragma omp parallel for num_threads(NUM_THREADS) schedule(static, 4)
    for (int i = 0; i < NUM_ITER; i++) {
        iter_count_static_2[omp_get_thread_num()]++;
        printf("thread %d operation %d\n", omp_get_thread_num(), i);
    }

    printf("\n");

    #pragma omp parallel for num_threads(NUM_THREADS) schedule(dynamic, 1)
    for (int i = 0; i < NUM_ITER; i++) {
        iter_count_dynamic_1[omp_get_thread_num()]++;
    }

    #pragma omp parallel for num_threads(NUM_THREADS) schedule(dynamic, 2)
    for (int i = 0; i < NUM_ITER; i++) {
        iter_count_dynamic_2[omp_get_thread_num()]++;
    }

    #pragma omp parallel for num_threads(NUM_THREADS) schedule(guided, 1)
    for (int i = 0; i < NUM_ITER; i++) {
        iter_count_guided_1[omp_get_thread_num()]++;
    }

    #pragma omp parallel for num_threads(NUM_THREADS) schedule(guided, 4)
    for (int i = 0; i < NUM_ITER; i++) {
        iter_count_guided_2[omp_get_thread_num()]++;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %d:\n", i);
        printf("Section without schedule: %d iterations\n", iter_count[i]);
        printf("Static 1: %d iterations\n", iter_count_static_1[i]);
        printf("Static 2: %d iterations\n", iter_count_static_2[i]);
        printf("Dynamic 1: %d iterations\n", iter_count_dynamic_1[i]);
        printf("Dynamic 2: %d iterations\n", iter_count_dynamic_2[i]);
        printf("Guided 1: %d iterations\n", iter_count_guided_1[i]);
        printf("Guided 2: %d iterations\n", iter_count_guided_2[i]);
        printf("\n");
    }

    return 0;
}