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
	char *line; /* pointer holds input line */
	char **args; /* pointer holds parsed arguments */
	int status = 1; /* status to control loop, initialize to 1 starts loop */

	while (status) /* loop continues as long as status non-zero */
	{
		printf("$ "); /* Print shell prompt */
		line = read_line(); /* read line of user input */

		if (line == NULL)
		{
			fprintf(stderr, "Error reading line\n");
			continue;
		}

		args = parse_line(line); /* parse input into arguments */

		if (args == NULL)
		{
			fprintf(stderr, "Error parsing line\n");
			free(line);
			continue;
		}

		status = execute(args); /* execute parsed commands & update status */

		printf("Status after execution: %d\n", status); /* debug print */

		free(line); /* free alloc mem for input line */
		free(args); /* free alloc mem for arguments */
	}

	return (0); /* indicate successful program termination */
}
