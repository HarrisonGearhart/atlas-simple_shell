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
	int interactive = isatty(STDIN_FILENO);

	(void)argc;
	status = 0;

	while (1)
	{
		if (interactive)
		{
			
			printf("($) ");
			fflush(stdout);
		}

		line = read_line();

		if (line == NULL)
		{
			if (interactive)
			{
				printf("\n");
			}
			break;
		}

		parsed_args = parse_line(line);

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

		if (status == 2)
		{
			printf("Error: Command execution failed\n");
		}
	}

	return ((status == 2) ? 1 : 0);
}
