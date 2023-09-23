#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

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

int execute_setenv(char *args[]) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "setenv: Invalid arguments\n");
        return 1;
    }

    if (setenv(args[1], args[2], 1) == -1) {
        fprintf(stderr, "setenv: Failed to set environment variable\n");
        return 1;
    }

    return 0;
}

int execute_unsetenv(char *args[]) {
    if (args[1] == NULL) {
        fprintf(stderr, "unsetenv: Invalid argument\n");
        return 1;
    }

    if (unsetenv(args[1]) == -1) {
        fprintf(stderr, "unsetenv: Failed to unset environment variable\n");
        return 1;
    }

    return 0;
}

int execute_cd(char *args[]) {
    char *dir = args[1];

    if (dir == NULL || strcmp(dir, "~") == 0) {
        dir = getenv("HOME");
    } else if (strcmp(dir, "-") == 0) {
        dir = getenv("OLDPWD");
    }

    if (dir == NULL) {
        fprintf(stderr, "cd: No home directory\n");
        return 1;
    }

    if (chdir(dir) == -1) {
        fprintf(stderr, "cd: Failed to change directory to %s\n", dir);
        return 1;
    }

    char cwd[MAX_ARG_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    if (setenv("PWD", cwd, 1) == -1) {
        fprintf(stderr, "cd: Failed to update PWD environment variable\n");
        return 1;
    }

    printf("Current directory: %s\n", cwd);

    return 0;
}

int execute_command(char *command) {
    char args[MAX_ARGS][MAX_ARG_LENGTH];
    int arg_count = tokenize_command(command, args);

    if (strcmp(args[0], "exit") == 0) {
        // Handle exit built-in command
        int exit_status = 0;

        if (arg_count > 1) {
            exit_status = atoi(args[1]);
        }

        exit(exit_status);
    } else if (strcmp(args[0], "setenv") == 0) {
        // Handle setenv built-in command
        return execute_setenv(args);
    } else if (strcmp(args[0], "unsetenv") == 0) {
        // Handle unsetenv built-in command
        return execute_unsetenv(args);
    } else if (strcmp(args[0], "cd") == 0) {
        // Handle cd built-in command
        return execute_cd(args);
    }

    char *full_path = get_path(args[0]);

    if (full_path == NULL) {
        printf("Command not found: %s\n", args[0]);
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        // Fork error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (execve(full_path, args, environ) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int main(void) {
    char *prompt = "simple_shell$ ";
    char command[MAX_ARG_LENGTH
