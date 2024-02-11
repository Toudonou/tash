//
// Created by toudonou on 09/02/24.
//

#ifndef TASH_TASH_H
#define TASH_TASH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define COMMAND_SIZE 1024
#define ARGS_SIZE 10

void tash_loop();

char *read_command();

char **split_command(const char *command, size_t *args_size);

int execute_command(const char **args);

int command_executive(const char **args);

void print_message(const char *command);

#endif //TASH_TASH_H
