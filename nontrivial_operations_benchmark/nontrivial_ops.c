#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>
#include "formula.h"

static char *GET_PID = "getpid()";
static char *FORK = "fork()";
static char *OPEN = "fopen()";
static char *FILE_PATH = "./dummy_file.txt";            // used for open test

static const int TOTAL_ITERATIONS = 105;
static const int WARMUP_ITERATIONS = 5;
// the iterations used to calculate mean & stddev
// is WARMUP_ITERATIONS to TOTAL_ITERATIONS

int main(int argc, char** argv)
{
    if (argc != 2) return 1;
    int state;                                          // 0=getpid(), 1=fork(), 2=fopen()
    state = atoi(argv[1]);
    if (state < 0 || state > 2) return 2;

    struct timespec tstart={0,0}, tend={0,0}; 
    double* times = malloc(TOTAL_ITERATIONS*sizeof(double));

    char* test;                                         // ptr to test name
    int i;

    if (state == 0) {
        test = GET_PID;
        for (i=0;i<TOTAL_ITERATIONS;i++) {
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            getpid();
            clock_gettime(CLOCK_MONOTONIC, &tend);
            times[i] =
                ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
        }
    }

    if (state == 1) {
        test = FORK;
        for (i=0;i<TOTAL_ITERATIONS;i++) {
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            pid_t pid = fork();
            if (pid == -1) exit(1);     // fork failed, exit parent process
            if (pid == 0) {             // child
                _exit(0);               // terminate child
            }
            // only the parent can get this far
            clock_gettime(CLOCK_MONOTONIC, &tend);
            times[i] = 
                ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

            // wait for child to terminate before next iteration
            waitpid(pid, NULL, 0);
        }
    }

    if (state == 2) {
        test = OPEN;
        FILE *fptr;
        for (i=0;i<TOTAL_ITERATIONS;i++) {
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            fptr = fopen(FILE_PATH, "r");
            clock_gettime(CLOCK_MONOTONIC, &tend);
            times[i] =
                ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

            if (fptr == NULL) {
                exit(1);
            }
        }
    }

    // compute average
    double mean = find_mean(&times[WARMUP_ITERATIONS], (TOTAL_ITERATIONS-WARMUP_ITERATIONS));
    
    // compute standard deviation
    double stddev = find_stddev(
            &times[WARMUP_ITERATIONS],
            (TOTAL_ITERATIONS-WARMUP_ITERATIONS),
             mean);
    
    // print
    printf("Total iterations: %d\twarmup iterations %d\n", TOTAL_ITERATIONS, WARMUP_ITERATIONS);
    printf("test: %s\n", test);
    printf("calculated mean: %.9f,\tstddev: %.9f over runs %d to %d\n",
            mean,
            stddev,
            WARMUP_ITERATIONS,
            TOTAL_ITERATIONS);

    printf("Timings over all iterations:\n");
    for (i=0;i<TOTAL_ITERATIONS;i++) {
        printf("i:%d = %.9f\n", i, times[i]);
    }

    free(times);
    printf("\n");
    return 0;
}
