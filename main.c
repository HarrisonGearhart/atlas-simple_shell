#include <stdio.h>
#include <stdlib.h>
#include "TEST_shell.h"

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
	char *line; /* stores input line */
	char **args; /* array of arguments parsed from input line */
	int status; /* status of last executed command */

	do
	{
		printf("($) "); /* display prompt */
		line = read_line(); /* read line of input */
		args = parse_line(line); /* parse line into arguments */
		status = execute(args); /* execute parsed args */

		/* free allocated memory */
		free(line);
		free(args);
	}
	while (status); /* loop until user exits */

	return (0); /* exit shell */
}
