#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

char **environ;

/**
 * command_exists_in_current_dir - Check if command exists in
 * current directory
 * @command: Command to check
 *
 * Return: 1 if command exists, 0 otherwise
 */
int command_exists_in_current_dir(char *command)
{
	struct stat statbuf;

	return (stat(command, &statbuf) == 0);
}

/**
 * find_command_in_path - Find a command in the PATH environment variable
 * @command: Command to find
 *
 * Return: Full path of command if found, NULL otherwise
 */
char *find_command_in_path(char *command)
{
	char *path_env;
	char *path_copy;
	char *token;
	char *full_path;
	struct stat statbuf;

	path_env = getenv("PATH");
	if (path_env == NULL)
	{
	return (NULL);
	}

	path_copy = strdup(path_env);
	token = strtok(path_copy, ":");

	while (token != NULL)
	{
	full_path = malloc(strlen(token) + strlen(command) + 2);
		sprintf(full_path, "%s/%s", token, command);

		if (stat(full_path, &statbuf) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		token = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * execute_command = Execute a command using execve
 * @path: Path of the command
 * @args: Array of arguments
 *
 * Return: Status of executed command
 */
int execute_command(char **args, char *path)
{
	if (execve(path, args, environ) == -1)
	{
		perror("execve");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * print_env - Prints current environment variables
 */
void print_env(void)
{
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}
