#include "main.h"
#include <stdio.h>

/**
 * _atoi - converts a string to an integer
 * @s: string to convert
 *
 * Return: integer value of the string
 */
int _atoi(char *s)
{
	int i = 0;
	int sign = 1;
	int n = 0;
	int found = 0;

	while (s[i] != '\0')
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			found = 1;
			n = n * 10 - (s[i] - '0');
		}
		else if (found)
			break;
		else if (s[i] == '-')
			sign = -sign;
		i++;
	}

	if (sign == 1)
		return (-n);
	return (n);
}
