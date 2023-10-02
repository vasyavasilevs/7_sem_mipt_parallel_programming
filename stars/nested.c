#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NESTED 3

int main(int argc, char *argv[]) {

    int thread_print = 0;
    omp_set_max_active_levels(MAX_NESTED);

    //activate nested area
    omp_set_nested(1);

    int total_prints[MAX_NESTED] = {0};

    #pragma omp parallel num_threads(3) shared (total_prints)
    {
        #pragma omp atomic
        total_prints[omp_get_level() - 1]++;

        printf("I am a thread number %d among %d threads on my level %d with team size %d and ansector %d\n", 
            omp_get_thread_num(),
            omp_get_num_threads(),
            omp_get_level(),
            omp_get_team_size(omp_get_level()),
            omp_get_ancestor_thread_num(omp_get_level() - 1));

        #pragma omp barrier
        //#pragma omp critical
        //#pragma omp master
        //printf("Current prints in level %d: %d\n", omp_get_level(), total_prints[omp_get_level() - 1]);

        #pragma omp parallel num_threads(2)
        {
            #pragma omp atomic
            total_prints[omp_get_level() - 1]++;

            printf("I am a thread number %d among %d threads on my level %d with team size %d and ansector %d\n", 
                omp_get_thread_num(),
                omp_get_num_threads(),
                omp_get_level(),
                omp_get_team_size(omp_get_level()),
                omp_get_ancestor_thread_num(omp_get_level() - 1));

            #pragma omp barrier
            //#pragma omp critical
            //#pragma omp master
            //printf("Current prints in level %d: %d\n", omp_get_level(), total_prints[omp_get_level() - 1]);

            #pragma omp parallel num_threads(2)
            {
                #pragma omp atomic
                total_prints[omp_get_level() - 1]++;

                printf("I am a thread number %d among %d threads on my level %d with team size %d and ansector %d\n", 
                    omp_get_thread_num(),
                    omp_get_num_threads(),
                    omp_get_level(),
                    omp_get_team_size(omp_get_level()),
                    omp_get_ancestor_thread_num(omp_get_level() - 1));

                #pragma omp barrier
                //#pragma omp critical
                //#pragma omp master
                //printf("Current prints in level %d: %d\n", omp_get_level(), total_prints[omp_get_level() - 1]);
            }
        }
    }

    #pragma omp master
    {
        printf("\n");
        for (int level = 0; level < MAX_NESTED; level++) {
            printf("Total prints in level %d: %d\n", level + 1, total_prints[level]);
        }
    }

    return 0;
}