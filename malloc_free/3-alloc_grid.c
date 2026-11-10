#include "main.h"
#include <stdlib.h>

/**
 * alloc_grid - returns a pointer to a 2 dimensional array of integers
 * @width: number of columns of the grid
 * @height: number of rows of the grid
 *
 * Description: each element of the grid is initialized to 0.
 *
 * Return: pointer to the new grid, or NULL if width or height is 0 or
 * negative, or if the allocation fails
 */
int **alloc_grid(int width, int height)
{
	int w = width, h = height;
	int i, j;
	int **grid;

	if (h <= 0 || w <= 0)
	{
		return (NULL);
	}
	grid = malloc(sizeof(int *) * h);
	if (grid == NULL)
	{
		return (NULL);
	}
	for (i = 0 ; i < h ; i++)
	{
	grid[i] = malloc(sizeof(int) * w);
	if (grid[i] == NULL)
	{
	while (i >= 0)
	{
	free(grid[i]);
	i--;
	}
		free(grid);
		return (NULL);
	}
	j = 0;
	while (j < w)
	{
		grid[i][j] = 0;
		j++;
	}
	}
		return (grid);
}
