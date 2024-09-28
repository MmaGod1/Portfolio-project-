#include "raycasting.h"

static void initialize_step_and_side(float rayDirX, float rayDirY,
                                      float playerX, float playerY,
                                      int *mapX, int *mapY,
                                      int *stepX, int *stepY,
                                      float *sideDistX, float *sideDistY)
{
    *mapX = (int)playerX;
    *mapY = (int)playerY;

    if (rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * fabs(1 / rayDirX);
    }
    else
    {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * fabs(1 / rayDirX);
    }

    if (rayDirY < 0)
    {
        *stepY = -1;
        *sideDistY = (playerY - *mapY) * fabs(1 / rayDirY);
    }
    else
    {
        *stepY = 1;
        *sideDistY = (*mapY + 1.0 - playerY) * fabs(1 / rayDirY);
    }
}

static int perform_dda(GameStats *gameStats, int *mapX, int *mapY,
                        int stepX, int stepY, float sideDistX,
                        float sideDistY)
{
    int hit = 0;
    int side;

    while (hit == 0)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += fabs(1 / rayDirX);
            *mapX += stepX;
            side = 0;
        }
        else
        {
            sideDistY += fabs(1 / rayDirY);
            *mapY += stepY;
            side = 1;
        }

        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 &&
            *mapY < MAP_HEIGHT && gameStats->maze_map[*mapX][*mapY] == 1)
        {
            hit = 1;
        }
    }
    return side;
}

float get_wall_hit_coordinates(GameStats *gameStats, float playerX,
                               float playerY, float rayAngle,
                               int *mapX, int *mapY)
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    int stepX, stepY;
    float sideDistX, sideDistY;
    
    initialize_step_and_side(rayDirX, rayDirY, playerX, playerY, mapX, mapY,
                              &stepX, &stepY, &sideDistX, &sideDistY);

    int side = perform_dda(gameStats, mapX, mapY, stepX, stepY,
                            sideDistX, sideDistY);

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
