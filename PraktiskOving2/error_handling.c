#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>

int main() {

    while(1) {
        int delay; //delay time in seconds
        int pid;   //process ID
        
        //According to Michael Engel on Piazza, it is not problematic to leave out error checking for printf
        //so we skip this for now
        printf("Enter a delay for the alarm: \n");
        int args = scanf("%d", &delay);                    //Will return 1 if scanf successfully read and stored                             
        if (args < 1)                                      //an integer to the variable and 0 if it was unable to.
        {                                                  //Then we check if the input was not an integer and in that case, the character is stored on the buffer.
            char dummy;                                    //We need to remove this character to avoid an infinite loop,
            scanf("%c", &dummy);                           // <-- and this is done here
            printf("Input can not be a character!\n");     // https://www.reddit.com/r/learnprogramming/comments/1d0w4x/c_scanf_d_but_error_if_user_enters_a_character/.
            continue;                                      //Continue statement to avoid creating a child process when input is not correct
        }
        else if (delay < 0) {
            printf("Input cannot be negative!\n");
            continue;                                      //Continue statement to avoid creating a child process when input is not correct
        }
        else {
            int wp;
            do {
                //Uses waitpid to remove processes that have terminated ("zombie-processes")
                wp = waitpid(-1, NULL, WNOHANG);
                printf("\nProcess ID of deceased processes: %d\n", wp);
            } while (wp > 0);


            if (wp == -1){
                perror("Error");   //Displays an error for the case when there are no child processes
            }

            pid = fork(); //create a child process


            if(pid > 0) { //in the parent process
                printf("Child process created. Child process ID:  %d\n", pid);
            }
            else if(pid == 0) { //in the child process
                int sleep_int = sleep(delay);  //Checking if the sleep was interrupted. If it is interrupted by a signal handler            
                if (sleep_int > 0) {           //it will return a positive integer, which indicates the number of seconds left to sleep.          
                    sleep(sleep_int);          //If it was interrupted, we sleep again. 
                }
                //We do not error-check the getpid-function, because it shall always successful. (As per the man page)
                printf("\nALARM: \aRING RING RING - Process ID: %d\n", getpid()); 
                system("ps -e \n");    //Simple way to display the processes status
                exit(0);
            }
            else { //Unsuccessful creation of child process
                perror("");
                exit(EXIT_FAILURE);
            }
        }
    }
    return(0);
}