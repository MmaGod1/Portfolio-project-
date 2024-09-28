#include "raycasting.h"

/**
 * initialize_side_distances - Initializes the side distances and steps
 */
static void initialize_side_distances(float playerX, float playerY,
                                      float rayDirX, float rayDirY,
                                      int *mapX, int *mapY,
                                      int *stepX, int *stepY,
                                      float *sideDistX, float *sideDistY)
{
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    if (rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * deltaDistX;
    }
    else
    {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * deltaDistX;
    }

    if (rayDirY < 0)
    {
        *stepY = -1;
        *sideDistY = (playerY - *mapY) * deltaDistY;
    }
    else
    {
        *stepY = 1;
        *sideDistY = (*mapY + 1.0 - playerY) * deltaDistY;
    }
}

/**
 * perform_dda - Performs DDA (Digital Differential Analysis) for raycasting
 */
static int perform_dda(GameStats *gameStats, int *mapX, int *mapY,
                       float *sideDistX, float *sideDistY,
                       int stepX, int stepY, float deltaDistX, float deltaDistY)
{
    int hit = 0;
    int side;

    while (hit == 0)
    {
        if (*sideDistX < *sideDistY)
        {
            *sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0;
        }
        else
        {
            *sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1;
        }

        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 &&
            *mapY < MAP_HEIGHT && gameStats->maze_map[*mapX][*mapY] == 1)
        {
            hit = 1;
        }
    }

    return (side);
}

/**
 * get_wall_hit_coordinates - Determines wall hit position during raycasting
 */
float get_wall_hit_coordinates(GameStats *gameStats, float playerX,
                               float playerY, float rayAngle,
                               int *mapX, int *mapY)
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    *mapX = (int)playerX;
    *mapY = (int)playerY;

    float sideDistX, sideDistY;
    int stepX, stepY;

    initialize_side_distances(playerX, playerY, rayDirX, rayDirY, mapX, mapY,
                              &stepX, &stepY, &sideDistX, &sideDistY);

    int side = perform_dda(gameStats, mapX, mapY, &sideDistX, &sideDistY,
                           stepX, stepY, fabs(1 / rayDirX), fabs(1 / rayDirY));

    float wallX;
    if (side == 0)
    {
        wallX = playerY + ((sideDistX - fabs(1 / rayDirX)) * rayDirY);
    }
    else
    {
        wallX = playerX + ((sideDistY - fabs(1 / rayDirY)) * rayDirX);
    }

    wallX -= floor(wallX);

    return (wallX);
}
