#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int num_of_terms = 0, num_of_threads = 0, i = 0;
    double total_sum = 0.0;

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

        #pragma omp parallel for reduction(+:total_sum)
            for (i = 1; i <= num_of_terms; i++) {
                total_sum += 1.0 / i;
            }

        printf("Total sum = %f\n", total_sum);

        return 0;
    }
}
