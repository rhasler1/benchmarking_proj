#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PARENTSOCKET 0
#define CHILDSOCKET 1

void fill_buffer(char* msg, int n)
{
    int i;
    for (i=0;i<n;i++) {
        msg[i] = 'a';
    }
}

int main(int argc, char** argv) {
    int fd[2];
    pid_t pid;

    if (argc != 2) return 1;

    struct timespec tstart={0,0}, tend={0,0};

    int n;                          // length of message
    n = atoi(argv[1]);

    int size;                       // size of message in bytes
    size = n*sizeof(char);

    char *rbuffer = malloc(size);   // buffer for receiving message
    char *sbuffer = malloc(size);   // buffer for sending message

    fill_buffer(sbuffer, n);

    // start timer
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    /* 1. call socketpair ... */
    socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);
    
    /* 2. call fork ... */
    pid = fork();
    if (pid == 0) { /* 2.1 if fork returned zero, you are the child */
        close(fd[PARENTSOCKET]); /* Close the parent file descriptor */
        write(fd[CHILDSOCKET], sbuffer, size); /* NB. this includes nul */
    }
    else { /* 2.2 ... you are the parent */
        char *rbuffer;
        close(fd[CHILDSOCKET]); /* Close the child file descriptor */
        int n = read(fd[PARENTSOCKET], rbuffer, size);

        // mark time
        clock_gettime(CLOCK_MONOTONIC, &tend);

        printf("parent received ’%.*s’\n", n, rbuffer);
    }


    return 0;
}
