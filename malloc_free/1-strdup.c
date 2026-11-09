#include "main.h"
#include <stdlib.h>

/**
 * _strdup - functions that return a pointer to a new string
 * @str: string is duplicate
 *
 * Return: NULL if str = NULL
 */
char *_strdup(char *str)
{
	int n;
	int i;
	char *ptr;

	if (str == NULL)
	{
	return (NULL);
	}
	n = 0;
	while (str[n] != '\0')
	n++;
	n = n + 1;
	ptr = malloc(sizeof(char) * n);
	if (ptr == NULL)
	{
	return (NULL);
	}
	i = 0;
	while (i <= n - 1)
	{
		ptr[i] = str[i];
		i++;
	}
	return (ptr);
}
