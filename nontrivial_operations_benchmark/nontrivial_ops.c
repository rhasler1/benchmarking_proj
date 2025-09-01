#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

static char *GET_PID = "getpid()";
static char *FORK = "fork()";
static char *OPEN = "fopen()";

static char *FILE_PATH = "./dummy_file.txt";

int main(int argc, char** argv)
{
    if (argc != 2) return 1;

    int state; // 0=getpid(), 1=fork(), 2=fopen()
    state = atoi(argv[1]);
    
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

    if (state == 1) {
        test = FORK;
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        int child_pid = fork();
        if (child_pid == -1) { // fork failed: terminate parent process
            exit(1);
        }

        if (child_pid > 0) { // fork success and is parent: record end time
            clock_gettime(CLOCK_MONOTONIC, &tend);
        }

        if (child_pid == 0) { // fork success and is child: end process
	        exit(1);
	    }
    }

    if (state == 2) {
        test = OPEN;
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        FILE *fptr;
        fptr = fopen(FILE_PATH, "r");
        if (fptr == NULL) { // fopen failed
	        exit(1);
        }

        clock_gettime(CLOCK_MONOTONIC, &tend);
    }

    printf("\n");
    printf("%s took about %.9f seconds\n",
            test,
            ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
            ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec)
            );

    return 0;
}
