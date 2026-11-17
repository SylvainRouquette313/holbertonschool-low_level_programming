#include <stdio.h>
#include "function_pointers.h"

/**
 * array_iterator - function given a parameter on each element
 * @array: array of each element
 * @size: the size of the array
 * @action: pointer to the function
 */
void array_iterator(int *array, size_t size, void (*action)(int))
{
	size_t i;

	if (array == NULL || action == NULL)
		return;
	for (i = 0; i < size; i++)
	action(array[i]);
}
