#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

#define RL_BUFSIZE 1024 /* initialize beffer size for reading lines */

/**
 * read_line - Read a line of input from stdin
 *
 * Return: Input line
 */
char *read_line(void)
{
	int bufsize = RL_BUFSIZE; /* set initial buffer size */
	int position = 0; /* position in the buffer */
	char *buffer = malloc(sizeof(char) * bufsize); /* alloc buffer mem */
	int char_read; /* variable to hold each char read */

	if (!buffer) /* check if mem alloc successful */
	{
		fprintf(stderr, "allocation error\n"); /* failure error message */
		exit(EXIT_FAILURE); /* exit program with failure status */
	}

	while (1) /* infinite loop to read chars until line complete */
	{
		char_read = getchar(); /* read character from standard input */

		if (char_read == EOF || char_read == '\n') /* EOF or newline */
		{
			buffer[position] = '\0'; /* null terminate string */
			return (buffer); /* buffer containing line */
		}
		else
		{
			buffer[position] = char_read; /* store char in buffer */
		}
		position++; /* next position on buffer */

		if (position >= bufsize) /* check if buffer full */
		{
			bufsize += RL_BUFSIZE; /* increase buffer size */
			buffer = realloc(buffer, bufsize); /* realloc buffer mem */
			if (!buffer)
			{
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE); /* exit with failure status */
			}
		}
	}
}

#define TOK_BUFSIZE 64 /* initial buffer size for tokens */
#define TOK_DELIM " \t\r\n\a" /* delimiters for tokenizing input */

/**
 * parse_line - Split a line into tokens (arguments) based on delimiters
 * @line: Input line to be tokenized
 *
 * Return: Array of tokens derived form input line. Array terminated 
 * by NULL pointer.
 */
char **parse_line(char *line)
{
	int bufsize = TOK_BUFSIZE; /* initialize buffer size for tokens */
	int position = 0; /* current position in token array */
	char **tokens = malloc(bufsize * sizeof(char*)); /* token array mem alloc */
	char *token; /* pointer to store each token */

	if (!tokens) /* check for allocation failure */
	{
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	/* tokenize input line using specified delimiters */
	token = strtok(line, TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token; /* store token in array */
		position++;

		if (position >= bufsize) /* if buffer is full */
		{
			bufsize += TOK_BUFSIZE; /* increase buffer size */
			tokens = realloc(tokens, bufsize *sizeof(char *));
			if (!tokens) /* check for realloc failure */
			{
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
			token = strtok(NULL, TOK_DELIM); /* get next token */
	}
	tokens[position] = NULL; /* null terminate tokens */

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
	int builtin_status; /* store status of built-in commands */
	pid_t pid; /* process ID */
	int status; /* status code for waitpid */

	if (args[0] == NULL) /* empty command entered */
	{
		return (1);
	}

	/* check for built-in commands first */
	builtin_status = handle_builtin_commands(args);
	if (builtin_status != -1) /* if builtin handled, return status */
	{
		return (builtin_status);
	}

	pid = fork(); /* fork new process to execute command */
	if (pid == 0) /* child process */
	{
		if (execvp(args[0], args) == -1) /* execvp failed */
		{
			perror("hsh"); /* print error message */
		}
		exit(EXIT_FAILURE); /* exit child process if execvp fails */
	}
	else if (pid < 0) /* error occurred while forking */
	{
		perror("hsh");
	}
	else
	{
		do { /* parent process */
			/* wait for child process to terminate */
			waitpid(pid, &status, WUNTRACED);
			/* wait until child process exited or signaled */
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return (1); /* indicate command was executed */
}

/**
 * handle_builtin_commands - Handles built_in shell commands
 * @args: Array of command arguments
 *
 * Return: 0 if command is "exit", 1 if command is "env", -1
 * otherwise
 */
int handle_builtin_commands(char **args)
{
	if (strcmp(args[0], "exit") == 0)
		return (0);

	if (strcmp(args[0], "env") == 0)
	{
		print_custom_env();
		return (1);
	}
	return (-1);
}
