#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * read_line - Read a line of input from stdin
 *
 * Return: Input line
 */
char *read_line(void)
{
	char *line = NULL; /* pointer to store input line */
	size_t bufsize = 0; /* buffer size for getline */

	/* read line from stdin */
	if (getline(&line, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{
			/* end of file (Ctrl+D) was received */
			exit(EXIT_SUCCESS);
		}
		else
		{
			/* error occured while reading line */
			perror("read_line");
			exit(EXIT_FAILURE);
		}
	}

	return (line); /* read line */
}

/**
 * parse_line - Split a line into tokens (arguments)
 * @line: Input line
 *
 * Return: Array of tokens
 */
char **parse_line(char *line)
{
	int bufsize = BUFFER_SIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		/* memory allocation error */
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	/* tokenize input line using specified delimiters */
	token = strtok(line, DELIM);
	while (token != NULL)
	{
		tokens[position++] = token;

		/* reallocate memory if buffer is exceeded */
		if (position >= bufsize)
		{
			bufsize += BUFFER_SIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));

			if (!tokens)
			{
				/* mem realloc error */
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		/* get next token */
		token = strtok(NULL, DELIM);
	}
	tokens[position] = NULL; /* null terminate array of tokens */
	return (tokens); /* array of tokens */
}

/**
 * execute - Execute shell built-in or launch program
 * @args: Array of arguments
 *
 * Return: 1 if shell should continue running, 0 if should terminate
 */
int execute(char **args)
{
	pid_t pid;
	int status;
	char *command_path;

	if (args[0] == NULL) /* emplty command entered */
	{
		return (1);
	}

	if (strcmp(args[0], "exit") == 0) /* check for exit command */
	{
		return (0); /* terminate shell */
	}

	if (strcmp(args[0], "env") == 0)
	{
		print_env();
		return (1);
	}

	if (command_exists_in_current_dir(args[0]))
	{
		command_path = args[0];
	}
	else
	{
		command_path = find_command_in_path(args[0]);
		if (command_path == NULL)
		{
			fprintf(stderr, "%s: command not found\n", args[0]);
			return (1);
		}
	}

	pid = fork(); /* create new process */
	if (pid == 0)
	{
		/* child process */
		execute_command(command_path, args);
	}
	else if (pid < 0)
	{
		/* error forking */
		perror("fork");
	}
	else
	{
	/*parent process */
		do {
			/* wait for child process to terminate */
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	if (command_path != args[0])
	{
		free(command_path);
	}

	return (1); /* continue running shell */
}
