#include "raycasting.h"

void initialize_ray(float playerX, float playerY, float rayAngle,
                    int *mapX, int *mapY, int *stepX, int *stepY,
                    float *sideDistX, float *sideDistY,
                    float *deltaDistX, float *deltaDistY)
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    *mapX = (int)playerX;
    *mapY = (int)playerY;

    *deltaDistX = fabs(1 / rayDirX);
    *deltaDistY = fabs(1 / rayDirY);

    if (rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * (*deltaDistX);
    }
    else
    {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * (*deltaDistX);
    }

    if (rayDirY < 0)
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





float perform_ray_cast(GameStats *gameStats, int *mapX, int *mapY,
                       int stepX, int stepY, float sideDistX,
                       float sideDistY, float deltaDistX, float deltaDistY,
                       float rayDirX, float rayDirY) // Removed playerX, playerY
{
    int side;
    bool hit = false;

    while (!hit)
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
            hit = true;
        }
    }

    // Correct the fish-eye effect by calculating the perpendicular distance
    float perpWallDist;
    if (side == 0)
        perpWallDist = (sideDistX - deltaDistX) / rayDirX;
    else
        perpWallDist = (sideDistY - deltaDistY) / rayDirY;

    return perpWallDist;
}

float cast_ray(GameStats *gameStats, float playerX, float playerY,
                float rayAngle)
{
    int stepX, stepY, mapX, mapY;
    float sideDistX, sideDistY, deltaDistX, deltaDistY;
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    initialize_ray(playerX, playerY, rayAngle, &mapX, &mapY,
                   &stepX, &stepY, &sideDistX, &sideDistY,
                   &deltaDistX, &deltaDistY);

    return (perform_ray_cast(gameStats, &mapX, &mapY, stepX, stepY,
                             sideDistX, sideDistY, deltaDistX,
                             deltaDistY, rayDirX, rayDirY));
}
