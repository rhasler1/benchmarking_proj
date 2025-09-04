#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

static const int ITERATIONS = 20;

int main(int argc, char** argv) {
    if (argc != 2) return 1;

    int n = atoi(argv[1]);                                      // loop length
    double* times = malloc(ITERATIONS*sizeof(double));          // store for times across runs
    struct timespec tstart={0,0}, tend={0,0};                   // struct for interacting with clock_gettime
    
    int i;
    int k;
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
    double sum;
    double avg;
    for (i=0;i<ITERATIONS;i++) {
        sum += times[i]; 
    }
    avg = sum / (double)ITERATIONS;

    // compute standard deviation
    double stddev;
    for (i=0;i<ITERATIONS;i++) {
        stddev += (times[i] - avg) * (times[i] - avg);
    }
    stddev = stddev / (double)ITERATIONS;
    stddev = sqrt(stddev);

    // print
    printf("loop length: %d, mean: %.9f, stddev: %.9f\n",
            n,
            avg,
            stddev);

    return 0;
}
