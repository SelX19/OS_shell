//  Created by Selma Djozic, and Idris Hasanovic on 15/3/24.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

// Function to parse the input command
void parse_command(char *command, char **args)
{
    char *token;
    int i = 0;

    token = strtok(command, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

// Built-in functions
void cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: cd [directory]\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("cd");
        }
    }
}

void exit_shell()
{
    printf("Exiting shell.\n");
    exit(0);
}

// Mapping of built-in functions to their names
char *built_in_function_names[] = {
    "cd",
    "exit"};

void (*built_in_functions[])(char **) = {
    &cd,
    &exit_shell};

int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1)
    {
        printf("$ ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        // Parse the command
        parse_command(command, args);

        if (args[0] == NULL)
        {
            continue;
        }

        // Check if it's a built-in function
        int i;
        int num_built_in_functions = sizeof(built_in_function_names) / sizeof(char *);
        for (i = 0; i < num_built_in_functions; i++)
        {
            if (strcmp(args[0], built_in_function_names[i]) == 0)
            {
                (*built_in_functions[i])(args);
                break;
            }
        }

        if (i == num_built_in_functions)
        {
            // Not a built-in function, execute as external command
            pid_t pid = fork();
            if (pid == 0)
            {
                // Child process
                if (execvp(args[0], args) == -1)
                {
                    perror("execvp");
                }
                exit(EXIT_FAILURE);
            }
            else if (pid < 0)
            {
                // Error forking
                perror("fork");
            }
            else
            {
                // Parent process
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    return 0;
}
