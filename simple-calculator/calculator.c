# include <stdio.h>

/**
* print a welcome line
* display a menu
* allow quitting with option 0
*/

int main (void)
{
	int nombre;

	printf("hello, calculator is here!");
	printf("\n");
	printf("1) Add");
	printf("\n");
	printf("2) Substract");
	printf("\n");
	printf("3) Multiply");
	printf("\n");
	printf("4) Divide");
	printf("\n");
	printf("0) Quit");
	printf("\n");
	printf("choice: ");
	scanf("%d", &nombre);
	if (nombre == 0)
	{
		printf("bye");
		printf("\n");
	}
	return (0);
}
