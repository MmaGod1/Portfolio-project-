#include "raycasting.h"

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

int read_character(FILE *file, int *ch)
{
    *ch = fgetc(file);
    while (*ch == '\r' || *ch == '\n')
        *ch = fgetc(file);

    if (*ch == EOF)
        return (1);

    return (0);
}

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
