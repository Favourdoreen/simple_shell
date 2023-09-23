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

int execute_command(char *command, char *aliases[][2], int *alias_count) {
    // ... Same execute_command() implementation as before ...
}

int execute_commands_from_file(char *filename, char *aliases[][2], int *alias_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 1;
    }

    char command[MAX_ARG_LENGTH];

    while (fgets(command, sizeof(command), file) != NULL) {
        // Remove the trailing newline character
        command[strcspn(command, "\n")] = 0;

        // Check for a comment (lines starting with '#')
        if (command[0] == '#') {
            continue;
        }

        int result = execute_command(command, aliases, alias_count);

        if (result != 0) {
            printf("Command execution failed\n");
        }
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        char *filename = argv[1];
        int alias_count = 0;
        return execute_commands_from_file(filename, NULL, &alias_count);
    }

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

        // Check for a comment (lines starting with '#')
        if (command[0] == '#') {
            continue;
        }

        int result = execute_command(command, aliases, &alias_count);

        if (result != 0) {
            printf("Command execution failed\n");
        }
    }

    return 0;
}
