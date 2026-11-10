#include "main.h"
#include <stdlib.h>

/**
 * free_grid - function that frees 2 dimensional grid
 * @grid: the address of the grids
 * @height: height of the grid
 *
 * Return nothing
 */
void free_grid(int **grid, int height)
{
	int i;

	if (grid == NULL)

		return;

	i = 0;
	while (i < height)
	{
		free(grid[i]);
		i++;
	}
	free(grid);
}
