#ifndef SHELL_H
#define SHELL_H

#define BUFFER_SIZE 1024 /* initial size input buffer */
#define DELIM " \t\r\n\a" /* chars used to split input line into tokens */

#include <stdio.h> /* printf, perror, getline */
#include <stdlib.h> /* free, exit, EXIT_FAILURE */
#include <unistd.h> /* POSIX API functions */
#include <string.h> /* string manipualtion */

char *read_line(void);
char **parse_line(char *line);
int execute(char **args);
int command_exists_in_current_dir(char *command);
char *find_command_in_path(char *command);
void execute_command(char *path, char **args);

#endif
