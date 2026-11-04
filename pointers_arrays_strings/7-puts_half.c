#include "main.h"

/**
* puts_half - print the second half of the string
* @str : the string to print
* Return : Nothing
*/

void puts_half(char *str)
{
	int i;
	int len = 0;

	while (str[len] != '\0')
		len++;

	for (i = (len + 1) / 2; i < len; i++)
		_putchar(str[i]);

	_putchar('\n');
}


