#ifndef DOG_H
#define DOG_H

/**
 * struct dog - new type of structure
 * @name: type of char *
 * @age: type of float
 * @owner: another type of char*
 *
 */
struct dog
{
	char *name;
	float age;
	char *owner;
};

typedef struct dog dog_t;
void init_dog(struct dog *d, char *name, float age, char *owner);
void print_dog(struct dog *d);

#endif /* DOG_H */
