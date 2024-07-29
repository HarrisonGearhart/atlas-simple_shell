#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

#define RL_BUFSIZE 1024

/**
 * read_line - Reads a line of user input
 *
 * Return: Input line as a string
 */
char *read_line(void)
{
	int bufsize = RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int bytes_read;
	char c;

	if (!buffer)
	{
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);

		if (bytes_read == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}

		if (bytes_read == 0 || c == '\n')
		{
			buffer[position] = '\0';
			if (bytes_read == 0 && position == 0)
			{
				free(buffer);
				return (NULL);
			}
			return (buffer);
		}
		else
		{
			buffer[position] = c;
		}
		position++;

		if (position >= bufsize)
		{
			bufsize += RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer)
			{
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

/**
 * parse_line - Splits line into tokens based on delimiters
 * @line: Input line to be tokenized
 *
 * Return: Array of tokens derived from input line
 */
char **parse_line(char *line)
{
	int bufsize = TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return (tokens);
}

/**
 * execute - Executes shell built-in or launch program
 * @args: Array of command arguments 
 * @prog_name: Name of program for error messages
 *
 * Return: Status code based on execution results
 */
int execute(char **args, char *prog_name)
{
	int built_in_status;
	pid_t pid;
	int status;

	if (args[0] == NULL)
	{
		return 0;
	}

	built_in_status = handle_builtin_commands(args);
	if (built_in_status != -1)
	{
		return (built_in_status);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (2);
	}

	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror(prog_name);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			return (WEXITSTATUS(status));
		}
		return (2);
	}
	return (2);
}

/**
 * handle_builtin_commands - Handles built-in shell commands
 * @args: Array of command arguments
 *
 * Return: 1 if command is "exit", 0 if command is "env", -1 otherwise
 */
int handle_builtin_commands(char **args)
{
	if (strcmp(args[0], "exit") == 0)
		return (1);

	if (strcmp(args[0], "env") == 0)
	{
		print_custom_env();
		return (1);
	}
	return (-1);
}
