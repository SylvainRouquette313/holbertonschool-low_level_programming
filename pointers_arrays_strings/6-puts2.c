#include "main.h"

/**
* puts2 print every character of a string
* starting with '0', followed by a new line
*
* Return : Nothing
*/

void puts2(char *str)
{
	int i;
	{
	for (i = 0; i < 10; i += 2)
	{
		_putchar(str[i]);
	}
	_putchar('\n');
}
}

