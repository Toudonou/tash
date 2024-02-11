//
// Created by toudonou on 09/02/24.
//

#include "tash.h"

const int commands_list_size = 11;
const char *commands_list[] = {
        "exit", "clear", "history", "help",
        "ls", "touch", "mv",
        "cat", "cp", "man", "pwd",
};


void tash_loop() {

    char *command = NULL;
    char **args = NULL;
    size_t args_size = 0;
    int status = 1;

    print_message("clear");
    do {
        printf("tash:>$ ");

        command = read_command();
        args = split_command(command, &args_size);
        status = command_executive((const char **) args);


        if(command) free(command);
        if(args){
            for (size_t i = 0; i < args_size; i++) {
                if (args[i]) free(args[i]);
            }
            free(args);
        }

    } while (status);
}

char *read_command() {
    char *command = (char *) malloc(sizeof(char) * COMMAND_SIZE);
    char c;
    size_t position = 0;

    if (!command) {
        printf("Error : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((c = (char) getchar()) != '\n') {
        command[position++] = c;

        // reallocation (in case of a too long command)
        {
            char *old_command = command;
            if (position >= strlen(command)) {
                command = realloc(command, (strlen(command) + COMMAND_SIZE) * sizeof(char));
                if (!command) {
                    free(old_command);
                    printf("Error : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    command[position] = '\0';

    return command;
}

char **split_command(const char *command, size_t *args_size) {
    if (!command || strcmp(command, "\0") == 0) {
        *args_size = 0;
        return NULL;
    }

    *args_size = ARGS_SIZE;
    char **args = (char **) malloc(*args_size * sizeof(char *));
    char *token = NULL, *command_cpy = (char *) malloc(strlen(command) * sizeof(char));
    size_t position = 0;

    if (!args) {
        printf("Error : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (!command_cpy) {
        printf("Error : %s\n", strerror(errno));
        free(args);
        exit(EXIT_FAILURE);
    }

    strcpy(command_cpy, command);

    token = strtok(command_cpy, " ");
    while (token != NULL) {
        args[position] = calloc(strlen(token), sizeof(char));
        strcpy(args[position++], token);
        token = strtok(NULL, " ");

        // reallocation (in case of a too long command)
        {
            char **old_args = args;
            if (position >= *args_size) {
                *args_size += ARGS_SIZE;

                args = (char **) realloc(args, *args_size * sizeof(char *));
                if (!args) {
                    for (size_t i = 0; i < *args_size - ARGS_SIZE; i++) {
                        if (old_args[i]) free(old_args[i]);
                    }
                    free(old_args);

                    printf("Error : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    free(command_cpy);

    return args;
}

int execute_command(const char **args) {
    int process_status;
    pid_t process;

    if (args) {
        process = fork();
        if (process == 0) {
            if (execvp(args[0], (char *const *) args) < 0) {
//                printf("tash error -> %s : command not found\n", args[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            waitpid(process, &process_status, 0);
        }
    }

    return 1;
}

int command_executive(const char **args) {
    if (args) {
        if (strcmp(args[0], "exit") == 0) {
            print_message("exit");
            return 0;
        } else if (strcmp(args[0], "clear") == 0) {
            execute_command(args);
            print_message("clear");
        } else if (strcmp(args[0], "history") == 0 || strcmp(args[0], "help") == 0) {
            print_message(args[0]);
        } else {
            for (int i = 4; i < commands_list_size; ++i) {
                if (strcmp(args[0], commands_list[i]) == 0) {
                    execute_command(args);
                    return 1;
                }
            }
            printf("tash error -> %s : command not found\n", args[0]);
        }
    }
    return 1;
}

void print_message(const char *command) {
    if (strcmp(command, "exit") == 0) {
        printf("Bye..........\n");
    } else if (strcmp(command, "clear") == 0) {
        printf("\n\n");
        printf("   TTTTTTTTT   AAAAAAAAA    SSSSSSSSS     HHH    HHH\n");
        printf("      TTT     AAA     AAA   SSS           HHH    HHH\n");
        printf("      TTT     AAAAAAAAAAA   SSSSSSSSS     HHHHHHHHHH\n");
        printf("      TTT     AAA     AAA         SSS     HHH    HHH\n");
        printf("      TTT     AAA     AAA  SSSSSSSSSS     HHH    HHH\n");

        printf("\nTASH : Toudonou Again Shell\n");
        printf("A simple shell written in C\n");
        printf("Author : Toudonou (https://github.com/Toudonou)\n");
        printf("Tap \"help\" for more informations\n\n");

    } else if (strcmp(command, "history") == 0) {
        printf("history\n");
    } else if (strcmp(command, "help") == 0) {
        printf("A simple shell written in C\n");
        printf("Available commands : ");
        for (int i = 0; i < commands_list_size - 1; ++i) {
            printf("%s, ", commands_list[i]);
        }
        printf("%s\n", commands_list[commands_list_size - 1]);
    }
}