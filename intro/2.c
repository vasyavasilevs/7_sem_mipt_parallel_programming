#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

double Sum (int start, int finish, int num_of_terms) {
    double proc_sum = 0;
    double i = start;
    for (i = start; i <= ((finish > num_of_terms) ? num_of_terms : finish); i++) {
        proc_sum += 1/i;
    }
    return proc_sum;
}

int main(int argc, char *argv[]) {
    int comm_size = 0, proc_rank = 0, start = 0, finish = 0, segment_size = 0, num_of_terms = 0, num_of_threads = 0;
    double proc_sum = 0, total_sum = 0, i = 0;

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
    
    omp_set_num_threads(num_of_threads);

    #pragma omp parallel private(proc_rank, segment_size, start, finish, proc_sum)
    {
        comm_size = omp_get_max_threads();
        proc_rank = omp_get_thread_num();

        segment_size = (num_of_terms - 1)/comm_size + 1;
        start = proc_rank * segment_size + 1;
        finish = (proc_rank + 1) * segment_size; 

        proc_sum = Sum(start, finish, num_of_terms);

        #pragma omp critical

        total_sum += proc_sum;
    }

    printf("Total sum = %f\n", total_sum);

    return 0;

    }
}
