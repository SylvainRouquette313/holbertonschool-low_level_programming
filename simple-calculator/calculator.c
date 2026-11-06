# include <stdio.h>

/**
* print a welcome line
* display a menu
* allow quitting with option 0
*/

int main (void)
{
	int nombre;
	int A;
	int B;


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
	do
		{
	printf("choice: ");
	scanf("%d", &nombre);

	if (nombre == 1)
		{
			printf ("A: ");
			scanf ("%d", &A);
			printf ("B: ");
			scanf ("%d", &B);
			printf("Result: %d\n", A + B);
		}
		else if(nombre == 2)
		{
			printf("substraction\n");
		}
		else if(nombre == 3)
		{
			printf("multiplication\n");
		}
		else if(nombre == 4)
		{
			printf("division\n");
		}
		else if(nombre >= 5 && nombre <= 9)
		{
			printf("Invalid choice");
			printf("\n");
		}

		}
	while(nombre != 0);
			printf("Bye!\n");
	return (0);
}
