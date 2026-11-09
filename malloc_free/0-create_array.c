#include "main.h"
#include <stdlib.h>

/**
 * create_array - create array of chars,
 * and initializes it with a specific char
 *
 * @size: size of the array of chars
 * @c : char of size
 * Return: a pointer to the array, or NULL if it fails
 *
 */

char *create_array(unsigned int size, char c)
{
	char *array;
	unsigned int i;

	if (size == 0)
	{
	return (NULL);
	}
	array = malloc(sizeof(char) * size);

	if (array == NULL)
	{
	return (NULL);
	}
	i = 0;

	while (i <= size - 1)
	{
	array[i] = c;
		i++;
	}
	return (array);
}
