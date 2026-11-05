#include "main.h"
#include <stdio.h>

/**
 * _memcpy - copies memory area
 * @dest: the memory area where the bytes are copied to
 * @src: the memory area where the bytes are copied from
 * @n: the number of bytes to copy
 *
 * Return: a pointer to @dest
 */
char *_memcpy(char *dest, char *src, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		dest[i] = src[i];

	return (dest);
}
