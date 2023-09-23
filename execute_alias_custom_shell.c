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
    if (args[1] == NULL) {
        // Print all aliases
        for (int i = 0; i < *alias_count; i++) {
            printf("%s='%s'\n", aliases[i][0], aliases[i][1]);
        }
    } else {
        // Define or print specific aliases
        for (int i = 1; args[i] != NULL; i++) {
            char *arg = args[i];
            char *equal_sign = strchr(arg, '=');

            if (equal_sign == NULL) {
                // Print the alias
                for (int j = 0; j < *alias_count; j++) {
                    if (strcmp(aliases[j][0], arg) == 0) {
                        printf("%s='%s'\n", aliases[j][0], aliases[j][1]);
                        break;
                    }
                }
            } else {
                // Define or update the alias
                char *name = strtok(arg, "=");
                char *value = strtok(NULL, "=");

                if (name == NULL || value == NULL) {
                    fprintf(stderr, "alias: Invalid argument\n");
                    return 1;
                }

                // Check if the alias already exists
                int alias_index = -1;
                for (int j = 0; j < *alias_count; j++) {
                    if (strcmp(aliases[j][0], name) == 0) {
                        alias_index = j;
                        break;
                    }
                }

                if (alias_index == -1) {
                    // Add a new alias
                    if (*alias_count == MAX_ALIAS_COUNT) {
                        fprintf(stderr, "alias: Maximum number of aliases reached\n");
                        return 1;
                    }

                    strncpy(aliases[*alias_count][0], name, MAX_ALIAS_SIZE);
                    strncpy(aliases[*alias_count][1], value, MAX_ALIAS_SIZE);
                    (*alias_count)++;
                } else {
                    // Update an existing alias
                    strncpy(aliases[alias_index][1], value, MAX_ALIAS_SIZE);
                }
            }
        }
    }

    return 0;
}

int execute_command(char *command, char *aliases[][2], int *alias_count) {
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
