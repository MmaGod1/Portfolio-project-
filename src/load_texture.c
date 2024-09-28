#include "raycasting.h"

int load_map(GameStats *gameStats, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error opening map file: %s\n", filename);
        return (-1);
    }

    // Assuming MAP_WIDTH and MAP_HEIGHT are defined properly
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            fscanf(file, "%d", &gameStats->maze_map[x][y]);
        }
    }

    fclose(file);
    return (0);
}
