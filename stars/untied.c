#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define LARGE_NUMBER 10000000
double item[LARGE_NUMBER];
void process(int item) {
    item++;
}
// void process(int thread_num, int shared_var) {
//     shared_var = thread_num;
//     printf("Thread %d processed item\n", thread_num);
// }
int private_var = 8;



int main() {

    // Tasks created with the untied clause are never tied
    // • Allowed to resume at task scheduling points in a different thread
    // • No scheduling restrictions, e.g., can be suspended at any point
    // • Gives more freedom to the implementation, e.g., load balancing

    // Unsafe use of untied Tasks
    // • Problem: Because untied tasks may migrate between threads at any
    // point, thread-centric constructs can yield unexpected results
    // • Remember when using untied tasks:
    // • Avoid threadprivate variables
    // • Avoid any use of thread-ids (i.e., omp_get_thread_num())
    // • Be careful with critical region and locks
    // • Possible solution:
    // • Create a tied task region with
    // #pragma omp task if(0)

    //#pragma omp firstprivate(private_var)

    double start_time, end_time, elapsed_time;

    start_time = omp_get_wtime(); 

    // int shared_var = 0;
    // int task_completed_by_thread = -1;

    // #pragma omp parallel num_threads(8) 
    // {
    //     #pragma omp single
    //     {
    //         printf("Thread %d creating the task\n", omp_get_thread_num());

    //        
    //         #pragma omp task untied
    //         {
    //             printf("Task started by Thread %d\n", omp_get_thread_num());
                
    //             //usleep(1000000); // Задержка в 1 секунду (1 миллион микросекунд)

    //             //shared_var += omp_get_thread_num();
    //             //                 
    //             for (int i = 0; i < LARGE_NUMBER; i++) {
    //          
    //                 #pragma omp task
    //                 process(omp_get_thread_num(), shared_var);
    //             }
    //             //sleep(20);

    //             task_completed_by_thread = omp_get_thread_num();
    //             printf("Task completed by Thread %d\n", task_completed_by_thread);
    //         }

    //         printf("Thread %d continues after the task\n", omp_get_thread_num());
    //     }
    // }

    // //#pragma omp taskwait

    // printf("Final value of shared_var: %d\n", shared_var);
    // printf("Task was completed by Thread %d\n", task_completed_by_thread);
    
    // #pragma omp parallel num_threads(8)
    // {
    //     int i = 0;
    //     #pragma omp single
    //     {
    //         #pragma omp task untied
    //         {
    //             // Unsafe usage: accessing thread-private variable
    //             private_var = omp_get_thread_num(); // Avoid accessing thread-private variables
    //             for (i = 0; i < LARGE_NUMBER; i++)
    //                 //#pragma omp task
    //                     process(omp_get_thread_num());
    //             printf("Thread %d modified private_var\n", omp_get_thread_num());
    //         }
    //     }
    // }

    // printf("Thread-private variable: %d\n", private_var);
    
    // 1 The following example is the same as the previous one, except that the tasks are generated in an
    // 2 untied task. While generating the tasks, the implementation may reach its limit on unassigned tasks.
    // 3 If it does, the implementation is allowed to cause the thread executing the task generating loop to
    // 4 suspend its task at the task scheduling point in the task directive, and start executing unassigned
    // 5 tasks. If that thread begins execution of a task that takes a long time to complete, the other threads
    // 6 may complete all the other tasks before it is finished.
    // 7 In this case, since the loop is in an untied task, any other thread is eligible to resume the task
    // 8 generating loop. In the previous examples, the other threads would be forced to idle until the
    // 9 generating thread finishes its long task, since the task generating loop was in a tied task.

    #pragma omp parallel
    {
        #pragma omp single
        {
            int i = 0;
            #pragma omp task untied
            {
                for (i = 0; i < LARGE_NUMBER; i++)
                    #pragma omp task
                        process(item[i]);
            }
        }
    }


    end_time = omp_get_wtime();

    elapsed_time = end_time - start_time;

    printf("Elapsed time: %f seconds\n", elapsed_time);
    return 0;
}