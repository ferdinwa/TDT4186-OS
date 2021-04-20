#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>

int main() {

    while(1) {
        int delay; //delay time in seconds
        int pid;   //process ID

        printf("Enter a delay for the alarm: \n");
        scanf("%d", &delay); //Scans the input from the user as an integer, and stores it in the location of the pointer

        pid = fork(); //create a child process

        if(pid > 0) { //in the parent process
            printf("Child process created. Child process ID:  %d\n", pid);
        }
        else if(pid == 0) { //in the child process
            sleep(delay);
            printf("Alarm: \aRING RING RING \n Process ID: %d\n", getpid());
            exit(0);
        }
        else { //Unsuccessful creation of child process
            printf("An error occured");
        }
    }
    return(0);
}