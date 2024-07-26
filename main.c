#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

/**
 * main - Entry point for the shell program.
 *
 * Description: This function initializes the shell, displays a
 * prompt, reads user input, parses the input into commands, and
 * executes the commands.
 *
 * Return: Always 0 (Success).
 */
int main(void)
{
	char *line = NULL; /* stores input line */
	char **commands = NULL; /* array commands separated by pipes */
	int num_commands = 0; /* number of commands */
	int status = 1; /* status of last executed command */
	size_t bufsize = 0;

	do {
		printf("($) "); /* display prompt */
		if (getline(&line, &bufsize, stdin) == -1)
		{
			free(line);
			break;
		}

		if (line[0] == '\n')
		{
			continue;
		}

		commands = parse_line(line, &num_commands); /* parse into commands*/

		if (num_commands > 1)
		{
			execute_pipes(commands, num_commands);
		}
		else
		{
			status = execute(commands);
		}

		free(commands);

	} while (status); /* loop until user exits */

	return (0); /* exit shell */
}
