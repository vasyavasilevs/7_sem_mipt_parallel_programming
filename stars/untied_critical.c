#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int shared_var = 0;

    #pragma omp parallel num_threads(10)
    {
        #pragma omp single
        {
            printf("Thread %d creating the task\n", omp_get_thread_num());

            #pragma omp task untied
            {
                sleep(20);
                printf("Task started by Thread %d\n", omp_get_thread_num());

                #pragma omp critical
                {
                    shared_var = omp_get_thread_num();
                }

                printf("Task completed by Thread %d\n", omp_get_thread_num());
            }

            printf("Thread %d continues after the task\n", omp_get_thread_num());
        }
    }
    
    #pragma omp taskwait

    printf("Final value of shared_var: %d\n", shared_var);

    return 0;
}
