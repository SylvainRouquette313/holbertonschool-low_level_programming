#include "dog.h"

/**
 * init_dog - a pointer to the structure
 * @d: pointer to the structure
 * @name: a pointer to the char of name
 * @age: value for the age
 * @owner: a pointer to the char of the owner
 *
 */
void init_dog(struct dog *d, char *name, float age, char *owner)
{
	if (d == NULL)
		return;
	d->name = name;
	d->age = age;
	d->owner = owner;
}
