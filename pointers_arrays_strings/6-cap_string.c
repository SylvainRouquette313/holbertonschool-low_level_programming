#include "main.h"
#include <stdio.h>

/**
 * is_separator - checks if a character separates words
 * @c: the character to check
 *
 * Return: 1 if @c is a separator, 0 otherwise
 */
static int is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == ',' ||
		c == ';' || c == '.' || c == '!' || c == '?' ||
		c == '"' || c == '(' || c == ')' || c == '{' || c == '}');
}

/**
 * cap_string - capitalizes all words of a string
 * @s: the string to modify
 *
 * Return: a pointer to the modified string @s
 */
char *cap_string(char *s)
{
	int i = 0;
	int new_word = 1;

	while (s[i] != '\0')
	{
		if (new_word && s[i] >= 'a' && s[i] <= 'z')
			s[i] = s[i] - 'a' + 'A';
		new_word = is_separator(s[i]);
		i++;
	}

	return (s);
}
