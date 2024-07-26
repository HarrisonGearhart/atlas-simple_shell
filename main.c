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
	int status; /* status of last executed command */

	do {
		printf("($) "); /* display prompt */
		line = read_line(); /* read line of input */
		commands = parse_line(line, &num_commands); /* parse into commands*/

		if (num_commands > 1)
		{
			execute_pipes(commands, num_commands);
		}
		else
		{
			status = execute(commands);
		}

		/* free allocated memory */
		free(line);
		free(commands);

	} while (status); /* loop until user exits */

	return (0); /* exit shell */
}
