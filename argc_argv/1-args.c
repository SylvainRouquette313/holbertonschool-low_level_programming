#include <stdio.h>

/**
 * main - program that prints the number of arguments
 * @argc: number of arguments
 * @argv: the array of the elements
 *
 * Return: 0
 */
int main(int argc, char *argv[])
{
	(void)argv;
	printf("%d\n", argc - 1);
	return (0);
}
