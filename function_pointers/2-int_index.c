#include <stdio.h>
#include "function_pointers.h"

/**
 * int_index - function that searches an integer
 * @array: array of elements
 * @size: the number of elements of the array
 * @cmp: pointer to the function
 *
 * Return: index of first match, or -1 if none
 */
int int_index(int *array, int size, int (*cmp)(int))
{
	int i;

	if (cmp == NULL || array == NULL || size <= 0)
		return (-1);
	i = 0;
	while (i < size)
	{
		if (cmp(array[i]) != 0)
			return (i);
	    i++;
	}
	return (-1);
}
