#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

extern char **environ;

char *get_path(char *command) {
    // ... Same get_path() implementation as before ...
}

char *read_line() {
    static char buffer[BUFFER_SIZE];
    static int buffer_index = 0;
    static int chars_read = 0;

    if (buffer_index >= chars_read) {
        chars_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        buffer_index = 0;
    }

    if (chars_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    } else if (chars_read == 0) {
        // End of file
        return NULL;
    }

    char *line_start = &buffer[buffer_index];
    char *newline = memchr(line_start, '\n', chars_read - buffer_index);

    if (newline != NULL) {
        *newline = '\0';
        buffer_index = newline - buffer + 1;
    } else {
        buffer_index = chars_read;
    }

    return line_start;
}

int main(void) {
    char *prompt = "simple_shell$ ";
    char *command;

    while (1) {
        printf("%s", prompt);

        command = read_line();

        if (command == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }

        // Tokenize the command into arguments
        char *token = strtok(command, " ");
        char *args[20];
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;  // Set the last argument as NULL

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
