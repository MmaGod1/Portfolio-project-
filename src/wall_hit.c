#include "raycasting.h"

float get_wall_hit_coordinates(GameStats *gameStats, float playerX, 
                               float playerY, float rayAngle, 
                               int *mapX, int *mapY)
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    *mapX = (int)playerX;
    *mapY = (int)playerY;

    float sideDistX, sideDistY, wallX;
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    int stepX, stepY, side, hit = 0;

    if (rayDirX < 0)
    {
        stepX = -1;
        sideDistX = (playerX - *mapX) * deltaDistX;
    }
    else
    {
        stepX = 1;
        sideDistX = (*mapX + 1.0 - playerX) * deltaDistX;
    }

    if (rayDirY < 0)
    {
        stepY = -1;
        sideDistY = (playerY - *mapY) * deltaDistY;
    }
    else
    {
        stepY = 1;
        sideDistY = (*mapY + 1.0 - playerY) * deltaDistY;
    }

    while (hit == 0)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0;
        }
        else
        {
            sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1;
        }

        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 && 
            *mapY < MAP_HEIGHT && gameStats->maze_map[*mapX][*mapY] == 1)
        {
            hit = 1;
        }
    }

    if (side == 0)
    {
        wallX = playerY + ((sideDistX - deltaDistX) * rayDirY);
    }
    else
    {
        wallX = playerX + ((sideDistY - deltaDistY) * rayDirX);
    }

    wallX -= floor(wallX);

    return (wallX);
}
