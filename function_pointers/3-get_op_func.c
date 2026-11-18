#include <stdio.h>
#include <string.h>
#include "3-calc.h"

/**
 * get_op_func - function that select the function to perform the operation
 * @s: operator that passed as argument to the program
 *
 * Return: NULL or to the pointer of function
 */
int (*get_op_func(char *s))(int, int)
{
	 op_t ops[] = {
		{"+", op_add},
		{"-", op_sub},
		{"*", op_mul},
		{"/", op_div},
		{"%", op_mod},
		{NULL, NULL}
	};
	int i;

	i = 0;
	while (ops[i].op != NULL)
	{
		if (strcmp(s, ops[i].op) == 0)
			return (ops[i].f);
		i++;
	}
	return (NULL);
}
