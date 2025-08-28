#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

static const char *GET_PID = "getpid()";

int main(int argc, char** argv)
{
    if (argc != 2) return 1;

    int state = atoi(argv[1]);
    
    if (state < 0 || state > 2) return 2;

    struct timespec tstart={0,0}, tend={0,0};
   
    // ptr to test name
    char* test;

    if (state == 0) {
	test = GET_PID;
        clock_gettime(CLOCK_MONOTONIC, &tstart);
	getpid();
        clock_gettime(CLOCK_MONOTONIC, &tend);
    }

    if (state == 1) {}

    if (state == 2) {}

    printf("\n");
    printf("%s took about %.5f seconds\n", test,
    ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
    ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    return 0;
}
