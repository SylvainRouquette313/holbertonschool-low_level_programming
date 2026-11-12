#include "main.h"
#include <stdlib.h>

/**
 * array_range - the array created to contain all the value
 * @min: value minimum
 * @max: value maximum
 *
 * Return: NULL if max < min or malloc fail
 * Return to the pointer
 */
int *array_range(int min, int max)
{
	int i;
	int *ptr;

	if (min > max)
		return (NULL);
	ptr = malloc((max - min + 1) * sizeof(int));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < (max - min + 1))
	{
		ptr[i] = i + min;
		i++;
	}
	return (ptr);
}
