#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(void) {
    char *prompt = "simple_shell$ ";
    char command[100];

    while (1) {
        printf("%s", prompt);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }

        // Remove newline character from command
        command[strcspn(command, "\n")] = '\0';

        pid_t pid = fork();

        if (pid == -1) {
            // Fork error
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (execve(command, NULL, environ) == -1) {
                // Execve error
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
