#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


void in_file(char *arr[])
{

    if (arr[1] != NULL && arr[2] != NULL)
    {

        char *command = arr[0];
        char *direction = arr[1];
        char *filename = arr[2];

        int outy;
        int inny;

        outy = strcmp(direction, ">");
        inny = strcmp(direction, "<");

        if (outy == 0)
        {
            printf("We are outputting to a file\n");

            FILE *fp;
            int file;

            fp = fopen(filename, "w");
            file = open(filename, O_WRONLY);
            dup2(file, STDOUT_FILENO);
        }

        if (inny == 0)
        {
            printf("We are taking in an input\n");
            FILE *fp;
            int file;
            fp = fopen(filename, "r");
            file = open(filename, O_RDONLY);
            dup2(file, STDIN_FILENO);
        }
    }
}


void parse(char *s)
{
    const char break_chars[] = " \t";
    char *p;

    int i = 0;
    char *token_list[3];

    p = strtok(s, break_chars);

    while (p != NULL)
    {
        printf("token was: %s\n", p);

        if (i < 3){
            token_list[i] = p; 
        }
        
        i++;
        p = strtok(NULL, break_chars);
    }

    in_file(token_list);
}

int main(int argc, const char *argv[])
{
    char input[BUFSIZ];
    char last_command[BUFSIZ];
    memset(input, 0, BUFSIZ * sizeof(char));
    memset(input, 0, BUFSIZ * sizeof(char));
    bool finished = false;

    while (!finished)
    {
        printf("osh> ");
        fflush(stdout);

        if ((fgets(input, BUFSIZ, stdin)) == NULL)
        { // or gets(input, BUFSIZ);
            fprintf(stderr, "no command entered\n");
            exit(1);
        }

        input[strlen(input) - 1] = '\0'; // wipe out newline at end of string
        printf("input was: \n'%s'\n", input);

        // check for history (!!) command
        if (strncmp(input, "!!", 2) == 0)
        {

            if (strlen(last_command) == 0)
            {
                fprintf(stderr, "no last command to execute\n");
            }

            printf("last command was: %s\n", last_command);
        }
        else if (strncmp(input, "exit", 4) == 0)
        { // only compare first 4 letters
            finished = true;
        }
        else
        {
            strcpy(last_command, input);
            printf("You entered: %s\n", input); // you will call fork/exec
            parse(input);

            //checker(input);

            printf("after parse, what is input: %s\n", input);
            printf("\n");
        }
    }

    printf("osh exited\n");
    printf("program finished\n");
    return 0;
}

/*
Lines used to run
$ clang project-Unix_Shell.c -o project-Unix_Shell -lpthread
$ ./project-Unix_Shell 
*/