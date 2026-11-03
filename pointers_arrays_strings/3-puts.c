#include "main.h"

/**
 * _puts - prints a string followed by a new line
 * @str: the string to print
 */
void _puts(char *str)
{
	int i = 0;

	while (str[i] != '\0')
	{
		_puts(str[i]);
		i++;
	}
	_puts('\n');
}

