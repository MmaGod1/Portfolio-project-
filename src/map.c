#include "raycasting.h"

int load_map(GameStats *gameStats, const char *filename)
{
    int ch;
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        fprintf(stderr, "Error opening file %s\n", filename);
        return (-1);
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            ch = fgetc(file);
            while (ch == '\r' || ch == '\n')
                ch = fgetc(file);

            if (ch == EOF)
            {
                fprintf(stderr, "Error reading file %s: unexpected EOF\n", filename);
                fclose(file);
                return (-1);
            }

            if (ch == '#')
                gameStats->maze_map[x][y] = 1;
            else if (ch == '.')
                gameStats->maze_map[x][y] = 0;
            else
            {
                fprintf(stderr, "Invalid character '%c' in map file at [%d,%d]\n", 
                        ch, x, y);
                fclose(file);
                return (-1);
            }
        }

        while ((ch = fgetc(file)) == '\r' || ch == '\n');
        if (ch != EOF)
            ungetc(ch, file);
    }

    fclose(file);
    return (0);
}
