#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
#define MAX_BUFFER_SIZE 256

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

// Function to retrive system information and cmd output
char* getCommandOutput(char* command) {
	// Creates a pipe for standard output
	FILE* pipe = popen(command, "r");
	if (!pipe) {
		perror("popen");
		exit(EXIT_FAILURE);
	}

	// Allocates memory space for the output
	char* output = (char*)malloc(MAX_BUFFER_SIZE * sizeof(char));
	if (!fgets(output, MAX_BUFFER_SIZE, pipe)) {
		perror("fgets");
		exit(EXIT_FAILURE);
	}

	pclose(pipe);

	size_t len = strlen(output);
	if (len > 0 && output[len - 1] == '\n') {
		output[len - 1] = '\0'; // Removes the newline character
	}

	return output;
}

// Function to print rainbow effect
void print_rainbow_effect(const char *string)
{
    const char *colors[] = {"\033[1;31m", "\033[1;33m", "\033[1;32m", "\033[1;34m", "\033[1;35m", "\033[1;36m"};
    int num_colors = sizeof(colors) / sizeof(colors[0]);
    int color_index = 0;

    for (int i = 0; string[i] != '\0'; i++)
    {
        // Apply rainbow color to each character
        printf("%s%c", colors[color_index], string[i]);

        // Move to the next color in the spectrum
        color_index = (color_index + 1) % num_colors;
    }
    printf("\033[0m"); // Reset color after printing
}

// Built-in functions and commands________________________________________________

// Rainbow command function
void rainbow_command(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: rainbow [filename]\n");
        return;
    }

    char *filename = args[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file))
    {
        print_rainbow_effect(buffer);
    }

    fclose(file);
}

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

void list_files(char **args)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        execlp("ls", "ls", args[1], NULL);
        perror("execlp");
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

void remove_file(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: rm [filename]\n");
        return;
    }
    if (remove(args[1]) != 0) {
        perror("Error deleting file");
    }
}

void cat_file(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: cat [filename]\n");
        return;
    }
    FILE *file = fopen(args[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}

void clear_screen() {
    system("clear");
}

void cowsay_message(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: cowsay [message]\n");
        return;
    }
    printf(" _____\n< %s >\n ----- \n        \\   ^__^\n         \\  (oo)\\_______\n            (__)\\       )\\/\\\n                ||----w |\n                ||     ||\n", args[1]);
}

void execute_command(char **args) {
    // Check if the command contains redirection
    int i;
    int redirect_index = -1;
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            redirect_index = i;
            break;
        }
    }

    if (redirect_index >= 0) {
        // Output redirection detected
        char *output_filename = args[redirect_index + 1];
        args[redirect_index] = NULL; // Null terminate the command before '>'
        
        // Fork a child process
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process: open the output file
            FILE *output_file = freopen(output_filename, "w", stdout);
            if (output_file == NULL) {
                perror("freopen");
                exit(EXIT_FAILURE);
            }

            // Execute the command
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process: wait for the child to finish
            int status;
            waitpid(pid, &status, 0);
        }
    } else {
        // No redirection, execute the command as usual
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
}


// Mapping of built-in functions to their names
char *built_in_function_names[] = {
    "cd",
    "exit",
    "ls",
    "rainbow",
    "rm",
    "cat",
    "clear",
    "cowsay",
    "pipe"};

void (*built_in_functions[])(char **) = {
    &cd,
    &exit_shell,
    &list_files,
    &rainbow_command,
    &remove_file,
    &cat_file,
    &clear_screen,
    &cowsay_message,
    &execute_command};

int main(void)
{
    system("clear");
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];


    printf("     ________  ___  ___  _______   ___       ___      \n");    
    printf("    |\\   ____\\|\\  \\|\\  \\|\\  ___ \\ |\\   \\    |\\  \\     \n");
    printf("    \\ \\  \\___|\\ \\  \\\\  \\ \\   __/|\\ \\   \\    \\ \\  \\    \n");
    printf("     \\ \\_____  \\ \\   __  \\ \\  \\_|/_\\ \\  \\    \\ \\  \\   \n");
    printf("      \\|____|\\  \\ \\  \\ \\  \\ \\  \\_|  \\ \\  \\____\\ \\  \\____\n");
    printf("        ____\\_\\  \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\_______\\\n");
    printf("       |\\_________\\|__|\\|__|\\|_______|\\|_______|\\|_______|\n");
    printf("       \\|_________|                                         \n");
    printf("Built in functions: \n");
    int num_functions = sizeof(built_in_function_names) / sizeof(built_in_function_names[0]);

    // Loop through the array and print each element
    for (int i = 0; i < num_functions; i++) {
        printf("%s\n", built_in_function_names[i]);
    }

    while (1)
    {
        //printf("$ ");
        printf("\033[1;36m%s@%s\033[0m:\033[1;33m%s\033[0m$ ", getCommandOutput("whoami"), getCommandOutput("hostname"), getCommandOutput("pwd"));
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
