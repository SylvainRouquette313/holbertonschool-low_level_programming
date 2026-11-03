#include "main.h"

/**
* puts2 - print every character of a string
* @str : the string to print
*
* Return : Nothing
*/

void puts2(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i += 2)

		_putchar(str[i]);

	_putchar('\n');

}
