/*

This program will numerically compute the integral of

                  4/(1+x*x)

from 0 to 1.  The value of this integral is pi -- which
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/

/*
  typedef enum omp_sched_t {
    omp_sched_static = 1,
    omp_sched_dynamic = 2,
    omp_sched_guided = 3,
    omp_sched_auto = 4
  } omp_sched_t;
*/

#include <stdio.h>
#include <omp.h>
#include <string.h>

#define NUM_THREADS 1

static long num_steps = 500000000;
// static long num_steps = 10;

int main()
{
    for (int th = 1; th <= 4; th++)
    {
        omp_set_schedule(omp_sched_auto, 1);

        omp_set_num_threads(th);
        int i;
        double x;
        double pi;
        double sum = 0.0;
        double start_time, run_time;

        double step = 1.0 / (double)num_steps;

        start_time = omp_get_wtime();

#pragma omp parallel
        {
            double x;
#pragma omp for reduction(+ \
                          : sum)
            for (i = 1; i <= num_steps; i++)
            {
                x = (i - 0.5) * step;
                sum = sum + 4.0 / (1.0 + x * x);
            }
        }

        pi = step * sum;
        run_time = omp_get_wtime() - start_time;

        omp_sched_t schedule;
        int chunk;
        omp_get_schedule(&schedule, &chunk);

        printf("\n threads: %d, schdule: %d - %d, pi with %ld steps is %lf in %lf seconds\n ", th, schedule, chunk, num_steps, pi, run_time);
    }
}
