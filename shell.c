#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// defining ANSI color codes

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

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
        fprintf(stderr, ANSI_COLOR_GREEN "Usage: cd [directory]\n" ANSI_COLOR_RESET);
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
    printf(ANSI_COLOR_GREEN "Exiting shell.\n" ANSI_COLOR_RESET);
    exit(0);
}

void list_files(char **args)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        execlp("ls", "ls", args[1], NULL);
        perror(ANSI_COLOR_RED "execlp" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror(ANSI_COLOR_RED "fork" ANSI_COLOR_RESET);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

// Mapping of built-in functions to their names
char *built_in_function_names[] = {
    "cd",
    "exit",
    "ls"};

void (*built_in_functions[])(char **) = {
    &cd,
    &exit_shell,
    &list_files};

int main(void)
{
    printf("\n");
    printf(ANSI_COLOR_YELLOW "Welcome to our Shell\n" ANSI_COLOR_RESET);

    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1)
    {
        printf(ANSI_COLOR_YELLOW "$ " ANSI_COLOR_RESET);
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
            // fokring(creating)a child process
            if (pid == 0)
            {
                // a child process
                printf(ANSI_COLOR_GREEN "Child process with a PID = %d\n" ANSI_COLOR_RESET, getpid());

                // replacing a current process image with a new program; executing ls command with -l option for executing long files

                char *args[] = {"ls", "-l", NULL};
                execv("bin/ls", args);
                // execv takes path to the given command as an argument, and an array of a command with its options as another argument

                // execv() will not return unless an error occurs
                perror(ANSI_COLOR_RED "execv" ANSI_COLOR_RESET);
                exit(1);
            }
            else if (pid < 0)
            {
                // Error when attempting to fork/create a child process
                perror(ANSI_COLOR_RED "fork" ANSI_COLOR_RESET);
                exit(1);
            }
            else
            {
                // Parent process

                printf(ANSI_COLOR_GREEN "Parent process with a PID = %d" ANSI_COLOR_RESET, getpid());

                // wait for the child process to terminate
                int status;
                wait(&status);

                printf(ANSI_COLOR_GREEN "Child process has terminated\n" ANSI_COLOR_RESET);

                // Sending a SIGKILL signal to the child process

                if (kill(pid, SIGKILL) == 0)
                {
                    printf(ANSI_COLOR_GREEN "Sent SIGKILL signal to the child process\n" ANSI_COLOR_RESET);
                }
                else
                {
                    perror(ANSI_COLOR_RED "kill" ANSI_COLOR_RESET);
                }
            }
        }
    }

    return 0;
}
