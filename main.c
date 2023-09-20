#include "shell.h"

/**
 * main - fabricates a simple shell
 * @ac: argument count
 * @av: argument vectors
 * @env: environmental variables
 * Return: 0 success,1 error
 */
int main(int ac, char **av, char **env)
{
(void)ac;
(void)av;

prompt(env);

return (0);
}
