#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 4
int main() {
    omp_set_num_threads(MAX_THREADS);
    int shared_var = 8;
    printf("Init value of shared_var = %d\n", shared_var);
    int i = 0;

    #pragma omp parallel shared(shared_var)
    {   
        #pragma omp for ordered
        for (i = 0; i < MAX_THREADS; i++) {
            #pragma omp ordered
            shared_var++;
            printf("Thread %d has caught shared_var and has incremented it by 1, shared_var = %d\n", omp_get_thread_num(), shared_var);
        }
    }
    return 0;
}