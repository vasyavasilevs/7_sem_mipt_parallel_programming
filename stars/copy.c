#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int for_copyin1 = 8;
int for_copyprivate = 0;
int for_copyin2 = 8;

int main() {
    
    omp_set_num_threads(4);

    #pragma omp threadprivate(for_copyin1, for_copyin2)

    printf("Before parallel region, for_copyin1 = %d\n", for_copyin1);
    printf("Before parallel region, for_copyin2 = %d\n", for_copyin2);

    for_copyin1 = 88;
    for_copyin2 = 88;
    printf("Values of variables has changed to 88\n\n");

    // 2 The copyin clause is used to initialize threadprivate data upon entry to a parallel region. The
    // 3 value of the threadprivate variable in the primary thread is copied to the threadprivate variable of
    // 4 each other team member.
    printf("copyin(for_copyin1)\n");
    #pragma omp parallel copyin(for_copyin1)
    {
        printf("Thread %d: for_copyin1 = %d\n", omp_get_thread_num(), for_copyin1);
        printf("Thread %d: for_copyin2 = %d\n", omp_get_thread_num(), for_copyin2);
        #pragma omp barrier
    }

    printf("\n");

    #pragma omp threadprivate(for_copyprivate)
    #pragma omp parallel 
    {
        // The copyprivate clause can be used to broadcast values acquired by a single thread directly to
        // all instances of the private variables in the other threads. 
        #pragma omp single copyprivate(for_copyprivate)
        {
            for_copyprivate += omp_get_thread_num();
            printf("Thread %d has caught for_copyprivate = %d\n", omp_get_thread_num(), for_copyprivate);
        }

        printf("Thread %d: for_copyprivate = %d\n", omp_get_thread_num(), for_copyprivate);
    }

    printf("for_copyprivate from master thread = %d\n", for_copyprivate);

    return 0;
}