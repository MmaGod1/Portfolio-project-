#include "raycasting.h"

  int maze_map[MAP_WIDTH][MAP_HEIGHT] = {
	  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	  {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	  {1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
	  {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
	  {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	  {1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1},
	  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	  {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
	  {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
	  {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
	  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
	  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
	  {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
	  {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	  {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	  {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
	  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};



/**
 * read_next_char - Read the next character from the file, skipping newlines.
 * @file: Pointer to the file being read.
 *
 * Return: The next valid character or EOF.
 */
int read_next_char(FILE *file)
{
	int ch = fgetc(file);
	while (ch == '\r' || ch == '\n')
	{
		ch = fgetc(file);
	}
	return (ch);
}

/**
 * process_char - Processes a character and updates the maze_map.
 * @ch: The character to process.
 * @x: The x-coordinate in the maze_map.
 * @y: The y-coordinate in the maze_map.
 * @maze_map: The 2D array where the map will be stored.
 *
 * Return: 0 on success, or -1 on error (invalid character).
 */
int process_char(int ch, int x, int y, int maze_map[MAP_WIDTH][MAP_HEIGHT])
{
	if (ch == '#')
	{
		maze_map[x][y] = 1;
	}
	else if (ch == '.')
	{
		maze_map[x][y] = 0;
	}
	else
	{
		fprintf(stderr, "Invalid character '%c' in map file at [%d,%d]\n",
				ch, x, y);
		return (-1);
	}
	return (0);
}

/**
 * load_map - Load the maze map from a file.
 *
 * This function reads a map file and populates the maze_map array.
 * It interprets '#' as walls (1) and '.' as empty spaces (0).
 *
 * @filename: The name of the file to read the map from.
 * @maze_map: The 2D array where the map will be stored.
 *
 * Return: 0 on success, or -1 on error (e.g., file not found, unexpected EOF,
 * invalid characters).
 */
int load_map(const char *filename, int maze_map[MAP_WIDTH][MAP_HEIGHT])
{
	int ch, y, x;
	FILE *file = fopen(filename, "r");

	if (!file)
	{
		fprintf(stderr, "Error opening file %s\n", filename);
		return (-1);
	}

	for (y = 0; y < MAP_HEIGHT; y++)
	{
		for (x = 0; x < MAP_WIDTH; x++)
		{
			ch = read_next_char(file);
			if (ch == EOF)
			{
				fprintf(stderr, "Error reading file %s: unexpected EOF\n",
						filename);
				fclose(file);
				return (-1);
			}

			if (process_char(ch, x, y, maze_map) == -1)
			{
				fclose(file);
				return (-1);
			}
		}
		/* Skip over remaining characters (newline or carriage return) */
		while ((ch = fgetc(file)) == '\r' || ch == '\n')
			;

		if (ch != EOF)
			ungetc(ch, file);
	}

	fclose(file);
	return (0);
}
