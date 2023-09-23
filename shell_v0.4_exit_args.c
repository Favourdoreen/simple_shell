#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 20
#define MAX_ARG_LENGTH 100

extern char **environ;

char *get_path(char *command) {
    // ... Same get_path() implementation as before ...
}

int tokenize_command(char *command, char *args[]) {
    int arg_count = 0;
    int arg_index = 0;
    int command_length = strlen(command);

    for (int i = 0; i < command_length; i++) {
        if (command[i] != ' ') {
            args[arg_count][arg_index++] = command[i];
        } else {
            args[arg_count++][arg_index] = '\0';
            arg_index = 0;
        }
    }

    args[arg_count][arg_index] = '\0';
    args[arg_count + 1] = NULL;

    return arg_count + 1;
}

int main(void) {
    char *prompt = "simple_shell$ ";
    char command[MAX_ARG_LENGTH * MAX_ARGS];
    char args[MAX_ARGS][MAX_ARG_LENGTH];

    while (1) {
        printf("%s", prompt);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }

        // Remove newline character from command
        command[strcspn(command, "\n")] = '\0';

        int arg_count = tokenize_command(command, args);

        if (strcmp(args[0], "exit") == 0) {
            // Handle exit built-in command
            int exit_status = 0;

            if (arg_count > 1) {
                exit_status = atoi(args[1]);
            }

            exit(exit_status);
        }

        char *full_path = get_path(args[0]);

        if (full_path == NULL) {
            printf("Command not found: %s\n", args[0]);
            continue;
        }

        pid_t pid = fork();

        if (pid == -1) {
            // Fork error
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (execve(full_path, args, environ) == -1) {
                // Execve error
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }

        free(full_path);
    }

    return 0;
}
