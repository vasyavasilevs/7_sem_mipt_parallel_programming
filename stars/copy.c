#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int for_copyin = 8;
int for_copyprivate = 0;

int main() {
    
    omp_set_num_threads(4);

    // 2 The copyin clause is used to initialize threadprivate data upon entry to a parallel region. The
    // 3 value of the threadprivate variable in the primary thread is copied to the threadprivate variable of
    // 4 each other team member.
    #pragma omp threadprivate(for_copyin)
    #pragma omp parallel copyin(for_copyin) 
    {
        for_copyin += omp_get_thread_num();
        printf("Thread %d: for_copyin = %d\n", omp_get_thread_num(), for_copyin);

        #pragma omp barrier
    }

    printf("for_copyin from master thread = %d\n\n", for_copyin);

    #pragma omp threadprivate(for_copyprivate)
    #pragma omp parallel 
    {
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