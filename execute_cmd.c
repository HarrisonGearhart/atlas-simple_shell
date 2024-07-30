#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

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
 * execute_command_in_child_process - Executes command in child process
 * @args: Array of command arguments
 * @command_path: Path to the command
 */
void execute_command_in_child_process(char **args, char *command_path)
{
	if (execvp(command_path, args) == -1)
		perror("hsh");
	exit(EXIT_FAILURE);
}

/**
 * get_command_path - Gets the path to a command
 * @command: Command name
 *
 * Return: Path to the command, or NULL if command not found
 */
char *get_command_path(char *command)
{
	char *command_path;

	if (command_exists_in_current_dir(command))
	{
		command_path = command;
	}
	else
	{
		command_path = find_command_in_path(command);
	}

	return (command_path);
}

/**
 * print_custom_env - Print a predefined list of environmental variables
 *
 * Return: void
 */
void print_custom_env(void)
{
	char *env_vars[] = {
		"PATH",
		"HOME",
		"USER",
		"SHELL",
		"LANG",
		NULL
	};

	char **env = env_vars;
	while (*env)
	{
		char *value = getenv(*env);
		if (value)
		{
			printf("%s=%s\n", *env, value);
		}
		env++;
	}
}
