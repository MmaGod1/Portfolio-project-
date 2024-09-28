#include "raycasting.h"

float cast_ray(float playerX, float playerY, float rayAngle)
{
int stepX, stepY, side;
float sideDistX, sideDistY;
/* Ray direction */
float rayDirX = cos(rayAngle);
float rayDirY = sin(rayAngle);

int mapX = (int)playerX;
int mapY = (int)playerY;

float deltaDistX = fabs(1 / rayDirX);
float deltaDistY = fabs(1 / rayDirY);
bool hit = false;

/* Calculate step and initial sideDist */
if (rayDirX < 0)
{
stepX = -1;
        sideDistX = (playerX - mapX) * deltaDistX;
    } 
    else
{
        stepX = 1;
        sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
    }
    if (rayDirY < 0)
{
        stepY = -1;
        sideDistY = (playerY - mapY) * deltaDistY;
}
else
{
        stepY = 1;
        sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
    }

    /* Perform DDA */
    while (!hit)
{
        /* Jump to next map square, either in x-direction or y-direction */
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        }
        else
        {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        /* Check if ray has hit a wall */
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && maze_map[mapX][mapY] == 1)
        {
            hit = true;
        }
    }

    /*Calculate the perpendicular wall distance for correction */
    float perpWallDist;
    if (side == 0)
    {
        perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
    }
    else
    {
        perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
    }

    return (perpWallDist);
}
