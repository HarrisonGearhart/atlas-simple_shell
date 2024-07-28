#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

/**
 * main - Entry point for the shell program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	(void)argc; /* prevent unused parameter warning */
	char *line; /* pointer holds input line */
	char **parsed_args; /* pointer holds parsed arguments */
	int status; /* status to control loop */

	status = 0; /* initialize to 0 (run) */

	while (1)
	{
		printf("($) "); /* print shell prompt */
		line = read_line(); /* read user input line */

		if (line == NULL)
		{
			fprintf(stderr, "Error reading line\n"); /* if line read fails */
			status = 2; /* set status to error exit */
			continue; /* skip rest of loop & prompt again */
		}

		parsed_args = parse_line(line); /* parse input into arguments */

		if (parsed_args == NULL)
		{
			fprintf(stderr, "Error parsing line\n"); /* if parsing fails */
			free(line); /* free alloc mem for line */
			status = 2; /* set status to error exit */
			continue; /* skip rest of loop & prompt again */
		}

		status = execute(parsed_args, argv[0]); /* execute command & update status */

		free(line); /* free alloc mem for line */
		free(parsed_args); /* free alloc mem for arguments */

		if (status == 1)
		{
			break; /* exit loop if status 1 (normal exit) */
		}
	}

	return ((status == 2) ? 1 : 0); /* 1 for error exit, 0 for normal exit */
}
