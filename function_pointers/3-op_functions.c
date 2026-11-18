#include <stdio.h>
#include "3-calc.h"

/**
 * op_add - sum of a and b
 * @a: the first integer
 * @b: the second integer
 *
 * Return: result of a + b
 */
int op_add(int a, int b)
{
	return (a + b);
}

/**
 * op_sub - difference of a and b
 * @a: the first integer
 * @b: the second integer
 *
 * Return: result of a - b
 */
int op_sub(int a, int b)
{
	return (a - b);
}

/**
 * op_mul - product of a and b
 * @a: the first integer
 * @b: the second integer
 *
 * Return: product of a * b
 */
int op_mul(int a, int b)
{
	return (a * b);
}

/**
 * op_div - result of the division of a by b
 * @a: the first integer
 * @b: the second integer
 *
 * Return: result of a / b
 */
int op_div(int a, int b)
{
	return (a / b);
}

/**
 * op_mod - the remainder of the division of a by b
 * @a: the first integer
 * @b: the second integer
 *
 * Return: remainder of the division of a by b
 */
int op_mod(int a, int b)
{
	return (a % b);
}
