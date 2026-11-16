#include <stdio.h>
#include <stdlib.h>
#include "dog.h"
#include <string.h>

/**
 * new_dog - function create a new dog
 * @name: name of the new dog
 * @age: age of the new dog
 * @owner: new owner of the new dog
 *
 * Return: ptr or NULL if malloc failed
 */

dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *ptr;
	unsigned int len;
	char *_name;
	char *_owner;

	ptr = malloc(sizeof(dog_t));
	if (ptr == NULL)
		return (NULL);
	len = strlen(name);
	_name = malloc(len + 1);
	if (_name == NULL)
	{
		free(ptr);
		return (NULL);
	}
	strcpy(_name, name);
	ptr->name = _name;
	ptr->age = age;
	len = strlen(owner);
	_owner = malloc(len + 1);
	if (_owner == NULL)
	{
		free(_name);
		free(ptr);
		return (NULL);
	}
	strcpy(_owner, owner);
	ptr->owner = _owner;
	return (ptr);
}
