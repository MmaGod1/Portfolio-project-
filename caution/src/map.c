#include "raycasting.h"


/**
 * open_map_file - Opens the map file for reading.
 *
 * @filename: The path to the map file.
 * @file: Pointer to the FILE pointer to be opened.
 *
 * Attempts to open the specified map file in read mode.
 * 
 * Return: 0 on success, -1 on failure.
 */
int open_map_file(const char *filename, FILE **file)
{
    *file = fopen(filename, "r");
    if (!(*file))
    {
        fprintf(stderr, "Error opening file %s\n", filename);
        return (-1);
    }
    return (0);
}


/**
 * read_character - Reads the next valid character from the map file.
 *
 * @file: Pointer to the FILE to read from.
 * @ch: Pointer to store the read character.
 *
 * Skips newline and carriage return characters, returning the next valid character.
 * 
 * Return: 0 on success, 1 on EOF.
 */
int read_character(FILE *file, int *ch)
{
    *ch = fgetc(file);
    while (*ch == '\r' || *ch == '\n')
        *ch = fgetc(file);

    if (*ch == EOF)
        return (1);

    return (0);
}


/**
 * process_character - Processes a character from the map and updates the maze map.
 *
 * @gameStats: Pointer to the GameStats struct containing the maze map.
 * @ch: The character to process.
 * @x: The x-coordinate in the maze map.
 * @y: The y-coordinate in the maze map.
 *
 * Sets the appropriate value in the maze map based on the character read.
 * 
 * Return: 0 on success, -1 on failure (invalid character).
 */
int process_character(GameStats *gameStats, int ch, int x, int y)
{
    if (ch == '#')
    {
        gameStats->maze_map[x][y] = 1;
    }
    else if (ch == '.')
    {
        gameStats->maze_map[x][y] = 0;
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
 * load_map - Loads the maze map from the specified file.
 *
 * @gameStats: Pointer to the GameStats struct to store the loaded map.
 * @filename: The path to the map file.
 *
 * Reads the map file, processes each character, and fills the maze map array.
 * 
 * Return: 0 on success, -1 on failure.
 */
int load_map(GameStats *gameStats, const char *filename)
{
    int ch;
    FILE *file;
    if (open_map_file(filename, &file) < 0)
        return (-1);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (read_character(file, &ch) != 0)
            {
                fprintf(stderr, "Error reading file %s: unexpected EOF\n", 
                        filename);
                fclose(file);
                return (-1);
            }

            if (process_character(gameStats, ch, x, y) < 0)
            {
                fclose(file);
                return (-1);
            }
        }

        while ((ch = fgetc(file)) == '\r' || ch == '\n')
            ;
        if (ch != EOF)
            ungetc(ch, file);
    }

    fclose(file);
    return (0);
}
