#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv) { 
    FILE* f;
    int n = 0;
    char terminal_line[100];
    static char * tokens[100];
    static char * filelines[300];

    //This is the case when shellscript is run from the command line (./wish shellscript.sh).
    //Then argc will be 2. 
    if(argc > 1) {
            f = fopen(argv[1], "r"); 
            int i = 0;
            //For each line in file (each command), 
            //we put each line in an array (filelines).
            //Each line is a separate command, which will be tokenized later. 
            while(fgets(terminal_line, sizeof(terminal_line), f)){
                //for the case with only one line in file, when we need to 
                //replace newline-character with '\0'-character.
                if ((i == 0) && (terminal_line[strlen(terminal_line)-1] == '\n')) {
                terminal_line[strlen(terminal_line)-1] = '\0';
                }
                char* line = strdup(terminal_line);
                filelines[i] = line;
                i++;
                }
        }

    //loop for executing commands from user or from shellscript.sh
    while(1) {
        char* input;
        char* output;
        //int to hold position of first encountered "less than" or "greater than" sign
        int crocodile = 1;
        //variable to hold count of tokens
        int m = 0;
        //if only ./wish command is run, and the user is queried for input
        //Then print prompt (wish£), and put command into terminal_line
        if (argc == 1) {
            //prompt
            printf("wish£ ");
            fgets(terminal_line, sizeof(terminal_line), stdin);
            //the last character of terminal_line when using fgets(3) is '\n',
            //and this has to be replaced with '\0' in order to execute the command 
            terminal_line[strlen(terminal_line)-1] = '\0';
        }

        //tokenize commands from file shellscript.sh with use of strtok(3)
        if (argc > 1) {
            char* token = strtok(filelines[n], " ");
            while(token != NULL){
                tokens[m] = token;
                token = strtok(NULL, " ");
                m++;
            }
        }
        //tokenize input from user with use of strtok(3)
        else {
            char* token = strtok(terminal_line, " ");
            while(token != NULL){
                tokens[m] = token;
                token = strtok(NULL, " ");
                m++;
            } 
        }
        //Commented out printing of command name, command parameters and redirections (used in task a)
        //printf("Command name: %s\n", tokens[0]);
        if (m > 1) {
            //printf("Command parameters: ");
            int n = 1;
            
            //Loop through command parameters to find position of first (if any)
            //redirections. Save position of the first "less than" or "greater than"
            //in variable "crocodile"
            while ((strcmp(tokens[n], "<")!=0) && (strcmp(tokens[n], ">")!=0)) {
                //printf("%s ", tokens[n]);
                n++;
                crocodile = n;
                //if no redirections
                if (n >= m) {
                    break;
                }
            }  
        //printf("\n");
        //printf("Redirections: ");
        
        //from first encountered "crocodile" to end of tokens
        //we save the paths for redirection (if any) to output and input

        for (int c = crocodile; c<m; c++) {
            //printf(" %s", tokens[c]);
            if(strcmp(tokens[c], ">")==0) {
                output = tokens[c+1];
            }
            if(strcmp(tokens[c], "<")==0) {
                input = tokens[c+1];
            }
        }
        //printf("\n");
        }
        //execvp stops ececuting when encountering NULL, 
        //so we need to put a NULL-element at the end of the tokens-array
        tokens[m] = NULL;

        //Task d) - we need to implementat some internal shell commands (cd and exit)
        //for the shell to work correctly
        if(strcmp(tokens[0], "cd")==0) {
            if(chdir(tokens[1]) == -1) {
                perror("chdir");
            }
            continue;
        }
        else if(strcmp(tokens[0], "exit")==0) {
            exit(0);
        }
        //implement I/O redirection (task c)
        int pid = fork();
        pid_t wpid;
        int status;

        //in the child process
        if (pid == 0) {
            //if we have a command with output redirection,
            //we change the standard output to given output path
            if(output) {
                int fd_out = creat(output, 0644);
                dup2(fd_out, STDOUT_FILENO);
                tokens[crocodile] = NULL;
            }
            //if we have a command with input redirection,
            //we change the standard input to given input path
            if(input) {
                int fd_in = open(input, O_RDONLY);
                dup2(fd_in, STDIN_FILENO);
                tokens[crocodile] = NULL;
            }
            //execute command using execvp(3)
            if (execvp(tokens[0], tokens) == -1) {
                perror("execvp"); 
            } 
        }
        //in the parent process 
        else if(pid > 0) {
            //wait for child process to finish executing
            do {
                //wait
            }
            while (wpid == wait(&status) > 0);
        }
        //error forking
        else {
            perror("fork");
        } 
        //we have to break if all the commands in shellscript has been executed
        if (filelines[0] != NULL && filelines[n+1] == NULL) {
             exit(0);
        }
        //Increases the count for iterating through lines in shellscript
        //Sets input and output back to NULL
        n++;
        output = NULL;
        input = NULL;
    } 
}