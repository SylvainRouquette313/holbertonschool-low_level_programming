#include <stdlib.h>
#include "dog.h"

/**
 * _strdup - copies a string into newly allocated memory
 * @s: the string to copy
 *
 * Return: a pointer to the copy, or NULL on failure
 */
static char *_strdup(char *s)
{
	char *copy;
	int i, len = 0;

	if (s == NULL)
		return (NULL);
	while (s[len])
		len++;
	copy = malloc(len + 1);
	if (copy == NULL)
		return (NULL);
	for (i = 0; i <= len; i++)
		copy[i] = s[i];
	return (copy);
}

/**
 * new_dog - creates a new dog
 * @name: name of the dog
 * @age: age of the dog
 * @owner: owner of the dog
 *
 * Return: a pointer to the new dog, or NULL if the function fails
 */
dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *dog = malloc(sizeof(dog_t));

	if (dog == NULL)
		return (NULL);
	dog->name = _strdup(name);
	dog->owner = _strdup(owner);
	if (dog->name == NULL || dog->owner == NULL)
	{
		free(dog->name);
		free(dog->owner);
		free(dog);
		return (NULL);
	}
	dog->age = age;
	return (dog);
}
