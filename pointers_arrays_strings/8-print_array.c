#include "main.h"
#include <stdio.h>

/**
 * print_array - prints the numbers in the same order
 * @a: array of integers to print
 * @n: number of elements of the array to print
 *
 * Return: Nothing
 */

void print_array(int *a, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		printf("%d", a[i]);
		if (i < n - 1)
			printf(", ");
	}
	printf("\n");
}
