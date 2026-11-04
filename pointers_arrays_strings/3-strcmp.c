#include "main.h"
#include <stdio.h>

/**
 * _strcmp - compares two strings
 * @s1 : first string
 * @s2 : second string
 *
 * Return: 0 if the strings are equal,
 * a negative value if s1 is less than s2,
 * and a positive value if s1 is greater than s2
 */
int _strcmp(char *s1, char *s2)
{
	int i = 0;

	while (s1[i] != '\0' && s1[i] == s2[i])
	i++;

	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
