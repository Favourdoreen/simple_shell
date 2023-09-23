#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

char *get_path(char *command) {
    // ... Same get_path() implementation as before ...
}

int main(void) {
    char *prompt = "simple_shell$ ";
    char command[100];
    char *args[20];

    while (1) {
        printf("%s", prompt);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }

        // Remove newline character from command
        command[strcspn(command, "\n")] = '\0';

        // Tokenize the command into arguments
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;  // Set the last argument as NULL

        if (strcmp(args[0], "exit") == 0) {
            // Handle exit built-in command
            break;
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
