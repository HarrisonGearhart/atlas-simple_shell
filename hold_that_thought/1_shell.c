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
  ssize_t len;

  /* read line from stdin */
  len = getline(&line, &bufsize, stdin);

  if (len == -1)
  {
    if (line)
    {
      /* if line not NULL, free alloc mem */
      free(line);
    }

    /* exit if error or EOF (Ctrl+D) */
    exit(EXIT_SUCCESS);
  }

  return (line); /* return read line */
}

/**
 * parse_line - Split a line into tokens (arguments)
 * @line: Input line
 *
 * Return: Array of tokens
 */
char **parse_line(char *line, int *num_commands)
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

    if (position >= bufsize)
    {
      bufsize += BUFFER_SIZE;
      tokens = realloc(tokens, bufsize *sizeof(char *));
      if (!tokens)
      {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
      token = strtok(NULL, DELIM);
  }
  tokens[position] = NULL;

  *num_commands = position;
  return (tokens);
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
    return (1);

  status = handle_builtin_commands(args);
  if (status != -1)
    return (status);

  command_path = get_command_path(args[0]);
  if (command_path == NULL)
  {
    fprintf(stderr, "%s: command not found\n", args[0]);
    return (1);
  }

  pid = fork();
  if (pid == 0)
  {
    execute_command_in_child_process(args, command_path);
  }
  else if (pid < 0)
  {
    perror("fork");
  }
  else
  {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  if (command_path != args[0])
    free(command_path);

  return (1);
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

/**
 * execute_pipes - Executes commands separated by pipes
 * @commands: Array of commands
 * @num_commands: Number of commands
 *
 * Description: Forks processes and sets up pipes to execute
 * commands separated by the pipe symbol ('|'). The output of
 * one command is used as the input for the next command.
 */
void execute_pipes(char **commands, int num_commands)
{
  int pipe_fds[2];
  int i;
  pid_t pid;

  for (i = 0; i < num_commands - 1; i++)
  {
    if (pipe(pipe_fds) == -1)
    {
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
      dup2(pipe_fds[1], STDOUT_FILENO);
      close(pipe_fds[0]);
      close(pipe_fds[1]);
      execute(parse_line(commands[i], &num_commands));
      exit(EXIT_FAILURE);
    }
    else
    {
      dup2(pipe_fds[0], STDIN_FILENO);
      close(pipe_fds[0]);
      close(pipe_fds[1]);
      wait(NULL);
    }
  }

  execute(parse_line(commands[num_commands -1], &num_commands));
}
