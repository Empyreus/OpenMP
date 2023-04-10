#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define N 1000

int A[N][N];
int B[N][N];
int C[N][N];

int main()
{
    int NUM_THREADS = 1;
    int i, j, k;
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;

    int devices = omp_get_default_device();
    printf("devices: %d \n", devices);

    for (int n = 1; n < 5; n++)
    {
        omp_set_num_threads(n);
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
            {
                A[i][j] = 2;
                B[i][j] = 2;
            }
        gettimeofday(&tv1, &tz);
#pragma omp parallel for private(i, j, k) shared(A, B, C)
        for (i = 0; i < N; ++i)
        {
            for (j = 0; j < N; ++j)
            {
                for (k = 0; k < N; ++k)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        gettimeofday(&tv2, &tz);
        elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
        printf("threads: %d, elapsed time = %f seconds.\n", n, elapsed);
    }

    /*for (i= 0; i< N; i++)
    {
        for (j= 0; j< N; j++)
        {
            printf("%d\t",C[i][j]);
        }
        printf("\n");
    }*/
}
