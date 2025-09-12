#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "formula.h"

#define PARENTSOCKET 0
#define CHILDSOCKET 1

void populate_buffer(char* msg, int n)
{
    int i;
    for (i=0;i<n;i++) {
        msg[i] = 'a';
    }
}

// computes & returns latency to send msg
// from parent process to child process
double message_latency(
    char* rbuffer,
    char* wbuffer,
    int size)
{
    struct timespec tstart={0,0}, tend={0,0};
    double send_latency = 0.0;                  // return var

    //PF_LOCAL=local socket, no networking
    //SOCK_STREAM=bidirectional byte stream communication (like TCP)
    //0=default protocol
    //fd=file descriptors managed by kernel
    int fd[2];
    socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);
    
    pid_t pid;                                  // pid for fork
    pid = fork();

    if (pid == 0) { // child
        // receive message from parent, then echo back
        // read message from parent
        close(fd[PARENTSOCKET]);
        int n = read(fd[CHILDSOCKET], rbuffer, size);
        //printf("child received ’%.*s’\n", n, rbuffer); //for testing purposes
        // echo back
        write(fd[CHILDSOCKET], rbuffer, n);
        close(fd[CHILDSOCKET]);
        _exit(0); // end child process
    }
    else { // parent
        close(fd[CHILDSOCKET]);
        clock_gettime(CLOCK_MONOTONIC, &tstart);        // start timer
        write(fd[PARENTSOCKET], wbuffer, size);         // write to parent socket
        int n = read(fd[PARENTSOCKET], rbuffer, size);  // read back msg from child
        clock_gettime(CLOCK_MONOTONIC, &tend);          // end timer
        close(fd[PARENTSOCKET]);

        // time
        double total_latency =
            (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
            ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
        //printf("Total latency from parent over 1000 iterations: %f\n", total_latency);
        send_latency = total_latency / 2.0;
        //printf("send latency: %.9f\n", send_latency);
    }

    //printf("send latency before return: %.9f\n", send_latency);
    return send_latency;
}

double throughput(
        char* rbuffer,
        char* sbuffer,
        int size)
{
    struct timespec tstart={0,0}, tend={0,0};
    double throughput = 0.0;                  // return var

    //PF_LOCAL=local socket, no networking
    //SOCK_STREAM=bidirectional byte stream communication (like TCP)
    //0=default protocol
    //fd=file descriptors managed by kernel
    int fd[2];
    socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);
    
    pid_t pid;                                  // pid for fork
    pid = fork();

    if (pid == 0) { // child 
        close(fd[PARENTSOCKET]);
        while (read(fd[CHILDSOCKET], rbuffer, size) > 0) {
            // drain
            asm("nop");
        }
        close(fd[CHILDSOCKET]);
        _exit(0);
    }
    else {  // parent
        //TODO: current code might not be processing partial writes
        close(fd[CHILDSOCKET]);
        clock_gettime(CLOCK_MONOTONIC, &tstart);        // start timer
        write(fd[PARENTSOCKET], sbuffer, size);         // write to parent socket 
        clock_gettime(CLOCK_MONOTONIC, &tend);          // end timer
        close(fd[PARENTSOCKET]);

        // time
        double total_time =
            (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
            ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
        //printf("Total latency from parent over 1000 iterations: %f\n", total_latency);
        throughput = (double)size / total_time;
    }
    //printf("throughput before returning: %.9f\n", throughput);
    return throughput;
}

static const int TOTAL_ITERATIONS = 105;
static const int WARMUP_ITERATIONS = 5;

int main(int argc, char** argv)
{
    if (argc != 3) return 1;
    int n = atoi(argv[1]);              // length of message
    int size = n*sizeof(char);          // size of message in bytes 
    char *rbuffer = malloc(size);       // buffer for receiving message
    char *sbuffer = malloc(size);       // buffer for sending message
    
    double *times = malloc(TOTAL_ITERATIONS*sizeof(double));
    populate_buffer(sbuffer, n);        // populates send buffer (see function)
    
    int state = atoi(argv[2]);          // benchmark to run: 0=message_latency; 1=throughput

    if (state == 0) {
        printf("Starting message latency test...\n");
        double send_latency;
        int i;
        for (i=0;i<TOTAL_ITERATIONS;i++) {
            times[i] = message_latency(rbuffer, sbuffer, size);
        }
        for (i=WARMUP_ITERATIONS;i<TOTAL_ITERATIONS;i++) {
            send_latency += times[i];
        }
        double avg_latency = send_latency / (double)(TOTAL_ITERATIONS-WARMUP_ITERATIONS);
        double stddev = find_stddev(
                &times[WARMUP_ITERATIONS],
                (TOTAL_ITERATIONS-WARMUP_ITERATIONS),
                avg_latency);
        //print
        printf("Total Iterations: %d, Warmup Iterations: %d, Message Size in Bytes: %d\n",
                TOTAL_ITERATIONS,
                WARMUP_ITERATIONS,
                size);
        printf("Avg Latency: %.9f, stddev: %.9f from iteration %d to iteration %d\n\n",
                avg_latency,
                stddev,
                WARMUP_ITERATIONS,
                TOTAL_ITERATIONS);
        printf("Time collected at all iterations:\n");
        for (i=0;i<TOTAL_ITERATIONS;i++) {
            printf("i: %d,\t%.9f\n", i, times[i]);
        }
    }

    if (state == 1) {
        printf("Starting throughput test...\n");
        double sum_throughput;
        int i;
        for (i=0;i<TOTAL_ITERATIONS;i++) {
            times[i] = throughput(rbuffer, sbuffer, size);
        }
        for (i=WARMUP_ITERATIONS;i<TOTAL_ITERATIONS;i++) {
            sum_throughput += times[i];
        }
        // avg
        double avg_throughput = sum_throughput / (double)(TOTAL_ITERATIONS-WARMUP_ITERATIONS);
        double stddev = find_stddev(
                &times[WARMUP_ITERATIONS],
                (TOTAL_ITERATIONS-WARMUP_ITERATIONS),
                avg_throughput);
        // print
        printf("Total Iterations: %d, Warmup Iterations: %d, Message Size in Bytes: %d\n",
                TOTAL_ITERATIONS,
                WARMUP_ITERATIONS,
                size);
        printf("Average throughput: %.9f, stddev: %.9f from iteration %d to iteration %d\n",
                avg_throughput,
                stddev,
                WARMUP_ITERATIONS,
                TOTAL_ITERATIONS);
        printf("Time collected at all iterations:\n");
        for (i=0;i<TOTAL_ITERATIONS;i++) {
            printf("i: %d,\t%.9f\n", i, times[i]);
        }
    }

    printf("\n");
    return 0;
}
