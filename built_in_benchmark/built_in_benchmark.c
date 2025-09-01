#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    int n = atoi(argv[1]);

    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    
    for(int k = 0; k < n; k++) {
        asm("nop");
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);
    
    printf("loop took about %.9f seconds\n",
    ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
    ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    return 0;
}
