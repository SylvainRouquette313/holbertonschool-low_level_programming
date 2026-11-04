#include "main.h"
#include <stdio.h>

/**
 * leet - encodes a string into 1337
 * @s: the string to encode
 *
 * Return: a pointer to the modified string @s
 */
char *leet(char *s)
{
	int i, j;
	char *letters = "aAeEoOtTlL";
	char *digits = "4433007711";

	for (i = 0; s[i] != '\0'; i++)
	{
		for (j = 0; letters[j] != '\0'; j++)
		{
			if (s[i] == letters[j])
			{
				s[i] = digits[j];
				break;
			}
		}
	}

	return (s);
}
