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

/*float perform_ray_cast(GameStats *gameStats, int *mapX, int *mapY,
                       int stepX, int stepY, float sideDistX,
                       float sideDistY, float deltaDistX, float deltaDistY,
                       float playerX, float playerY, float rayAngle)
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

    float perpWallDist;
    if (side == 0)
    {
        perpWallDist = (*mapX - playerX + (1 - stepX) / 2) / cos(rayAngle);
    }
    else
    {
        perpWallDist = (*mapY - playerY + (1 - stepY) / 2) / sin(rayAngle);
    }

    return (perpWallDist);
}
*/

float perform_ray_cast(GameStats *gameStats, int *mapX, int *mapY,
                       int stepX, int stepY, float sideDistX,
                       float sideDistY, float deltaDistX, float deltaDistY,
                       float playerX, float playerY, float rayAngle)
{
    int side;
    bool hit = false;

    while (!hit)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0; // Vertical wall
        }
        else
        {
            sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1; // Horizontal wall
        }

        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 &&
            *mapY < MAP_HEIGHT && gameStats->maze_map[*mapX][*mapY] == 1)
        {
            hit = true;
        }
    }

    // Calculate the perpendicular distance to the wall
    float perpWallDist;
    if (side == 0)
    {
        perpWallDist = (*mapX - playerX + (1 - stepX) / 2) / cos(rayAngle);
    }
    else
    {
        perpWallDist = (*mapY - playerY + (1 - stepY) / 2) / sin(rayAngle);
    }

    // Calculate the height of the wall to be drawn
    int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

    // Calculate the start and end pixel to draw
    int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

    // Now you can use drawStart and drawEnd to render the wall segment

    return perpWallDist; // Return the perpendicular wall distance if needed
}



float cast_ray(GameStats *gameStats, float playerX, float playerY,
                float rayAngle)
{
    int stepX, stepY, mapX, mapY;
    float sideDistX, sideDistY, deltaDistX, deltaDistY;

    initialize_ray(playerX, playerY, rayAngle, &mapX, &mapY,
                   &stepX, &stepY, &sideDistX, &sideDistY,
                   &deltaDistX, &deltaDistY);

    return (perform_ray_cast(gameStats, &mapX, &mapY, stepX, stepY,
                             sideDistX, sideDistY, deltaDistX,
                             deltaDistY, playerX, playerY, rayAngle));
}
