#include <stdio.h>
#include <unistd.h>

int main()
{
    while(42) { //Infinite loop. Interestingly this works for any non-zero integer. 
        int delay;
        printf("Enter a delay for the alarm: ");
        scanf("%d", &delay);   //Scans the input from the user as an integer, and stores it in the location of the pointer
        sleep(delay);          //Sleeps for "delay" seconds
        printf("\a*RING* *RING* *RING*. Alarm after %d seconds\n", delay);
    }  
    return 0;
}