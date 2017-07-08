// COP4610 – Principles of Operating Systems – Summer C 2017
// Prof. Jose F. Osorio
// Student: Alicia Rodriguez – 5162522
//
// Assignment#: 3
// Project: Multithreaded Programming
// Specs: Work Sharing
// Due Date: 07/09/2017 by 11:55pm
//
// Module Name: Work Sharing
// I Certify that this program code has been written by me
// and no part of it has been taken from any sources.

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNKSIZE    10
#define N            100

int main (int argc, char *argv[]) {
  int nthreads, tid, i, chunk;
  double start_time, end_time, execution_time;
  float a[N], b[N], c[N];

  for (i=0; i < N; i++) {
    a[i] = b[i] = i * 1.0; // initialize arrays
  }

  chunk = CHUNKSIZE;

  #pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,tid)
  {
    tid = omp_get_thread_num();

    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }

    printf("Thread %d starting...\n",tid);
    start_time = omp_get_wtime();
    printf("Starting at %d seconds...\n", start_time);

    #pragma omp for schedule(static,chunk)
    for (i=0; i<N; i++) {
      c[i] = a[i] + b[i];
      printf("Thread %d: c[%d]= %f\n",tid,i,c[i]);
    }

    end_time = omp_get_wtime();
    printf("Thread %d, ending at %d seconds...\n", tid, end_time);

    execution_time = end_time - start_time;
    printf("Execution time for Thread %d is %d seconds.\n", tid, execution_time);

  } /* end of parallel section */

}
