/*

This program will numerically compute the integral of

                  4/(1+x*x)

from 0 to 1.  The value of this integral is pi -- which
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/

#include <stdio.h>
#include <omp.h>
#include <string.h>

#define NUM_THREADS 1

static long num_steps = 300000000;
// static long num_steps = 10;

int main()
{
    for (int th = 1; th < 5; th++)
    {

        omp_set_num_threads(th);
        double pi = 0.0;
        double start_time, run_time;

        int actual_Threads;

        double checkpoints[th];
        memset(checkpoints, 0.0, sizeof(checkpoints));

        double step = 1.0 / (double)num_steps;

        start_time = omp_get_wtime();

#pragma omp parallel
        {
            int currentThread = omp_get_thread_num();

            double x;
            double sum = 0.0;

            for (int i = currentThread; i < num_steps; i = i + th)
            {
                x = (i - 0.5) * step;
                sum += 4.0 / (1.0 + x * x);
            }

            sum = sum * step; // atomic
#pragma omp atomic
            pi += sum;

            // #pragma omp critical
            //             pi += sum * step;
        }

        // for (int i = 0; i < th; i++)
        // {
        //     sum = sum + checkpoints[i];
        // }

        // double pi = step * sum;

        run_time = omp_get_wtime() - start_time;

        printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
    }
}
