#include <stdlib.h>
#include <stdio.h>

/**
 * main - program that adds positive numbers
 * @argc: numbers of arguments
 * @argv: array of elements
 *
 * Return: 1 if error or 0
 */
int main(int argc, char *argv[])
{
	int i;
	int j;
	int sum;

	sum = 0;
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if ((argv[i][j] < '0') || (argv[i][j] > '9'))
			{
			printf("Error\n");
			return (1);
			}
			j++;
		}
		sum += atoi(argv[i]);
		i++;
	}
	printf("%d\n", sum);
	return (0);
}
