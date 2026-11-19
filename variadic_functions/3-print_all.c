#include "variadic_functions.h"
#include <stdarg.h>
#include <stdio.h>

/**
 * print_all - function that prints everything
 * @format: the format to print
 */
void print_all(const char * const format, ...)
{
	va_list args;
	int i;
	char *str;
	char *sep;

	va_start(args, format);
	i = 0;
	sep = "";
	while (format[i] != NULL && format[i] != '\0')
	{
		switch (format[i])
		{
		case 'c':
			printf("%s%c", sep, va_arg(args, int));
			sep = ", ";
			break;
		case 'i':
			printf("%s%d", sep, va_arg(args, int));
			sep = ", ";
			break;
		case 'f':
			printf("%s%f", sep, va_arg(args, double));
			sep = ", ";
			break;
		case 's':
			str = va_arg(args, char *);
			if (str == NULL)
				str = "(nil)";
			printf("%s%s", sep, str);
			sep = ", ";
			break;
		}
		i++;
	}
	printf("\n");
	va_end(args);
}
