#include <omp.h>
#include <stdio.h>

int main(void) {
    omp_set_num_threads(4);
    #pragma omp parallel
        printf(
        "Hello, World! from thread № %d, max number of threads is %d\n", 
        omp_get_thread_num(),
        omp_get_max_threads());
    return 0;
}
