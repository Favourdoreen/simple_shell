#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX_ARGS 20
#define MAX_ARG_LENGTH 100
#define MAX_ALIAS_SIZE 100
#define MAX_ALIAS_COUNT 20

extern char **environ;

char *get_path(char *command) {
    // ... Same get_path() implementation as before ...
}

int tokenize_command(char *command, char *args[]) {
    // ... Same tokenize_command() implementation as before ...
}

int execute_setenv(char *args[]) {
    // ... Same execute_setenv() implementation as before ...
}

int execute_unsetenv(char *args[]) {
    // ... Same execute_unsetenv() implementation as before ...
}

int execute_cd(char *args[]) {
    // ... Same execute_cd() implementation as before ...
}

int execute_alias(char *args[], char *aliases[][2], int *alias_count) {
    // ... Same execute_alias() implementation as before ...
}

int replace_variables(char *args[], char *command) {
    char *var_replacement;
    char *var_start;
    char var[MAX_ARG_LENGTH];

    // Replace $? with the exit status of the last command
    var_replacement = strstr(command, "$?");
    if (var_replacement != NULL) {
        sprintf(var, "%d", WEXITSTATUS(last_command_status));
        var_start = var_replacement;
        while (*var_replacement != '\0') {
            *var_replacement = ' ';
            var_replacement++;
        }
        strncpy(var_start, var, strlen(var));
    }

    // Replace $$ with the process ID of the shell
    var_replacement = strstr(command, "$$");
    if (var_replacement != NULL) {
        sprintf(var, "%d", getpid());
        var_start = var_replacement;
        while (*var_replacement != '\0') {
            *var_replacement = ' ';
            var_replacement++;
        }
        strncpy(var_start, var, strlen(var));
    }

    return tokenize_command(command, args);
}

int execute_command(char *command, char *aliases[][2], int *alias_count) {
    char *args[MAX_ARGS];
    int result;

    result = replace_variables(args, command);
    if (result != 0) {
        fprintf(stderr, "Failed to replace variables in command\n");
        return 1;
    }

    // ... Same execute_command() implementation as before ...
}

int main() {
    char command[MAX_ARG_LENGTH];
    char aliases[MAX_ALIAS_COUNT][2][MAX_ALIAS_SIZE];
    int alias_count = 0;

    while (1) {
        printf(">> ");
        fgets(command, sizeof(command), stdin);

        // Remove the trailing newline character
        command[strcspn(command, "\n")] = 0;

        if (strlen(command) == 0) {
            // Empty command, skip to next iteration
            continue;
        }

        int result = execute_command(command, aliases, &alias_count);

        if (result != 0) {
            printf("Command execution failed\n");
        }
    }

    return 0;
}
