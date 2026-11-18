#include <stdio.h>
#include <stdlib.h>
#include "3-calc.h"

/**
 * main - performs simple operations
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: 0
 */
int main(int argc, char *argv[])
{
	int (*operator)(int a, int b);
	int i;
	int j;

	if (argc != 4)
	{
		printf("Error\n");
		return (98);
	}
	operator = get_op_func(argv[2]);

	if (operator == NULL)
	{
		printf("Error\n");
		return (99);
	}
	i = atoi(argv[1]);
	j = atoi(argv[3]);
	if ((argv[2][0] == '/' || argv[2][0] == '%') && j == 0)
	{
		printf("Error\n");
		return (100);
	}
	printf("%d\n", operator(i, j));

	return (0);
}
