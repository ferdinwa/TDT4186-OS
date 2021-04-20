#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int bytes_recieved = 0;

static void signal_handler(int signal_sum) {
    printf("Bandwidth %d\n", bytes_recieved);
    bytes_recieved = 0;
    alarm(1);
}

int main(int argc, char **argv) { 
    //The array pipefd is used to return two file descriptors referring to the ends of the pipe.
    //pipefd[0] refers to the read end of the pipe.  
    //pipefd[1] refers to the write end of the pipe.
    int pipefd[2];
    pid_t pid;
    int received = 0;
    signal(SIGALRM, signal_handler);

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <block size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int *buf = malloc(atoi(argv[1]));

    //If error in pipe
    if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    alarm(1);
    pid = fork();

    //If error in fork
    if(pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
        }
    int size = atoi(argv[1]);
    while(1) { 
        //in child process
        if(pid == 0) {
            //errorcheck handling on write, and write argv[1] to pipe
            if(write(pipefd[1], buf, size) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }    
        }
        //in the parent process
        else {
            int returned;
            returned = read(pipefd[0], buf, size);
            if(returned == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            //cumulative number of received bytes
           
            received += returned;
            bytes_recieved += returned;

        } 
    }
}