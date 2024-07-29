#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

/**
 * main - entry point for shell program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	char *line;
	char **parsed_args;
	int status;

	(void)argc;
	status = 0;

	while (1)
	{
		printf("($) ");
		line = read_line();

		if (line == NULL)
		{
			printf("\n");
			break;
		}

		parsed_args = parsed_line(line);

		if (parsed_args == NULL)
		{
			free(line);
			status = 2;
			continue;
		}

		status = execute(parsed_args, argv[0]);

		free(line);
		free(parsed_args);

		if (status == 1)
		{
			break;
		}
	}

	return ((status == 2) ? 1 : 0);
}
