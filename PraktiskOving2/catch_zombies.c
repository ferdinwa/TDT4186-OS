#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>

int main() {

    while(1) {
        int delay; //delay time in seconds
        int pid;   //process ID

        printf("Enter a delay for the alarm: \n");
        scanf("%d", &delay);

        int wp;
        do {
            //Uses waitpid to remove processes that have terminated ("zombie-processes")
            //With option "WNOHANG" it keeps the waitpid from blocking
            wp = waitpid(-1, NULL, WNOHANG); 
            printf("\nProcess ID of deceased processes: %d\n", wp);
        } while (wp > 0);

        pid = fork(); //create a child process

        if(pid > 0) { //in the parent process
            printf("Child process created. Child process ID:  %d\n", pid);
        }
        else if(pid == 0) { //in the child process
            sleep(delay);
            printf("\nAlarm: \aRING RING RING - Process ID: %d\n", getpid());
            system("ps -e"); //Simple way to display the processes status
            exit(0);
        }
        else { //Unsuccessful creation of child process
            printf("An error occured");
        }
    }
    return(0);
}