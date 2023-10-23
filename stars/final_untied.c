#include <stdio.h>
#include <omp.h>
#include <unistd.h>

#define LARGE_NUMBER 1000000

extern void process(int task_id, int thead_rank) {
    printf("Task %d is executed by thread: %d\n", task_id, thead_rank);
    sleep(100);
    printf("Task %d is executed by thread: %d\n", task_id, thead_rank);
}

int main(int argc, char* argv[]) {
    omp_set_num_threads(8);
    #pragma omp parallel
    {
        #pragma omp single
        {
            int i = 0;
            #pragma omp task untied
            {
                for (i = 0; i < LARGE_NUMBER; i++) {
                    printf("Task %d was born by thread %d\n", i, omp_get_thread_num());
                    #pragma omp task
                    {
                        process(i, omp_get_thread_num());
                    }
                }
            }
        }
    }

    return 0;
}