#include "raycasting.h"

void initialize_ray(float rayAngle, float *rayDirX, float *rayDirY, 
                    int *stepX, int *stepY, float playerX, float playerY,
                    int *mapX, int *mapY, float *sideDistX, 
                    float *sideDistY, float *deltaDistX, 
                    float *deltaDistY)
{
    *rayDirX = cos(rayAngle);
    *rayDirY = sin(rayAngle);

    *mapX = (int)playerX;
    *mapY = (int)playerY;

    *deltaDistX = fabs(1 / (*rayDirX));
    *deltaDistY = fabs(1 / (*rayDirY));

    if (*rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * (*deltaDistX);
    }
    else
    {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * (*deltaDistX);
    }

    if (*rayDirY < 0)
    {
        *stepY = -1;
        *sideDistY = (playerY - *mapY) * (*deltaDistY);
    }
    else
    {
        *stepY = 1;
        *sideDistY = (*mapY + 1.0 - playerY) * (*deltaDistY);
    }
}

float calculate_distance(int stepX, int stepY, int *mapX, int *mapY, 
                         float playerX, float playerY, 
                         float rayDirX, float rayDirY, 
                         GameStats *gameStats)
{
    float sideDistX = 0;
    float sideDistY = 0;
    int side = 0;
    bool hit = false;

    while (!hit)
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

        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 && *mapY < MAP_HEIGHT && 
            gameStats->maze_map[*mapX][*mapY] == 1)
            hit = true;
    }

    float perpWallDist;
    if (side == 0)
    {
        perpWallDist = (*mapX - playerX + (1 - stepX) / 2) / rayDirX;
    }
    else
    {
        perpWallDist = (*mapY - playerY + (1 - stepY) / 2) / rayDirY;
    }

    return (perpWallDist);
}

float cast_ray(GameStats *gameStats, float playerX, float playerY, 
                float rayAngle)
{
    float rayDirX, rayDirY;
    int stepX, stepY;
    int mapX, mapY;
    float sideDistX, sideDistY;
    float deltaDistX, deltaDistY;

    initialize_ray(rayAngle, &rayDirX, &rayDirY, &stepX, &stepY, 
                   playerX, playerY, &mapX, &mapY, 
                   &sideDistX, &sideDistY, &deltaDistX, &deltaDistY);
    
    return (calculate_distance(stepX, stepY, &mapX, &mapY, 
                               playerX, playerY, 
                               rayDirX, rayDirY, 
                               gameStats));
}
