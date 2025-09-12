#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "formula.h"

static const int ITERATIONS = 20;

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    long n = atol(argv[1]);                                      // loop length
    double* times = malloc(ITERATIONS*sizeof(double));          // store for times across runs
    struct timespec tstart={0,0}, tend={0,0};                   // struct for interacting with clock_gettime
    
    int i;
    long k;
    for (i=0;i<ITERATIONS;i++) {
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        for (k=0;k<n;k++) {
            asm("nop");
        }
        clock_gettime(CLOCK_MONOTONIC, &tend);
        
        double time_delta =
            ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
            ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
        
        times[i] = time_delta;
    }

    // compute average
    double mean = find_mean(&times[0], ITERATIONS);
    double stddev = find_stddev(&times[0], ITERATIONS, mean);
    // print
    printf("loop length: %ld,\tmean: %.9f,\tstddev: %.9f\n",
            n,
            mean,
            stddev);
    // individual times
    printf("\n\n");
    for (i=0;i<ITERATIONS;i++) {
        printf("i: %d,\t%.9f\n", i, times[i]);
    }

    printf("\n");
    return 0;
}
